package com.example6767gh.mytestauthentication;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;


import com.bumptech.glide.Glide;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.StorageReference;
import com.squareup.picasso.Picasso;




public class ProfileInfo  extends AppCompatActivity implements View.OnClickListener {


    ImageView profilePic;
    TextView profileName, profileAge, profileEmail,ivNumber;
    TextView profileEmail1;
    Button btnProfileUpdate;
    FirebaseAuth firebaseAuth;
    FirebaseDatabase firebaseDatabase;
    FirebaseStorage firebaseStorage;
    FirebaseAuth mAuth;

    String profileImageUrl;

    FirebaseUser test = FirebaseAuth.getInstance().getCurrentUser();

    String id = test.getUid();




    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_profile_info);

        profileName = (TextView) findViewById(R.id.tvProfileName);
        profileAge = (TextView) findViewById(R.id.tvProfileAge);
        profileEmail = (TextView) findViewById(R.id.tvProfileEmail);
        profilePic = (ImageView) findViewById(R.id.ivProfilePic);
        btnProfileUpdate = (Button) findViewById(R.id.btnProfileUpdate);
        profileEmail1 = (TextView) findViewById(R.id.tvProfileEmail1);
        ivNumber = (TextView) findViewById(R.id.ivNumber);


        findViewById(R.id.btnProfileUpdate).setOnClickListener(this);

        firebaseAuth = FirebaseAuth.getInstance();
        FirebaseUser test = FirebaseAuth.getInstance().getCurrentUser();
        firebaseDatabase = FirebaseDatabase.getInstance();
        firebaseStorage = FirebaseStorage.getInstance();

        DatabaseReference mDatabase = FirebaseDatabase.getInstance().getReference("Users").child("Images");
        DatabaseReference mData = FirebaseDatabase.getInstance().getReference("Users");


        mDatabase.addListenerForSingleValueEvent(new ValueEventListener() {

            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {

                ImagesUrl userProfile = dataSnapshot.child(id).getValue(ImagesUrl.class);



                Log.i("url", String.valueOf(userProfile.getuImageUrl()));

                StorageReference profileImageRef =
                        FirebaseStorage.getInstance().getReference().child("profilepics/").child(String.valueOf(userProfile.getuImageUrl()));

                String trial = userProfile.getuImageUrl();

                Picasso.with(getApplication()).load(trial)
                        .into(profilePic);


                //    profileName.setText( userProfile.getuFirstName());
                //  profileAge.setText( userProfile.getuAge());
                //  profileEmail.setText(userProfile.getuTrustedEmail());
                //     profileLast.setText(userProfile.getuLastName());

            }


            @Override
            public void onCancelled(DatabaseError databaseError) {
                Toast.makeText(ProfileInfo.this, databaseError.getCode(), Toast.LENGTH_SHORT).show();
            }
        });
        mData.addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {

                users userProfileData = dataSnapshot.child(id).getValue(users.class);
                EmailUsers emails = dataSnapshot.child("Email").child(id).getValue(EmailUsers.class);

                profileName.setText(userProfileData.getuFirstName() + " " + userProfileData.getuLastName());
                profileAge.setText(userProfileData.getuAge());
                profileEmail.setText(userProfileData.getuTrustedEmail());
                profileEmail1.setText(emails.getuEmail());
                ivNumber.setText(userProfileData.getuNumber());


            }

            @Override
            public void onCancelled(DatabaseError databaseError) {
                Toast.makeText(ProfileInfo.this, databaseError.getCode(), Toast.LENGTH_SHORT).show();


            }
        });


    }


    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.btnProfileUpdate:
                finish();
                startActivity(new Intent(this, EditProfile.class));
                break;

        }
    }

}