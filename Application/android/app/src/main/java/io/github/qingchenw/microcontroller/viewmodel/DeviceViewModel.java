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
import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;

import java.util.List;

import io.github.qingchenw.microcontroller.device.DeviceManager;
import io.github.qingchenw.microcontroller.device.IDevice;
import io.github.qingchenw.microcontroller.service.DeviceDiscoveryService;

public class DeviceViewModel extends AndroidViewModel
        implements DeviceDiscoveryService.ScanCallback, DeviceManager.OnChangedListener {
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
    private DeviceManager deviceManager;
    private MutableLiveData<ScanState> scanState;
    private MutableLiveData<List<IDevice>> devices;

    public DeviceViewModel(@NonNull Application application) {
        super(application);
        deviceManager = DeviceManager.getInstance();
        deviceManager.addOnChangedListener(this);
        Intent intent = new Intent(application, DeviceDiscoveryService.class);
        application.bindService(intent, connection, Service.BIND_AUTO_CREATE);
        scanState = new MutableLiveData<>(ScanState.STOPPED);
        devices = new MutableLiveData<>(deviceManager.getDevices());
    }

    @Override
    protected void onCleared() {
        if (service != null) {
            getApplication().unbindService(connection);
        }
        if (deviceManager != null) {
            deviceManager.removeOnChangedListener(this);
        }
        super.onCleared();
    }

    public void startScan() {
        service.startScan(10);
    }

    public void stopScan() {
        service.stopScan();
    }

    public int getDeviceCount() {
        return deviceManager.getDeviceCount();
    }

    public int getConnectedDeviceCount() {
        return deviceManager.getConnectedDeviceCount();
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
        deviceManager.addDevice(device);
    }

    @Override
    public void onScanStop() {
        scanState.postValue(ScanState.STOPPED);
    }

    @Override
    public void onDeviceChanged(IDevice device, boolean removed) {
        devices.setValue(deviceManager.getDevices());
    }

    public enum ScanState {
        STOPPED,
        SCANNING
    }
}
