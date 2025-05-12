#include "models/Temp.h"
#include <sstream>
#include <iostream>

Temp::Temp(Timestamp _timestamp, std::string _country, float _tempValue) : timestamp(_timestamp), country(_country), tempValue(_tempValue)
{
}

Timestamp Temp::stringToTimestamp(std::string s)
{
    Timestamp ts{};
    ts.str = s;
    char t, z;
    int i;
    std::istringstream ss(s);

    ss >> ts.year >> t >> ts.month >> t >> ts.day >> t >> ts.hour >> t >> i >> t >> i >> z;

    if (ss.fail() || z != 'Z')
    {
        throw std::invalid_argument("Temp::stringToTimestamp -> Invalid timestamp format");
    }
    /** An easy way to compare time, since the range of the int type is still enough */
    ts.sum = ts.year * 1000000 + ts.month * 10000 + ts.day * 100 + ts.hour;

    return ts;
}

std::string Temp::indexToCountry(int i)
{
    const std::string countryName[] = {"AT", "BE", "BG", "CH", "CZ", "DE", "DK", "EE", "ES", "FI", "FR", "GB", "GR", "HR", "HU", "IE", "IT", "LT", "LU", "LV", "NL", "NO", "PL", "PT", "RO", "SE", "SI", "SK"};
    return countryName[i];
}

bool Temp::compareByTimestamp(Temp &t1, Temp &t2)
{
    return t1.timestamp.sum < t2.timestamp.sum;
}