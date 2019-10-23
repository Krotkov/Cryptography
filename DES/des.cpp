//
// Created by kranya on 21.10.2019.
//

#include <bitset>
#include <vector>
#include <algorithm>
#include "des.h"
#include "constants.h"

class Bitvect : public std::vector<bool> {
public:
    Bitvect() : std::vector<bool>() {}

    explicit Bitvect(const std::string& text) : std::vector<bool>() {
        for (char c: text) {
            auto bits = std::bitset<8>(c);
            for (int i = 7; i >= 0; i--) {
                this->push_back(bits[i] == 1);
            }
        }
    }

    explicit Bitvect(const std::bitset<8>& bits) : std::vector<bool>() {
        for (int i = 7; i >= 0; i--) {
            this->push_back(bits[i] == 1);
        }
    }

    explicit Bitvect(char num) : std::vector<bool>() {
        auto bits = std::bitset<8>(num);
        for (int i = 7; i >= 0; i--) {
            this->push_back(bits[i] == 1);
        }
    }

    explicit Bitvect(int num) : std::vector<bool>() {
        while (num != 0) {
            this->push_back(num % 2 == 1);
            num /= 2;
        }
        std::reverse(this->begin(), this->end());
    }

    Bitvect& operator+=(const Bitvect& other) {
        for (auto c: other) {
            this->push_back(c);
        }
        return *this;
    }

    Bitvect& operator+=(const bool& a) {
        this->push_back(a);
        return *this;
    }

    [[nodiscard]] Bitvect getInterval(size_t a, size_t b) const {
        Bitvect answer;
        for (size_t i = a; i < b ;i++) {
            answer += (*this)[i];
        }
        return answer;
    }

    [[nodiscard]] size_t countTrue() const {
        size_t ans = 0;
        for (auto a: (*this)) {
            if (a) ans++;
        }
        return ans;
    }

    void push_front(bool a) {
        Bitvect ans;
        ans.push_back(a);
        ans += (*this);
        (*this) = ans;
    }

    std::string to_string() {
        std::string ans;
        for (size_t i = 0; i < this->size(); i += 8) {
            char c = 0;
            for (size_t j = 0; j < 8; j++) {
                c = (c * 2) + (*this)[i+j];
            }
            ans += char(c);
        }
        return ans;
    }

    void insert(size_t pos, bool val) {
        Bitvect left = (*this).getInterval(0, pos);
        left.push_back(val);
        Bitvect right = (*this).getInterval(pos, this->size());
        left += right;
        (*this) = left;
    }

    void leftShift(int shift) {
        for (int i = 0; i < shift; i++) {
            Bitvect new_bitvect = this->getInterval(1, this->size());
            new_bitvect.push_back((*this)[0]);
            (*this) = new_bitvect;
        }
    }
};

Bitvect toExpand(const Bitvect& bits) {
    Bitvect new_bits;
    for (int i : expansion) {
        new_bits.push_back(bits[i]);
    }
    return new_bits;
}

Bitvect cXor(const Bitvect& a, const Bitvect& b) {
    Bitvect ans;
    for (size_t i = 0; i < a.size(); i++) {
        ans += (a[i] ^ b[i]);
    }
    return ans;
}

Bitvect feystelFunction(const Bitvect& bits, const Bitvect& key) {
    Bitvect exp_bits = toExpand(bits);

    Bitvect b_bits = cXor(exp_bits, key);

    Bitvect coded_bits;

    for (int i = 0; i < 8; i++) {
        Bitvect b = b_bits.getInterval(i * 6, i * 6 + 6);
        size_t row = b[0] * 2 + b[5];
        size_t column = ((b[1] * 2 + b[2]) * 2 + b[3]) * 2 + b[4];
        b = Bitvect(s_convert[i][row][column]);
        while (b.size() != 4) {
            b.push_front(false);
        }
        coded_bits += b;
    }

    Bitvect ans;

    ans.resize(32, false);

    for (size_t i = 0; i < 32; i++) {
        ans[i] = coded_bits[p_per[i] - 1];
    }

    return ans;
}

