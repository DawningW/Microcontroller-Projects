package io.github.qingchenw.microcontroller;

import android.app.Application;

import io.github.qingchenw.microcontroller.device.DeviceManager;

public class MCUApplication extends Application {
    private DeviceManager deviceManager;

    @Override
    public void onCreate() {
        super.onCreate();
        deviceManager = new DeviceManager();
    }

    public DeviceManager getDeviceManager() {
        return deviceManager;
    }
}
