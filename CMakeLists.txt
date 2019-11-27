cmake_minimum_required(VERSION 3.15)
project(Cryptography)

set(CMAKE_CXX_STANDARD 17)

add_executable(Kasiski Kasiski/main.cpp Kasiski/kasiski.h Kasiski/kasiski.cpp)

add_executable(DES DES/main.cpp DES/des.h DES/des.cpp DES/des.h DES/des.cpp DES/main.cpp DES/constants.h comp128/main.cpp)

add_executable(RC4 RC4/rc4.cpp)

add_executable(RC6 RC6/rc6.cpp)

add_executable(comp128 comp128/main.cpp)

add_executable(RSA RSA/main.cpp)