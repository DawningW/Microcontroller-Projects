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
import com.permissionx.guolindev.callback.ForwardToSettingsCallback;
import com.permissionx.guolindev.callback.RequestCallback;
import com.permissionx.guolindev.request.ForwardScope;

import java.util.ArrayList;
import java.util.List;

import io.github.qingchenw.microcontroller.R;
import io.github.qingchenw.microcontroller.Utils;
import io.github.qingchenw.microcontroller.device.IDevice;
import io.github.qingchenw.microcontroller.service.DeviceDiscoveryService;

/*
* Main Activity
*
* @author wc
**/
// TODO 标题栏菜单加入扫一扫和NFC
public class MainActivity extends AppCompatActivity implements
        NavigationView.OnNavigationItemSelectedListener, DeviceDiscoveryService.ScanCallback {
    private DrawerLayout drawer;
    private ActionBarDrawerToggle drawerToggle;

    private DeviceDiscoveryService service;
    private final ServiceConnection connection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder binder) {
            service = ((DeviceDiscoveryService.ServiceBinder) binder).getService();
            service.addScanCallback(MainActivity.this);
            // TODO 先在这开始搜索, 以后再说
            service.startScan(0);
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            service.removeScanCallback(MainActivity.this);
            service = null;
        }
    };

    protected List<IDevice> devices = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
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

        Intent intent = new Intent(this, DeviceDiscoveryService.class);
        bindService(intent, connection, Service.BIND_AUTO_CREATE);

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
        } else if (id == R.id.nav_test) {
            switchFragment(DebugFragment.class);
        } else if (id == R.id.nav_scan) {
            switchFragment(ScanFragment.class);
        }
        drawer.closeDrawers();
        return true;
    }

    @Override
    protected void onDestroy() {
        if (service != null) {
            unbindService(connection);
        }
        super.onDestroy();
    }

    @Override
    public void onScanStart() {

    }

    @Override
    public void onDeviceScanned(IDevice device) {
        devices.add(device);
    }

    @Override
    public void onScanStop() {

    }

    public int getDeviceCount() {
        return devices.size();
    }

    public int getConnectedDeviceCount() {
        int count = 0;
        for (IDevice device : devices) {
            if (device.isConnected()) count++;
        }
        return count;
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
                .onForwardToSettings(new ForwardToSettingsCallback() {
                    @Override
                    public void onForwardToSettings(ForwardScope scope, List<String> deniedList) {
                        scope.showForwardToSettingsDialog(deniedList, getString(R.string.permission_failed), getString(android.R.string.ok));
                    }
                })
                .request(new RequestCallback() {
                    @Override
                    public void onResult(boolean allGranted, List<String> grantedList, List<String> deniedList) {
                        if (allGranted) {
                            Utils.toast(MainActivity.this, getString(R.string.permission_ok));
                        }
                    }
                });
    }
}
