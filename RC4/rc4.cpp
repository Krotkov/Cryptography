//
// Created by kranya on 14.11.2019.
//

#include <iostream>

using namespace std;

string text, key;

int k[256];


string gen(const string &s) {
    for (int i = 0; i < 256; ++i) {
        k[i] = i;
    }
    int j = 0;
    for (int i = 0; i < 256; ++i) {
        j = (j + k[i] + key[i % key.size()]) % 256;
        swap(k[i], k[j]);
    }
    int i = 0;
    j = 0;
    string ret;
    for (char t : s) {
        i = (i + 1) % 256;
        j = (j + k[i]) % 256;
        swap(k[i], k[j]);
        ret += t ^ k[(k[i] + k[j]) % 256];
    }
    return ret;
}

int main() {
    text = "I want to krotk";
    key = "Yourke";
    string cur = gen(text);
    cout << cur << std::endl;
    cout << gen(cur) << std::endl;
    return 0;
}