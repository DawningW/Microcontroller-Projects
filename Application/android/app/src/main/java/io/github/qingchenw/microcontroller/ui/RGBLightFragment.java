package io.github.qingchenw.microcontroller.ui;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import io.github.qingchenw.microcontroller.R;

/**
 * RGBLight
 *
 * @author wc
 */
public class RGBLightFragment extends Fragment {

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View root = inflater.inflate(R.layout.fragment_rgblight, container, false);
        return root;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }
}
