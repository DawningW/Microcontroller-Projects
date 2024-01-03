package io.github.qingchenw.microcontroller.device;

import android.os.Handler;
import android.os.Looper;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Manage devices that have been connected.
 *
 * @author wc
 */
// TODO 发心跳包
public final class DeviceManager {
    private static DeviceManager instance;
    private final Handler handler;
    private final List<OnChangedListener> listeners;
    private final Map<String, IDevice> devices;

    public DeviceManager() {
        handler = new Handler(Looper.getMainLooper());
        listeners = new ArrayList<>();
        devices = Collections.synchronizedMap(new HashMap<>());
        instance = this;
    }

    public void addOnChangedListener(OnChangedListener listener) {
        if (!listeners.contains(listener))
            listeners.add(listener);
    }

    public void removeOnChangedListener(OnChangedListener listener) {
        listeners.remove(listener);
    }

    public int getDeviceCount() {
        return devices.size();
    }

    public boolean hasDevice(IDevice device) {
        return devices.containsValue(device);
    }

    public boolean hasDevice(String address) {
        return devices.containsKey(address);
    }

    public IDevice getDevice(String address) {
        return devices.get(address);
    }

    // TODO 仅供临时使用
    @Deprecated
    public IDevice getDeviceByModel(String model) {
        for (IDevice device : devices.values()) {
            if (model.equals(device.getModel())) {
                return device;
            }
        }
        return null;
    }

    public void addDevice(IDevice device) {
        devices.put(device.getAddress(), device);
        if (!listeners.isEmpty()) {
            handler.post(() -> {
                for (DeviceManager.OnChangedListener listener : listeners)
                    listener.onDeviceChanged(device, false);
            });
        }
    }

    public boolean removeDevice(IDevice device) {
        if (devices.remove(device.getAddress()) != null) {
            if (!listeners.isEmpty()) {
                handler.post(() -> {
                    for (DeviceManager.OnChangedListener listener : listeners)
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
