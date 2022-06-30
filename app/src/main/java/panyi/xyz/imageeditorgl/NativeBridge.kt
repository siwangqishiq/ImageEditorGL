package panyi.xyz.imageeditorgl

object NativeBridge {
    init {
        System.loadLibrary("imageeditorgl")
    }

    external fun sayHello() : String
}