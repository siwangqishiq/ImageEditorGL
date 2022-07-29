package panyi.xyz.imageeditorgl.model

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.launch
import panyi.xyz.gleditorlib.util.LogUtil

class UserViewModel : ViewModel() {
    companion object {
        private const val loginUrl = "https://baidu.com"
    }

    var name : String ?= null

    fun pullData(){
        LogUtil.d("pull data ...")
        viewModelScope.launch{

        }
    }
}