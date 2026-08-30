package com.wzvideni.ffmpegstudy.data

import android.net.Uri

data class VideoInfo(
    val uri: Uri,
    val path: String,
    val name: String,
    val format: String,
    val size: Long,
    val duration: Long
)