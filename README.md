```plantuml
Interface SCPPInput {
    - AVCodec* inCodec
    - AVFormatContext *inFormatContext
    

    + open()
    + getCodec()
    + getFormatContext()
    + getStreamIndex()
}


Class SCPPVideoInput {
    - AVCodec* inCodec
    - AVFormatContext *inFormatContext
    

    + open()
    + getCodec()
    + getFormatContext()
    + getStreamIndex()
}

Class SCPPAudioInput {
    - AVCodec* inCodec
    - AVFormatContext *inFormatContext
    

    + open()
    + getCodec()
    + getFormatContext()
    + getStreamIndex()
}

SCPPInput -> SCPPVideoInput

SCPPInput -> SCPPAudioInput
```