package com.example6767gh.mytestauthentication;

import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.provider.MediaStore;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import com.bumptech.glide.Glide;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.auth.UserProfileChangeRequest;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.StorageReference;
import com.google.firebase.storage.UploadTask;
import com.squareup.picasso.Picasso;

import java.io.IOException;

public class EditProfile extends AppCompatActivity implements View.OnClickListener   {


    private static final int CHOOSE_IMAGE1 = 1001;

    private Uri uriProfileImage;
    String profileImageUrl1;

    ImageView ivProfilePic;
    TextView profileName, profileAge, profileEmail,ivNumber, profileLast;
    TextView profileEmail1;
    Button btnProfileUpdate;
    FirebaseAuth firebaseAuth;
    FirebaseDatabase firebaseDatabase;
    FirebaseStorage firebaseStorage;
    FirebaseAuth mAuth;
    ProgressBar progressBar;

    String profileImageUrl;

    DatabaseReference databaseUser ;

    FirebaseUser test = FirebaseAuth.getInstance().getCurrentUser();

    String id = test.getUid();




    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_edit_profile);



        profileName = (TextView) findViewById(R.id.tvProfileName);
        profileAge = (TextView) findViewById(R.id.tvProfileAge);
        profileEmail = (TextView) findViewById(R.id.tvProfileEmail);
        profileLast = (TextView) findViewById(R.id.tvProfileLast);
        ivProfilePic = (ImageView) findViewById(R.id.ivProfilePic);
        btnProfileUpdate = (Button) findViewById(R.id.btnProfileUpdate);
        profileEmail1 = (TextView) findViewById(R.id.tvProfileEmail1);
        ivNumber=(TextView) findViewById(R.id.ivNumber);
        findViewById(R.id.btnProfileUpdate).setOnClickListener(this);
        progressBar = (ProgressBar) findViewById(R.id.progressbar2);
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


                StorageReference profileImageRef =
                        FirebaseStorage.getInstance().getReference().child("profilepics/").child(String.valueOf(userProfile.getuImageUrl()));

                String trial = userProfile.getuImageUrl();


                Picasso.with(getApplication()).load(trial)
                        .into(ivProfilePic);

            }


            @Override
            public void onCancelled(DatabaseError databaseError) {
                Toast.makeText(EditProfile.this, databaseError.getCode(), Toast.LENGTH_SHORT).show();
            }
        });
//_____________________________________________________________________


        mData.addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {

                users userProfileData = dataSnapshot.child(id).getValue(users.class);
                EmailUsers emails = dataSnapshot.child("Email").child(id).getValue(EmailUsers.class);

                profileName.setText(userProfileData.getuFirstName());
                profileAge.setText(userProfileData.getuAge());
                profileEmail.setText(userProfileData.getuTrustedEmail());
                profileLast.setText(userProfileData.getuLastName());
                profileEmail1.setText(emails.getuEmail());
                ivNumber.setText(userProfileData.getuNumber());


            }

            @Override
            public void onCancelled(DatabaseError databaseError) {
                Toast.makeText(EditProfile.this, databaseError.getCode(), Toast.LENGTH_SHORT).show();


            }
        });



        ivProfilePic.setOnClickListener(new View.OnClickListener() {


            @Override
            public void onClick(View view) {

                showImageChooser();           // method permit user to select image
            }
        });

        //loadUserInformation();

        findViewById(R.id.btnProfileUpdate).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                editUser();
            }
        });
    }

//_____________________________________________________________________


    private void editUser() {

        databaseUser = FirebaseDatabase.getInstance().getReference("Users");

        String   name     = profileName.getText().toString();
        String   old      =profileAge.getText().toString();
        String   trusted  =profileEmail.getText().toString();
        String   lastname =profileLast.getText().toString();
        String number = ivNumber.getText().toString();

        if( (!TextUtils.isEmpty(name)) && (!TextUtils.isEmpty(lastname)) && (!TextUtils.isEmpty(old)) && (!TextUtils.isEmpty(number))&& (!TextUtils.isEmpty(trusted)) ){

            final String   id = test.getUid();
            FirebaseDatabase firebaseDatabase = FirebaseDatabase.getInstance();
            DatabaseReference mDatabase = FirebaseDatabase.getInstance().getReference("Users").child(id);
            users hh = new users(id,name,lastname,old,number,trusted);
            databaseUser.child(test.getUid()).setValue(hh);
            Toast.makeText(this,"information edited ",Toast.LENGTH_LONG).show();

        }
        else{
            Toast.makeText(this,"please Fill all the information needed",Toast.LENGTH_LONG).show();
        }
        if ( profileImageUrl1 != null) {

            DatabaseReference Database2 = FirebaseDatabase.getInstance().getReference("Users").child("Images");
            ImagesUrl userr= new ImagesUrl (profileImageUrl1);
            Database2.child(test.getUid()).setValue(userr);
        }
        finish();
        startActivity(new Intent(this, ProfileInfo.class));
    }



    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {


        super.onActivityResult(requestCode, resultCode, data);

        // check for the image selected
        if (requestCode == CHOOSE_IMAGE1 && resultCode == RESULT_OK && data != null && data.getData() != null) {
            uriProfileImage = data.getData();
            try {
                Bitmap bitmap = MediaStore.Images.Media.getBitmap(getContentResolver(), uriProfileImage);
                ivProfilePic.setImageBitmap(bitmap);
                uploadImageToFirebaseStorage();

            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }


// uplods to firebase the image

    /* */
    private void uploadImageToFirebaseStorage() {
        // storage


        StorageReference profileImageRef =
                FirebaseStorage.getInstance().getReference("profilepics/" + System.currentTimeMillis()  + ".jpg");
        // system.currenttimemillis , provide unique name
        // 3andy image
        if (uriProfileImage != null) {
            progressBar.setVisibility(View.VISIBLE);
            profileImageRef.putFile(uriProfileImage)
                    .addOnSuccessListener(new OnSuccessListener<UploadTask.TaskSnapshot>() {
                        // check uplod successful or not
                        @Override
                        public void onSuccess(UploadTask.TaskSnapshot taskSnapshot) {
                            progressBar.setVisibility(View.GONE);
                            profileImageUrl1 = taskSnapshot.getDownloadUrl().toString(); // get the url as user informations


                        }
                    })
                    .addOnFailureListener(new OnFailureListener() {
                        @Override
                        public void onFailure(@NonNull Exception e) {
                            progressBar.setVisibility(View.GONE);
                            Toast.makeText(EditProfile.this, e.getMessage(), Toast.LENGTH_SHORT).show();
                        }
                    });
        }
    }


    //  selects image of the user

    /* */
    private void showImageChooser() {


        Intent intent = new Intent(Intent.ACTION_PICK);
        intent.setType("image/*");
        intent.setAction(Intent.ACTION_GET_CONTENT); // get the image
        startActivityForResult(Intent.createChooser(intent,"profile picture update"), CHOOSE_IMAGE1);


    }







    @Override // same as in main 1 buttonsignup  --> methods register user      //sign up
    //                   textviewlogin --> end acti2, start active 1  // login
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.btnProfileUpdate:
                editUser();
                break;


        }
    }



}