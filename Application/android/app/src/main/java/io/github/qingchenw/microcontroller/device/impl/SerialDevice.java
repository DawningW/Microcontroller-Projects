package io.github.qingchenw.microcontroller.device.impl;

import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;

import com.hoho.android.usbserial.driver.UsbSerialPort;
import com.hoho.android.usbserial.util.SerialInputOutputManager;

import java.io.IOException;
import java.nio.ByteBuffer;

import io.github.qingchenw.microcontroller.MCUApplication;
import io.github.qingchenw.microcontroller.R;

public class SerialDevice extends BaseDevice implements SerialInputOutputManager.Listener {
    static private final byte[] CRLF = { '\r', '\n' };

    private UsbDevice usbDevice;
    private UsbSerialPort serialPort;
    private SerialInputOutputManager serialIoManager;
    private ByteBuffer buffer;
    private Callback callback;

    public SerialDevice(UsbDevice usbDevice, UsbSerialPort serialPort) {
        this.usbDevice = usbDevice;
        this.serialPort = serialPort;
        this.buffer = ByteBuffer.allocate(512);
    }

    @Override
    public int getIcon() {
        return R.drawable.ic_usb_24;
    }

    @Override
    public String getAddress() {
        return usbDevice.getDeviceName();
    }

    @Override
    public void setCallback(Callback callback) {
        this.callback = callback;
    }

    @Override
    public boolean isConnected() {
        return serialPort.isOpen();
    }

    @Override
    public void connect() {
        UsbDeviceConnection usbConnection = MCUApplication.getUsbManager().openDevice(usbDevice);
        try {
            serialPort.open(usbConnection);
            serialIoManager = new SerialInputOutputManager(serialPort, this);
            serialIoManager.start();
            if (callback != null)
                callback.onConnected(this);
        } catch (IOException | IllegalStateException e) {
            if (callback != null)
                callback.onError(this, e.getLocalizedMessage());
        }
    }

    @Override
    public void disconnect() {
        try {
            serialPort.close();
            serialIoManager.stop();
            serialIoManager = null;
            if (callback != null)
                callback.onDisconnected(this);
        } catch (IOException e) {
            if (callback != null)
                callback.onError(this, e.getLocalizedMessage());
        }
    }

    @Override
    public void send(byte[] data) {
        serialIoManager.writeAsync(data);
        serialIoManager.writeAsync(CRLF);
    }

    @Override
    public void onNewData(byte[] data) {
        for (byte b : data) {
            int position = buffer.position();
            if (!buffer.hasRemaining() || b == '\n' && position > 0 && buffer.get(position - 1) == '\r') {
                buffer.flip();
                byte[] bytes = new byte[buffer.limit() - 1];
                if (bytes.length > 0) {
                    buffer.get(bytes);
                    if (callback != null)
                        callback.onDataReceived(this, bytes);
                }
                buffer.clear();
            } else {
                buffer.put(b);
            }
        }
    }

    @Override
    public void onRunError(Exception e) {
        if (callback != null)
            callback.onError(SerialDevice.this, e.getLocalizedMessage());
        disconnect();
        notifyRemove();
    }
}
