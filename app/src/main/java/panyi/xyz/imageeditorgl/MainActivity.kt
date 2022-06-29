package panyi.xyz.imageeditorgl

import android.Manifest
import android.content.pm.PackageManager
import android.os.Bundle
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat

class MainActivity : AppCompatActivity() {
    companion object {
        const val PERMISSION_READ_GALLERY = 100;
    }

    private lateinit var pickImageBtn : View

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        requestPermission()

        pickImageBtn = findViewById(R.id.sample_text)
        pickImageBtn.apply {
            setOnClickListener{
                SelectFileActivity.start(this@MainActivity)
            }
        }
    }

    fun requestPermission(){
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE)
            != PackageManager.PERMISSION_GRANTED
        ) {
            ActivityCompat.requestPermissions(this,
                arrayOf(Manifest.permission.READ_EXTERNAL_STORAGE), PERMISSION_READ_GALLERY)
        }
    }
}