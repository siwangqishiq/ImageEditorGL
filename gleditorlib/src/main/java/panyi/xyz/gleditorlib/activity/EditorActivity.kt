package panyi.xyz.gleditorlib.activity

import android.app.Activity
import android.content.Intent
import android.os.Bundle
import android.view.ScaleGestureDetector
import android.view.View
import android.widget.SeekBar
import androidx.appcompat.app.AppCompatActivity
import panyi.xyz.gleditorlib.R

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
    private lateinit var mainView : panyi.xyz.gleditorlib.MainView
    private lateinit var seekBar:SeekBar

    private lateinit var resetBtn : View

    private lateinit var scaleDetect : ScaleGestureDetector

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_editor)

        mainView = findViewById(R.id.editor_view)

        fileData = intent.getStringExtra(INTENT_DATA)?:""
        val path = fileData
        mainView.setContent(path , -1 , -1 , null)
    }

    override fun onDestroy() {
        mainView.onDestroy()
        super.onDestroy()
    }
}