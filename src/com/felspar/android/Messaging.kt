package com.felspar.android

import android.os.Handler
import android.os.Message
import org.json.JSONObject

interface MessageRecipient {
    fun process(msg : JSONObject)
}
internal var application : Messaging? = null

class Messaging(a : MessageRecipient) : Handler() {
    var app : MessageRecipient;

    init {
        app = a
    }

    override fun handleMessage(msg : Message) {
        if (msg.obj is String) {
            val raw = msg.obj as String
            val payload = JSONObject(raw)
            app.process(payload)
        }
    }

    companion object {
        @JvmStatic
        external fun registerApplication(r : Messaging)
    }
}

fun RegisterApplication(r : MessageRecipient) {
    val msging = Messaging(r)
    application = msging
    Messaging.registerApplication(msging)
}
