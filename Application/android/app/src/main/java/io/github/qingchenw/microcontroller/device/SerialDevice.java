package io.github.qingchenw.microcontroller.device;

import com.felhr.usbserial.UsbSerialDevice;
import com.felhr.usbserial.UsbSerialInterface;

public class SerialDevice implements IDevice, UsbSerialInterface.UsbReadCallback {
    UsbSerialDevice serialDevice;
    String name;
    Callback callback;

    public SerialDevice(UsbSerialDevice serialDevice) {
        this.serialDevice = serialDevice;
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
        return this.serialDevice.getPortName();
    }

    @Override
    public void setCallback(Callback callback) {
        this.callback = callback;
    }

    @Override
    public boolean isConnected() {
        return serialDevice.isOpen();
    }

    @Override
    public void connect() {
        if (serialDevice.open()) {
            serialDevice.read(this);
            if (this.callback != null)
                this.callback.onConnected();
        }
    }

    @Override
    public void disconnect() {
        serialDevice.close();
        if (this.callback != null)
            this.callback.onDisconnected();
    }

    @Override
    public void send(byte[] data) {
        serialDevice.getOutputStream().write(data);
    }

    @Override
    public void onReceivedData(byte[] data) {
        if (this.callback != null)
            this.callback.onDataReceived(data);
    }
}
