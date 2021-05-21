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
import io.github.qingchenw.microcontroller.databinding.FragmentScanBinding;
import io.github.qingchenw.microcontroller.viewmodel.DeviceViewModel;

/**
 * Scan devices Fragment
 *
 * @author wc
 */
// TODO 搜索不到设备的时候显示点提示
public class ScanFragment extends Fragment {
    private DeviceViewModel deviceViewModel;
    private FragmentScanBinding viewBinding;

    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        viewBinding = FragmentScanBinding.inflate(inflater, container, false);
        viewBinding.swipeLayout.setColorSchemeResources(R.color.colorPrimary);
        viewBinding.swipeLayout.setOnRefreshListener(() -> {
            deviceViewModel.startScan();
        });
        return viewBinding.getRoot();
    }

    @Override
    public void onViewCreated(@NonNull View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        AndroidViewModelFactory factory = AndroidViewModelFactory.getInstance(requireActivity().getApplication());
        deviceViewModel = new ViewModelProvider(requireActivity(), factory).get(DeviceViewModel.class);
        deviceViewModel.getScanState().observe(getViewLifecycleOwner(), state -> {
            viewBinding.swipeLayout.setRefreshing(state == DeviceViewModel.ScanState.SCANNING);
        });
    }
}
