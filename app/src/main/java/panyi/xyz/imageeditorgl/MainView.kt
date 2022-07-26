package panyi.xyz.imageeditorgl

import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.opengl.GLSurfaceView
import android.opengl.GLUtils
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.ScaleGestureDetector
import panyi.xyz.imageeditorgl.activity.EditorActivity
import panyi.xyz.imageeditorgl.util.LogUtil
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MainView : GLSurfaceView, GLSurfaceView.Renderer {
    constructor(context : Context, attr : AttributeSet?) :super(context , attr)
    constructor(context : Context) :super(context , null)

    init {
        LogUtil.d(EditorActivity.TAG,"MainView init")
        setEGLContextClientVersion(3)
        setEGLConfigChooser(8,8 ,8 , 8, 16 , 0)
    }

    var path : String?= null

    override fun onTouchEvent(event: MotionEvent?) : Boolean
        = NativeBridge.onTouch(event?.actionMasked?:MotionEvent.ACTION_CANCEL , event?.x?:0.0f , event?.y?:0.0f)

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        LogUtil.d(EditorActivity.TAG,"MainView onSurfaceCreated")
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        LogUtil.d(EditorActivity.TAG,"MainView onSurfaceChanged $width x $height")
        NativeBridge.onResize(width , height)

        val imgBitmap = readImageBitmap(path!!)
        NativeBridge.setImageBitmap(imgBitmap!!)

        NativeBridge.onInit()
    }

    override fun onDrawFrame(gl: GL10?) {
        // LogUtil.d(TAG,"MainView onDrawFrame")
        NativeBridge.onRender()
    }

    fun setContent(path : String , imgWidth : Int  , imgHeight : Int , bitmap : Bitmap?){
        this.path = path

        NativeBridge.setImageContent(path , imgWidth , imgHeight)
        setRenderer(this)
        renderMode = RENDERMODE_WHEN_DIRTY
    }

    fun readImageBitmap(path : String) : Bitmap {
        val options = BitmapFactory.Options()
        return BitmapFactory.decodeFile(path , options)
    }

    fun onDestroy(){
        NativeBridge.onDestroy()
    }
}