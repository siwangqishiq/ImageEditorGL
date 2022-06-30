package panyi.xyz.imageeditorgl.activity

import android.Manifest
import android.content.pm.PackageManager
import android.os.Bundle
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import panyi.xyz.imageeditorgl.R
import panyi.xyz.imageeditorgl.model.SelectFileItem

/**
 * MainActivity
 *
 */
class MainActivity : AppCompatActivity() {
    companion object {
        const val PERMISSION_READ_GALLERY = 100
    }

    private lateinit var pickImageBtn : View

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        requestPermission()

        pickImageBtn = findViewById(R.id.sample_text)
        val pickImageContent = registerForActivityResult(SelectFileActivity.PickImageActivityContent()){
            handlePickImage(it)
        }

        pickImageBtn.apply {
            setOnClickListener{
                pickImageContent.launch(1)
            }
        }
    }

    /**
     *  open gl image editor
     */
    private fun handlePickImage(data : SelectFileItem){
        EditorActivity.start(this , data)
    }

    /**
     *  request read gallery permission
     *
     */
    private fun requestPermission(){
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE)
            != PackageManager.PERMISSION_GRANTED
        ) {
            ActivityCompat.requestPermissions(this,
                arrayOf(Manifest.permission.READ_EXTERNAL_STORAGE), PERMISSION_READ_GALLERY
            )
        }
    }

}//end class