std::vector<Bitvect> generateKeys(Bitvect key) {
    for (size_t i = 0; i < key.size(); i += 8) {
        if (key.getInterval(i, i + 7).countTrue() % 2 == 0) {
            key.insert(7, 1);
        } else {
            key.insert(7, 0);
        }
    }
    Bitvect c_bits, d_bits;
    for (size_t i = 0; i < 28; i++) {
        c_bits.push_back(key[com_key[i] - 1]);
    }
    for (size_t i = 28; i < 56; i++) {
        d_bits.push_back(key[com_key[i] - 1]);
    }
    std::vector<Bitvect> keys;
    for (int i : shift_table) {
        Bitvect cd_bits = (c_bits += d_bits); //pizdec
        Bitvect new_key;
        for (int j : key_comp) {
            new_key.push_back(cd_bits[j - 1]);
        }
        keys.emplace_back(new_key);
        c_bits.leftShift(i);
        d_bits.leftShift(i);
    }
    return keys;
}

Bitvect feystelConvert(const Bitvect& bits, const Bitvect& key) {
    Bitvect left = bits.getInterval(0, 32);
    Bitvect right = bits.getInterval(32, 64);
    Bitvect f = feystelFunction(right, key);
    return (right += cXor(left, f));
}

Bitvect codeBlock(const Bitvect& block, const Bitvect& key) {
    Bitvect per_block;

    std::vector<Bitvect> keys = generateKeys(key);

    per_block.resize(64, false);
    for (int i = 0; i < 64; i++) {
        per_block[i] = block[initial_perm[i] - 1];
    }
    for (int i = 0; i < 16; i++) {
        per_block = feystelConvert(per_block, keys[i]);
    }
    Bitvect ans_block;
    ans_block.resize(64, false);
    for (int i = 0; i < 64; i++) {
        ans_block[initial_perm[i] - 1] = per_block[i];
    }
    return ans_block;
}

std::string codeText(const std::string& text, const std::string& key) {

    Bitvect bit_text = Bitvect(text);

    size_t text_size = text.size();

    // Дополняем строку до размера кратного 8.
    while (text_size % 8 != 0) {
        bit_text += Bitvect('0');
        text_size++;
    }

    Bitvect bit_key = Bitvect(key);

    Bitvect coded_text;

    int i = 0;

    while (i < bit_text.size()) {
        coded_text += codeBlock(bit_text.getInterval(i, i + 64), bit_key);
        i += 64;
    }

    return coded_text.to_string();
}

Bitvect RfeystelConvert(const Bitvect& bits, const Bitvect& key) {
    Bitvect left = bits.getInterval(0, 32);
    Bitvect right = bits.getInterval(32, 64);
    return (cXor(right, feystelFunction(left, key)) += left);
}

Bitvect encodeBlock(const Bitvect& block, const Bitvect& key) {
    Bitvect per_block;

    std::vector<Bitvect> keys = generateKeys(key);

    per_block.resize(64, false);
    for (int i = 0; i < 64; i++) {
        per_block[i] = block[initial_perm[i] - 1];
    }
    for (int i = 15; i >= 0; i--) {
        per_block = RfeystelConvert(per_block, keys[i]);
    }
    Bitvect ans_block;
    ans_block.resize(64, false);
    for (int i = 0; i < 64; i++) {
        ans_block[initial_perm[i] - 1] = per_block[i];
    }
    return ans_block;
}

std::string encodeText(const std::string& code, const std::string& key) {
    Bitvect bit_code = Bitvect(code);

    Bitvect bit_key = Bitvect(key);

    Bitvect encoded_text;

    int i = 0;

    while (i < bit_code.size()) {
        encoded_text += encodeBlock(bit_code.getInterval(i, i + 64), bit_key);
        i += 64;
    }

    return encoded_text.to_string();
}