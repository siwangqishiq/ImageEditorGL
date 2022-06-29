package panyi.xyz.imageeditorgl.util

import android.util.Log

object LogUtil {
    val DEF_TAG = "LogUtil";

    fun d(tag : String , msg : String?) = Log.d(tag , msg?:"")

    fun d(msg : String?) = d(DEF_TAG , msg)
}