#include <iostream>
#include <exceptions.h>

int main() {
    try {
        char* prova = "nop";
        std::string msg= "Cannot open selected device (";
        msg += prova;
        msg += ")";
        throw openSourceException(msg.c_str());
    }catch (std::exception& e){
        std::cout<< e.what() <<std::endl;
    }
    return 0;
}
