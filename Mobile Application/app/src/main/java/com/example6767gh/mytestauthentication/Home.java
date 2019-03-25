package com.example6767gh.mytestauthentication;

import android.content.Intent;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.util.Log;
import android.view.View;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.StorageReference;
import com.squareup.picasso.Picasso;

import java.lang.reflect.Type;

public class Home extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener {

    FirebaseDatabase firebaseDatabase;
    View v1;
    View v2;
    View v3;
    ImageView img1;
    ImageView img2;
    ImageView img3;
    TextView tit1;
    TextView tit2;
    TextView tit3;
    TextView c1;
    TextView c2;
    TextView c3;
    TextView d1;
    TextView d2;
    TextView d3;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home);
        v1 = findViewById(R.id.f1);
        v2 = findViewById(R.id.f2);
        v3 = findViewById(R.id.f3);
        img1 = v1.findViewById(R.id.ivProfilePic);
        img2 = v2.findViewById(R.id.ivProfilePic);
        img3 = v3.findViewById(R.id.ivProfilePic);
        tit1 = v1.findViewById(R.id.name);
        tit2 = v2.findViewById(R.id.name);
        tit3 = v3.findViewById(R.id.name);
        c1= v1.findViewById(R.id.color);
        c2 = v2.findViewById(R.id.color);
        c3 = v3.findViewById(R.id.color);
        d1 = v1.findViewById(R.id.dist);
        d2 = v2.findViewById(R.id.dist);
        d3 = v3.findViewById(R.id.dist);

        firebaseDatabase = FirebaseDatabase.getInstance();

        DatabaseReference mDatabase = FirebaseDatabase.getInstance().getReference("Cars");


        mDatabase.addListenerForSingleValueEvent(new ValueEventListener() {

            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {

                Cars carsInformation = dataSnapshot.child("123").getValue(Cars.class);


                StorageReference profileImageRef =
                        FirebaseStorage.getInstance().getReference().child("cars/").child(String.valueOf(carsInformation.getImage()));

                String trial = carsInformation.getImage();

                Picasso.with(getApplication()).load(trial)
                        .into(img1);
                tit1.setText( carsInformation.getType());
                c1.setText(carsInformation.getColor());
                d1.setText(carsInformation.getNumber());


                Picasso.with(getApplication()).load(trial)
                        .into(img2);
                tit2.setText( carsInformation.getType());
                c2.setText(carsInformation.getColor());
                d2.setText(carsInformation.getNumber());

                Picasso.with(getApplication()).load(trial)
                        .into(img3);
                tit3.setText( carsInformation.getType());
                c3.setText(carsInformation.getColor());
                d3.setText(carsInformation.getNumber());
            }


            @Override
            public void onCancelled(DatabaseError databaseError) {
                Toast.makeText(Home.this, databaseError.getCode(), Toast.LENGTH_SHORT).show();
            }
        });







        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);



        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawer.addDrawerListener(toggle);
        toggle.syncState();

        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);
    }

    @Override
    public void onBackPressed() {
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
            super.onBackPressed();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.home, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @SuppressWarnings("StatementWithEmptyBody")
    @Override
    public boolean onNavigationItemSelected(MenuItem item) {

        int id = item.getItemId();

        if (id == R.id.nav_profile) {
        /*    Toast toast = Toast.makeText(Home.this, "HAHAHAHA", Toast.LENGTH_SHORT);
            toast.show();    */

            startActivity(new Intent(this, ProfileInfo.class));

        } else if (id == R.id.nav_notifications) {

        } else if (id == R.id.nav_history) {
            finish();
            startActivity(new Intent(this, History.class));

        } else if (id == R.id.nav_about) {

            finish();
            startActivity(new Intent(this, About.class));

        } else if (id == R.id.nav_contact) {

        }

        else if (id == R.id.nav_logout) {

            FirebaseAuth.getInstance().signOut();
            finish();
            startActivity(new Intent(this, MainActivity1.class));

        }


        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);
        return true;
    }
}
