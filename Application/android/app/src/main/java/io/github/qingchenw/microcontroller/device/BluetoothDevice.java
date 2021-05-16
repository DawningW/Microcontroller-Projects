package io.github.qingchenw.microcontroller.device;

public class BluetoothDevice implements IDevice {
    String name;

    public BluetoothDevice() {

    }

    @Override
    public String getName() {
        return name;
    }

    @Override
    public void setName(String name) {
        this.name = name;
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
