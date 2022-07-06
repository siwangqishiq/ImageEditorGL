package panyi.xyz.imageeditorgl

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
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

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        LogUtil.d(EditorActivity.TAG,"MainView onSurfaceCreated")
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        LogUtil.d(EditorActivity.TAG,"MainView onSurfaceChanged $width x $height")
        NativeBridge.onResize(width , height)
    }

    override fun onDrawFrame(gl: GL10?) {
        // LogUtil.d(TAG,"MainView onDrawFrame")
        NativeBridge.onRender()
    }

    fun setContent(path : String , imgWidth : Int  , imgHeight : Int){
        NativeBridge.setImageContent(path , imgWidth , imgHeight)
        setRenderer(this)
        renderMode = RENDERMODE_WHEN_DIRTY
    }

    fun onDestroy(){
        NativeBridge.onDestroy()
    }
}