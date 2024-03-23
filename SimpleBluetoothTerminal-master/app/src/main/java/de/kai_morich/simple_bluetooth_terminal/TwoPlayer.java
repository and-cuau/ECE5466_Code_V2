package de.kai_morich.simple_bluetooth_terminal;

import android.animation.ObjectAnimator;
import android.app.Activity;
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
import android.text.Spannable;
import android.text.SpannableStringBuilder;
import android.text.style.ForegroundColorSpan;
import android.view.View;
import android.widget.RadioButton;

import android.animation.AnimatorListenerAdapter;
import android.widget.TextView;
import android.animation.Animator;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;

import java.util.ArrayDeque;










public class TwoPlayer extends Activity implements SensorEventListener, SerialListener, ServiceConnection {
    private SensorManager sensorManager;

    RadioButton radioButton1;
    RadioButton radioButton2;
    RadioButton radioButton3;
    RadioButton radioButton4;
    RadioButton radioButton5;
    RadioButton radioButton6;
    RadioButton radioButton7;
    private boolean hexEnabled = false;
    private boolean pendingNewline = false;
    private String newline = TextUtil.newline_crlf;
    private TextView receiveText;

    private enum Connected { False, Pending, True }

    private TwoPlayer.Connected connected = TwoPlayer.Connected.False;

    private SerialService service;

    private boolean initialStart = true;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.player_two);
        sensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);

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
        sensorManager.registerListener(this,sensorManager.getDefaultSensor(Sensor.TYPE_LIGHT),SensorManager.SENSOR_DELAY_NORMAL);
        int misses = 0;
        while (misses < 2) {
            //if ( recieve() = TRUE) {
            // misses++;
            //}
        }


    }

    public void LightActivity(View view){
        Intent intent = new Intent(this, GameOver.class);
        startActivity(intent);
    }


    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }
    @Override
    public void onPause(){
        super.onPause();
        sensorManager.unregisterListener(this);
    }

    private void status(String str) {
        SpannableStringBuilder spn = new SpannableStringBuilder(str + '\n');
        spn.setSpan(new ForegroundColorSpan(getResources().getColor(R.color.colorStatusText)), 0, spn.length(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
        receiveText.append(spn);
    }

//    private void connect() {
//        try {
//            BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
//            BluetoothDevice device = bluetoothAdapter.getRemoteDevice(deviceAddress);
//            status("connecting...");
//            connected = TerminalFragment.Connected.Pending;
//            SerialSocket socket = new SerialSocket(getActivity().getApplicationContext(), device);
//            service.connect(socket);
//        } catch (Exception e) {
//            onSerialConnectError(e);
//        }
//    }



    @Override
    public void onSerialConnect() {
        status("connected");
        connected = TwoPlayer.Connected.True;
    }

    public void onSerialConnectError(Exception e) {
        status("connection failed: " + e.getMessage());
        disconnect();
    }
    private void disconnect() {
        connected = TwoPlayer.Connected.False;
        service.disconnect();
    }


    @Override
    public void onSerialRead(byte[] data) {
        ArrayDeque<byte[]> datas = new ArrayDeque<>();
        datas.add(data);
        receive(datas);
    }


    @Override
    public void onSerialIoError(Exception e) {
        status("connection lost: " + e.getMessage());
        disconnect();
    }

    public void onSerialRead(ArrayDeque<byte[]> datas) {
        receive(datas);
    }

    private String receive(ArrayDeque<byte[]> datas) {
        SpannableStringBuilder spn = new SpannableStringBuilder();
        String msg = "";
        for (byte[] data : datas) {
            if (hexEnabled) {
                spn.append(TextUtil.toHexString(data)).append('\n');
            } else {
                msg = new String(data);
                if (newline.equals(TextUtil.newline_crlf) && msg.length() > 0) {
                    // don't show CR as ^M if directly before LF
                    msg = msg.replace(TextUtil.newline_crlf, TextUtil.newline_lf);
                    // special handling if CR and LF come in separate fragments
                    if (pendingNewline && msg.charAt(0) == '\n') {
                        if(spn.length() >= 2) {
                            spn.delete(spn.length() - 2, spn.length());
                        } else {
                            Editable edt = receiveText.getEditableText();
                            if (edt != null && edt.length() >= 2)
                                edt.delete(edt.length() - 2, edt.length());
                        }
                    }
                    pendingNewline = msg.charAt(msg.length() - 1) == '\r';
                }
                spn.append(TextUtil.toCaretString(msg, newline.length() != 0));
            }


        }
        receiveText.append(spn);
        return msg;
    }

    @Override
    public void onServiceConnected(ComponentName name, IBinder binder) {
        service = ((SerialService.SerialBinder) binder).getService();
        service.attach(this);
//        if(initialStart && isResumed()) {
//            initialStart = false;
//            getActivity().runOnUiThread(this::connect);
//        }
    }

    @Override
    public void onServiceDisconnected(ComponentName name) {
        service = null;
    }

}

