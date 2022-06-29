package panyi.xyz.imageeditorgl

import android.app.Activity
import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import panyi.xyz.imageeditorgl.fragment.FileSelectFragment

class SelectFileActivity :  AppCompatActivity() {
    companion object{
        fun start(context : Activity){
            val it = Intent(context , SelectFileActivity::class.java)
            context.startActivity(it)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_select_file)
        supportFragmentManager.beginTransaction().replace(R.id.container , FileSelectFragment()).commitNow()
    }
}