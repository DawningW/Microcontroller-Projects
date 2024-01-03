package io.github.qingchenw.microcontroller;

import android.app.Application;
import android.app.Service;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.IBinder;

import io.github.qingchenw.microcontroller.device.DeviceManager;
import io.github.qingchenw.microcontroller.service.DeviceDiscoveryService;

public class MCUApplication extends Application {
    private static MCUApplication instance;

    private DeviceDiscoveryService deviceService;
    private final ServiceConnection connection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder binder) {
            deviceService = ((DeviceDiscoveryService.ServiceBinder) binder).getService();
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            deviceService = null;
        }
    };
    private DeviceManager deviceManager;

    @Override
    public void onCreate() {
        super.onCreate();
        Intent intent = new Intent(this, DeviceDiscoveryService.class);
        bindService(intent, connection, Service.BIND_AUTO_CREATE);
        deviceManager = new DeviceManager();
        instance = this;
    }

    public DeviceDiscoveryService getDeviceService() {
        return deviceService;
    }

    public DeviceManager getDeviceManager() {
        return deviceManager;
    }

    public static MCUApplication getInstance() {
        return instance;
    }
}
