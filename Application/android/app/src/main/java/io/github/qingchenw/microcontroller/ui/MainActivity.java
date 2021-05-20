package io.github.qingchenw.microcontroller.ui;

import android.Manifest;
import android.app.Service;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.res.Configuration;
import android.os.Bundle;
import android.os.IBinder;
import android.view.Menu;
import android.view.MenuItem;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.fragment.app.Fragment;

import com.google.android.material.navigation.NavigationView;
import com.permissionx.guolindev.PermissionX;

import io.github.qingchenw.microcontroller.R;
import io.github.qingchenw.microcontroller.Utils;
import io.github.qingchenw.microcontroller.device.DeviceManager;
import io.github.qingchenw.microcontroller.device.IDevice;
import io.github.qingchenw.microcontroller.service.DeviceDiscoveryService;

/**
 * Main Activity
 *
 * @author wc
 */
// TODO 标题栏菜单加入扫一扫和NFC
public class MainActivity extends AppCompatActivity implements
        NavigationView.OnNavigationItemSelectedListener,
        DeviceDiscoveryService.ScanCallback, DeviceManager.OnChangedListener {
    private DrawerLayout drawer;
    private ActionBarDrawerToggle drawerToggle;

    private DeviceDiscoveryService service;
    private final ServiceConnection connection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder binder) {
            service = ((DeviceDiscoveryService.ServiceBinder) binder).getService();
            service.addScanCallback(MainActivity.this);
            service.startScan(30);
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            service.removeScanCallback(MainActivity.this);
            service = null;
        }
    };
    private DeviceManager deviceManager;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        drawer = findViewById(R.id.drawer_layout);
        drawerToggle = new ActionBarDrawerToggle(this,
                drawer, toolbar, R.string.drawer_open, R.string.drawer_close);
        drawer.addDrawerListener(drawerToggle);
        NavigationView navigationView = findViewById(R.id.navigation_view);
        navigationView.setNavigationItemSelectedListener(this);

        if (savedInstanceState == null) {
            navigationView.setCheckedItem(R.id.nav_home);
            switchFragment(HomeFragment.class);
        }
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
        deviceManager = DeviceManager.getInstance();
        deviceManager.addOnChangedListener(this);
        Intent intent = new Intent(this, DeviceDiscoveryService.class);
        bindService(intent, connection, Service.BIND_AUTO_CREATE);
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
        } else if (id == R.id.nav_mcs) {
            switchFragment(MCSFragment.class);
        } else if (id == R.id.nav_debug) {
            switchFragment(DebugFragment.class);
        } else if (id == R.id.nav_scan) {
            switchFragment(ScanFragment.class);
        }
        drawer.closeDrawers();
        return true;
    }

    @Override
    protected void onStop() {
        if (service != null) {
            unbindService(connection);
        }
        if (deviceManager != null) {
            deviceManager.removeOnChangedListener(this);
        }
        super.onStop();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    @Override
    public void onScanStart() {

    }

    @Override
    public void onDeviceScanned(IDevice device) {
        deviceManager.addDevice(device);
    }

    @Override
    public void onScanStop() {

    }

    @Override
    public void onDeviceChanged() {

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
