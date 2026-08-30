package com.wzvideni.ffmpegstudy.expansion

import java.text.SimpleDateFormat
import java.util.Date
import java.util.Locale

fun Long.dateTime(): String =
    SimpleDateFormat("yyyy-MM-dd_HH-mm-ss", Locale.getDefault()).format(Date(this))