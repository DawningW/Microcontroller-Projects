package io.github.qingchenw.microcontroller.ui;

import android.Manifest;
import android.content.res.Configuration;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;
import androidx.lifecycle.ViewModelProvider.AndroidViewModelFactory;

import com.google.android.material.navigation.NavigationView;
import com.permissionx.guolindev.PermissionX;

import io.github.qingchenw.microcontroller.R;
import io.github.qingchenw.microcontroller.Utils;
import io.github.qingchenw.microcontroller.databinding.ActivityMainBinding;
import io.github.qingchenw.microcontroller.viewmodel.DeviceViewModel;

/**
 * Main Activity
 *
 * @author wc
 */
// TODO 标题栏菜单加入扫一扫和NFC
public class MainActivity extends AppCompatActivity implements
        NavigationView.OnNavigationItemSelectedListener {
    private ActivityMainBinding viewBinding;
    private ActionBarDrawerToggle drawerToggle;
    private DeviceViewModel deviceViewModel;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        viewBinding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(viewBinding.getRoot());
        setSupportActionBar(viewBinding.toolbar);
        drawerToggle = new ActionBarDrawerToggle(this,
                viewBinding.drawerLayout, viewBinding.toolbar,
                R.string.drawer_open, R.string.drawer_close);
        viewBinding.drawerLayout.addDrawerListener(drawerToggle);
        NavigationView navigationView = findViewById(R.id.navigation_view);
        navigationView.setNavigationItemSelectedListener(this);
        if (savedInstanceState == null) {
            navigationView.setCheckedItem(R.id.nav_home);
            switchFragment(HomeFragment.class);
        }

        AndroidViewModelFactory factory = AndroidViewModelFactory.getInstance(getApplication());
        deviceViewModel = new ViewModelProvider(this, factory).get(DeviceViewModel.class);
        deviceViewModel.getDevices().observe(this, devices -> {

        });
    }

    @Override
    protected void onPostCreate(@Nullable Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);
        drawerToggle.syncState();

        // TODO 先在这申请权限, 以后再说
        findViewById(R.id.toolbar).postDelayed(this::requestPermissions, 1000);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        return false;
    }

    @Override
    protected void onStart() {
        super.onStart();
    }

    @Override
    public void onConfigurationChanged(@NonNull Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        drawerToggle.onConfigurationChanged(newConfig);
    }

    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {
        if (drawerToggle.onOptionsItemSelected(item)) return true;
        return super.onOptionsItemSelected(item);
    }

    @Override
    public boolean onNavigationItemSelected(@NonNull MenuItem item) {
        int id = item.getItemId();
        if (id == R.id.nav_home) {
            switchFragment(HomeFragment.class);
        } else if (id == R.id.nav_rgblight) {
            switchFragment(RGBLightFragment.class);
        } else if (id == R.id.nav_mcs) {
            switchFragment(MCSFragment.class);
        } else if (id == R.id.nav_debug) {
            switchFragment(DebugFragment.class);
        } else if (id == R.id.nav_scan) {
            switchFragment(ScanFragment.class);
        }
        viewBinding.drawerLayout.closeDrawers();
        return true;
    }

    @Override
    protected void onStop() {
        super.onStop();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    private void switchFragment(Class<? extends Fragment> clazz) {
        getSupportFragmentManager()
                .beginTransaction()
                .replace(R.id.fragment_container, clazz, null)
                .commit();
    }

    private void requestPermissions() {
        PermissionX.init(this)
                .permissions(Manifest.permission.BLUETOOTH, Manifest.permission.ACCESS_FINE_LOCATION)
                .explainReasonBeforeRequest()
                .onExplainRequestReason((scope, deniedList) -> scope.showRequestReasonDialog(deniedList, getString(R.string.permission_reason), getString(android.R.string.ok)))
                .onForwardToSettings((scope, deniedList) -> scope.showForwardToSettingsDialog(deniedList, getString(R.string.permission_failed), getString(android.R.string.ok)))
                .request((allGranted, grantedList, deniedList) -> {
                    if (allGranted) {
                        Utils.toast(MainActivity.this, getString(R.string.permission_ok));
                    }
                });
    }
}
