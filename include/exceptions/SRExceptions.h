//
// Created by Alex on 05/01/2022.
//


#ifndef SCREENRECLIB_SREXCEPTIONS_H
#define SCREENRECLIB_SREXCEPTIONS_H
#include <exception>

class SRException: public std::exception {
protected:
    const char* exception_message;
public:
    explicit SRException(const char* msg): std::exception(), exception_message(msg){};
    const char * what() const noexcept override {
        return exception_message;
    }
};

class SRNonJoinableException: public SRException{
public:
    explicit SRNonJoinableException(const char* msg): SRException(msg){};
};
class SRDeviceDictionaryException: public SRException{
public:
    explicit SRDeviceDictionaryException(const char* msg): SRException(msg){};
};

class SRDeviceException: public SRException{
public:
    explicit SRDeviceException(const char* msg): SRException(msg){};
};

class SRStreamInformationException: public SRException{
public:
    explicit SRStreamInformationException(const char* msg): SRException(msg){};
};

class SRStreamIndexException: public SRException{
public:
    explicit SRStreamIndexException(const char* msg): SRException(msg){};
};

class findDecoderException: public SRException{
public:
    explicit findDecoderException(const char* msg): SRException(msg){};
};

class openAVCodecException: public SRException{
public:
    explicit openAVCodecException(const char* msg): SRException(msg){};
};

class ConfigurationParserException: public SRException{
public:
    explicit ConfigurationParserException(const char* msg): SRException(msg){};
};

class SRNullInputException: public SRException{
public:
    explicit SRNullInputException(const char* msg): SRException(msg){};
};
class SRFilterException: public SRException{
public:
    explicit SRFilterException(const char* msg): SRException(msg){};
};
class DecoderException: public SRException{
public:
    explicit DecoderException(const char* msg): SRException(msg){};
};

class EncoderException: public SRException{
public:
    explicit EncoderException(const char* msg): SRException(msg){};
};

class UninitializedFilterException: public SRException{
public:
    explicit UninitializedFilterException(const char* msg): SRException(msg){};
};

class CropperException: public SRException{
public:
    explicit CropperException(const char* msg): SRException(msg){};
};


class InvalidFilterParametersException: public SRException{
public:
    explicit InvalidFilterParametersException(const char* msg): SRException(msg){};
};

class BufferAllocationException: public SRException{
public:
    explicit BufferAllocationException(const char* msg): SRException(msg){};
};

class OutputFormatException: public SRException{
public:
    explicit OutputFormatException(const char* msg): SRException(msg){};
};

class OutputContextAllocationException: public SRException{
public:
    explicit OutputContextAllocationException(const char* msg): SRException(msg){};
};

class FileOpeningException: public SRException{
public:
    explicit FileOpeningException(const char* msg): SRException(msg){};
};

class NoValidStreamsException: public SRException{
public:
    explicit NoValidStreamsException(const char* msg): SRException(msg){};
};

class OutputHeaderWritingException: public SRException{
public:
    explicit OutputHeaderWritingException(const char* msg): SRException(msg){};
};

class StreamException: public SRException{
public:
    explicit StreamException(const char* msg): SRException(msg){};
};

class FindEncoderException: public SRException{
public:
    explicit FindEncoderException(const char* msg): SRException(msg){};
};

class AVCodecAllocationException: public SRException{
public:
    explicit AVCodecAllocationException(const char* msg): SRException(msg){};
};

class CodecOpeningException: public SRException{
public:
    explicit CodecOpeningException(const char* msg): SRException(msg){};
};

class NoFreeStreamException: public SRException{
public:
    explicit NoFreeStreamException(const char* msg): SRException(msg){};
};

class SRNullFrameException: public SRException{
public:
    explicit SRNullFrameException(const char* msg): SRException(msg){};
};

class SRNullPacketException: public SRException{
public:
    explicit SRNullPacketException(const char* msg): SRException(msg){};
};

class SRNullContextException: public SRException{
public:
    explicit SRNullContextException(const char* msg): SRException(msg){};
};
#endif //SCREENRECLIB_SREXCEPTIONS_H
