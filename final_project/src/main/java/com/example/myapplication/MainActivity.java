package com.example.myapplication;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import java.io.IOException;
import java.io.OutputStream;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {
    private static final int REQUEST_ENABLE_BT = 0;
    private static final int REQUEST_DISCOVER_BT = 1;
    TextView mstate, mmod;
    static final UUID mUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    Button mbtn1, mbtn2, mbtn3, mbtn4, mbtn5,mbtn6;
    BluetoothSocket btSocket = null;
    BluetoothAdapter btAdapter;
    BluetoothDevice hc05;
    Handler handler;
    boolean isTurboMode = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mbtn1 = findViewById(R.id.btn1);
        mbtn2 = findViewById(R.id.btn2);
        mbtn3 = findViewById(R.id.btn3);
        mbtn4 = findViewById(R.id.btn4);
        mbtn5 = findViewById(R.id.btn5);
        mbtn6 = findViewById(R.id.btn6);
        mstate = findViewById(R.id.state);
        mmod = findViewById(R.id.mode);

        btAdapter = BluetoothAdapter.getDefaultAdapter();
        if (btAdapter == null) {
            // 裝置不支持藍牙
            showToast("裝置不支持藍牙");
            return;
        }

        hc05 = btAdapter.getRemoteDevice("00:19:10:08:21:4C");
        handler = new Handler(Looper.getMainLooper());

        // 檢查是否開啟藍牙
        if (!btAdapter.isEnabled()) {
            // 請求開啟藍牙
            showToast("藍牙未啟動");
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
        } else {
            connectBluetooth();
        }

        // 設定按鈕點擊事件
        setButtonListeners();
    }

    private void setButtonListeners() {
        View.OnTouchListener onTouchListener = new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                int action = event.getAction();
                char command = ' ';

                if (v.getId() == R.id.btn1) { // Forward
                    command = '1';
                } else if (v.getId() == R.id.btn2) { // Right
                    command = '2';
                } else if (v.getId() == R.id.btn3) { // Backward
                    command = '3';
                } else if (v.getId() == R.id.btn4) { // Left
                    command = '4';
                } else if (v.getId() == R.id.btn5) { // Toggle mode
                    if (action == MotionEvent.ACTION_DOWN) {
                        sendBluetoothCommand('5'); // Change speed
                        toggleMode();
                    }
                    return false;
                } else if (v.getId() == R.id.btn6) {
                    if (action == MotionEvent.ACTION_DOWN) {
                        sendBluetoothCommand('R');
                        mmod.setText("normal mode");
                        isTurboMode = false;
                    }
                    return false;
                }


                if (action == MotionEvent.ACTION_DOWN) {
                    sendBluetoothCommand(command);
                } else if (action == MotionEvent.ACTION_UP) {
                    sendBluetoothCommand('6'); // Stop
                }
                return false;
            }
        };

        mbtn1.setOnTouchListener(onTouchListener);
        mbtn2.setOnTouchListener(onTouchListener);
        mbtn3.setOnTouchListener(onTouchListener);
        mbtn4.setOnTouchListener(onTouchListener);
        mbtn5.setOnTouchListener(onTouchListener);
        mbtn6.setOnTouchListener(onTouchListener);
    }

    private void connectBluetooth() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                int counter = 0;
                while (counter < 3) {
                    try {
                        btSocket = hc05.createRfcommSocketToServiceRecord(mUUID);
                        btSocket.connect();
                        updateConnectionStatus(true);
                        return;
                    } catch (IOException e) {
                        e.printStackTrace();
                        counter++;
                    }
                }
                updateConnectionStatus(false);
                retryConnection();
            }
        }).start();
    }

    private void retryConnection() {
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                connectBluetooth();
            }
        }, 5000); // 5秒後重試連接
    }

    private void updateConnectionStatus(final boolean isConnected) {
        handler.post(new Runnable() {
            @Override
            public void run() {
                if (isConnected) {
                    mstate.setText("已連接");
                } else {
                    mstate.setText("已斷開");
                }
            }
        });
    }

    private void sendBluetoothCommand(char command) {
        if (btSocket != null && btSocket.isConnected()) {
            try {
                OutputStream outputStream = btSocket.getOutputStream();
                outputStream.write(command);
            } catch (IOException e) {
                e.printStackTrace();
            }
        } else {
            showToast("藍牙未連接");
        }
    }

    private void toggleMode() {
        isTurboMode = !isTurboMode;
        if (isTurboMode) {
            mmod.setText("turbo mode");
        } else {
            mmod.setText("normal mode");
        }
    }

    private void showToast(String msg) {
        Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
    }
}
