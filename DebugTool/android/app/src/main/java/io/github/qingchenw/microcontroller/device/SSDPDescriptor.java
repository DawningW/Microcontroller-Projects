package io.github.qingchenw.microcontroller.device;

import androidx.annotation.NonNull;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;
import org.xmlpull.v1.XmlPullParserFactory;

import java.io.IOException;
import java.io.StringReader;

public class SSDPDescriptor {
    private static final String LINE_END = "\r\n";

    private String URLBase;
    private String deviceType;
    private String friendlyName;
    private String presentationURL;
    private String serialNumber;
    private String modelName;
    private String modelNumber;
    private String modelURL;
    private String manufacturer;
    private String manufacturerURL;
    private String UDN;

    public String getDeviceType() {
        return deviceType;
    }

    public String getFriendlyName() {
        return friendlyName;
    }

    public String getPresentationURL() {
        return presentationURL;
    }

    public String getSerialNumber() {
        return serialNumber;
    }

    public String getModelName() {
        return modelName;
    }

    public String getModelNumber() {
        return modelNumber;
    }

    public String getModelURL() {
        return modelURL;
    }

    public String getManufacturer() {
        return manufacturer;
    }

    public String getManufacturerURL() {
        return manufacturerURL;
    }

    public String getUDN() {
        return UDN;
    }

    public String getURLBase() {
        return URLBase;
    }

    @NonNull
    @Override
    public String toString() {
        return "SSDPDescriptor{URLBase=" + URLBase + LINE_END +
                "DeviceType=" + deviceType + LINE_END +
                "FriendlyName=" + friendlyName + LINE_END +
                "PresentationURL=" + presentationURL + LINE_END +
                "SerialNumber=" + serialNumber + LINE_END +
                "ModelName=" + modelName + LINE_END +
                "ModelNumber=" + modelNumber + LINE_END +
                "ModelURL=" + modelURL + LINE_END +
                "Manufacturer=" + manufacturer + LINE_END +
                "ManufacturerURL=" + manufacturerURL + LINE_END +
                "UDN=" + UDN + "}";
    }

    public static SSDPDescriptor parse(String xml) {
        SSDPDescriptor descriptor = new SSDPDescriptor();
        try {
            XmlPullParser xmlParser = XmlPullParserFactory.newInstance().newPullParser();
            xmlParser.setInput(new StringReader(xml));
            int event = xmlParser.getEventType();
            while (event != XmlPullParser.END_DOCUMENT) {
                String name = xmlParser.getName();
                switch (event) {
                    case XmlPullParser.START_DOCUMENT:
                        break;
                    case XmlPullParser.START_TAG:
                        if ("URLBase".equals(name)) {
                            descriptor.URLBase = xmlParser.nextText();
                        } else if("friendlyName".equals(name)) {
                            descriptor.friendlyName = xmlParser.nextText();
                        } else if("deviceType".equals(name)) {
                            descriptor.deviceType = xmlParser.nextText();
                        } else if("presentationURL".equals(name)) {
                            descriptor.presentationURL = xmlParser.nextText();
                        } else if("serialNumber".equals(name)) {
                            descriptor.serialNumber = xmlParser.nextText();
                        } else if("modelName".equals(name)) {
                            descriptor.modelName = xmlParser.nextText();
                        } else if("modelNumber".equals(name)) {
                            descriptor.modelNumber = xmlParser.nextText();
                        } else if("modelURL".equals(name)) {
                            descriptor.modelURL = xmlParser.nextText();
                        } else if("manufacturer".equals(name)) {
                            descriptor.manufacturer = xmlParser.nextText();
                        } else if("manufacturerURL".equals(name)) {
                            descriptor.manufacturerURL = xmlParser.nextText();
                        } else if("UDN".equals(name)) {
                            descriptor.UDN = xmlParser.nextText();
                        }
                        break;
                }
                event = xmlParser.next();
            }
        } catch (XmlPullParserException | IOException e) {
            e.printStackTrace();
        }
        return descriptor;
    }
}
