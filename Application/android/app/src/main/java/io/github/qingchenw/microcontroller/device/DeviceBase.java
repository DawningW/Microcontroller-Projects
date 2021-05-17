package io.github.qingchenw.microcontroller.device;

public abstract class DeviceBase implements IDevice {
    String name;
    String id;
    String model;
    String version;
    String producer;

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
}
