package panyi.xyz.gleditorlib

import android.graphics.Bitmap

/**
 *  java -> c++
 *
 */
object NativeBridge {
    init {
        System.loadLibrary("imageeditorgl")
    }

    external fun setImageContent(path : String , imgW:Int , imgH : Int)

    external fun setImageBitmap(imageBitmap : Bitmap)

    external fun onResize(viewWidth:Int , viewHeight:Int)

    external fun onInit()

    external fun onRender()

    external fun onDestroy()

    external fun scale(scale:Float)

    external fun onTouch(action:Int , x :Float , y : Float , x2 : Float , y2 : Float) : Boolean

    external fun exportBitmap(output : Bitmap) : Int

    external fun originImageWidth() : Int

    external fun originImageHeight() : Int

    external fun setPaintMode()

    external fun setIdleMode()
}