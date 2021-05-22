package io.github.qingchenw.microcontroller.ui.adapter;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.List;

import io.github.qingchenw.microcontroller.R;
import io.github.qingchenw.microcontroller.device.IDevice;

public class DeviceListAdapter extends RecyclerView.Adapter<DeviceListAdapter.DeviceViewHolder> {
    private final List<IDevice> deviceList;

    public DeviceListAdapter(List<IDevice> deviceList) {
        this.deviceList = deviceList;
    }

    @NonNull
    @Override
    public DeviceViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.item_device, parent, false);
        return new DeviceViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull DeviceViewHolder holder, int position) {
        holder.getImageViewIcon().setImageResource(deviceList.get(position).getIcon());
        holder.getTextViewName().setText(deviceList.get(position).getName());
        holder.getTextViewAddress().setText(deviceList.get(position).getAddress());
    }

    @Override
    public int getItemCount() {
        return deviceList.size();
    }

    public static class DeviceViewHolder extends RecyclerView.ViewHolder {
        private final ImageView imageIcon;
        private final TextView textName;
        private final TextView textAddress;

        public DeviceViewHolder(View view) {
            super(view);
            imageIcon = view.findViewById(R.id.image_device_icon);
            textName = view.findViewById(R.id.text_device_name);
            textAddress = view.findViewById(R.id.text_device_address);
        }

        public ImageView getImageViewIcon() {
            return imageIcon;
        }

        public TextView getTextViewName() {
            return textName;
        }

        public TextView getTextViewAddress() {
            return textAddress;
        }
    }
}
