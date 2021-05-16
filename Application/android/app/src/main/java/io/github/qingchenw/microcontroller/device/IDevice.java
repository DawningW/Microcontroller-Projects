package io.github.qingchenw.microcontroller.device;

public interface IDevice {
    // TODO 改成DeviceInfo
    String getName();
    void setName(String name);
    String getAddress();
    void setCallback(Callback callback);
    boolean isConnected();
    void connect();
    void disconnect();
    void send(byte[] data);

    interface Callback {
        void onConnected();
        void onDisconnected();
        void onError(String error);
        void onDataReceived(byte[] data);
    }
}
