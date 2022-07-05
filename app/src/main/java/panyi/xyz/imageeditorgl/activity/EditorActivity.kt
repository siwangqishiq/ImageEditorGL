package panyi.xyz.imageeditorgl.activity

import android.app.Activity
import android.content.Context
import android.content.Intent
import android.opengl.GLSurfaceView
import android.os.Bundle
import android.util.AttributeSet
import androidx.appcompat.app.AppCompatActivity
import panyi.xyz.imageeditorgl.NativeBridge
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

    private lateinit var fileData : SelectFileItem;
    private lateinit var mainView : MainView;

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        mainView = MainView(this)
        setContentView(mainView)
        fileData = intent.getSerializableExtra(INTENT_DATA) as SelectFileItem

        mainView.setContent(fileData.path?:"" , fileData.width , fileData.height)
    }

    override fun onDestroy() {
        mainView.onDestroy()
        super.onDestroy()
    }

    inner class MainView : GLSurfaceView , GLSurfaceView.Renderer{
        constructor(context : Context, attr : AttributeSet?) :super(context , attr)
        constructor(context : Context) :super(context , null)

        init {
            LogUtil.d(TAG,"MainView init")
            setEGLContextClientVersion(3)
            setEGLConfigChooser(8,8 ,8 , 8, 16 , 0)
        }

        override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
            LogUtil.d(TAG,"MainView onSurfaceCreated")
        }

        override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
            LogUtil.d(TAG,"MainView onSurfaceChanged $width x $height")
            NativeBridge.onResize(width , height)
        }

        override fun onDrawFrame(gl: GL10?) {
            // LogUtil.d(TAG,"MainView onDrawFrame")
            NativeBridge.onRender()
        }

        fun setContent(path : String , imgWidth : Int  , imgHeight : Int){
            NativeBridge.setImageContent(path , imgWidth , imgHeight)
            setRenderer(this)
            renderMode = RENDERMODE_CONTINUOUSLY
        }

        fun onDestroy(){
            NativeBridge.onDestroy()
        }
    }//inner class end
}