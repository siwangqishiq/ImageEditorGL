package panyi.xyz.imageeditorgl.activity

import android.app.Activity
import android.content.Context
import android.content.Intent
import android.os.Bundle
import androidx.activity.result.contract.ActivityResultContract
import androidx.appcompat.app.AppCompatActivity
import panyi.xyz.imageeditorgl.R
import panyi.xyz.imageeditorgl.fragment.FileSelectFragment
import panyi.xyz.imageeditorgl.model.SelectFileItem

class SelectFileActivity :  AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_select_file)
        supportFragmentManager.beginTransaction().replace(R.id.container, FileSelectFragment()).commitNow()
    }

    class PickImageActivityContent : ActivityResultContract<Int, SelectFileItem?>() {
        override fun createIntent(context: Context, reqCode: Int?): Intent {
            return Intent(context , SelectFileActivity::class.java)
        }

        override fun parseResult(resultCode: Int, retData: Intent?): SelectFileItem? {
            if (resultCode != Activity.RESULT_OK) {
                return null
            }

            return retData?.getSerializableExtra("data") as SelectFileItem
        }
    }
}