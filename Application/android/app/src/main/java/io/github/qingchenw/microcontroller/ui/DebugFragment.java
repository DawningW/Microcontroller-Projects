package io.github.qingchenw.microcontroller.ui;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import java.util.List;

import io.github.qingchenw.microcontroller.MCUApplication;
import io.github.qingchenw.microcontroller.R;
import io.github.qingchenw.microcontroller.Utils;
import io.github.qingchenw.microcontroller.service.DeviceDiscoveryService;
import io.github.qingchenw.microcontroller.device.IDevice;

/*
 * Debug serial, bluetooth and wifi devices
 *
 * @author wc
 **/
public class DebugFragment extends Fragment implements IDevice.Callback {
    private IDevice device;
    private TextView textViewData;
    private EditText editTextData;

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View root = inflater.inflate(R.layout.fragment_debug, container, false);
        textViewData = root.findViewById(R.id.text_data);
        editTextData = root.findViewById(R.id.editText_data);
        Button buttonConnect = root.findViewById(R.id.button_connect);
        buttonConnect.setOnClickListener(view -> {
            if (device == null || !device.isConnected()) {
                List<IDevice> devices = ((MainActivity) getActivity()).devices;
                if (!devices.isEmpty()) {
                    device = devices.get(0);
                    device.setCallback(this);
                    device.connect();
                }
            } else {
                device.disconnect();
            }
        });
        Button buttonSend = root.findViewById(R.id.button_send);
        buttonSend.setOnClickListener(view -> {
            if (device != null && device.isConnected()) {
                String data = editTextData.getText().toString().trim();
                if (!data.isEmpty()) {
                    // TODO 可选择字符串或16进制
                    device.send(data.getBytes());
                    textViewData.append("\n发送: " + data);
                }
                editTextData.getText().clear();
            } else {
                Utils.toast(getContext(), "尚未连接至设备");
            }
        });
        return root;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if (device != null && device.isConnected())
            device.disconnect();
        device = null;
    }

    @Override
    public void onConnected() {
        textViewData.append("\n已连接至 " + device.getName() + "(" + device.getAddress() + ")");
    }

    @Override
    public void onDisconnected() {
        textViewData.append("\n已与 " + device.getName() + " 断开连接");
    }

    @Override
    public void onError(String error) {
        textViewData.append("\n错误: " + new String(error));
    }

    @Override
    public void onDataReceived(byte[] data) {
        textViewData.append("\n接收: " + new String(data));
    }
}
