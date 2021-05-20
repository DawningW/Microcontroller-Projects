package io.github.qingchenw.microcontroller.device;

import androidx.annotation.DrawableRes;

public interface IDevice {
    String getName();
    void setName(String name);
    String getID();
    void setID(String id);
    String getModel();
    void setModel(String model);
    String getVersion();
    void setVersion(String version);
    String getProducer();
    void setProducer(String producer);

    @DrawableRes int getIcon();
    String getAddress();
    void setCallback(Callback callback);
    boolean isConnected();
    void connect();
    void disconnect();
    void send(byte[] data);

    interface Callback {
        void onConnected(IDevice device);
        void onDisconnected(IDevice device);
        void onError(IDevice device, String error);
        void onDataReceived(IDevice device, byte[] data);
    }
}
