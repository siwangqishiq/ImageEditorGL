package panyi.xyz.gleditorlib.activity

import android.app.Activity
import android.content.Context
import android.content.Intent
import android.graphics.Bitmap
import android.graphics.Color
import android.graphics.Matrix
import android.os.Bundle
import android.view.View
import android.widget.Button
import android.widget.RadioGroup
import android.widget.TextView

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
        fun start(context : Activity , data : String , requestCode : Int){
            val it = Intent(context , EditorActivity::class.java).apply {
                putExtra(INTENT_DATA , data)
            }
            context.startActivityForResult(it , requestCode)
        }
    }

    private lateinit var fileData : String
    private lateinit var mainView : MainView

    private lateinit var operatePanel : RadioGroup

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

        operatePanel = findViewById(R.id.operate_group)

        operatePanel.setOnCheckedChangeListener { group, checkedId ->
                when (checkedId) {
                    R.id.op_idle -> mainView.setIdleMode()
                    R.id.op_mosaic->mainView.setMosaicMode()
                    R.id.op_paint->mainView.setPaintMode()
                    R.id.op_clip->mainView.setClipMode()
                }//end when
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
            val path = saveBitmap(outputBitmap , this@EditorActivity)
            runOnUiThread{
                val it = Intent()
                it.putExtra("path" , path)
                setResult(Activity.RESULT_OK , it)
                finish()
            }
        }
    }

    override fun onBackPressed() {
        NativeBridge.onDestroy()
        super.onBackPressed()
    }

    private fun saveBitmap(bitmap:Bitmap, ctx:Context) : String? {
        val savePath:String = "${ctx.cacheDir.absolutePath}/${System.currentTimeMillis()}_img.jpg"
        LogUtil.d(TAG , "savePath : $savePath")
        try {
            val matrix = Matrix()
            matrix.postRotate(-180.0f)
            matrix.postScale(-1.0f , 1.0f , bitmap.width / 2.0f , bitmap.height/2.0f )
            val scaleBitmap = Bitmap.createScaledBitmap(bitmap , bitmap.width , bitmap.height , true)
            val rotatedBitmap = Bitmap.createBitmap(scaleBitmap , 0 , 0, scaleBitmap.width , scaleBitmap.height ,
                matrix , true)
            val filePic = File(savePath)
            if (!filePic.exists()) {
                filePic.getParentFile().mkdirs()
                filePic.createNewFile()
            }
            val fos = FileOutputStream(filePic);
            rotatedBitmap.compress(Bitmap.CompressFormat.JPEG, 100, fos);
            fos.flush();
            fos.close();
            LogUtil.d(TAG , "save file success! ${filePic.absolutePath}")

            return savePath
        } catch (e: IOException) {
            LogUtil.d(TAG , e.message)
            return null
        }
    }
}