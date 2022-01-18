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


class openSourceParameterException: public SRException{
public:
    explicit openSourceParameterException(const char* msg): SRException(msg){};
};

class openSourceException: public SRException{
public:
    explicit openSourceException(const char* msg): SRException(msg){};
};

class streamInformationException: public SRException{
public:
    explicit streamInformationException(const char* msg): SRException(msg){};
};

class streamIndexException: public SRException{
public:
    explicit streamIndexException(const char* msg): SRException(msg){};
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

class DeviceNotOpenException: public SRException{
public:
    explicit DeviceNotOpenException(const char* msg): SRException(msg){};
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

#endif //SCREENRECLIB_SREXCEPTIONS_H
