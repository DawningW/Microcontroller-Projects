package io.github.qingchenw.microcontroller.service;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;

import com.felhr.usbserial.SerialPortBuilder;
import com.felhr.usbserial.SerialPortCallback;
import com.felhr.usbserial.UsbSerialDevice;
import com.felhr.usbserial.UsbSerialInterface;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import io.github.qingchenw.microcontroller.Utils;
import io.github.qingchenw.microcontroller.device.DeviceSerial;
import io.github.qingchenw.microcontroller.device.DeviceWebSocket;
import io.github.qingchenw.microcontroller.device.IDevice;
import io.github.qingchenw.microcontroller.device.SSDPDescriptor;
import io.resourcepool.ssdp.client.SsdpClient;
import io.resourcepool.ssdp.model.DiscoveryListener;
import io.resourcepool.ssdp.model.SsdpRequest;
import io.resourcepool.ssdp.model.SsdpService;
import io.resourcepool.ssdp.model.SsdpServiceAnnouncement;
import okhttp3.Call;
import okhttp3.Request;
import okhttp3.Response;

public final class DeviceDiscoveryService extends Service implements SerialPortCallback, DiscoveryListener {
    static private final String TAG = "DeviceDiscoverySrv";
    // Supported baud rates
    // 300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600
    static private final int BAUD_RATE = 9600;

    private final ServiceBinder binder = new ServiceBinder();
    private boolean isScanning = false;
    private List<ScanCallback> callbacks;
    private Timer timer;
    private SerialPortBuilder serialBuilder;
    private SsdpClient SSDPClient;

    @Override
    public void onCreate() {
        super.onCreate();
        callbacks = new ArrayList<>();
        timer = new Timer();
        serialBuilder = SerialPortBuilder.createSerialPortBuilder(this);
        SSDPClient = SsdpClient.create();
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
        super.onDestroy();
    }

    public void addScanCallback(ScanCallback callback) {
        callbacks.add(callback);
    }

    public boolean removeScanCallback(ScanCallback callback) {
        return callbacks.remove(callback);
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
            serialBuilder.openSerialPorts(this, BAUD_RATE,
                    UsbSerialInterface.DATA_BITS_8,
                    UsbSerialInterface.STOP_BITS_1,
                    UsbSerialInterface.PARITY_NONE,
                    UsbSerialInterface.FLOW_CONTROL_OFF);
            SSDPClient.discoverServices(SsdpRequest.discoverAll(), this);
            if (timeout > 0) {
                timer.schedule(new TimerTask() {
                    @Override
                    public void run() {
                        stopScan();
                    }
                }, timeout * 1000);
            }
        } else {
            Log.w(TAG, "Scanning devices now. Please stop first.");
        }
    }

    public void stopScan() {
        if (isScanning) {
            timer.cancel();
            serialBuilder.unregisterListeners(this);
            SSDPClient.stopDiscovery();
            for (ScanCallback callback : callbacks) {
                callback.onScanStop();
            }
            Log.i(TAG, "Stop scanning devices.");
            isScanning = false;
        }
    }

    @Override
    public void onSerialPortsDetected(List<UsbSerialDevice> serialPorts) {
        for (UsbSerialDevice serialPort : serialPorts) {
            if (serialPort.isOpen()) {
                Log.i(TAG,"Connect to serial successfully.");
                DeviceSerial serialDevice = new DeviceSerial(serialPort);
                try {
                    serialPort.syncWrite("info\r\n".getBytes(), 200);
                    byte[] buffer = new byte[256];
                    serialPort.syncRead(buffer, 1000);
                    String str = new String(buffer).trim();
                    JSONObject json = new JSONObject(str);
                    serialDevice.setName(json.getString("name"));
                    serialDevice.setID(json.getString("id"));
                    serialDevice.setModel(json.getString("model"));
                    serialDevice.setVersion(json.getString("version"));
                    serialDevice.setProducer(json.optString("producer"));
                    Log.i(TAG,"Found serial device.");
                } catch (JSONException e) {
                    serialDevice.setName("未知设备");
                    Log.i(TAG,"Unknown serial device.");
                } finally {
                    serialPort.syncClose();
                }
                for (ScanCallback callback : callbacks) {
                    callback.onDeviceScanned(serialDevice);
                }
            } else {
                Log.e(TAG,"Connect to serial failed.");
                serialBuilder.disconnectDevice(serialPort);
            }
        }
    }

    @Override
    public void onServiceDiscovered(SsdpService ssdpService) {
        Log.i(TAG,"Discovery device through SSDP.");
        DeviceWebSocket device = new DeviceWebSocket(ssdpService.getRemoteIp().getHostAddress());
        try {
            Request request = new Request.Builder()
                    .url(ssdpService.getLocation())
                    .build();
            Call call = Utils.getHttpClient().newCall(request);
            Response response = call.execute();
            String str = response.body().string();
            SSDPDescriptor descriptor = SSDPDescriptor.parse(str);
            device.setWebpage(descriptor.getURLBase());
            device.setName(descriptor.getFriendlyName());
            device.setID(descriptor.getSerialNumber());
            device.setModel(descriptor.getModelName());
            device.setVersion(descriptor.getModelNumber());
            device.setProducer(descriptor.getManufacturer());
        } catch (IOException e) {
            e.printStackTrace();
        }
        for (ScanCallback callback : callbacks) {
            callback.onDeviceScanned(device);
        }
    }

    @Override
    public void onServiceAnnouncement(SsdpServiceAnnouncement ssdpServiceAnnouncement) {

    }

    @Override
    public void onFailed(Exception e) {
        e.printStackTrace();
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
    }
}
