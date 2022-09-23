package panyi.xyz.gleditorlib.activity

import android.app.Activity
import android.content.Context
import android.content.Intent
import android.graphics.Bitmap
import android.graphics.Matrix
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.View
import android.widget.Button
import android.widget.RadioGroup
import androidx.appcompat.app.AppCompatActivity
import panyi.xyz.gleditorlib.IEditorModeChangeListener
import panyi.xyz.gleditorlib.MainView
import panyi.xyz.gleditorlib.R
import panyi.xyz.gleditorlib.util.LogUtil
import java.io.File
import java.io.FileOutputStream
import java.io.IOException


/**
 *  EditorActivity base opengl es 3.0
 *
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

    private lateinit var clipBtn : Button

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_editor)

        val exportBtn = findViewById<Button>(R.id.export_btn)
        exportBtn.setOnClickListener{
            exportProcessedBitmap()
        }

        clipBtn = findViewById<Button>(R.id.do_clip_btn)
        clipBtn.setOnClickListener {
            mainView.doClip()
        }

        mainView = findViewById(R.id.editor_view)
        fileData = intent.getStringExtra(INTENT_DATA)?:""
        val path = fileData
        mainView.setContent(path , -1 , -1 , null)

        operatePanel = findViewById(R.id.operate_group)

        operatePanel.setOnCheckedChangeListener { group, checkedId ->
                when (checkedId) {
                    R.id.op_idle -> {
                        mainView.setIdleMode()
                        clipBtn.visibility = View.GONE
                    }
                    R.id.op_mosaic->{
                        mainView.setMosaicMode()
                        clipBtn.visibility = View.GONE
                    }
                    R.id.op_paint->{
                        mainView.setPaintMode()
                        clipBtn.visibility = View.GONE
                    }
                    R.id.op_clip->{
                        mainView.setClipMode()
                        clipBtn.visibility = View.VISIBLE
                    }
                }//end when
        }

        findViewById<View>(R.id.reset_btn).setOnClickListener {
            mainView.resetImage()
        }

        Handler(Looper.getMainLooper()).postDelayed({
            mainView.setModeChangeListener(object : IEditorModeChangeListener{
                override fun onModeChanged() {
                    runOnUiThread{
                        LogUtil.d(TAG , "mode changed!! ${mainView.currentMode()}")
                        when(mainView.currentMode()){
                            0,1,2->{
                                operatePanel.check(R.id.op_idle)
                            }
                        }
                    }
                }
            })
        },1000)

    }

    fun exportProcessedBitmap(){
        val bitWidth = mainView.bridge?.originImageWidth()?:1
        val bitHeight = mainView.bridge?.originImageHeight()?:1

        LogUtil.d(TAG , "export image size: $bitWidth , $bitHeight ")
        mainView.queueEvent{
            val outputBitmap = Bitmap.createBitmap(bitWidth , bitHeight , Bitmap.Config.ARGB_8888)
            val result = mainView.bridge?.exportBitmap(outputBitmap)?:-1
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

    private fun saveBitmap(bitmap:Bitmap, ctx:Context) : String? {
        val savePath:String = "${ctx.cacheDir.absolutePath}/${System.currentTimeMillis()}_img.jpg"
        LogUtil.d(TAG , "savePath : $savePath")
        try {
            val matrix = Matrix()
            matrix.postRotate(-180.0f)
            matrix.postScale(-1.0f , 1.0f , bitmap.width / 2.0f , bitmap.height/2.0f ) //flip
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

    override fun onDestroy() {
        mainView.onDestroy()
        super.onDestroy()
    }

}//end class