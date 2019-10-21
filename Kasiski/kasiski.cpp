//
// Created by kranya on 20.10.2019.
//

#include <cstring>
#include <map>
#include <iostream>
#include "kasiski.h"

size_t gcd(size_t a, size_t b) {
    if (b == 0) return a;
    else return gcd(b, a % b);
}

size_t findPeriod(const std::string& text) {
    std::map<std::string, size_t> lastTrigramPos, curTrigramGcd;
    for (size_t i = 0; i < int(text.size()) - 2; i++) {
        std::string trigram = text.substr(i, 3);
        if (lastTrigramPos.find(trigram) != lastTrigramPos.end()) {
            size_t potentialPeriod = i - lastTrigramPos[trigram];
            if (curTrigramGcd.find(trigram) != curTrigramGcd.end()) {
                curTrigramGcd[trigram] = gcd(curTrigramGcd[trigram], potentialPeriod);
            } else {
                curTrigramGcd[trigram] = potentialPeriod;
            }
        }
        lastTrigramPos[trigram] = i;
    }

    std::map<size_t, size_t> mostPopularPeriod;

    for (const auto& period: curTrigramGcd) {
        if (mostPopularPeriod.find(period.second) != mostPopularPeriod.end()) {
            mostPopularPeriod[period.second]++;
        } else {
            mostPopularPeriod[period.second] = 1;
        }
    }

    size_t maxPeriodCount = 0;
    size_t maxPeriod = 0;

    for (const auto& period: mostPopularPeriod) {
        if (maxPeriodCount <= period.second) {
            maxPeriodCount = period.second;
            maxPeriod = period.first;
        }
    }

    return maxPeriod;
}