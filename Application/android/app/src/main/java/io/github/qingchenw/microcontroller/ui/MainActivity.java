package io.github.qingchenw.microcontroller.ui;

import android.Manifest;
import android.os.Bundle;
import android.os.Handler;
import android.view.Menu;
import android.view.MenuItem;

import androidx.annotation.NonNull;
import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.view.menu.MenuBuilder;
import androidx.lifecycle.ViewModelProvider;
import androidx.lifecycle.ViewModelProvider.AndroidViewModelFactory;
import androidx.navigation.NavController;
import androidx.navigation.fragment.NavHostFragment;
import androidx.navigation.ui.NavigationUI;

import com.hjq.permissions.OnPermissionCallback;
import com.hjq.permissions.XXPermissions;

import java.lang.reflect.Method;
import java.util.List;

import io.github.qingchenw.microcontroller.R;
import io.github.qingchenw.microcontroller.Utils;
import io.github.qingchenw.microcontroller.databinding.ActivityMainBinding;
import io.github.qingchenw.microcontroller.viewmodel.DeviceViewModel;

/**
 * Main Activity
 *
 * @author wc
 */
public class MainActivity extends AppCompatActivity {
    private ActivityMainBinding viewBinding;
    private ActionBarDrawerToggle drawerToggle;
    private NavController navController;
    private DeviceViewModel deviceViewModel;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        viewBinding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(viewBinding.getRoot());
        setSupportActionBar(viewBinding.toolbar);
        NavHostFragment navHostFragment = (NavHostFragment) getSupportFragmentManager()
                .findFragmentById(R.id.fragment_container);
        navController = navHostFragment.getNavController();
        NavigationUI.setupWithNavController(viewBinding.toolbar, navController, viewBinding.drawerLayout);
        NavigationUI.setupWithNavController(viewBinding.navigationView, navController);

        AndroidViewModelFactory factory = AndroidViewModelFactory.getInstance(getApplication());
        deviceViewModel = new ViewModelProvider(this, factory).get(DeviceViewModel.class);
        deviceViewModel.getDevices().observe(this, devices -> {

        });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_main, menu);
        if (menu instanceof MenuBuilder) {
            try {
                Class<?> clazz = Class.forName("androidx.appcompat.view.menu.MenuBuilder");
                Method m = clazz.getDeclaredMethod("setOptionalIconsVisible", boolean.class);
                m.setAccessible(true);
                m.invoke(menu, true);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return true;
    }

    @Override
    protected void onStart() {
        super.onStart();
        new Handler().postDelayed(this::requestPermissions, 1000);
    }

    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {
        if (item.getItemId() == R.id.action_qrcode) {
            Utils.toast(this, "扫什么二维码, 我咕咕咕了");
        } else if (item.getItemId() == R.id.action_nfc) {
            Utils.toast(this, "碰什么nfc, 我咕咕咕了");
        } else {
            return super.onOptionsItemSelected(item);
        }
        return true;
    }

    private void requestPermissions() {
        Utils.toast(MainActivity.this, getString(R.string.permission_reason));
        XXPermissions.with(this)
                .permission(new String[]{Manifest.permission.ACCESS_FINE_LOCATION})
                .request(new OnPermissionCallback() {
                    @Override
                    public void onGranted(List<String> permissions, boolean all) {
                        if (all) {
                            Utils.toast(MainActivity.this, getString(R.string.permission_ok));
                        }
                    }

                    @Override
                    public void onDenied(List<String> permissions, boolean never) {
                        if (never) {
                            Utils.toast(MainActivity.this, getString(R.string.permission_failed));
                            XXPermissions.startPermissionActivity(MainActivity.this, permissions);
                        } else {
                            requestPermissions();
                        }
                    }
                });
    }
}
