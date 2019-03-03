package com.example6767gh.mytestauthentication;

import android.net.Uri;

/**
 * Created by Dana on 2/15/2019.
 */

public class users {

    private String userId;
    private String uFirstName;
    private String uLastName;
    private String uAge;
    private String uNumber;
    private String uTrustedEmail;


    public users(){}




    public users(String userId, String uFirstName, String uLastName, String uAge, String uNumber, String uTrustedEmail) {
        this.userId = userId;
        this.uFirstName = uFirstName;
        this.uLastName = uLastName;
        this.uAge = uAge;
        this.uNumber = uNumber;
        this.uTrustedEmail = uTrustedEmail;

    }

    public String getUserId()
    {
        return userId;
    }




    public String getuFirstName() {
        return uFirstName;
    }

    public String getuLastName() {
        return uLastName;
    }

    public String getuAge() {
        return uAge;
    }


    public String getuNumber() {
        return uNumber;
    }
    public String getuTrustedEmail() {
        return uTrustedEmail;
    }

    public void setUserId(String userId) {
        this.userId = userId;
    }

    public void setuTrustedEmail(String uTrustedEmail) {
        this.uTrustedEmail = uTrustedEmail;
    }

    public void setuNumber(String uNumber) {
        this.uNumber = uNumber;
    }

    public void setuAge(String uAge) {
        this.uAge = uAge;
    }

    public void setuLastName(String uLastName) {
        this.uLastName = uLastName;
    }

    public void setuFirstName(String uFirstName) {
        this.uFirstName = uFirstName;
    }


}