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
    private OnItemClickListener onClickListener;

    public DeviceListAdapter(List<IDevice> deviceList) {
        this.deviceList = deviceList;
    }

    public void setOnClickListener(OnItemClickListener listener) {
        this.onClickListener = listener;
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
        IDevice device = deviceList.get(position);
        holder.imageIcon.setImageResource(device.getIcon());
        if (device.getName() != null) {
            holder.textName.setText(device.getName());
        } else {
            holder.textName.setText(R.string.device_name_unknown);
        }
        holder.textAddress.setText(device.getAddress());
    }

    @Override
    public int getItemCount() {
        return deviceList.size();
    }

    public class DeviceViewHolder extends RecyclerView.ViewHolder implements View.OnClickListener {
        public final ImageView imageIcon;
        public final TextView textName;
        public final TextView textAddress;

        public DeviceViewHolder(View view) {
            super(view);
            view.setOnClickListener(this);
            imageIcon = view.findViewById(R.id.image_device_icon);
            textName = view.findViewById(R.id.text_device_name);
            textAddress = view.findViewById(R.id.text_device_address);
        }

        @Override
        public void onClick(View v) {
            onClickListener.onItemClick(v, getAdapterPosition());
        }
    }

    public interface OnItemClickListener {
        void onItemClick(View view, int position);
    }
}
