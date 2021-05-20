package io.github.qingchenw.microcontroller.device;

import android.os.Handler;
import android.os.Looper;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;

import io.github.qingchenw.microcontroller.Utils;

/**
 * Manage devices that have been discovered.
 *
 * @author wc
 */
// TODO 发心跳包
public final class DeviceManager {
    private static DeviceManager instance;
    private Handler handler;
    private List<OnChangedListener> listeners;
    private List<IDevice> devices;

    public DeviceManager() {
        handler = new Handler(Looper.getMainLooper());
        listeners = new ArrayList<>();
        devices = Collections.synchronizedList(new ArrayList<>());
        instance = this;
    }

    public void addOnChangedListener(OnChangedListener listener) {
        if (!listeners.contains(listener))
            listeners.add(listener);
    }

    public void removeOnChangedListener(OnChangedListener listener) {
        listeners.remove(listener);
    }

    public List<IDevice> getDevices() {
        return devices;
    }

    public int getDeviceCount() {
        return devices.size();
    }

    public int getConnectedDeviceCount() {
        int count = 0;
        for (IDevice device : devices) {
            if (device.isConnected()) count++;
        }
        return count;
    }

    public boolean hasDevice(IDevice device) {
        return devices.contains(device);
    }

    public boolean hasDevice(String id) {
        return getDevice(id) != null;
    }

    public IDevice getDevice(String id) {
        for (IDevice device : devices) {
            if (Utils.stringsAreEqualled(device.getID(), id)) {
                return device;
            }
        }
        return null;
    }

    public void addDevice(IDevice device) {
        Iterator<IDevice> iterator = devices.iterator();
        while (iterator.hasNext()) {
            IDevice oldDevice = iterator.next();
            if (oldDevice.getAddress().equals(device.getAddress())) {
                if (Utils.stringsAreEqualled(oldDevice.getID(), device.getID())) {
                    return;
                }
                oldDevice.disconnect();
                iterator.remove();
                break;
            }
        }
        devices.add(device);
        if (!listeners.isEmpty()) {
            handler.post(() -> {
                for (OnChangedListener listener : listeners)
                    listener.onDeviceChanged(device, false);
            });
        }
    }

    public boolean removeDevice(IDevice device) {
        if (devices.remove(device)) {
            if (!listeners.isEmpty()) {
                handler.post(() -> {
                    for (OnChangedListener listener : listeners)
                        listener.onDeviceChanged(device, true);
                });
            }
            return true;
        }
        return false;
    }

    public static DeviceManager getInstance() {
        return instance;
    }

    public interface OnChangedListener {
        void onDeviceChanged(IDevice device, boolean removed);
    }
}
