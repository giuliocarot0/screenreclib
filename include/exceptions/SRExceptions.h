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

#endif //SCREENRECLIB_SREXCEPTIONS_H
