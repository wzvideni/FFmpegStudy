package com.wzvideni.ffmpegstudy

import android.Manifest
import android.os.Bundle
import android.os.Environment
import android.widget.Toast
import androidx.activity.ComponentActivity
import androidx.activity.compose.rememberLauncherForActivityResult
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.activity.result.contract.ActivityResultContracts
import androidx.compose.foundation.gestures.detectTapGestures
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Refresh
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.Scaffold
import androidx.compose.runtime.derivedStateOf
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clip
import androidx.compose.ui.input.pointer.pointerInput
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.platform.LocalDensity
import androidx.compose.ui.unit.dp
import coil3.compose.AsyncImage
import coil3.request.ImageRequest
import coil3.request.crossfade
import coil3.video.videoFrameMillis
import com.wzvideni.ffmpegstudy.data.VideoInfo
import com.wzvideni.ffmpegstudy.expansion.queryAllVideos
import com.wzvideni.ffmpegstudy.ui.theme.FFmpegStudyTheme
import java.io.File


class MainActivity : ComponentActivity() {
    companion object {
        val ffmpegNative by lazy { FFmpegNative() }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        initFFmpeg()
        setContent {
            val density = LocalDensity.current.density
            val context = LocalContext.current
            val screenWidthPx = LocalContext.current.resources.displayMetrics.widthPixels

            val displaySize by remember { derivedStateOf { (screenWidthPx / density / 3).dp } }

            var videoInfoList by remember { mutableStateOf<List<VideoInfo>>(emptyList()) }

            val publicMusic by remember {
                mutableStateOf(
                    Environment.getExternalStoragePublicDirectory(
                        Environment.DIRECTORY_MUSIC
                    )
                )
            }

            val requestPermission =
                rememberLauncherForActivityResult(ActivityResultContracts.RequestMultiplePermissions()) { granted ->
                    videoInfoList = queryAllVideos(context)
                }
            FFmpegStudyTheme {
                Scaffold(
                    modifier = Modifier.fillMaxSize(),
                    floatingActionButton = {
                        IconButton(
                            onClick = {
                                requestPermission.launch(
                                    listOf(
                                        Manifest.permission.READ_MEDIA_VIDEO,
                                        Manifest.permission.READ_MEDIA_AUDIO
                                    ).toTypedArray()
                                )
                            }
                        ) {
                            Icon(Icons.Default.Refresh, contentDescription = null)
                        }
                    }
                ) { innerPadding ->
                    LazyColumn(
                        modifier = Modifier
                            .fillMaxSize()
                            .padding(innerPadding)
                    ) {
                        items(videoInfoList.chunked(3)) { rowItems ->
                            Row {
                                rowItems.forEach { item ->
                                    AsyncImage(
                                        model = ImageRequest.Builder(context)
                                            .data(item.uri)
                                            .videoFrameMillis(0)
                                            .crossfade(true)
                                            .build(),
                                        contentDescription = null,
                                        contentScale = ContentScale.Crop,
                                        modifier = Modifier
                                            .size(displaySize)
                                            .clip(RoundedCornerShape(8.dp))
                                            .pointerInput(Unit) {
                                                detectTapGestures {
                                                    val result = ffmpegNative.extractAudio(
                                                        item.path,
                                                        File(
                                                            publicMusic,
                                                            item.name
                                                        ).absolutePath
                                                    )
                                                    Toast.makeText(
                                                        context,
                                                        "提取${if (result) "成功" else "失败"}",
                                                        Toast.LENGTH_SHORT
                                                    ).show()
                                                }
                                            }
                                    )
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    private fun initFFmpeg() {
        ffmpegNative.initFFmpeg()
//        Log.d("FFmpegNative", "FFmpeg Version ${ffmpegNative.ffmpegVersion()}")
//        Log.d("FFmpegNative", "Supported Encoders")
//        Log.d("FFmpegNative", ffmpegNative.getSupportedEncoders().joinToString("、"))
//        Log.d("FFmpegNative", "Supported Decoders")
//        Log.d("FFmpegNative", ffmpegNative.getSupportedDecoders().joinToString("、"))
    }
}