package io.github.qingchenw.microcontroller.ui;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;
import androidx.lifecycle.ViewModelProvider.AndroidViewModelFactory;
import androidx.recyclerview.widget.LinearLayoutManager;

import io.github.qingchenw.microcontroller.R;
import io.github.qingchenw.microcontroller.Utils;
import io.github.qingchenw.microcontroller.databinding.FragmentScanBinding;
import io.github.qingchenw.microcontroller.device.IDevice;
import io.github.qingchenw.microcontroller.ui.adapter.DeviceListAdapter;
import io.github.qingchenw.microcontroller.viewmodel.DeviceViewModel;

/**
 * Scan devices Fragment
 *
 * @author wc
 */
public class ScanFragment extends Fragment implements DeviceListAdapter.OnItemClickListener {
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
        adapter.setOnClickListener(this);
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

    @Override
    public void onItemClick(View view, int position) {
        IDevice device = deviceViewModel.getDevices().getValue().get(position);
        if (device.getName() == null) {
            Utils.toast(getContext(), "暂不支持此设备");
            return;
        }
        LayoutInflater inflater = requireActivity().getLayoutInflater();
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        View root = inflater.inflate(R.layout.dialog_connect_device, null);
        ((TextView) root.findViewById(R.id.text_device_name)).setText(String.format(getString(R.string.device_name), device.getName() != null ? device.getName() : getString(R.string.device_name_unknown)));
        ((TextView) root.findViewById(R.id.text_device_address)).setText(String.format(getString(R.string.device_address), device.getAddress()));
        ((TextView) root.findViewById(R.id.text_device_id)).setText(String.format(getString(R.string.device_id), device.getID() != null ? device.getID() : getString(R.string.device_id_unknown)));
        ((TextView) root.findViewById(R.id.text_device_model)).setText(String.format(getString(R.string.device_model), device.getModel() != null ? device.getModel() : getString(R.string.device_model_unknown)));
        ((TextView) root.findViewById(R.id.text_device_version)).setText(String.format(getString(R.string.device_version), device.getVersion() != null ? device.getVersion() : getString(R.string.device_version_unknown)));
        ((TextView) root.findViewById(R.id.text_device_producer)).setText(String.format(getString(R.string.device_producer), device.getProducer() != null ? device.getProducer() : getString(R.string.device_producer_unknown)));
        builder.setTitle(R.string.dialog_connect_device)
                .setView(root)
                .setPositiveButton("连接", (dialog, id) -> {
                    Utils.toast(getContext(), "添加设备尚未实现");
                    // 添加设备
                })
                .setNegativeButton(android.R.string.cancel, null);
        builder.show();
    }
}
