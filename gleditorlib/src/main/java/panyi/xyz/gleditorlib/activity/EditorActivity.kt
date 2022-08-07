package panyi.xyz.gleditorlib.activity

import android.app.Activity
import android.content.Context
import android.content.Intent
import android.graphics.Bitmap
import android.os.Bundle
import android.view.View
import android.widget.Button

import androidx.appcompat.app.AppCompatActivity
import panyi.xyz.gleditorlib.MainView
import panyi.xyz.gleditorlib.NativeBridge
import panyi.xyz.gleditorlib.R
import panyi.xyz.gleditorlib.util.LogUtil
import java.io.File
import java.io.FileOutputStream
import java.io.IOException

/**
 *  just for fun
 *
 */
class EditorActivity : AppCompatActivity() {
    companion object{
        const val INTENT_DATA = "_data"
        const val TAG = "MainGLView"
        fun start(context : Activity , data : String){
            val it = Intent(context , EditorActivity::class.java).apply {
                putExtra(INTENT_DATA , data)
            }
            context.startActivity(it)
        }
    }

    private lateinit var fileData : String
    private lateinit var mainView : MainView

    private var paintMode = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_editor)

        val btn = findViewById<Button>(R.id.complete_btn)
        btn.setOnClickListener{
            onBackPressed()
        }


        val exportBtn = findViewById<Button>(R.id.export_btn)
        exportBtn.setOnClickListener{
            exportProcessedBitmap()
        }

        mainView = findViewById(R.id.editor_view)
        fileData = intent.getStringExtra(INTENT_DATA)?:""
        val path = fileData
        mainView.setContent(path , -1 , -1 , null)

        findViewById<View>(R.id.paint_action).setOnClickListener {
            if(paintMode){
                mainView.setIdleMode()
                paintMode = false
            }else{
                mainView.setPaintMode()
                paintMode = true
            }
        }

        findViewById<View>(R.id.reset_btn).setOnClickListener {
            mainView.resetImage()
        }
    }

    fun exportProcessedBitmap(){
        val bitWidth = NativeBridge.originImageWidth()
        val bitHeight = NativeBridge.originImageHeight()

        LogUtil.d(TAG , "export image size: $bitWidth , $bitHeight ")
        mainView.queueEvent{
            val outputBitmap = Bitmap.createBitmap(bitWidth , bitHeight , Bitmap.Config.ARGB_8888)
            val result = NativeBridge.exportBitmap(outputBitmap)
            if(result < 0){
                LogUtil.d(TAG , "bitmap copy error")
                return@queueEvent
            }
            saveBitmap(outputBitmap , this@EditorActivity)
        }
    }

    override fun onBackPressed() {
        NativeBridge.onDestroy()
        super.onBackPressed()
    }

    private fun saveBitmap(bitmap:Bitmap, ctx:Context) {
        val savePath:String = "${ctx.cacheDir.absolutePath}/${System.currentTimeMillis()}_img.jpg"
        LogUtil.d(TAG , "savePath : $savePath")
        try {
            val filePic = File(savePath)
            if (!filePic.exists()) {
                filePic.getParentFile().mkdirs()
                filePic.createNewFile()
            }
            val fos = FileOutputStream(filePic);
            bitmap.compress(Bitmap.CompressFormat.JPEG, 100, fos);
            fos.flush();
            fos.close();
            LogUtil.d(TAG , "save file success! ${filePic.absolutePath}")
        } catch (e: IOException) {
            LogUtil.d(TAG , e.message)
            return;
        }
    }
}