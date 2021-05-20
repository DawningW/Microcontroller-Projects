package io.github.qingchenw.microcontroller.ui;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;
import androidx.lifecycle.ViewModelProvider.AndroidViewModelFactory;

import io.github.qingchenw.microcontroller.R;
import io.github.qingchenw.microcontroller.viewmodel.DeviceViewModel;

/**
 * Home
 *
 * @author wc
 */
public class HomeFragment extends Fragment {
    private DeviceViewModel deviceViewModel;

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        AndroidViewModelFactory factory = AndroidViewModelFactory.getInstance(requireActivity().getApplication());
        deviceViewModel = new ViewModelProvider(requireActivity(), factory).get(DeviceViewModel.class);

        View root = inflater.inflate(R.layout.fragment_home, container, false);
        TextView textUserTitle = root.findViewById(R.id.text_user_title);
        textUserTitle.setText(String.format(getString(R.string.home_hello1), "wc"));
        TextView textUserBroadcast = root.findViewById(R.id.text_user_broadcast);
        textUserBroadcast.setText("沉舟侧畔千帆过，病树前头万木春。");

        TextView textDeviceCount = root.findViewById(R.id.text_devices_count);
        TextView textDeviceFind = root.findViewById(R.id.text_devices_find);
        deviceViewModel.getDevices().observe(getViewLifecycleOwner(), devices -> {
            int all = deviceViewModel.getDeviceCount();
            int connected = deviceViewModel.getConnectedDeviceCount();
            textDeviceCount.setText(String.format(getString(R.string.home_devices_count),
                    connected, all));
            textDeviceFind.setText(String.format(getString(R.string.home_devices_find),
                    all - connected));
        });
        return root;
    }
}
