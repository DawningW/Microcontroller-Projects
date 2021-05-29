package io.github.qingchenw.microcontroller.service;

import android.app.PendingIntent;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;

import com.clj.fastble.BleManager;
import com.clj.fastble.callback.BleScanCallback;
import com.clj.fastble.data.BleDevice;
import com.clj.fastble.data.BleScanState;
import com.hoho.android.usbserial.driver.SerialTimeoutException;
import com.hoho.android.usbserial.driver.UsbSerialDriver;
import com.hoho.android.usbserial.driver.UsbSerialPort;
import com.hoho.android.usbserial.driver.UsbSerialProber;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Objects;
import java.util.Timer;
import java.util.TimerTask;

import io.github.qingchenw.microcontroller.BuildConfig;
import io.github.qingchenw.microcontroller.Utils;
import io.github.qingchenw.microcontroller.device.IDevice;
import io.github.qingchenw.microcontroller.device.SSDPDescriptor;
import io.github.qingchenw.microcontroller.device.impl.SerialDevice;
import io.github.qingchenw.microcontroller.device.impl.WebSocketDevice;
import io.resourcepool.ssdp.client.SsdpClient;
import io.resourcepool.ssdp.model.DiscoveryListener;
import io.resourcepool.ssdp.model.SsdpRequest;
import io.resourcepool.ssdp.model.SsdpService;
import io.resourcepool.ssdp.model.SsdpServiceAnnouncement;
import okhttp3.Call;
import okhttp3.Request;
import okhttp3.Response;

public final class DeviceDiscoveryService extends Service implements DiscoveryListener, IDevice.Manager {
    private static final String TAG = "DeviceDiscoverySrv";
    private static final String INTENT_ACTION_GRANT_USB = BuildConfig.APPLICATION_ID + ".GRANT_USB";
    // Supported baud rates
    // 300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600
    private static final int BAUD_RATE = 9600;

