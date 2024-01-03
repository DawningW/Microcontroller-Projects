package io.github.qingchenw.microcontroller.ui;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;
import androidx.lifecycle.ViewModelProvider.AndroidViewModelFactory;

import io.github.qingchenw.microcontroller.R;
import io.github.qingchenw.microcontroller.databinding.FragmentHomeBinding;
import io.github.qingchenw.microcontroller.device.DeviceManager;
import io.github.qingchenw.microcontroller.viewmodel.DeviceViewModel;

/**
 * Home Fragment
 *
 * @author wc
 */
public class HomeFragment extends Fragment {
    private FragmentHomeBinding viewBinding;
    private DeviceViewModel deviceViewModel;

    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        viewBinding = FragmentHomeBinding.inflate(inflater, container, false);
        viewBinding.textUserTitle.setText(String.format(getString(R.string.home_hello1), "wc"));
        viewBinding.textUserBroadcast.setText("沉舟侧畔千帆过，病树前头万木春。");
        return viewBinding.getRoot();
    }

    @Override
    public void onViewCreated(@NonNull View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        AndroidViewModelFactory factory = AndroidViewModelFactory.getInstance(requireActivity().getApplication());
        deviceViewModel = new ViewModelProvider(requireActivity(), factory).get(DeviceViewModel.class);
        deviceViewModel.getDevices().observe(getViewLifecycleOwner(), devices -> {
            int all = deviceViewModel.getDeviceCount();
            int connected = DeviceManager.getInstance().getDeviceCount();
            viewBinding.textDevicesCount.setText(String.format(getString(R.string.home_devices_count),
                    connected, all));
            viewBinding.textDevicesFind.setText(String.format(getString(R.string.home_devices_find),
                    all - connected));
        });
    }
}
