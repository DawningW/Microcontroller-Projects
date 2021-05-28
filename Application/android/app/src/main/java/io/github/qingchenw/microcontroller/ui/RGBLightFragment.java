package io.github.qingchenw.microcontroller.ui;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import com.skydoves.colorpickerview.ColorEnvelope;
import com.skydoves.colorpickerview.listeners.ColorEnvelopeListener;

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
    private FragmentRgblightBinding viewBinding;

    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        viewBinding = FragmentRgblightBinding.inflate(inflater, container, false);
        viewBinding.colorPicker.setLifecycleOwner(this);
        viewBinding.colorPicker.setColorListener((ColorEnvelopeListener) (envelope, fromUser) -> {
            if (fromUser) {
                // TODO
            }
        });
        return viewBinding.getRoot();
    }

    @Override
    public void onViewCreated(@NonNull View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
    }
}
