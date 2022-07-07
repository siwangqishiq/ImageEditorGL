package panyi.xyz.imageeditorgl.activity

import android.app.Activity
import android.content.Context
import android.content.Intent
import android.opengl.GLSurfaceView
import android.os.Bundle
import android.util.AttributeSet
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

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_editor)

        mainView = findViewById(R.id.editor_view)

        seekBar = findViewById(R.id.seek_bar)

        fileData = intent.getSerializableExtra(INTENT_DATA) as SelectFileItem
        mainView.setContent(fileData.path?:"" , fileData.width , fileData.height)

        seekBar.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener{
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                LogUtil.d(TAG , "progress = $progress")
                NativeBridge.scale(convertScaleValue(progress))
                mainView.requestRender()
            }

            override fun onStartTrackingTouch(seekBar: SeekBar?) {
            }

            override fun onStopTrackingTouch(seekBar: SeekBar?) {
            }
        })

        resetBtn = findViewById<View?>(R.id.reset_btn).apply {
            setOnClickListener {
                NativeBridge.scale(1.0f)
                mainView.requestRender()
            }
        }
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