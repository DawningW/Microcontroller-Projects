package io.github.qingchenw.microcontroller.ui;

import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.ScrollView;
import android.widget.Spinner;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;
import androidx.lifecycle.ViewModelProvider.AndroidViewModelFactory;

import java.util.ArrayList;
import java.util.List;

import io.github.qingchenw.microcontroller.R;
import io.github.qingchenw.microcontroller.Utils;
import io.github.qingchenw.microcontroller.device.IDevice;
import io.github.qingchenw.microcontroller.viewmodel.DeviceViewModel;

/**
 * Debug devices
 *
 * @author wc
 */
public class DebugFragment extends Fragment implements IDevice.Callback {
    private DeviceViewModel deviceViewModel;

    private Spinner spinnerDevices;
    private Button buttonConnect;
    private ScrollView scrollViewData;
    private TextView textViewData;
    private RadioButton radioButtonString;
    private RadioButton radioButtonHex;
    private EditText editTextData;

    private Handler handler;
    private List<String> deviceNamesList;
    private ArrayAdapter<String> devicesAdapter;
    private IDevice device;

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        handler = new Handler();
        deviceNamesList = new ArrayList<>();
    }

    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        AndroidViewModelFactory factory = AndroidViewModelFactory.getInstance(requireActivity().getApplication());
        deviceViewModel = new ViewModelProvider(requireActivity(), factory).get(DeviceViewModel.class);

        View root = inflater.inflate(R.layout.fragment_debug, container, false);
        spinnerDevices = root.findViewById(R.id.spinner_devices);
        spinnerDevices.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                if (device == null || !device.isConnected())
                    device = deviceViewModel.getDevices().getValue().get(position);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {
                if (device != null && !device.isConnected())
                    device = null;
            }
        });
        devicesAdapter = new ArrayAdapter<>(getContext(),
                android.R.layout.simple_spinner_item, deviceNamesList);
        devicesAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinnerDevices.setAdapter(devicesAdapter);
        deviceViewModel.getDevices().observe(getViewLifecycleOwner(), devices -> {
            deviceNamesList.clear();
            for (IDevice d :devices) {
                deviceNamesList.add(d.getName());
            }
            devicesAdapter.notifyDataSetChanged();
        });
        buttonConnect = root.findViewById(R.id.button_connect);
        buttonConnect.setOnClickListener(view -> {
            if (device != null) {
                if (!device.isConnected()) {
                    device.setCallback(this);
                    device.connect();
                } else {
                    device.disconnect();
                }
            } else {
                Utils.toast(getContext(), "请选择设备");
            }
        });
        scrollViewData = root.findViewById(R.id.scrollView_data);
        textViewData = root.findViewById(R.id.text_data);
        radioButtonString = root.findViewById(R.id.radioButton_string);
        radioButtonHex = root.findViewById(R.id.radioButton_hex);
        editTextData = root.findViewById(R.id.editText_data);
        Button buttonSend = root.findViewById(R.id.button_send);
        buttonSend.setOnClickListener(view -> {
            if (device != null && device.isConnected()) {
                String data = editTextData.getText().toString().trim();
                if (!data.isEmpty()) {
                    sendData(data);
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
    public void onConnected(IDevice device) {
        handler.post(() -> {
            spinnerDevices.setEnabled(false);
            buttonConnect.setText("断开");
            addText("已连接至 " + device.getName() + "(" + device.getAddress() + ")");
        });
    }

    @Override
    public void onDisconnected(IDevice device) {
        handler.post(() -> {
            spinnerDevices.setEnabled(true);
            buttonConnect.setText("连接");
            addText("已与 " + device.getName() + " 断开连接");
        });
    }

    @Override
    public void onError(IDevice device, String error) {
        handler.post(() -> {
            Log.e("Debug", error);
            addText("错误: " + error);
        });
    }

    @Override
    public void onDataReceived(IDevice device, byte[] data) {
        handler.post(() -> {
            if (radioButtonString.isChecked()) {
                addText("接收: " + new String(data));
            } else if (radioButtonHex.isChecked()) {
                StringBuilder sb = new StringBuilder();
                for (byte b : data) {
                    sb.append(Integer.toHexString(b & 0xff).toUpperCase())
                            .append(' ');
                }
                addText("接收: " + sb.toString());
            } else {
                addText("错误: 未选择接收数据种类");
            }
        });
    }

    private void sendData(String data) {
        if (radioButtonString.isChecked()) {
            device.send(data.getBytes());
        } else if (radioButtonHex.isChecked()) {
            try {
                data = data.replace(" ", "");
                if (data.length() % 2 == 1) data += '0';
                byte[] hex = new byte[data.length() / 2];
                for (int i = 0; i < data.length() / 2; i++) {
                    String sub = data.substring(i * 2, i * 2 + 2);
                    hex[i] = (byte) Short.parseShort(sub, 16);
                }
                device.send(hex);
            } catch (NumberFormatException e) {
                addText("错误: 输入的16进制数有误");
            }
        } else {
            addText("错误: 未选择发送数据种类");
        }
        addText("发送: " + data);
    }

    private void addText(String text) {
        if (!text.isEmpty()) textViewData.append("\n");
        textViewData.append(text);
        scrollViewData.post(() -> scrollViewData.fullScroll(ScrollView.FOCUS_DOWN));
    }
}
