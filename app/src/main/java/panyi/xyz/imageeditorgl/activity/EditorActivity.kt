package panyi.xyz.imageeditorgl.activity

import android.app.Activity
import android.content.Intent
import android.os.Bundle
import android.view.ScaleGestureDetector
import android.view.View
import android.widget.SeekBar
import androidx.appcompat.app.AppCompatActivity
import panyi.xyz.imageeditorgl.MainView
import panyi.xyz.imageeditorgl.R
import panyi.xyz.imageeditorgl.model.SelectFileItem

/**
 *  just for fun
 *
 */
class EditorActivity : AppCompatActivity() {
    companion object{
        const val INTENT_DATA = "_data"
        const val TAG = "MainGLView"
        fun start(context : Activity , data : SelectFileItem){
            val it = Intent(context , EditorActivity::class.java).apply {
                putExtra(INTENT_DATA , data)
            }
            context.startActivity(it)
        }
    }

    private lateinit var fileData : SelectFileItem
    private lateinit var mainView : MainView
    private lateinit var seekBar:SeekBar

    private lateinit var resetBtn : View

    private lateinit var scaleDetect : ScaleGestureDetector

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_editor)

        mainView = findViewById(R.id.editor_view)

        fileData = intent.getSerializableExtra(INTENT_DATA) as SelectFileItem
        val path = fileData.path?:""
        mainView.setContent(path , fileData.width , fileData.height , null)
    }

    override fun onDestroy() {
        mainView.onDestroy()
        super.onDestroy()
    }
}