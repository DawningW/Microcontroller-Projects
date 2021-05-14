package io.github.qingchenw.microcontroller.ui;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import com.felhr.usbserial.PL2303SerialDevice;
import com.felhr.usbserial.SerialPortBuilder;
import com.felhr.usbserial.SerialPortCallback;
import com.felhr.usbserial.UsbSerialDevice;
import com.felhr.usbserial.UsbSerialInterface;

import java.util.List;

import io.github.qingchenw.microcontroller.R;

/*
* 用于调试串口,蓝牙,wifi
* 包含选择设备, 设置参数, 连接/断开连接, 日志框, 输入框(可能还需要设置以16进制还是文本形式发送), 发送按钮
*/
/*
 * Test
 *
 * @author wc
 **/
public class TestFragment extends Fragment {
    private UsbSerialDevice mSerialDevice;
    private TextView mTextView;

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        SerialPortBuilder builder = SerialPortBuilder.createSerialPortBuilder(new SerialPortCallback() {
            @Override
            public void onSerialPortsDetected(List<UsbSerialDevice> serialPorts) {
                for (UsbSerialDevice serialDevice : serialPorts) {
                    if (serialDevice instanceof PL2303SerialDevice)
                    {
                        serialDevice.read(new UsbSerialInterface.UsbReadCallback()
                                          {
                                              @Override
                                              public void onReceivedData(byte[] data)
                                              {
                                                  System.out.println(new String(data));
                                              }
                                          }
                        );
                        if (!serialDevice.isOpen())
                        {
                            serialDevice.open();
                        }
                        mSerialDevice = serialDevice;
                        System.out.println("Connect successfully.");
                        getActivity().runOnUiThread(new Runnable()
                        {
                            @Override
                            public void run()
                            {
                                mTextView.setText("已连接");
                            }
                        });
                    }
                }
            }
        });
        // Supported baud rates
        //300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600
        builder.openSerialPorts(getContext(), 9600,
                UsbSerialInterface.DATA_BITS_8,
                UsbSerialInterface.STOP_BITS_1,
                UsbSerialInterface.PARITY_NONE,
                UsbSerialInterface.FLOW_CONTROL_OFF);
    }

    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
    {
        View root = inflater.inflate(R.layout.fragment_test, container, false);
        mTextView = root.findViewById(R.id.text_status);

        Button buttonSend = root.findViewById(R.id.button_send);
        buttonSend.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View view)
            {
                if (mSerialDevice != null)
                {
                    mSerialDevice.getOutputStream().write(new byte[] {127});
                }
            }
        });

        Button buttonClear = root.findViewById(R.id.button_clear);
        buttonClear.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View view)
            {
                if (mSerialDevice != null)
                {
                    mSerialDevice.getOutputStream().write(new byte[] {0});
                }
            }
        });

        return root;
    }

    @Override
    public void onDestroy()
    {
        super.onDestroy();
        if (mSerialDevice != null)
        {
            mSerialDevice.close();
        }
    }

}
