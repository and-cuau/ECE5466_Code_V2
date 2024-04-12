package de.kai_morich.simple_bluetooth_terminal;

import android.animation.ObjectAnimator;
import android.app.Activity;
import android.bluetooth.BluetoothSocket;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.IBinder;
import android.text.Editable;
import android.text.SpannableStringBuilder;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;
import android.widget.RadioButton;
import android.util.Log;
import android.animation.AnimatorListenerAdapter;
import android.widget.TextView;
import android.animation.Animator;

import java.util.ArrayDeque;

public class OnePlayer extends Activity implements SensorEventListener {
    private SensorManager sensorManager;
    ImageView img;


    RadioButton radioButton1;
    RadioButton radioButton2;
    RadioButton radioButton3;
    RadioButton radioButton4;
    RadioButton radioButton5;
    RadioButton radioButton6;
    RadioButton radioButton7;

    // public static BluetoothSocket bluetoothSocket;


    // OnePlayer.bluetoothSocket = btServer.createInsecureRfcommSocketToServiceRecord();




    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.player_one);
        sensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);

        img = findViewById(R.id.heart1);

        Animation animation = AnimationUtils.loadAnimation(getApplicationContext(), R.anim.blink);
        img.startAnimation(animation);

        TextView myLabel = findViewById(R.id.whack);
        myLabel.setText("Updated Label Text");
        myLabel.setTextColor(getResources().getColor(android.R.color.holo_red_dark));

        // Create a fade-in animation
        ObjectAnimator fadeIn = ObjectAnimator.ofFloat(myLabel, "alpha", 0f, 1f);
        fadeIn.setDuration(1000); // 1000 milliseconds or 1 second

        // Create a fade-out animation
        ObjectAnimator fadeOut = ObjectAnimator.ofFloat(myLabel, "alpha", 1f, 0f);
        fadeOut.setDuration(1000); // 1000 milliseconds or 1 second

        // Chain the animations for a fade-in, stay visible, and then fade-out effect
        fadeIn.addListener(new AnimatorListenerAdapter() {
            public void onAnimationEnd(Animator animation) {
                super.onAnimationEnd(animation);
                fadeOut.start();
            }
        });

        // Start the fade-in animation
        fadeIn.start();


    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        if (event.sensor.getType() != Sensor.TYPE_LIGHT) {
            return;
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        // setContentView(R.layout.activity_acceleration);
        sensorManager.registerListener(this, sensorManager.getDefaultSensor(Sensor.TYPE_LIGHT), SensorManager.SENSOR_DELAY_NORMAL);
        int misses = 0;
        while (misses < 2) {
            //if ( recieve() = TRUE) {
            // misses++;
            //}
        }


    }

    public void LightActivity(View view) {
        Intent intent = new Intent(this, GameOver.class);
        startActivity(intent);
    }


    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }

    @Override
    public void onPause() {
        super.onPause();
        sensorManager.unregisterListener(this);
    }

    /*
     * SerialListener
     */
}