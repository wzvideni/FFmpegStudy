package com.wzvideni.ffmpegstudy.expansion

import android.content.ContentUris
import android.content.Context
import android.provider.MediaStore
import com.wzvideni.ffmpegstudy.data.VideoInfo


fun queryAllVideos(context: Context): List<VideoInfo> {
    val videoList = mutableListOf<VideoInfo>()
    val collection = MediaStore.Video.Media.EXTERNAL_CONTENT_URI

    val projection = arrayOf(
        MediaStore.Video.Media._ID,
        MediaStore.Video.Media.DATA,
        MediaStore.Video.Media.TITLE,
        MediaStore.Video.Media.MIME_TYPE,
        MediaStore.Video.Media.SIZE,
        MediaStore.Video.Media.DURATION
    )

    val sortOrder = "${MediaStore.Video.Media.DATE_ADDED} DESC"

    context.contentResolver.query(collection, projection, null, null, sortOrder)?.use { cursor ->
        val idColumn = cursor.getColumnIndexOrThrow(MediaStore.Video.Media._ID)
        val nameColumn = cursor.getColumnIndexOrThrow(MediaStore.Video.Media.TITLE)
        val mimeTypeColumn = cursor.getColumnIndexOrThrow(MediaStore.Video.Media.MIME_TYPE)
        val sizeColumn = cursor.getColumnIndexOrThrow(MediaStore.Video.Media.SIZE)
        val durationColumn = cursor.getColumnIndexOrThrow(MediaStore.Video.Media.DURATION)

        while (cursor.moveToNext()) {
            val id = cursor.getLong(idColumn)
            val uri = ContentUris.withAppendedId(collection, id)
            val path = cursor.getString(cursor.getColumnIndexOrThrow(MediaStore.Video.Media.DATA))

            val name = cursor.getString(nameColumn)
            val format = cursor.getString(mimeTypeColumn).split("/").last()
            val size = cursor.getLong(sizeColumn)
            val duration = cursor.getLong(durationColumn)

            videoList.add(
                VideoInfo(
                    uri = uri,
                    path = path,
                    name = name,
                    format = format,
                    size = size,
                    duration = duration
                )
            )
        }
    }

    return videoList
}
