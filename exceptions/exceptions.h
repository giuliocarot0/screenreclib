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


#endif //SCREENRECLIB_EXCEPTIONS_H
