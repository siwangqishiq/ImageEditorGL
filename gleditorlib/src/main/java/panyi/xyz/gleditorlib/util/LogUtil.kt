package panyi.xyz.gleditorlib.util

import android.util.Log

object LogUtil {
    private const val DEF_TAG = "LogUtil";

    fun d(tag : String , msg : String?) = Log.d(tag , "${Thread.currentThread().name} - ${msg?:""}")

    fun d(msg : String?) = d(DEF_TAG , msg)
}