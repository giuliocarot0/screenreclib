```plantuml
Interface SCPPInput {
    - AVCodec* inCodec
    - AVFormatContext *inFormatContext
    

    +SCPPInput()
    + open() 
    + getCodec()
    + getFormatContext()
    + getStreamIndex()
}


Class SCPPVideoInput {
    + SCPPVideoInput()
    + open()
 
}

Class SCPPAudioInput {
    + SCPPAudioInput()
    + open()
}

SCPPInput -> SCPPVideoInput

SCPPInput -> SCPPAudioInput
```