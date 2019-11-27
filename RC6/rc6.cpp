#include <iostream>
#include <string>
#include <vector>

using namespace std;

string key, s1;

vector<unsigned int> L_int, S_int;

int get_bit_char(char x, int i) {
    return ((x >> (7 - i)) & 1);
}

int get_bit_int(unsigned int x, int i) {
    return ((x >> (31 - i)) & 1);
}

string to_bitstring(string s) {
    string sl = "";
    for (int i = 0; i < s.size(); i++)
        for (int j = 0; j < 7; j++)
            if (get_bit_char(s[i], j))
                sl += '1';
            else
                sl += '0';
    return sl;
}

string to_bitstring(unsigned int t) {
    string sl = "";
    for (int i = 0; i < 32; i++)
        if (get_bit_int(t, i))
            sl += '1';
        else
            sl += '0';

    return sl;
}

int left_rot(unsigned int a, unsigned int b){
    b <<= 32 - 5;
    b >>= 32 - 5;
    return (a << b) | (a >> (5 - b));
}

unsigned int right_rot(unsigned int a, unsigned int b){
    b <<= 32 - 5;
    b >>= 32 - 5;
    return (a >> b) | (a << (5 - b));
}

void gen_key() {
    string cur = to_bitstring(key);
    while (cur.size() % 32 != 0)
        cur += '0';
    unsigned int P = 0xB7E15163;
    unsigned int Q = 0x9E3779B9;
    for (int i = 0; i < cur.size(); i += 32) {
        unsigned int l = 0;
        for (int j = i; j < i + 32; j++)
            if (cur[j])
                l = ((l << 1) + 1);
            else
                l = (l << 1);
        L_int.push_back(l);
    }
    S_int.push_back(P);
    unsigned int val = P;
    for (int i = 1; i < 2 * 20 + 4; i++) {
        val += Q;
        S_int.push_back(val);
    }
    unsigned int A = 0;
    unsigned int B = 0;
    unsigned int i = 0;
    unsigned int j = 0;
    int c = (L_int.size());
    int v = 3 * max(c , 2 * 20 + 4);
    for (int q = 1; q < v; q++) {
        A = S_int[i] = left_rot((S_int[i] + A + B), 3);
        B = L_int[j] = left_rot((L_int[j] + A + B), (A + B));
        i = (i + 1) % (2 * 20 + 4);
        j = (j + 1) % c;
    }
}

std::string to_str(unsigned int a) {
    std::string ans;
    ans += char(a >> 24);
    ans += char(a >> 16);
    ans += char(a >> 8);
    ans += a;
    return ans;
}

std::string ecnrypt(std::string text) {
    unsigned int A;
    for (int i = 0; i < 4; i++) {
        A = (A << 8) + text[i];
    }
    unsigned int B;
    for (int i = 4; i < 8; i++) {
        B = (B << 8) + text[i];
    }
    unsigned int C;
    for (int i = 8; i < 12; i++) {
        C = (C << 8) + text[i];
    }
    unsigned int D;
    for (int i = 12; i < 16; i++) {
        D = (D << 8) + text[i];
    }

    B = B + S_int[0];
    D = D + S_int[1];

    for (int i = 1; i < 20; i++) {
        unsigned int t = left_rot(B * (B * 2 + 1), 5);
        unsigned int u = left_rot(D * (D * 2 + 1), 5);
        A = left_rot(A ^ t, u) + S_int[2*i];
        C = left_rot(C ^ u, t) + S_int[2*i+1];
        unsigned int f = A;
        A = B, B = C, C = D, D = f;
    }

    A = A + S_int[2*20+2];
    C = C + S_int[2*20+3];

    std::string ans;

    ans += to_str(A);
    ans += to_str(B);
    ans += to_str(C);
    ans += to_str(D);
    return ans;
}

std::string decode(std::string text) {
    unsigned int A;
    for (int i = 0; i < 4; i++) {
        A = (A << 8) + text[i];
    }
    unsigned int B;
    for (int i = 4; i < 8; i++) {
        B = (B << 8) + text[i];
    }
    unsigned int C;
    for (int i = 8; i < 12; i++) {
        C = (C << 8) + text[i];
    }
    unsigned int D;
    for (int i = 12; i < 16; i++) {
        D = (D << 8) + text[i];
    }

    A = A - S_int[2*20+2];
    C = C - S_int[2*20+3];

    for (int i = 19; i >= 1; i--) {
        unsigned int t = left_rot(B * (B * 2 + 1), 5);
        unsigned int u = left_rot(D * (D * 2 + 1), 5);
        A = right_rot((A - S_int[2*i]) ^ t, u);
        C = right_rot((C - S_int[2*i+1]) ^ u, t);
        unsigned int f = A;
        A = B, B = C, C = D, D = f;
    }

    B = B - S_int[0];
    D = D - S_int[1];

    std::string sl;

    sl += to_str(A);
    sl += to_str(B);
    sl += to_str(C);
    sl += to_str(D);
    return s1;
}

int main() {
    key = "qaqawsedrftgyhuj";
    gen_key();

    std::string text = "abacabababaqwert";
    std::cout << text << std::endl;
    s1 = text;
    std::string code = ecnrypt(text);
    std::cout << code << std::endl;
    std::cout << decode(code) << std::endl;
    return 0;
}