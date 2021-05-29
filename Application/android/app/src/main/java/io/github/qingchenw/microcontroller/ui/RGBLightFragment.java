package io.github.qingchenw.microcontroller.ui;

import android.graphics.Color;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import com.skydoves.colorpickerview.listeners.ColorListener;

import io.github.qingchenw.microcontroller.R;
import io.github.qingchenw.microcontroller.databinding.FragmentRgblightBinding;
import io.github.qingchenw.microcontroller.viewmodel.RGBLightViewModel;

/**
 * RGBLight Fragment
 *
 * @author wc
 */
public class RGBLightFragment extends Fragment implements View.OnClickListener {
    private FragmentRgblightBinding viewBinding;
    private RGBLightViewModel rgbLightViewModel;

    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        viewBinding = FragmentRgblightBinding.inflate(inflater, container, false);
        viewBinding.buttonConstant.setOnClickListener(this);
        viewBinding.buttonBlink.setOnClickListener(this);
        viewBinding.buttonBreath.setOnClickListener(this);
        viewBinding.buttonChase.setOnClickListener(this);
        viewBinding.buttonRainbow.setOnClickListener(this);
        viewBinding.buttonStream.setOnClickListener(this);
        viewBinding.buttonAnimation.setOnClickListener(this);
        viewBinding.buttonMusic.setOnClickListener(this);
        viewBinding.buttonCustom.setOnClickListener(this);
        // viewBinding.colorPicker.setHsvPaletteDrawable();
        viewBinding.colorPicker.setLifecycleOwner(this);
        viewBinding.colorPicker.setColorListener((ColorListener) (color, fromUser) -> {
            if (fromUser) {
                // int a = Color.alpha(color);
                int r = Color.red(color);
                int g = Color.green(color);
                int b = Color.blue(color);
                String colorStr = String.format("#%02x%02x%02x", r, g, b);
                rgbLightViewModel.setColor(colorStr);
            }
        });
        return viewBinding.getRoot();
    }

    @Override
    public void onViewCreated(@NonNull View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        rgbLightViewModel = new ViewModelProvider(this).get(RGBLightViewModel.class);
        rgbLightViewModel.isConnected().observe(getViewLifecycleOwner(), isConnected -> {
            // TODO 禁用或者启用
        });
        rgbLightViewModel.getLightMode().observe(getViewLifecycleOwner(), lightMode -> {
            // TODO 选中按钮
        });
        rgbLightViewModel.getLightColor().observe(getViewLifecycleOwner(), lightColor -> {
            int color = 0xFFFFFF;
            try {
                color = Integer.parseInt(lightColor.substring(1), 16);
            } catch (NumberFormatException ignored) {}
            try {
                viewBinding.colorPicker.selectByHsvColor(color);
            } catch (IllegalAccessException e) {
                e.printStackTrace();
            }
        });
    }

    @Override
    public void onClick(View v) {
        int id = v.getId();
        if (id == R.id.button_constant) {
            rgbLightViewModel.setMode(RGBLightViewModel.LightMode.CONSTANT);
        } else if (id == R.id.button_blink) {
            rgbLightViewModel.setMode(RGBLightViewModel.LightMode.BLINK);
        } else if (id == R.id.button_breath) {
            rgbLightViewModel.setMode(RGBLightViewModel.LightMode.BREATH);
        } else if (id == R.id.button_chase) {
            rgbLightViewModel.setMode(RGBLightViewModel.LightMode.CHASE);
        } else if (id == R.id.button_rainbow) {
            rgbLightViewModel.setMode(RGBLightViewModel.LightMode.RAINBOW);
        } else if (id == R.id.button_stream) {
            rgbLightViewModel.setMode(RGBLightViewModel.LightMode.STREAM);
        } else if (id == R.id.button_animation) {
            rgbLightViewModel.setMode(RGBLightViewModel.LightMode.ANIMATION);
        } else if (id == R.id.button_music) {
            rgbLightViewModel.setMode(RGBLightViewModel.LightMode.MUSIC);
        } else if (id == R.id.button_custom) {
            rgbLightViewModel.setMode(RGBLightViewModel.LightMode.CUSTOM);
        }
    }
}
