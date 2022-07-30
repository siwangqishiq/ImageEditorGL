package panyi.xyz.gleditorlib.activity

import android.app.Activity
import android.content.Intent
import android.os.Bundle
import android.widget.Button

import androidx.appcompat.app.AppCompatActivity
import panyi.xyz.gleditorlib.MainView
import panyi.xyz.gleditorlib.NativeBridge
import panyi.xyz.gleditorlib.R
import panyi.xyz.gleditorlib.util.LogUtil

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

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_editor)

        val btn = findViewById<Button>(R.id.complete_btn)
        btn.setOnClickListener{
            onBackPressed()
        }

        mainView = findViewById(R.id.editor_view)
        fileData = intent.getStringExtra(INTENT_DATA)?:""
        val path = fileData
        mainView.setContent(path , -1 , -1 , null)
    }

    override fun onBackPressed() {
        NativeBridge.onDestroy()
        super.onBackPressed()
    }
}