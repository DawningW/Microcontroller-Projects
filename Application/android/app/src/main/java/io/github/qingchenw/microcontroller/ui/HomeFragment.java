package io.github.qingchenw.microcontroller.ui;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import io.github.qingchenw.microcontroller.R;

/*
* Home
*
* @author wc
**/
public class HomeFragment extends Fragment {

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View root = inflater.inflate(R.layout.fragment_home, container, false);
        TextView textUserTitle = root.findViewById(R.id.text_user_title);
        textUserTitle.setText(String.format(getString(R.string.home_hello1), "wc"));
        TextView textUserBroadcast = root.findViewById(R.id.text_user_broadcast);
        textUserBroadcast.setText("沉舟侧畔千帆过，病树前头万木春。");
        return root;
    }
}
