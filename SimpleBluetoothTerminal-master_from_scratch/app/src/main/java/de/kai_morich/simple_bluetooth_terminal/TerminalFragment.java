package de.kai_morich.simple_bluetooth_terminal;

import android.Manifest;
import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.animation.ObjectAnimator;
import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.media.MediaPlayer;
import android.os.Build;
import android.os.Bundle;
import android.os.IBinder;
import android.text.Editable;
import android.text.Spannable;
import android.text.SpannableStringBuilder;
import android.text.method.ScrollingMovementMethod;
import android.text.style.ForegroundColorSpan;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import java.util.ArrayDeque;
import java.util.Arrays;

public class TerminalFragment extends Fragment implements ServiceConnection, SerialListener {

    private enum Connected { False, Pending, True }

    private String deviceAddress;
    private SerialService service;

    private TextView receiveText;
    private TextView sendText;
    private TextUtil.HexWatcher hexWatcher;

    private Connected connected = Connected.False;
    private boolean initialStart = true;
    private boolean hexEnabled = false;
    private boolean pendingNewline = false;
    private String newline = TextUtil.newline_crlf;

    private MediaPlayer miss_sound, g_o_sound;

    MediaPlayer squeak;
    MediaPlayer hit_sound;
    Button b1p, b2p, b1, b2, b3, b4, b5, b6, b7, b8;
    ImageView h1, h2;
    ImageView g_o;
    TextView sc;

    View mole;

    Animation animation;
    int miss = 0;
    int score = 0;

    ObjectAnimator fadeIn;

