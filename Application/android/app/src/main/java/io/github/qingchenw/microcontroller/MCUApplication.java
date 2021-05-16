package io.github.qingchenw.microcontroller;

import android.app.Application;

import io.github.qingchenw.microcontroller.device.DeviceManager;

public class MCUApplication extends Application {
    private DeviceManager deviceManager = null;

    @Override
    public void onCreate() {
        super.onCreate();
        if (deviceManager == null) {
            deviceManager = new DeviceManager(this);
        }
    }

    public DeviceManager getDeviceManager() {
        return deviceManager;
    }
}
