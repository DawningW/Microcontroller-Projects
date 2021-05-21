package io.github.qingchenw.microcontroller.ui;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import io.github.qingchenw.microcontroller.R;
import io.github.qingchenw.microcontroller.databinding.FragmentRgblightBinding;
import io.github.qingchenw.microcontroller.databinding.FragmentScanBinding;
import io.github.qingchenw.microcontroller.viewmodel.DeviceViewModel;

/**
 * RGBLight Fragment
 *
 * @author wc
 */
public class RGBLightFragment extends Fragment {
    private DeviceViewModel deviceViewModel;
    private FragmentRgblightBinding viewBinding;

    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        viewBinding = FragmentRgblightBinding.inflate(inflater, container, false);

        return viewBinding.getRoot();
    }
}
