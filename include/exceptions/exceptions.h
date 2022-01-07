//
// Created by Alex on 05/01/2022.
//


#ifndef SCREENRECLIB_EXCEPTIONS_H
#define SCREENRECLIB_EXCEPTIONS_H
#include <exception>

class SCPPException: public std::exception {
protected:
    const char* exception_message;
public:
    explicit SCPPException(const char* msg): std::exception(), exception_message(msg){};
    const char * what() const noexcept override {
        return exception_message;
    }
};


class openSourceParameterException: public SCPPException{
public:
    explicit openSourceParameterException(const char* msg): SCPPException(msg){};
};

class openSourceException: public SCPPException{
public:
    explicit openSourceException(const char* msg): SCPPException(msg){};
};

class streamInformationException: public SCPPException{
public:
    explicit streamInformationException(const char* msg): SCPPException(msg){};
};

class streamIndexException: public SCPPException{
public:
    explicit streamIndexException(const char* msg): SCPPException(msg){};
};

class findDecoderException: public SCPPException{
public:
    explicit findDecoderException(const char* msg): SCPPException(msg){};
};

class openAVCodecException: public SCPPException{
public:
    explicit openAVCodecException(const char* msg): SCPPException(msg){};
};


#endif //SCREENRECLIB_EXCEPTIONS_H
