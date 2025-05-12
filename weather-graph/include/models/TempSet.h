#pragma once
#include "models/Temp.h"
#include <string>
#include <vector>
#include "models/Candlestick.h"
#include <map>

class TempSet
{
public:
    TempSet(std::string file);

    std::vector<Temp> getTemps(int timeA, int timeB, std::string country);
    std::vector<Temp> getTemps(int timeA, int timeB, float tempA, float tempB, std::string country);
    std::vector<Temp> getTemps(int timeA, int timeB, float tempA, float tempB);

    /** Functions for candlestick */
    /** Group candlestick functions */
    std::vector<Candlestick> getCandlesticks(int begin, int end, std::string country, std::string period);
    /** Print candlesticks data to the console */
    static void printCandlesticks(const std::vector<Candlestick> &candlesticks);
    /** Plot candlesticks in text */
    static void plotCandlesticks(const std::vector<Candlestick> &candlesticks, int maxWidth);
    /** Plot others */
    static void plotLine(const std::vector<Temp> &temps, const std::vector<Temp> &timeRange, int maxWidth);
    static void plotBar(const std::vector<Temp> &temps, int maxWidth);
    static void plotPredict(const std::vector<Temp> &temps, const std::vector<float> &prediction, int maxWidth);
    /** EMA function for prediction */
    static std::vector<float> computeEMA(const std::vector<Temp> &temps, int predictPoints);

private:
    std::vector<Temp> tempSet;
    /** Compute required field */
    static float getMean(const std::vector<Temp> &temps);
    static float getHigh(const std::vector<Temp> &temps);
    static float getLow(const std::vector<Temp> &temps);
    /** Divide temps by period */
    static std::map<std::string, std::vector<Temp>> divideTemps(const std::vector<Temp> &temps, std::string period);
    /** Compute single line of candlestick from a period of temps */
    static Candlestick computeCandlestick(const std::string &key, const std::vector<Temp> &temps);
    /** Group candlesticks into a set, and handle the open value */
    static std::vector<Candlestick> groupCandlesticks(const std::map<std::string, std::vector<Temp>> &tempsGroup);
    /** Compute the trend of recent data to assist predicting */
    static float computeTrend(const std::vector<Temp> &temps, int recentPoints);
};