package panyi.xyz.imageeditorgl.fragment

import android.app.Activity
import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.provider.MediaStore
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.bumptech.glide.Glide
import panyi.xyz.imageeditorgl.R
import panyi.xyz.imageeditorgl.model.SelectFileItem
import panyi.xyz.imageeditorgl.util.LogUtil
import java.io.File

class FileSelectFragment : Fragment() {
    private val mFileItemList: MutableList<SelectFileItem> = ArrayList<SelectFileItem>(8)
    private lateinit var mListView: RecyclerView

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View? {
        val v: View = inflater.inflate(R.layout.fragment_file_select_list, null)
        loadVideoFileData()
        return v
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        mListView = requireView().findViewById<RecyclerView>(R.id.file_list_view)
        mListView.setLayoutManager(GridLayoutManager(context, 3))
        mListView.setAdapter(FileItemListAdapter())
    }

    private fun loadVideoFileData() {
        mFileItemList.clear()
        mFileItemList.addAll(queryPictureData())
    }

    private fun onClickItem(pos: Int, fileItem: SelectFileItem) {
        val result = Intent()
        result.putExtra("data", fileItem)
        requireActivity().setResult(Activity.RESULT_OK, result)
        requireActivity().finish()
    }

    fun queryPictureData() : List<SelectFileItem> {
        LogUtil.d("query picture file start")
        val result: MutableList<SelectFileItem> = java.util.ArrayList(16)

        val projection = arrayOf(
            MediaStore.Images.Media.DATA,
            MediaStore.Images.Media.DISPLAY_NAME,
            MediaStore.Images.Media.SIZE,
            MediaStore.Images.Media.WIDTH,
            MediaStore.Images.Media.HEIGHT,
            MediaStore.Images.Media.DATE_MODIFIED,
            MediaStore.Images.Media.BUCKET_ID
        )

        val cur = requireContext().contentResolver.query(
            MediaStore.Images.Media.EXTERNAL_CONTENT_URI,
            projection, null, null,
            MediaStore.Video.Media.DATE_MODIFIED + " DESC"
        )

        if (cur != null) {
            if (cur.moveToFirst()) {
                while (!cur.isAfterLast) {
                    val item = SelectFileItem()
                    item.name = cur.getString(cur.getColumnIndexOrThrow(MediaStore.Images.Media.DISPLAY_NAME))
                    item.path = cur.getString(cur.getColumnIndexOrThrow(MediaStore.Images.Media.DATA))
                    item.width = cur.getInt(cur.getColumnIndexOrThrow(MediaStore.Images.Media.WIDTH))
                    item.height = cur.getInt(cur.getColumnIndexOrThrow(MediaStore.Images.Media.HEIGHT))
                    item.size = cur.getLong(cur.getColumnIndexOrThrow(MediaStore.Images.Media.SIZE))
                    result.add(item)
                    cur.moveToNext()
                }
            }
            cur.close()
        }
        LogUtil.d("query video file end")
        return result
    }

    /**
     *
     */
    inner class FileItemListAdapter : RecyclerView.Adapter<FileItemViewHolder>() {
        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): FileItemViewHolder {
            val itemView: View = LayoutInflater.from(requireContext()).inflate(
                R.layout.view_select_file_item,
                parent, false
            )
            return FileItemViewHolder(itemView)
        }

        override fun onBindViewHolder(holder: FileItemViewHolder, position: Int) {
            holder.refresh(position, mFileItemList.get(position))
        }

        override fun getItemCount(): Int {
            return mFileItemList.size
        }
    }

    inner class FileItemViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        var imgView: ImageView
        var descTextView: TextView

        init {
            imgView = itemView.findViewById(R.id.file_image)
            descTextView = itemView.findViewById(R.id.file_desc)
        }

        fun refresh(pos: Int, itemData: SelectFileItem) {
            descTextView.text = itemData.name
            Glide.with(requireActivity()).load(Uri.fromFile(File(itemData.path))).into(imgView)
            itemView.setOnClickListener { v: View? ->
                onClickItem(
                    pos,
                    itemData
                )
            }
        }
    } //end inner class

}