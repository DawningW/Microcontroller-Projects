package io.github.qingchenw.microcontroller;

import android.content.Context;
import android.widget.Toast;

import androidx.annotation.StringRes;

import okhttp3.OkHttpClient;

public final class Utils {
    static private OkHttpClient httpClient;

    private Utils() {}

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
}