    /*
     * Lifecycle
     */
    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setHasOptionsMenu(true);
        setRetainInstance(true);
        deviceAddress = getArguments().getString("device");
    }

    @Override
    public void onDestroy() {
        if (connected != Connected.False)
            disconnect();
        getActivity().stopService(new Intent(getActivity(), SerialService.class));
        super.onDestroy();
    }

    @Override
    public void onStart() {
        super.onStart();
        if(service != null)
            service.attach(this);
        else
            getActivity().startService(new Intent(getActivity(), SerialService.class)); // prevents service destroy on unbind from recreated activity caused by orientation change
    }

    @Override
    public void onStop() {
        if(service != null && !getActivity().isChangingConfigurations())
            service.detach();
        super.onStop();
    }

    @SuppressWarnings("deprecation") // onAttach(context) was added with API 23. onAttach(activity) works for all API versions
    @Override
    public void onAttach(@NonNull Activity activity) {
        super.onAttach(activity);
        getActivity().bindService(new Intent(getActivity(), SerialService.class), this, Context.BIND_AUTO_CREATE);
    }

    @Override
    public void onDetach() {
        try { getActivity().unbindService(this); } catch(Exception ignored) {}
        super.onDetach();
    }

    @Override
    public void onResume() {
        super.onResume();
        if(initialStart && service != null) {
            initialStart = false;
            getActivity().runOnUiThread(this::connect);
        }
    }

    @Override
    public void onServiceConnected(ComponentName name, IBinder binder) {
        service = ((SerialService.SerialBinder) binder).getService();
        service.attach(this);
        if(initialStart && isResumed()) {
            initialStart = false;
            getActivity().runOnUiThread(this::connect);
        }
    }

    @Override
    public void onServiceDisconnected(ComponentName name) {
        service = null;
    }

    /*
     * UI
     */
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_terminal, container, false);
        receiveText = view.findViewById(R.id.receive_text);                          // TextView performance decreases with number of spans
        receiveText.setTextColor(getResources().getColor(R.color.colorRecieveText)); // set as default color to reduce number of spans
        receiveText.setMovementMethod(ScrollingMovementMethod.getInstance());

        sendText = view.findViewById(R.id.send_text);
        hexWatcher = new TextUtil.HexWatcher(sendText);
        hexWatcher.enable(hexEnabled);
        sendText.addTextChangedListener(hexWatcher);
        sendText.setHint(hexEnabled ? "HEX mode" : "");

        View sendBtn = view.findViewById(R.id.send_btn);
        sendBtn.setOnClickListener(v -> send(sendText.getText().toString()));

        g_o_sound = MediaPlayer.create(getActivity().getApplicationContext(), R.raw.negative_beeps); // replace 'your_audio_file' with your file's name without extension

        miss_sound = MediaPlayer.create(getActivity().getApplicationContext(), R.raw.error_sound);
        miss_sound.setVolume(1.0f, 1.0f);
        hit_sound = MediaPlayer.create(getActivity().getApplicationContext(), R.raw.tennis_smash);
        mole = view.findViewById(R.id.mole);

        b1p = view.findViewById(R.id.b1p);
        b2p = view.findViewById(R.id.b2p);
        b1 = view.findViewById(R.id.b1);
        b2 = view.findViewById(R.id.b2);
        b3 = view.findViewById(R.id.b3);
        b4 = view.findViewById(R.id.b4);
        b5 = view.findViewById(R.id.b5);
        b6 = view.findViewById(R.id.b6);
        b7 = view.findViewById(R.id.b7);
        b8 = view.findViewById(R.id.b8);

        h1 = view.findViewById(R.id.h1);
        h2 = view.findViewById(R.id.h2);

        g_o = view.findViewById(R.id.g_o);
        g_o.setVisibility(View.INVISIBLE);

        sc = view.findViewById(R.id.score);

        animation = AnimationUtils.loadAnimation(getActivity().getApplicationContext(), R.anim.blink);

        fadeIn = ObjectAnimator.ofFloat(g_o, "alpha", 0f, 1f);
        fadeIn.setDuration(1000);

        ObjectAnimator fadeOut = ObjectAnimator.ofFloat(g_o, "alpha", 1f, 0f);
        fadeOut.setDuration(1000);

        // Chain the animations for a fade-in, stay visible, and then fade-out effect
        fadeIn.addListener(new AnimatorListenerAdapter() {
            public void onAnimationEnd(Animator animation) {
                super.onAnimationEnd(animation);
                fadeOut.start();

            }
        });






        buttonsOff();

        b1p.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String binary = "9";
                send(binary);
                startButtonsOff();
                mole.setVisibility(View.VISIBLE);
                mole.startAnimation(animation);
                g_o.setVisibility(View.INVISIBLE);
                sc.setText("0");
                h1.setVisibility(View.VISIBLE);
                h2.setVisibility(View.VISIBLE);
                score = 0;
            }
        });
        b2p.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String binary = "10";
                send(binary);
                buttonsOn();
                startButtonsOff();
                mole.setVisibility(View.VISIBLE);
                mole.startAnimation(animation);
                g_o.setVisibility(View.INVISIBLE);
                sc.setText("0");
                h1.setVisibility(View.VISIBLE);
                h2.setVisibility(View.VISIBLE);
                score = 0;
            }
        });
        b1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String binary = "1";
                send(binary);
                // MediaPlayer squeak;
                squeak = MediaPlayer.create(getActivity().getApplicationContext(), R.raw.squeak);
                squeak.start();
            }
        });
        b2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String binary = "2";
                send(binary);
                // MediaPlayer squeak;
                squeak = MediaPlayer.create(getActivity().getApplicationContext(), R.raw.squeak);
                squeak.start();
                //mole.startAnimation(animation);
                //mole.clearAnimation();
            }
        });
        b3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String binary = "3";
                send(binary);
                // MediaPlayer squeak;
                squeak = MediaPlayer.create(getActivity().getApplicationContext(), R.raw.squeak);
                squeak.start();
                // mole.startAnimation(animation);
                //mole.clearAnimation();
            }
        });
        b4.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String binary = "4";
                send(binary);
                // MediaPlayer squeak;
                squeak = MediaPlayer.create(getActivity().getApplicationContext(), R.raw.squeak);
                squeak.start();
                // mole.startAnimation(animation);
                //mole.clearAnimation();
            }
        });
        b5.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String binary = "5";
                send(binary);
                // MediaPlayer squeak;
                squeak = MediaPlayer.create(getActivity().getApplicationContext(), R.raw.squeak);
                squeak.start();
                // mole.startAnimation(animation);
                // mole.clearAnimation();
            }
        });
        b6.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String binary = "6";
                send(binary);
                // MediaPlayer squeak;
                squeak = MediaPlayer.create(getActivity().getApplicationContext(), R.raw.squeak);
                squeak.start();
                // mole.startAnimation(animation);
                //mole.clearAnimation();
            }
        });
