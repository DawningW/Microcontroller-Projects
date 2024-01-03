package io.github.qingchenw.microcontroller.device.impl;

import androidx.annotation.NonNull;

import org.jetbrains.annotations.Nullable;

import io.github.qingchenw.microcontroller.R;
import io.github.qingchenw.microcontroller.Utils;
import okhttp3.Request;
import okhttp3.Response;
import okhttp3.WebSocket;
import okhttp3.WebSocketListener;

public class WebSocketDevice extends BaseDevice {
    private static final String PORT = "81";

    private String address;
    private String webpage;
    private boolean isConnected;
    private WebSocket webSocket;

    public WebSocketDevice(String address) {
        this.address = address;
    }

    @Override
    public int getIcon() {
        return R.drawable.ic_wifi_24;
    }

    @Override
    public String getAddress() {
        return address;
    }


    @Override
    public boolean isConnected() {
        return isConnected;
    }

    @Override
    public void connect() {
        Request request = new Request.Builder()
                .url("ws://" + address + ":" + PORT)
                .build();
        webSocket = Utils.getHttpClient().newWebSocket(request, new Listener());
    }

    @Override
    public void disconnect() {
        webSocket.close(1000, null);
    }

    @Override
    public void send(byte[] data) {
        webSocket.send(new String(data));
    }

    public String getWebpage() {
        return webpage;
    }

    public void setWebpage(String webpage) {
        this.webpage = webpage;
    }

    private class Listener extends WebSocketListener {
        @Override
        public void onOpen(@NonNull WebSocket webSocket, @NonNull Response response) {
            isConnected = true;
            if (callback != null)
                callback.onConnected(WebSocketDevice.this);
        }

        @Override
        public void onClosed(@NonNull WebSocket webSocket, int code, @NonNull String reason) {
            isConnected = false;
            if (callback != null)
                callback.onDisconnected(WebSocketDevice.this);
        }

        @Override
        public void onMessage(@NonNull WebSocket webSocket, @NonNull String text) {
            if (callback != null)
                callback.onDataReceived(WebSocketDevice.this, text.getBytes());
        }

        @Override
        public void onFailure(@NonNull WebSocket webSocket, @NonNull Throwable t, @Nullable Response response) {
            isConnected = false;
            if (callback != null) {
                callback.onError(WebSocketDevice.this, t.getLocalizedMessage());
                callback.onDisconnected(WebSocketDevice.this);
            }
            if (manager != null)
                manager.onReleased(WebSocketDevice.this);
        }
    }
}
