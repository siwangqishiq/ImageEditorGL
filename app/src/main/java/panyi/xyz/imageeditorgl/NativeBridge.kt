package panyi.xyz.imageeditorgl

import android.graphics.Bitmap

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

    external fun onTouch(action:Int , x :Float , y : Float) : Boolean
}