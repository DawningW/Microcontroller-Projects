package io.github.qingchenw.microcontroller.ui;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;
import androidx.lifecycle.ViewModelProvider.AndroidViewModelFactory;
import androidx.recyclerview.widget.RecyclerView;
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout;

import io.github.qingchenw.microcontroller.R;
import io.github.qingchenw.microcontroller.viewmodel.DeviceViewModel;

/**
 * Scan devices
 *
 * @author wc
 */
public class ScanFragment extends Fragment {
    private DeviceViewModel deviceViewModel;

    private SwipeRefreshLayout swipeLayout;
    private RecyclerView recyclerView;

    public ScanFragment() {}

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        AndroidViewModelFactory factory = AndroidViewModelFactory.getInstance(requireActivity().getApplication());
        deviceViewModel = new ViewModelProvider(requireActivity(), factory).get(DeviceViewModel.class);

        View root = inflater.inflate(R.layout.fragment_scan, container, false);
        swipeLayout = root.findViewById(R.id.swipeLayout);
        swipeLayout.setColorSchemeResources(R.color.colorPrimary);
        // swipeLayout.setProgressBackgroundColorSchemeColor();
        swipeLayout.setOnRefreshListener(() -> {
            deviceViewModel.startScan();
        });
        deviceViewModel.getScanState().observe(getViewLifecycleOwner(), state -> {
            swipeLayout.setRefreshing(state == DeviceViewModel.ScanState.SCANNING);
        });
        recyclerView = root.findViewById(R.id.list_devices);
        return root;
    }
}
