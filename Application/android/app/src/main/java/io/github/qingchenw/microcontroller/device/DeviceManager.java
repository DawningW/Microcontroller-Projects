package io.github.qingchenw.microcontroller.device;

import android.content.Context;
import android.util.Log;

import com.felhr.usbserial.SerialPortBuilder;
import com.felhr.usbserial.SerialPortCallback;
import com.felhr.usbserial.UsbSerialDevice;
import com.felhr.usbserial.UsbSerialInterface;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

public final class DeviceManager implements SerialPortCallback {
    static private final String TAG = "DeviceManager";
    // Supported baud rates
    // 300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600
    static private final int BAUD_RATE = 9600;

    private Context context;
    private List<IDevice> devices;
    private SerialPortBuilder serialBuilder;

    public DeviceManager(Context ctx) {
        this.context = ctx;
        devices = new ArrayList<>();
        serialBuilder = SerialPortBuilder.createSerialPortBuilder(this);
    }

    public void scan() {
        serialBuilder.openSerialPorts(this.context, BAUD_RATE,
                UsbSerialInterface.DATA_BITS_8,
                UsbSerialInterface.STOP_BITS_1,
                UsbSerialInterface.PARITY_NONE,
                UsbSerialInterface.FLOW_CONTROL_OFF);
    }

    public List<IDevice> getDevices() {
        return devices;
    }

    @Override
    public void onSerialPortsDetected(List<UsbSerialDevice> serialPorts) {
        for (UsbSerialDevice serialPort : serialPorts) {
            if (!serialPort.isOpen())
                serialPort.syncOpen();
            if (serialPort.isOpen()) {
                Log.i(TAG,"Connect to serial successfully.");
                serialPort.syncWrite("info".getBytes(), 100);
                byte[] buffer = new byte[256];
                serialPort.syncRead(buffer, 400);
                String info = new String(buffer).trim();
                try {
                    JSONObject json = new JSONObject(info);
                    String name = json.getString("name");
                    SerialDevice serialDevice = new SerialDevice(serialPort);
                    serialDevice.setName(name);
                    devices.add(serialDevice);
                    Log.i(TAG,"Found my serial device: " + name);
                } catch (JSONException e) {
                    Log.i(TAG,"Not my serial device.");
                }
            } else {
                Log.e(TAG,"Connect to serial failed.");
            }
        }
    }
}
