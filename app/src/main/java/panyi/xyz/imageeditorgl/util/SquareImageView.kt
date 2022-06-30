package panyi.xyz.imageeditorgl.util

import android.content.Context
import android.util.AttributeSet
import androidx.appcompat.widget.AppCompatImageView

class SquareImageView : AppCompatImageView {
    constructor(context : Context , attr : AttributeSet? , defStyleAttr : Int) :super(context , attr , defStyleAttr)
    constructor(context : Context , attr : AttributeSet?) :super(context , attr , 0)
    constructor(context : Context) :super(context , null , 0)

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        super.onMeasure(widthMeasureSpec, widthMeasureSpec)
    }
}