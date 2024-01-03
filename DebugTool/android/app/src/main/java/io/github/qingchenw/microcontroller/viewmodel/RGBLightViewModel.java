package io.github.qingchenw.microcontroller.viewmodel;

import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import org.json.JSONException;
import org.json.JSONObject;

import io.github.qingchenw.microcontroller.device.DeviceManager;
import io.github.qingchenw.microcontroller.device.IDevice;

public class RGBLightViewModel extends ViewModel implements DeviceManager.OnChangedListener, IDevice.Callback {
    private IDevice device;
    private MutableLiveData<Boolean> isConnected = new MutableLiveData<>(false);
    private MutableLiveData<LightMode> lightMode = new MutableLiveData<>(LightMode.CONSTANT);

    private MutableLiveData<String> lightColor = new MutableLiveData<>("#ffffff");

    public RGBLightViewModel() {
        DeviceManager.getInstance().addOnChangedListener(this);
        setDevice(DeviceManager.getInstance().getDeviceByModel("WC-5"));
    }

    @Override
    protected void onCleared() {
        DeviceManager.getInstance().removeOnChangedListener(this);
        super.onCleared();
    }

    public MutableLiveData<Boolean> isConnected() {
        return isConnected;
    }

    public MutableLiveData<LightMode> getLightMode() {
        return lightMode;
    }

    public MutableLiveData<String> getLightColor() {
        return lightColor;
    }

    public IDevice getDevice() {
        return device;
    }

    public void setDevice(IDevice d) {
        if (device != null) {
            device.setCallback(null);
        }
        device = d;
        if (device != null) {
            device.setCallback(this);
            send("info");
        }
        isConnected.postValue(device != null);
    }

    public void send(String msg) {
        if (device != null)
            device.send(msg.getBytes());
    }

    public void setMode(LightMode mode) {
        if (lightMode.getValue() == mode)
            return;
        send("mode " + mode.name().toLowerCase());
        lightMode.postValue(mode);
    }

    public void setColor(String hexColor) {
        LightMode mode = lightMode.getValue();
        if (mode == LightMode.CONSTANT || mode == LightMode.BLINK ||
                mode == LightMode.BREATH || mode == LightMode.CHASE) {
            send("mode " + mode.name().toLowerCase() + " " + hexColor);
        }
    }

    @Override
    public void onDeviceChanged(IDevice d, boolean removed) {
        if (d.getModel().equals("WC-5")) {
            if (removed) {
                if (d.equals(device)) {
                    setDevice(null);
                }
            } else {
                setDevice(d);
            }
        }
    }

    @Override
    public void onConnected(IDevice device) {}

    @Override
    public void onDisconnected(IDevice device) {}

    @Override
    public void onError(IDevice device, String error) {}

    @Override
    public void onDataReceived(IDevice device, byte[] data) {
        String msg = new String(data);
        try {
            JSONObject json = new JSONObject(msg);
            String mode = json.optString("mode", "constant");
            String color = json.optString("color", "#ffffff");
            int brightness = json.optInt("brightness");
            int temperature = json.optInt("temperature");
            int fps = json.optInt("fps");
            LightMode enumMode = LightMode.CONSTANT;
            try {
                enumMode = LightMode.valueOf(mode.toUpperCase());
            } catch (IllegalArgumentException ignored) {}
            lightMode.postValue(enumMode);
            lightColor.postValue(color);
        } catch (JSONException ignored) {}
    }

    public enum LightMode {
        CONSTANT,
        BLINK,
        BREATH,
        CHASE,
        RAINBOW,
        STREAM,
        ANIMATION,
        MUSIC,
        CUSTOM
    }
}
