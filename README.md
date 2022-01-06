## Class Diagram

```plantuml
Interface SCPPInput {
    - AVCodec* inCodec
    - AVFormatContext *inFormatContext
    - AVPacket* deliverable_packet

    + SCPPInput()
    + open() 
    + getCodec()
    + getFormatContext()
    + getStreamIndex()
    + readPacket()
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