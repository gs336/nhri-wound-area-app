package nhri.edu.woundareaapp;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.Toast;
import android.widget.ToggleButton;

public class WelcomeActivity extends AppCompatActivity {

    private ToggleButton tbUseGpu;
    private Button yolov5s;
    private Button yolov4tiny;
    private Button mobilenetyolov3nano;
    private Button simplepose;
    private Button yolact;
    private Button chineseocrlite;
    private Button enet;
    private Button faceLandmark;
    private Button dbface;
    private Button mobilenetv2Fcn;
    private Button mobilenetv3Seg;
    private Button yolov5sCustomLayer;
    private Button nanoDet;
    private Button yoloFastestXL;
    private Button lightOpenPose;

    private boolean useGPU = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_welcome);

        tbUseGpu = findViewById(R.id.tb_use_gpu);
        tbUseGpu.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                useGPU = isChecked;
                MainActivity.USE_GPU = useGPU;
                if (useGPU) {
                    AlertDialog.Builder builder = new AlertDialog.Builder(WelcomeActivity.this);
                    builder.setTitle("Warning");
                    builder.setMessage("If the GPU is too old, it may not work well in GPU mode.");
                    builder.setCancelable(true);
                    builder.setPositiveButton("OK", null);
                    AlertDialog dialog = builder.create();
                    dialog.show();
                } else {
                    Toast.makeText(WelcomeActivity.this, "CPU mode", Toast.LENGTH_SHORT).show();
                }
            }
        });

        yolact = findViewById(R.id.btn_start_yolact);
        yolact.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                MainActivity.USE_MODEL = MainActivity.YOLACT;
                Intent intent = new Intent(WelcomeActivity.this, MainActivity.class);
                WelcomeActivity.this.startActivity(intent);
            }
        });

    }


}
