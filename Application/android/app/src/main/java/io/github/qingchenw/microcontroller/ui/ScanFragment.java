package io.github.qingchenw.microcontroller.ui;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;
import androidx.lifecycle.ViewModelProvider.AndroidViewModelFactory;
import androidx.recyclerview.widget.LinearLayoutManager;

import io.github.qingchenw.microcontroller.R;
import io.github.qingchenw.microcontroller.databinding.FragmentScanBinding;
import io.github.qingchenw.microcontroller.ui.adapter.DeviceListAdapter;
import io.github.qingchenw.microcontroller.viewmodel.DeviceViewModel;

/**
 * Scan devices Fragment
 *
 * @author wc
 */
public class ScanFragment extends Fragment {
    private DeviceViewModel deviceViewModel;
    private FragmentScanBinding viewBinding;
    private DeviceListAdapter adapter;

    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        viewBinding = FragmentScanBinding.inflate(inflater, container, false);
        viewBinding.swipeLayout.setColorSchemeResources(R.color.colorPrimary);
        viewBinding.swipeLayout.setOnRefreshListener(() -> {
            deviceViewModel.startScan();
        });
        LinearLayoutManager layoutManager = new LinearLayoutManager(getContext());
        viewBinding.listDevices.setLayoutManager(layoutManager);
        return viewBinding.getRoot();
    }

    @Override
    public void onViewCreated(@NonNull View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        AndroidViewModelFactory factory = AndroidViewModelFactory.getInstance(requireActivity().getApplication());
        deviceViewModel = new ViewModelProvider(requireActivity(), factory).get(DeviceViewModel.class);
        adapter = new DeviceListAdapter(deviceViewModel.getDevices().getValue());
        viewBinding.listDevices.setAdapter(adapter);
        deviceViewModel.getScanState().observe(getViewLifecycleOwner(), state -> {
            viewBinding.swipeLayout.setRefreshing(state == DeviceViewModel.ScanState.SCANNING);
        });
        deviceViewModel.getDevices().observe(getViewLifecycleOwner(), devices -> {
            if (!devices.isEmpty()) {
                viewBinding.textNoDevices.setVisibility(View.GONE);
            } else {
                viewBinding.textNoDevices.setVisibility(View.VISIBLE);
            }
            adapter.notifyDataSetChanged();
        });
    }
}
