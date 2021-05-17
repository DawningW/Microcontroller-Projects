package io.github.qingchenw.microcontroller.device;

import io.github.qingchenw.microcontroller.R;

public class DeviceWebSocket extends DeviceBase {
    String address;
    String webpage;
    // WebSocket
    private Callback callback;

    public DeviceWebSocket(String address) {
        this.address = address;
    }

    @Override
    public int getIcon() {
        return R.drawable.ic_debug_24;
    }

    @Override
    public String getAddress() {
        return this.address;
    }

    @Override
    public void setCallback(Callback callback) {
        this.callback = callback;
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

    public String getWebpage() {
        return this.webpage;
    }

    public void setWebpage(String webpage) {
        this.webpage = webpage;
    }
}
