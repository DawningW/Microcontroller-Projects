package io.github.qingchenw.microcontroller;

import android.app.Application;
import android.content.Context;
import android.hardware.usb.UsbManager;

import io.github.qingchenw.microcontroller.device.DeviceManager;

public class MCUApplication extends Application {
    private static UsbManager usbManager;
    private static DeviceManager deviceManager;

    @Override
    public void onCreate() {
        super.onCreate();
        usbManager = (UsbManager) getSystemService(Context.USB_SERVICE);
        deviceManager = new DeviceManager();
    }

    public static UsbManager getUsbManager() {
        return usbManager;
    }

    public static DeviceManager getDeviceManager() {
        return deviceManager;
    }
}
