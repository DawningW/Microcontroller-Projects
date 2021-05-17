package io.github.qingchenw.microcontroller.device;

import io.github.qingchenw.microcontroller.R;

public class DeviceBluetooth extends DeviceBase {

    public DeviceBluetooth() {

    }

    @Override
    public int getIcon() {
        return R.drawable.ic_debug_24;
    }

    @Override
    public String getAddress() {
        return null;
    }

    @Override
    public void setCallback(Callback callback) {

    }

    @Override
    public boolean isConnected() {
        return false;
    }

    @Override
    public void connect() {

    }

    @Override
    public void disconnect() {

    }

    @Override
    public void send(byte[] data) {

    }
}
