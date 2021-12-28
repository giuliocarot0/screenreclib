## Class Diagram

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

Class SCPPDecoder {
    - inputCodecContext
    - options
    
    + decode(AVPacket* encodedPkt)
}

Class SCPPEncoder {
    - outputCodecContext
    - options
    + decode(AVPacket* encodedPkt)
}

Interface SCPPTranscoder {
    - encoder
    - decoder 

    
    + transcode(AVPacket* encodedPkt)
}

SCPPInput --> SCPPVideoInput
SCPPInput -> SCPPAudioInput
```