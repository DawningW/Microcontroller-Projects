package io.github.qingchenw.microcontroller.device;

import com.felhr.usbserial.UsbSerialDevice;
import com.felhr.usbserial.UsbSerialInterface;

import java.nio.ByteBuffer;

import io.github.qingchenw.microcontroller.R;

public class DeviceSerial extends DeviceBase implements UsbSerialInterface.UsbReadCallback {
    static private final byte[] CRLF = { '\r', '\n' };

    private UsbSerialDevice serialDevice;
    private ByteBuffer buffer;
    private Callback callback;

    public DeviceSerial(UsbSerialDevice serialDevice) {
        this.serialDevice = serialDevice;
        this.buffer = ByteBuffer.allocate(512);
    }

    @Override
    public int getIcon() {
        return R.drawable.ic_debug_24;
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
        return this.serialDevice.isOpen();
    }

    @Override
    public void connect() {
        if (this.serialDevice.open()) {
            this.serialDevice.read(this);
            if (this.callback != null)
                this.callback.onConnected();
        }
    }

    @Override
    public void disconnect() {
        this.serialDevice.close();
        if (this.callback != null)
            this.callback.onDisconnected();
    }

    @Override
    public void send(byte[] data) {
        this.serialDevice.write(data);
        this.serialDevice.write(CRLF);
    }

    @Override
    public void onReceivedData(byte[] data) {
        for (byte b : data) {
            int position = this.buffer.position();
            if (!this.buffer.hasRemaining() || b == '\n' && position > 0 && this.buffer.get(position - 1) == '\r') {
                this.buffer.flip();
                byte[] bytes = new byte[this.buffer.limit() - 1];
                if (bytes.length > 0) {
                    this.buffer.get(bytes);
                    if (this.callback != null)
                        this.callback.onDataReceived(bytes);
                }
                this.buffer.clear();
            } else {
                this.buffer.put(b);
            }
        }
    }
}
