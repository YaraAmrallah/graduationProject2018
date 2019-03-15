package com.example6767gh.mytestauthentication;

import android.net.Uri;

/**
 * Created by Dana on 2/15/2019.
 */

public class Cars {

    private String color;
    private String type;
    private String number;
    private String longitude;
    private String latitude;
    private String image;


    public Cars() {
    }


    public Cars(String color, String type, String number, String longitude, String latitude, String image) {
        this.color = color;
        this.type = type;
        this.number = number;
        this.longitude = longitude;
        this.latitude = latitude;
        this.image = image;
    }

    public String getColor() {
        return color;
    }

    public void setColor(String color) {
        this.color = color;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getNumber() {
        return number;
    }

    public void setNumber(String number) {
        this.number = number;
    }

    public String getLongitude() {
        return longitude;
    }

    public void setLongitude(String longitude) {
        this.longitude = longitude;
    }

    public String getLatitude() {
        return latitude;
    }

    public void setLatitude(String latitude) {
        this.latitude = latitude;
    }

    public String getImage() {
        return image;
    }


}