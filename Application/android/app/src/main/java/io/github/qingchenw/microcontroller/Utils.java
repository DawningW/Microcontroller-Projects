package io.github.qingchenw.microcontroller;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.widget.Toast;

import androidx.annotation.StringRes;

import okhttp3.OkHttpClient;

public final class Utils {
    static private OkHttpClient httpClient;

    private Utils() {}

    public static boolean isWifiConnected(Context context) {
        ConnectivityManager manager = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo networkInfo = manager.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
        return networkInfo.isConnected();
    }

    public static OkHttpClient getHttpClient() {
        if (httpClient == null) {
            httpClient = new OkHttpClient();
        }
        return httpClient;
    }

    public static void toast(Context context, String text) {
        Toast.makeText(context, text, text.length() > 15 ? Toast.LENGTH_LONG : Toast.LENGTH_SHORT).show();
    }

    public static void toast(Context context, @StringRes int resId) {
        toast(context, context.getResources().getString(resId));
    }

    public static boolean stringsAreEqualled(String s1, String s2) {
        return s1 != null && s1.equals(s2);
    }
}
