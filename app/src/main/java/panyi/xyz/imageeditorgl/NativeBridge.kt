package panyi.xyz.imageeditorgl

object NativeBridge {
    init {
        System.loadLibrary("imageeditorgl")
    }

    external fun setImageContent(path : String , imgW:Int , imgH : Int)

    external fun onResize(viewWidth:Int , viewHeight:Int)

    external fun onRender()

    external fun onDestroy()
}