    private final ServiceBinder binder = new ServiceBinder();
    private final BroadcastReceiver broadcastReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (INTENT_ACTION_GRANT_USB.equals(intent.getAction())) {
                boolean hasPermission =
                        intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false);
                if (hasPermission) {
                    scanSerial();
                } else {
                    Utils.toast(getBaseContext(), "您拒绝了授予访问USB设备的权限, 已停止扫描USB串口");
                }
            }
        }
    };
    private final List<ScanCallback> callbacks = new ArrayList<>();
    private final List<IDevice> devices = new ArrayList<>();
    private boolean isScanning = false;
    private Timer timer;
    private UsbManager usbManager;
    private BleManager bleManager;
    private SsdpClient SSDPClient;

    @Override
    public void onCreate() {
        registerReceiver(broadcastReceiver, new IntentFilter(INTENT_ACTION_GRANT_USB));
        usbManager = (UsbManager) getSystemService(Context.USB_SERVICE);
        bleManager = BleManager.getInstance();
        bleManager.init(getApplication());
    }

    @Override
    public IBinder onBind(Intent intent) {
        return binder;
    }

    @Override
    public boolean onUnbind(Intent intent) {
        return super.onUnbind(intent);
    }

    @Override
    public void onDestroy() {
        stopScan();
        bleManager.destroy();
        unregisterReceiver(broadcastReceiver);
        devices.clear();
    }

    public void addScanCallback(ScanCallback callback) {
        callbacks.add(callback);
    }

    public void removeScanCallback(ScanCallback callback) {
        callbacks.remove(callback);
    }

    public List<IDevice> getScannedDevices() {
        return devices;
    }

    public boolean isScanning() {
        return isScanning;
    }

    public void startScan(int timeout) {
        if (!isScanning) {
            isScanning = true;
            Log.i(TAG, "Start to scan devices.");
            for (ScanCallback callback : callbacks) {
                callback.onScanStart();
            }
            Iterator<IDevice> iterator = devices.iterator();
            while (iterator.hasNext()) {
                if (!iterator.next().isConnected()) {
                    iterator.remove();
                }
            }
            scanSerial();
            if (bleManager.isSupportBle() && bleManager.isBlueEnable()) {
                scanBluetooth();
            }
            if (Utils.isWifiConnected(this)) {
                scanSSDP();
            }
        } else {
            timer.cancel();
            timer.purge();
            timer = null;
        }
        if (timeout > 0) {
            timer = new Timer();
            timer.schedule(new TimerTask() {
                @Override
                public void run() {
                    stopScan();
                }
            }, timeout * 1000);
        }
    }

    public void stopScan() {
        if (isScanning) {
            timer.cancel();
            timer.purge();
            timer = null;
            if (bleManager.getScanSate() == BleScanState.STATE_SCANNING) {
                bleManager.cancelScan();
            }
            if (SSDPClient != null) {
                SSDPClient.stopDiscovery();
                SSDPClient = null;
            }
            for (ScanCallback callback : callbacks) {
                callback.onScanStop();
            }
            Log.i(TAG, "Stop scanning devices.");
            isScanning = false;
        }
    }

    private boolean hasDevice(String address) {
        for (IDevice device : devices) {
            if (device.getAddress().equals(address))
                return true;
        }
        return false;
    }

    private void scanSerial() {
        List<UsbSerialDriver> drivers = UsbSerialProber.getDefaultProber().findAllDrivers(usbManager);
        for (UsbSerialDriver driver : drivers) {
            if (usbManager.hasPermission(driver.getDevice())) {
                for (UsbSerialPort port : driver.getPorts()) {
                    onSerialPortsDetected(port);
                }
            } else {
                PendingIntent usbPermissionIntent = PendingIntent.getBroadcast(
                        this, 0, new Intent(INTENT_ACTION_GRANT_USB), 0);
                usbManager.requestPermission(driver.getDevice(), usbPermissionIntent);
                return;
            }
        }
    }

    public void onSerialPortsDetected(UsbSerialPort serialPort) {
        if (hasDevice(serialPort.getDevice().getDeviceName()))
            return;
        UsbDeviceConnection usbConnection = usbManager.openDevice(serialPort.getDevice());
        if (usbConnection != null) {
            try {
                serialPort.open(usbConnection);
                serialPort.setParameters(BAUD_RATE, UsbSerialPort.DATABITS_8,
                        UsbSerialPort.STOPBITS_1, UsbSerialPort.PARITY_NONE);
                Log.i(TAG,"Connect to serial device successfully.");
                SerialDevice device = new SerialDevice(usbManager, serialPort);
                device.setManager(this);
                try {
                    serialPort.write("info\r\n".getBytes(), 200);
                    byte[] buffer = new byte[256];
                    serialPort.read(buffer, 1000);
                    String str = new String(buffer).trim();
                    JSONObject json = new JSONObject(str);
                    device.setName(json.getString("name"));
                    device.setID(json.getString("id"));
                    device.setModel(json.getString("model"));
                    device.setVersion(json.getString("version"));
                    device.setProducer(json.optString("producer"));
                    Log.i(TAG,"Found serial device.");
                } catch (SerialTimeoutException | JSONException e) {
                    Log.i(TAG,"Unknown serial device.");
                } finally {
                    serialPort.close();
                }
                devices.add(device);
                for (ScanCallback callback : callbacks) {
                    callback.onDeviceScanned(device);
                }
            } catch (Exception e) {
                Log.e(TAG,"Connect to serial device failed.");
            }
        } else {
            Log.e(TAG, "Can't open USB connection.");
        }
    }

    private void scanBluetooth() {
        BleManager.getInstance().scan(new BleScanCallback() {
            @Override
            public void onScanStarted(boolean success) {

            }

            @Override
            public void onScanning(BleDevice bleDevice) {

            }

            @Override
            public void onLeScan(BleDevice bleDevice) {

            }

            @Override
            public void onScanFinished(List<BleDevice> scanResultList) {

            }
        });
    }

    private void scanSSDP() {
        SSDPClient = SsdpClient.create();
        SSDPClient.discoverServices(SsdpRequest.discoverAll(), this);
    }

    @Override
    public void onServiceDiscovered(SsdpService ssdpService) {
        if (hasDevice(ssdpService.getRemoteIp().getHostAddress()))
            return;
        Log.i(TAG,"Discovery device through SSDP.");
        WebSocketDevice device = new WebSocketDevice(ssdpService.getRemoteIp().getHostAddress());
        device.setManager(this);
        try {
            Request request = new Request.Builder()
                    .url(ssdpService.getLocation())
                    .build();
            Call call = Utils.getHttpClient().newCall(request);
            Response response = call.execute();
            String str = Objects.requireNonNull(response.body()).string();
            SSDPDescriptor descriptor = SSDPDescriptor.parse(str);
            device.setWebpage(descriptor.getURLBase());
            device.setName(descriptor.getFriendlyName());
            device.setID(descriptor.getSerialNumber());
            device.setModel(descriptor.getModelName());
            device.setVersion(descriptor.getModelNumber());
            device.setProducer(descriptor.getManufacturer());
        } catch (IOException | NullPointerException e) {
            Log.i(TAG,"Unknown ssdp device.");
        }
        devices.add(device);
        for (ScanCallback callback : callbacks) {
            callback.onDeviceScanned(device);
        }
    }

    @Override
    public void onServiceAnnouncement(SsdpServiceAnnouncement ssdpServiceAnnouncement) {

    }

    @Override
    public void onFailed(Exception e) {
        Log.e(TAG, "SSDP error: " + e.getLocalizedMessage());
        stopScan();
    }

    @Override
    public void onReleased(IDevice device) {
        if (devices.remove(device)) {
            for (ScanCallback callback : callbacks) {
                callback.onDeviceRemoved(device);
            }
        }
    }

    public class ServiceBinder extends Binder {
        public DeviceDiscoveryService getService() {
            return DeviceDiscoveryService.this;
        }
    }

    public interface ScanCallback {
        void onScanStart();
        void onDeviceScanned(IDevice device);
        void onScanStop();
        void onDeviceRemoved(IDevice device);
    }
}
