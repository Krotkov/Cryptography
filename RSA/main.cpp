//
// Created by kranya on 27.11.2019.
//
#include <bits/stdc++.h>

using namespace std;

bool check_prime(uint64_t p) {
    for (uint64_t i = 2; i*i <= p; i++) {
        if (p % i == 0) return false;
    }
    return true;
}


uint64_t random_prime() {
    uint64_t p = rand() % 10000;
    while (!check_prime(p)) {
        p++;
    }
    return p;
}

uint64_t find_e(uint64_t fi) {
    uint64_t a = rand() % fi;
    while (__gcd(fi, a) != 1) a++;
    return a;
}

uint64_t find_d(uint64_t d, uint64_t fi) {
    uint64_t c = 2;
    while ((c * d) % fi != 1) {
        c++;
    }
    return c;
}

uint64_t bin_pow(uint64_t a, uint64_t b, uint64_t mo) {
    if (b == 1) {
        return a % mo;
    } else if (b % 2 == 1) {
        return (bin_pow(a, b - 1, mo) * a) % mo;
    } else {
        uint64_t ans = bin_pow(a, b / 2, mo) % mo;
        return (ans * ans) % mo;
    }
}

uint64_t encAndDec(uint64_t text, uint64_t d, uint64_t n) {
    return bin_pow(text, d, n) % n;
}


int main() {
    uint64_t text;
    cin >> text;

    tryAgainForAlice:

    uint64_t pa = random_prime();
    uint64_t qa = random_prime();
    uint64_t na = pa * qa;
    uint64_t fia = (pa - 1) * (qa - 1);
    uint64_t da = 1;
    uint64_t ca;
    while (true) {
        da = find_e(fia);
        if (da >= fia || da >= 10000) {
            goto tryAgainForAlice;
        }
        ca = find_d(da, fia);
        if (ca != -1)
            break;
    }

    tryAgainForBob:

    uint64_t pb = random_prime();
    uint64_t qb = random_prime();
    uint64_t nb = pb * qb;
    uint64_t fib = (pb - 1) * (qb - 1);
    uint64_t db = 1;
    uint64_t cb;
    while (true) {
        db = find_e(fib);
        if (db >= fib || db >= 10000) {
            goto tryAgainForBob;
        }
        cb = find_d(db, fib);
        if (cb != -1)
            break;
    }
    uint64_t firstEncStep = encAndDec(text, ca, na);
    uint64_t textEncForBob = encAndDec(firstEncStep, db, nb);

    uint64_t firstDecStep = encAndDec(textEncForBob, cb, nb);
    uint64_t textDecBob = encAndDec(firstDecStep, da, na);
    cout << text << ' ' << textEncForBob << ' ' << textDecBob << '\n';
}