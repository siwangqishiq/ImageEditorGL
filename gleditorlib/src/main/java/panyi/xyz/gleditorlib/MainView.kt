package panyi.xyz.gleditorlib

import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.view.MotionEvent
import panyi.xyz.gleditorlib.util.LogUtil
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MainView : GLSurfaceView, GLSurfaceView.Renderer {
    companion object{
        const val TAG:String = "MainView"
    }

    constructor(context : Context, attr : AttributeSet?) :super(context , attr)
    constructor(context : Context) :super(context , null)

    init {
        LogUtil.d(TAG,"MainView init")
        setEGLContextClientVersion(3)
        setEGLConfigChooser(8,8 ,8 , 8, 16 , 0)
    }

    override fun queueEvent(r: Runnable?) {
        super.queueEvent(r)
    }

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec)
        LogUtil.d(TAG , "measure width height $measuredWidth $measuredHeight");
    }

    var path : String?= null

    override fun onTouchEvent(event: MotionEvent) : Boolean{
        // LogUtil.d(TAG , "point count ${event.pointerCount}")
        var x = 0.0f
        var y = 0.0f
        var x2 = 0.0f
        var y2 = 0.0f

        if(event.pointerCount > 1){
            val pFirstIndex = event.getPointerId(0)
            val pSecondIndex = event.getPointerId(1)

            x = event.getX(pFirstIndex)
            y = measuredHeight - event.getY(pFirstIndex)

            x2 = event.getX(pSecondIndex)
            y2 = measuredHeight - event.getY(pSecondIndex)

            // LogUtil.d(TAG , "p1->p2   ($x , $y) -> ($x2 , $y2)")
        }else{//one finger
            x = event.x
            y = measuredHeight - event.y
        }
//        requestRender()
        return NativeBridge.onTouch(event.actionMasked , x, y, x2 , y2)
    }

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        LogUtil.d(TAG,"MainView onSurfaceCreated")
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        LogUtil.d(TAG,"MainView onSurfaceChanged $width x $height")
        NativeBridge.onResize(width, height)

        val imgBitmap = readImageBitmap(path!!)
        NativeBridge.setImageBitmap(imgBitmap)

        NativeBridge.onInit()
    }

    override fun onDrawFrame(gl: GL10?) {
        // LogUtil.d(TAG,"MainView onDrawFrame")
        NativeBridge.onRender()
    }

//    override fun onDetachedFromWindow() {
//        super.onDetachedFromWindow()
//    }

    fun setContent(path : String , imgWidth : Int  , imgHeight : Int , bitmap : Bitmap?){
        this.path = path

        setRenderer(this)
        renderMode = RENDERMODE_CONTINUOUSLY
    }

    fun readImageBitmap(path : String) : Bitmap {
        val options = BitmapFactory.Options()
        return BitmapFactory.decodeFile(path , options)
    }

    fun setPaintMode() = NativeBridge.setPaintMode()

    fun setIdleMode() = NativeBridge.setIdleMode()
}