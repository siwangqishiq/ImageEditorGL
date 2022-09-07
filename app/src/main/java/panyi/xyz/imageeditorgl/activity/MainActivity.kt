package panyi.xyz.imageeditorgl.activity

import android.Manifest
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Bundle
import android.view.View
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.lifecycle.ViewModelProvider
import com.bumptech.glide.Glide
import panyi.xyz.gleditorlib.activity.EditorActivity
import panyi.xyz.imageeditorgl.R
import panyi.xyz.imageeditorgl.model.SelectFileItem
import panyi.xyz.imageeditorgl.model.UserViewModel
import java.io.File

/**
 * MainActivity
 *
 */
class MainActivity : AppCompatActivity() {//end class
    companion object {
        const val PERMISSION_READ_GALLERY = 100
        const val REQUEST_CODE_EDITOR = 101
    }

    private lateinit var pickImageBtn : View

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        requestPermission()

        pickImageBtn = findViewById(R.id.sample_text)
        val pickImageContent = registerForActivityResult(SelectFileActivity.PickImageActivityContent()){
            it?.let {
                handlePickImage(it)
            }
        }

        pickImageBtn.apply {
            setOnClickListener{
                pickImageContent.launch(1)
            }
        }

        val userModel = ViewModelProvider(this).get(UserViewModel::class.java)
        userModel.pullData()
    }

    /**
     *  open gl image editor
     */
    private fun handlePickImage(data : SelectFileItem){
        EditorActivity.start(this , data.path?:"" , REQUEST_CODE_EDITOR)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if(requestCode == REQUEST_CODE_EDITOR){
            val editorImagePath = data?.getStringExtra("path")
            Glide.with(this).load(File(editorImagePath)).into(findViewById(R.id.preview))
        }
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

}