#pragma once
#include <string>

struct Timestamp
{
    /** sum means adding up timestamp parts as value like "YYYYMMDDHH", make it easier for comparison */
    unsigned int year, month, day, hour, sum;
    /** str stores the original string format timestamp, for output*/
    std::string str;
};

class Temp
{
public:
    Temp(Timestamp _timestamp, std::string _country, float _tempValue);

    Timestamp timestamp;
    std::string country;
    float tempValue;

    /** Utility functions to read and convert data */
    static Timestamp stringToTimestamp(std::string s);
    /** Convert the country columns into a key storing country name */
    static std::string indexToCountry(int i);

    static bool compareByTimestamp(Temp &t1, Temp &t2);
};