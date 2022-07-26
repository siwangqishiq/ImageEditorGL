package panyi.xyz.imageeditorgl.activity

import android.app.Activity
import android.content.Context
import android.content.Intent
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.opengl.GLSurfaceView
import android.os.Bundle
import android.util.AttributeSet
import android.view.GestureDetector
import android.view.ScaleGestureDetector
import android.view.View
import android.widget.SeekBar
import androidx.appcompat.app.AppCompatActivity
import panyi.xyz.imageeditorgl.MainView
import panyi.xyz.imageeditorgl.NativeBridge
import panyi.xyz.imageeditorgl.R
import panyi.xyz.imageeditorgl.model.SelectFileItem
import panyi.xyz.imageeditorgl.util.LogUtil
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

/**
 *  just for fun
 *
 */
class EditorActivity : AppCompatActivity() {
    companion object{
        const val INTENT_DATA = "_data"
        const val TAG = "MainGLView"
        fun start(context : Activity , data : SelectFileItem){
            val it = Intent(context , EditorActivity::class.java).apply {
                putExtra(INTENT_DATA , data)
            }
            context.startActivity(it)
        }
    }

    private lateinit var fileData : SelectFileItem
    private lateinit var mainView : MainView
    private lateinit var seekBar:SeekBar

    private lateinit var resetBtn : View

    private lateinit var scaleDetect : ScaleGestureDetector

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_editor)

        mainView = findViewById(R.id.editor_view)

        fileData = intent.getSerializableExtra(INTENT_DATA) as SelectFileItem
        val path = fileData.path?:""
        mainView.setContent(path , fileData.width , fileData.height , null)

//        resetBtn = findViewById<View?>(R.id.reset_btn).apply {
//            setOnClickListener {
//                NativeBridge.scale(1.0f)
//                mainView.requestRender()
//            }
//        }
    }

    private fun readImageConfig(path : String){
        val options = BitmapFactory.Options()
        options.inJustDecodeBounds = false
        BitmapFactory.decodeFile(path , options)
        LogUtil.d(TAG , "read image $path config : ${options.outConfig}")
    }

    fun convertScaleValue(progress: Int) : Float{
        val min = 0.4f;
        val max = 4.0f;
        val ret = (max - min ) * (progress.toFloat() / 100.0f)
        LogUtil.d(TAG ,  "scale value : $ret")
        return ret;
    }

    override fun onDestroy() {
        mainView.onDestroy()
        super.onDestroy()
    }
}