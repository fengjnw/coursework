#pragma once
#include <string>
#include "models/Temp.h"

class Candlestick
{
public:
    Candlestick(std::string _date, float _open, float _close, float _high, float _low);
    std::string date;
    float open, close, high, low;
};