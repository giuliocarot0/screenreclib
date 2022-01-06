//
// Created by Alex on 05/01/2022.
//

#include <exception>

#ifndef SCREENRECLIB_EXCEPTIONS_H
#define SCREENRECLIB_EXCEPTIONS_H

class openSourceParameterException: public std::exception{
public:
    explicit openSourceParameterException(const char* msg): std::exception(msg){};
};

class openSourceException: public std::exception{
public:
    explicit openSourceException(const char* msg): std::exception(msg){};
};

class streamInformationException: public std::exception{
public:
    explicit streamInformationException(const char* msg): std::exception(msg){};
};

class streamIndexException: public std::exception{
public:
    explicit streamIndexException(const char* msg): std::exception(msg){};
};

class findDecoderException: public std::exception{
public:
    explicit findDecoderException(const char* msg): std::exception(msg){};
};

class openAVCodecException: public std::exception{
public:
    explicit openAVCodecException(const char* msg): std::exception(msg){};
};


#endif //SCREENRECLIB_EXCEPTIONS_H
