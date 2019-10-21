    #include <iostream>
#include "kasiski.h"

void deleteWhiteSpaces(std::string& text) {
    std::string answer;
    for (auto s: text) {
        if (s != ' ' && s != '\n') answer += s;
    }
    text = answer;
}

int main() {
    std::string text, str;
    getline(std::cin, str);
    while (!str.empty()) {
        text += str;
        getline(std::cin, str);
    }
    std::string s = "";
    for (auto c: text) {
        if (('a' <= c && c >= 'z') || ('A' <= c && c >= 'Z')) s += c;
    }
    text = s;
    deleteWhiteSpaces(text);
    std::cout << findPeriod(text) << "\n";
    return 0;
}