//        b7.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View v) {
//                String binary = "7";
//                send(binary);
//                // MediaPlayer squeak;
//                squeak = MediaPlayer.create(getActivity().getApplicationContext(), R.raw.squeak);
//                squeak.start();
//                // mole.startAnimation(animation);
//                //mole.clearAnimation();
//            }
//        });
//        b8.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View v) {
//                String binary = "8";
//                send(binary);
//                //MediaPlayer squeak;
//                squeak = MediaPlayer.create(getActivity().getApplicationContext(), R.raw.squeak);
//                squeak.start();
//                // mole.startAnimation(animation);
//                //mole.clearAnimation();
//            }
//        });





        return view;
    }

    @Override
    public void onCreateOptionsMenu(@NonNull Menu menu, MenuInflater inflater) {
        inflater.inflate(R.menu.menu_terminal, menu);
    }

    public void onPrepareOptionsMenu(@NonNull Menu menu) {
        menu.findItem(R.id.hex).setChecked(hexEnabled);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            menu.findItem(R.id.backgroundNotification).setChecked(service != null && service.areNotificationsEnabled());
        } else {
            menu.findItem(R.id.backgroundNotification).setChecked(true);
            menu.findItem(R.id.backgroundNotification).setEnabled(false);
        }
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        if (id == R.id.clear) {
            receiveText.setText("");
            return true;
        } else if (id == R.id.newline) {
            String[] newlineNames = getResources().getStringArray(R.array.newline_names);
            String[] newlineValues = getResources().getStringArray(R.array.newline_values);
            int pos = java.util.Arrays.asList(newlineValues).indexOf(newline);
            AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
            builder.setTitle("Newline");
            builder.setSingleChoiceItems(newlineNames, pos, (dialog, item1) -> {
                newline = newlineValues[item1];
                dialog.dismiss();
            });
            builder.create().show();
            return true;
        } else if (id == R.id.hex) {
            hexEnabled = !hexEnabled;
            sendText.setText("");
            hexWatcher.enable(hexEnabled);
            sendText.setHint(hexEnabled ? "HEX mode" : "");
            item.setChecked(hexEnabled);
            return true;
        } else if (id == R.id.backgroundNotification) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                if (!service.areNotificationsEnabled() && Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
                    requestPermissions(new String[]{Manifest.permission.POST_NOTIFICATIONS}, 0);
                } else {
                    showNotificationSettings();
                }
            }
            return true;
        } else {
            return super.onOptionsItemSelected(item);
        }
    }

    /*
     * Serial + UI
     */
    private void connect() {
        try {
            BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
            BluetoothDevice device = bluetoothAdapter.getRemoteDevice(deviceAddress);
            status("connecting...");
            connected = Connected.Pending;
            SerialSocket socket = new SerialSocket(getActivity().getApplicationContext(), device);
            service.connect(socket);
        } catch (Exception e) {
            onSerialConnectError(e);
        }
    }


    private void buttonsOff(){
        b1.setEnabled(false);
        b2.setEnabled(false);
        b3.setEnabled(false);
        b4.setEnabled(false);
        b5.setEnabled(false);
        b6.setEnabled(false);
        b7.setEnabled(false);
        b8.setEnabled(false);
    }
    private void buttonsOn(){
        b1.setEnabled(true);
        b2.setEnabled(true);
        b3.setEnabled(true);
        b4.setEnabled(true);
        b5.setEnabled(true);
        b6.setEnabled(true);
        //b7.setEnabled(true);
        //b8.setEnabled(true);
    }

    private void startButtonsOff(){
        b1p.setEnabled(false);
        b2p.setEnabled(false);
    }
    private void startButtonsOn(){
        b1p.setEnabled(true);
        b2p.setEnabled(true);
    }

    private void disconnect() {
        connected = Connected.False;
        service.disconnect();
    }

    private void send(String str) {
        if(connected != Connected.True) {
            Toast.makeText(getActivity(), "not connected", Toast.LENGTH_SHORT).show();
            return;
        }
        try {
            String msg;
            byte[] data;
            if(hexEnabled) {
                StringBuilder sb = new StringBuilder();
                TextUtil.toHexString(sb, TextUtil.fromHexString(str));
                TextUtil.toHexString(sb, newline.getBytes());
                msg = sb.toString();
                data = TextUtil.fromHexString(msg);
            } else {
                msg = str;
                data = (str + newline).getBytes();
            }
            SpannableStringBuilder spn = new SpannableStringBuilder(msg + '\n');
            spn.setSpan(new ForegroundColorSpan(getResources().getColor(R.color.colorSendText)), 0, spn.length(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
            receiveText.append(spn);
            service.write(data);
        } catch (Exception e) {
            onSerialIoError(e);
        }
    }

    private void receive(ArrayDeque<byte[]> datas) {
        SpannableStringBuilder spn = new SpannableStringBuilder();
        for (byte[] data : datas) {
            if (hexEnabled) {
                spn.append(TextUtil.toHexString(data)).append('\n');
            } else {
                String msg = new String(data);
                // sc.setText(msg);
                if (msg.equals("m")) {
                    miss_sound.start();

                    miss += 1;
                    if (miss == 1) {
                        h2.setVisibility(View.INVISIBLE);
                    } else if (miss == 2) {
                        g_o_sound.start();
                        buttonsOff();
                        startButtonsOn();
                        mole.clearAnimation();
                        h1.setVisibility(View.INVISIBLE);
                        h2.setVisibility(View.INVISIBLE);
                        mole.setVisibility(View.INVISIBLE);
                        g_o.setVisibility(View.VISIBLE);
                        miss = 0;
                    }
                }
                else {
                    //MediaPlayer hit_sound;
                    // sc.setText("test hit!!!");
                    score += 1;
                    hit_sound.start();
                    sc.setText(msg);
                }
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
    }

    private void status(String str) {
        SpannableStringBuilder spn = new SpannableStringBuilder(str + '\n');
        spn.setSpan(new ForegroundColorSpan(getResources().getColor(R.color.colorStatusText)), 0, spn.length(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
        receiveText.append(spn);
    }

    /*
     * starting with Android 14, notifications are not shown in notification bar by default when App is in background
     */

    private void showNotificationSettings() {
        Intent intent = new Intent();
        intent.setAction("android.settings.APP_NOTIFICATION_SETTINGS");
        intent.putExtra("android.provider.extra.APP_PACKAGE", getActivity().getPackageName());
        startActivity(intent);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        if(Arrays.equals(permissions, new String[]{Manifest.permission.POST_NOTIFICATIONS}) &&
                Build.VERSION.SDK_INT >= Build.VERSION_CODES.O && !service.areNotificationsEnabled())
            showNotificationSettings();
    }

    /*
     * SerialListener
     */
    @Override
    public void onSerialConnect() {
        status("connected");
        connected = Connected.True;
    }

    @Override
    public void onSerialConnectError(Exception e) {
        status("connection failed: " + e.getMessage());
        disconnect();
    }

    @Override
    public void onSerialRead(byte[] data) {
        ArrayDeque<byte[]> datas = new ArrayDeque<>();
        datas.add(data);
        receive(datas);
    }

    public void onSerialRead(ArrayDeque<byte[]> datas) {
        receive(datas);
    }

    @Override
    public void onSerialIoError(Exception e) {
        status("connection lost: " + e.getMessage());
        disconnect();
    }

}
