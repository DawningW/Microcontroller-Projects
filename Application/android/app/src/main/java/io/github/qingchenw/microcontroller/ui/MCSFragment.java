package io.github.qingchenw.microcontroller.ui;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import java.util.UUID;

import io.github.qingchenw.microcontroller.R;
import io.github.qingchenw.microcontroller.device.SampleGattAttributes;
import io.github.qingchenw.microcontroller.Utils;

/**
 * Mini chat server
 *
 * @author wc
 */
// TODO 尚未重构
public class MCSFragment extends Fragment implements BluetoothAdapter.LeScanCallback {
    private BluetoothAdapter bluetoothAdapter;
    private BluetoothGatt bluetoothGatt;
    private int connectionStatus = BluetoothProfile.STATE_DISCONNECTED;

    private TextView textViewMessages;
    private EditText editTextMessage;

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getActivity() != null &&
                getActivity().getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE)) {
            BluetoothManager bluetoothManager =
                    (BluetoothManager) getActivity().getSystemService(Context.BLUETOOTH_SERVICE);
            bluetoothAdapter = bluetoothManager.getAdapter();
        } else {
            Utils.toast(getActivity(), "您的设备不支持低功耗蓝牙");
        }
    }

    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View root = inflater.inflate(R.layout.fragment_mcs, container, false);
        textViewMessages = root.findViewById(R.id.text_messages);
        editTextMessage = root.findViewById(R.id.editText_message);
        root.findViewById(R.id.button_connect).setOnClickListener(this::onClicked);
        root.findViewById(R.id.button_send).setOnClickListener(this::onClicked);
        return root;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        bluetoothAdapter.stopLeScan(this);
        bluetoothAdapter = null;
        if (bluetoothGatt != null) {
            bluetoothGatt.close();
            bluetoothGatt = null;
        }
    }

    public void onClicked(View view) {
        int id = view.getId();
        if (id == R.id.button_connect) {
            link();
        } else if (id == R.id.button_send) {
            if (editTextMessage.getText().length() > 0) {
                send(editTextMessage.getText());
                editTextMessage.getText().clear();
            }
        }
    }

    public void link() {
        if (connectionStatus == BluetoothProfile.STATE_DISCONNECTED) {
            if (bluetoothAdapter == null || !bluetoothAdapter.isEnabled()) {
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, 1);
                return;
            }
            bluetoothAdapter.stopLeScan(this);
            bluetoothAdapter.startLeScan(this);
        }
    }

    public void send(CharSequence text) {
        textViewMessages.append("\n发送: " + text);
        if (connectionStatus == BluetoothProfile.STATE_CONNECTED) {
            BluetoothGattService service = bluetoothGatt.getService(UUID.fromString(SampleGattAttributes.BLE_SERVICE));
            if (service != null) {
                BluetoothGattCharacteristic characteristic = service.getCharacteristic(
                        UUID.fromString(SampleGattAttributes.BLE_RX));
                if (characteristic != null) {
                    characteristic.setValue("" + text);
                    bluetoothGatt.writeCharacteristic(characteristic);
                }
            }
        } else {
            Utils.toast(getActivity(), "未连接至聊天服务器");
        }
    }

    @Override
    public void onLeScan(BluetoothDevice device, int rssi, byte[] scanRecord) {
        getActivity().runOnUiThread(new Runnable() {
            @Override
            public void run() {
                // TODO 先写死
                if (device.getAddress().equals("3C:61:05:11:DD:DE")) {
                    Utils.toast(getActivity(), "发现服务器, 正在连接");
                    bluetoothAdapter.stopLeScan(MCSFragment.this);
                    bluetoothGatt = device.connectGatt(getContext(), false, new BLECallback());
                }
            }
        });
    }

    private class BLECallback extends BluetoothGattCallback {

        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            connectionStatus = newState;
            if (newState == BluetoothProfile.STATE_CONNECTED) {
                if (gatt.discoverServices()) {
                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                        bluetoothGatt.requestMtu(500);
                    }
                }
            }
        }

        @Override
        public void onServicesDiscovered(BluetoothGatt gatt, int status) {
            BluetoothGattService service = gatt.getService(UUID.fromString(SampleGattAttributes.BLE_SERVICE));
            if (service != null) {
                getActivity().runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    Utils.toast(getActivity(), "设备发现运行完毕, 成功连接");
                                }
                            });
                BluetoothGattCharacteristic characteristic = service.getCharacteristic(
                        UUID.fromString(SampleGattAttributes.BLE_TX));
                if (characteristic != null) {
                    BluetoothGattDescriptor descriptor = characteristic.getDescriptor(
                            UUID.fromString(SampleGattAttributes.CLIENT_CHARACTERISTIC_CONFIG));
                    if (descriptor != null) {
                        descriptor.setValue(BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE);
                        gatt.writeDescriptor(descriptor);
                        gatt.setCharacteristicNotification(characteristic, true);
                    }
                }
            }
        }

        @Override
        public void onMtuChanged(BluetoothGatt gatt, int mtu, int status) {
            if (status == BluetoothGatt.GATT_SUCCESS) {
                getActivity().runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Utils.toast(getActivity(), "已设置MTU: " + mtu);
                    }
                });
            }
        }

        @Override
        public void onCharacteristicChanged(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic) {
            if (characteristic.getUuid().equals(UUID.fromString(SampleGattAttributes.BLE_TX))) {
                getActivity().runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        MCSFragment.this.textViewMessages.append("\n接收: " + characteristic.getStringValue(0));
                    }
                });
            }
        }
    }
}
