package io.github.qingchenw.microcontroller.ui;

import android.os.Bundle;
import android.os.Handler;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ScrollView;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;
import androidx.lifecycle.ViewModelProvider.AndroidViewModelFactory;

import java.util.ArrayList;
import java.util.List;

import io.github.qingchenw.microcontroller.Utils;
import io.github.qingchenw.microcontroller.databinding.FragmentDebugBinding;
import io.github.qingchenw.microcontroller.device.IDevice;
import io.github.qingchenw.microcontroller.viewmodel.DeviceViewModel;

/**
 * Debug Fragment
 *
 * @author wc
 */
public class DebugFragment extends Fragment implements IDevice.Callback {
    private FragmentDebugBinding viewBinding;
    private DeviceViewModel deviceViewModel;
    private final Handler handler = new Handler();
    private final List<String> deviceNameList = new ArrayList<>();
    private ArrayAdapter<String> devicesAdapter;
    private IDevice device;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        viewBinding = FragmentDebugBinding.inflate(inflater, container, false);
        viewBinding.spinnerDevices.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
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
        viewBinding.buttonConnect.setOnClickListener(view -> {
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
        viewBinding.textData.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) { }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) { }

            @Override
            public void afterTextChanged(Editable s) {
                viewBinding.scrollViewData.post(
                        () -> viewBinding.scrollViewData.fullScroll(ScrollView.FOCUS_DOWN));
            }
        });
        viewBinding.buttonSend.setOnClickListener(view -> {
            if (device != null && device.isConnected()) {
                String data = viewBinding.editTextData.getText().toString().trim();
                if (!data.isEmpty()) {
                    sendData(data);
                }
                viewBinding.editTextData.getText().clear();
            } else {
                Utils.toast(getContext(), "尚未连接至设备");
            }
        });
        return viewBinding.getRoot();
    }

    @Override
    public void onViewCreated(@NonNull View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        AndroidViewModelFactory factory = AndroidViewModelFactory.getInstance(requireActivity().getApplication());
        deviceViewModel = new ViewModelProvider(requireActivity(), factory).get(DeviceViewModel.class);
        devicesAdapter = new ArrayAdapter<>(getContext(), android.R.layout.simple_spinner_item, deviceNameList);
        devicesAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        viewBinding.spinnerDevices.setAdapter(devicesAdapter);
        deviceViewModel.getDevices().observe(getViewLifecycleOwner(), devices -> {
            deviceNameList.clear();
            for (IDevice device : devices) {
                deviceNameList.add(device.getName());
            }
            devicesAdapter.notifyDataSetChanged();
        });
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
            viewBinding.spinnerDevices.setEnabled(false);
            viewBinding.buttonConnect.setText("断开");
            addText("已连接至 " + device.getName() + "(" + device.getAddress() + ")");
        });
    }

    @Override
    public void onDisconnected(IDevice device) {
        handler.post(() -> {
            viewBinding.spinnerDevices.setEnabled(true);
            viewBinding.buttonConnect.setText("连接");
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
            if (viewBinding.radioButtonString.isChecked()) {
                addText("接收: " + new String(data));
            } else if (viewBinding.radioButtonHex.isChecked()) {
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
        if (viewBinding.radioButtonString.isChecked()) {
            device.send(data.getBytes());
        } else if (viewBinding.radioButtonHex.isChecked()) {
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
        if (!text.isEmpty()) viewBinding.textData.append("\n");
        viewBinding.textData.append(text);
    }
}
