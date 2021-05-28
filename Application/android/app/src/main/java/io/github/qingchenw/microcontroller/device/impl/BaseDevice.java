package io.github.qingchenw.microcontroller.device.impl;

import io.github.qingchenw.microcontroller.MCUApplication;
import io.github.qingchenw.microcontroller.device.IDevice;

public abstract class BaseDevice implements IDevice {
    private String name;
    private String id;
    private String model;
    private String version;
    private String producer;

    @Override
    public String getName() {
        return this.name;
    }

    @Override
    public void setName(String name) {
        this.name = name;
    }

    @Override
    public String getID() {
        return this.id;
    }

    @Override
    public void setID(String id) {
        this.id = id;
    }

    @Override
    public String getModel() {
        return this.model;
    }

    @Override
    public void setModel(String model) {
        this.model = model;
    }

    @Override
    public String getVersion() {
        return this.version;
    }

    @Override
    public void setVersion(String version) {
        this.version = version;
    }

    @Override
    public String getProducer() {
        return this.producer;
    }

    @Override
    public void setProducer(String producer) {
        this.producer = producer;
    }

    protected void notifyRemove() {
        MCUApplication.getInstance().getDeviceService().removeDevice(this);
    }
}
