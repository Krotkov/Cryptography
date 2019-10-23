//
// Created by kranya on 21.10.2019.
//

#include <iostream>
#include "des.h"

int main(int argc, char** argv) {
    std::string text, key;

    std::cin >> text >> key;
    for (char c: text) {
        std::cout << int(c) << " ";
    }
    std::cout << std::endl;


    std::string code = codeText(text, key);

    std::cout << code << std::endl;
    for (char c: code) {
        std::cout << int(c) << " ";
    }
    std::cout << std::endl;

    std::string encode = encodeText(code, key);

    std::cout << encode << std::endl;
    for (char c: encode) {
        std::cout << int(c) << " ";
    }
    std::cout << std::endl;

    return 0;
}
