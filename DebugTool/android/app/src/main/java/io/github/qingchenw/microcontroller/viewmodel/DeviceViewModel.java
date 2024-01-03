package io.github.qingchenw.microcontroller.viewmodel;

import android.annotation.SuppressLint;
import android.app.Application;
import android.app.Service;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.IBinder;

import androidx.annotation.NonNull;
import androidx.lifecycle.AndroidViewModel;
import androidx.lifecycle.MutableLiveData;

import java.util.Collections;
import java.util.List;
import java.util.Objects;

import io.github.qingchenw.microcontroller.device.DeviceManager;
import io.github.qingchenw.microcontroller.device.IDevice;
import io.github.qingchenw.microcontroller.service.DeviceDiscoveryService;

public class DeviceViewModel extends AndroidViewModel implements DeviceDiscoveryService.ScanCallback {
    private static final int SCAN_PERIOD = 10;

    @SuppressLint("StaticFieldLeak")
    private DeviceDiscoveryService service;
    private final ServiceConnection connection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder binder) {
            service = ((DeviceDiscoveryService.ServiceBinder) binder).getService();
            service.addScanCallback(DeviceViewModel.this);
            service.startScan(30);
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            service.removeScanCallback(DeviceViewModel.this);
            service = null;
        }
    };
    private final MutableLiveData<ScanState> scanState = new MutableLiveData<>(ScanState.STOPPED);
    private final MutableLiveData<List<IDevice>> devices = new MutableLiveData<>(Collections.emptyList());

    public DeviceViewModel(@NonNull Application application) {
        super(application);
        Intent intent = new Intent(application, DeviceDiscoveryService.class);
        application.bindService(intent, connection, Service.BIND_AUTO_CREATE);
    }

    @Override
    protected void onCleared() {
        if (service != null) {
            getApplication().unbindService(connection);
        }
        super.onCleared();
    }

    public void startScan() {
        service.startScan(SCAN_PERIOD);
    }

    public void stopScan() {
        service.stopScan();
    }

    public int getDeviceCount() {
        return Objects.requireNonNull(devices.getValue()).size();
    }

    public MutableLiveData<ScanState> getScanState() {
        return scanState;
    }

    public MutableLiveData<List<IDevice>> getDevices() {
        return devices;
    }

    @Override
    public void onScanStart() {
        scanState.postValue(ScanState.SCANNING);
    }

    @Override
    public void onDeviceScanned(IDevice device) {
        // TODO 改成从数据库和服务器获取已绑定的设备
        if (device.getModel().equals("WC-5")) {
            device.connect();
            DeviceManager.getInstance().addDevice(device);
        }
        devices.postValue(service.getScannedDevices());
    }

    @Override
    public void onScanStop() {
        scanState.postValue(ScanState.STOPPED);
    }

    @Override
    public void onDeviceRemoved(IDevice device) {
        if (DeviceManager.getInstance().hasDevice(device))
            DeviceManager.getInstance().removeDevice(device);
        devices.postValue(service.getScannedDevices());
    }

    public enum ScanState {
        STOPPED,
        SCANNING
    }
}
