#include "models/TempSet.h"
#include "utils/CSVReader.h"
#include <vector>
#include <algorithm>
#include <map>
#include <iomanip>
#include <iostream>

TempSet::TempSet(std::string file)
{
    tempSet = CSVReader::readCSV(file);
    // Debug: Print the number of temperatures loaded
    std::cout << "TempSet loaded " << tempSet.size() << " temperatures from file: " << file << std::endl;
}

std::vector<Temp> TempSet::getTemps(int timeA, int timeB, std::string country)
{
    int begin, end;
    /** Handle the time range */
    begin = std::min(timeA, timeB);
    end = std::max(timeA, timeB);

    std::vector<Temp> temps;

    /** Retrieve from tempSet */
    for (Temp &t : tempSet)
    {
        bool match = true;

        if (t.timestamp.sum < begin || t.timestamp.sum > end)
            match = false;
        if (t.country != country)
            match = false;
        if (match)
            temps.push_back(t);
    }
    std::sort(temps.begin(), temps.end(), Temp::compareByTimestamp);
    return temps;
}

std::vector<Temp> TempSet::getTemps(int timeA, int timeB, float tempA, float tempB, std::string country)
{
    int begin, end;
    float low, high;
    /** Handle the time range */
    begin = std::min(timeA, timeB);
    end = std::max(timeA, timeB);
    /** Handle the temp range */
    low = std::min(tempA, tempB);
    high = std::max(tempA, tempB);

    std::vector<Temp> temps;

    /** Retrieve from tempSet */
    for (Temp &t : tempSet)
    {
        bool match = true;

        if (t.timestamp.sum < begin || t.timestamp.sum > end)
            match = false;
        if (t.tempValue < low || t.tempValue > high)
            match = false;
        if (t.country != country)
            match = false;
        if (match)
            temps.push_back(t);
    }
    std::sort(temps.begin(), temps.end(), Temp::compareByTimestamp);
    return temps;
}

std::vector<Temp> TempSet::getTemps(int timeA, int timeB, float tempA, float tempB)
{
    int begin, end;
    float low, high;
    /** Handle the time range */
    begin = std::min(timeA, timeB);
    end = std::max(timeA, timeB);
    /** Handle the temp range */
    low = std::min(tempA, tempB);
    high = std::max(tempA, tempB);

    std::vector<Temp> temps;

    /** Retrieve from tempSet */
    for (Temp &t : tempSet)
    {
        bool match = true;

        if (t.timestamp.sum < begin || t.timestamp.sum > end)
            match = false;
        if (t.tempValue < low || t.tempValue > high)
            match = false;
        if (match)
            temps.push_back(t);
    }
    std::sort(temps.begin(), temps.end(), Temp::compareByTimestamp);
    return temps;
}

float TempSet::getMean(const std::vector<Temp> &temps)
{
    float sum = 0.0f;
    for (const Temp &t : temps)
    {
        sum += t.tempValue;
    }
    return sum / temps.size();
}

float TempSet::getHigh(const std::vector<Temp> &temps)
{
    float high = temps.front().tempValue;
    for (const Temp &t : temps)
    {
        high = std::max(high, t.tempValue);
    }
    return high;
}

float TempSet::getLow(const std::vector<Temp> &temps)
{
    float low = temps.front().tempValue;
    for (const Temp &t : temps)
    {
        low = std::min(low, t.tempValue);
    }
    return low;
}

std::map<std::string, std::vector<Temp>> TempSet::divideTemps(const std::vector<Temp> &temps, std::string period)
{
    std::map<std::string, std::vector<Temp>> tempsGroup;
    for (const auto &temp : temps)
    {
        std::string key = std::to_string(temp.timestamp.year);
        if (period == "month")
            key += "-" + std::string(temp.timestamp.month < 10 ? "0" : "") + std::to_string(temp.timestamp.month);
        if (period == "day")
            key += "-" + std::string(temp.timestamp.month < 10 ? "0" : "") + std::to_string(temp.timestamp.month) +
                   "-" + std::string(temp.timestamp.day < 10 ? "0" : "") + std::to_string(temp.timestamp.day);

        tempsGroup[key].push_back(temp);
    }
    return tempsGroup;
}

Candlestick TempSet::computeCandlestick(const std::string &key, const std::vector<Temp> &temps)
{
    if (temps.empty())
    {
        throw std::invalid_argument("Input temps vector is empty.");
    }
    return Candlestick(key, temps[0].tempValue, getMean(temps), getHigh(temps), getLow(temps));
}

std::vector<Candlestick> TempSet::groupCandlesticks(const std::map<std::string, std::vector<Temp>> &tempsGroup)
{
    std::vector<Candlestick> candlesticks;
    for (const auto &entry : tempsGroup)
    {
        const std::string &key = entry.first;
        const std::vector<Temp> &temps = entry.second;

        Candlestick candlestick = computeCandlestick(key, temps);
        candlesticks.push_back(candlestick);
    }
    /** Change the open value of each candlestick into the close value of the previous one */
    if (candlesticks.size() >= 1)
    {
        for (int i = 1; i < candlesticks.size(); ++i)
        {
            candlesticks[i].open = candlesticks[i - 1].close;
        }
    }
    return candlesticks;
}

std::vector<Candlestick> TempSet::getCandlesticks(int timeA, int timeB, std::string country, std::string period)
{
    /** Filter temps data */
    std::vector<Temp> temps = getTemps(timeA, timeB, country);
    /** Divide the data by period */
    std::map<std::string, std::vector<Temp>> tempsGroup = divideTemps(temps, period);
    /** Compute and group candlesticks */
    std::vector<Candlestick> candlesticks = groupCandlesticks(tempsGroup);

    return candlesticks;
}

void TempSet::printCandlesticks(const std::vector<Candlestick> &candlesticks)
{
    std::cout << std::left << std::setw(15) << "Date"
              << std::setw(10) << "Open"
              << std::setw(10) << "High"
              << std::setw(10) << "Low"
              << std::setw(10) << "Close" << std::endl;

    std::cout << std::string(55, '-') << std::endl;

    for (const auto &c : candlesticks)
    {
        std::cout << std::left << std::setw(15) << c.date
                  << std::setw(10) << c.open
                  << std::setw(10) << c.high
                  << std::setw(10) << c.low
                  << std::setw(10) << c.close << std::endl;
    }
}

void TempSet::plotCandlesticks(const std::vector<Candlestick> &candlesticks, int maxWidth)
{
    /** Set height and chars */
    const int chartHeight = 40;
    const char candleBody = '=';
    const char candleHighLow = '|';

    /** ANSI escape codes for colors */
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string RESET = "\033[0m";

    /** Skip some data if too large to display */
    int step = candlesticks.size() / maxWidth * 2 + 1;

    /** Find the range of temps to scale the chart */
    float max = candlesticks.front().high;
    float min = candlesticks.front().low;
    for (const auto &c : candlesticks)
    {
        max = std::max(max, c.high);
        min = std::min(min, c.low);
    }
    float range = max - min;
    if (range <= 0)
    {
        throw std::invalid_argument("Error: Invalid temperature range.");
        return;
    }
    float scale = chartHeight / range;

    /** Plot row by row */
    /** Two empty row */
    std::cout << "         ^" << std::endl;
    std::cout << "         |" << std::endl;
    for (int row = chartHeight; row >= 0; --row)
    {
        /** Plot the y axis */
        float tempLevel = min + row / scale;
        std::cout << std::setw(8) << std::fixed << std::setprecision(3) << tempLevel << " | ";
        /** Plot each candlestick for this row */
        for (int i = 0; i < candlesticks.size(); i += step)
        {
            auto &c = candlesticks[i];
            /** Choose color based on open and close */
            const std::string &color = (c.close >= c.open) ? GREEN : RED;

            /** In the temp range */
            if (tempLevel >= c.low && tempLevel <= c.high)
            {
                /** Inside the body */
                if (tempLevel >= std::min(c.open, c.close) && tempLevel <= std::max(c.open, c.close))
                {
                    std::cout << " " << color << candleBody << RESET;
                }
                else
                /** High or low */
                {
                    std::cout << " " << color << candleHighLow << RESET;
                }
            }
            else
            /** Empty place */
            {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }
    /** Plot the x axis */
    std::cout << "         |" << std::endl;
    std::cout << "         |" << std::endl;
    std::cout << "         +";
    for (size_t i = 1; i < candlesticks.size() / step * 2 + 10; ++i)
    {
        std::cout << '-';
    }
    std::cout << '>' << std::endl;
    std::cout << "          " << candlesticks.front().date << std::string(static_cast<size_t>((candlesticks.size() / step * 2 - 13)), ' ') << candlesticks.back().date << std::endl;
}

void TempSet::plotLine(const std::vector<Temp> &temps, const std::vector<Temp> &timeRange, int maxWidth)
{
    /** Set height and chars */
    const int chartHeight = 40;
    const char pointChar = '*';
    const char gapChar = '`';

    /** ANSI escape codes for colors */
    const std::string RED = "\033[31m";
    const std::string RESET = "\033[0m";

    /** Skip some data if too large to display */
    int step = timeRange.size() / maxWidth + 1;

    /** Find the range of temps to scale the chart */
    float max = temps.front().tempValue;
    float min = max;
    for (const auto &t : temps)
    {
        max = std::max(max, t.tempValue);
        min = std::min(min, t.tempValue);
    }
    float range = max - min;
    if (range <= 0)
    {
        throw std::invalid_argument("Error: Invalid temperature range.");
    }
    float scale = static_cast<float>(chartHeight) / range;

    /** Plot row by row */
    std::cout << "         ^" << std::endl;
    std::cout << "         |" << std::endl;
    for (int row = chartHeight; row >= 0; --row)
    {
        /** Plot the y axis */
        float tempLevel = min + static_cast<float>(row) / scale;
        std::cout << std::setw(8) << std::fixed << std::setprecision(3) << tempLevel << " | ";
        /** Plot each column */
        for (size_t i = 0; i < timeRange.size(); i += step)
        {
            auto it = std::find_if(temps.begin(), temps.end(), [&](const Temp &t)
                                   { return t.timestamp.sum == timeRange[i].timestamp.sum; });
            if (it != temps.end())
            {
                if (std::abs(it->tempValue - tempLevel) < 1 / (scale * 2))
                {
                    std::cout << pointChar;
                }
                else
                {
                    std::cout << " ";
                }
            }
            else
            /** Plot in red if data is missing */
            {
                std::cout << RED << gapChar << RESET;
            }
        }
        std::cout << std::endl;
    }

    /** Plot the x axis */
    std::cout << "         +";
    for (size_t i = 0; i < timeRange.size() / step + 5; ++i)
    {
        std::cout << '-';
    }
    std::cout << '>' << std::endl;

    /** Plot time range labels */
    std::string frontLabel = timeRange.front().timestamp.str;
    std::string backLabel = timeRange.back().timestamp.str;
    size_t padding = std::max(static_cast<int>(timeRange.size() / step - frontLabel.size() - backLabel.size() + 4), 1);

    std::cout << "         " << frontLabel
              << std::string(padding, ' ') << backLabel << std::endl;
}

void TempSet::plotBar(const std::vector<Temp> &temps, int maxWidth)
{
    /** Set and chars */
    const char barChar = ']';

    /** ANSI escape codes for colors */
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string RESET = "\033[0m";

    /** Count items of various countries */
    std::map<std::string, int> countryCount;
    for (const auto &temp : temps)
    {
        countryCount[temp.country]++;
    }

    /** Find the max value and scale rate */
    int maxCount = 0;
    int sum = 0;
    for (const auto &[country, count] : countryCount)
    {
        maxCount = std::max(maxCount, count);
        sum += count;
    }
    float mean = sum / static_cast<float>(countryCount.size());
    float scale = static_cast<float>(maxWidth) / maxCount;

    /** Plot bars */
    std::cout << "         | " << std::endl;
    std::cout << "         | " << std::endl;
    for (const auto &[country, count] : countryCount)
    {
        std::cout << std::setw(8) << country << " | ";
        int scaledCount = static_cast<int>(count * scale);
        const std::string &color = (count < mean) ? RED : GREEN;
        for (int i = 0; i < scaledCount; ++i)
        {
            std::cout << color << barChar;
        }
        std::cout << " (" << count << ") " << RESET << std::endl;
        std::cout << std::string(8, ' ') << " | " << std::endl;
    }

    /** Plot the x axis */
    std::cout << "         +";
    for (int i = 0; i < maxWidth + 10; ++i)
    {
        std::cout << '-';
    }
    std::cout << ">" << std::endl;
}

void TempSet::plotPredict(const std::vector<Temp> &temps, const std::vector<float> &prediction, int maxWidth)
{
    /** Set height and chars */
    const int chartHeight = 40;
    const char pointChar = '*';
    const char predictChar = '*';

    /** ANSI escape codes for colors */
    const std::string GREEN = "\033[32m";
    const std::string RESET = "\033[0m";

    /** Skip some data if too large to display */
    int step = prediction.size() / maxWidth + 1;

    /** Find the range of temps to scale the chart */
    float max = temps.front().tempValue;
    float min = max;
    for (const auto &t : temps)
    {
        max = std::max(max, t.tempValue);
        min = std::min(min, t.tempValue);
    }
    for (const auto &p : prediction)
    {
        max = std::max(max, p);
        min = std::min(min, p);
    }
    float range = max - min;
    if (range <= 0)
    {
        throw std::invalid_argument("Error: Invalid temperature range.");
    }
    float scale = static_cast<float>(chartHeight) / range;

    /** Plot row by row */
    std::cout << "         ^" << std::endl;
    std::cout << "         |" << std::endl;
    for (int row = chartHeight; row >= 0; --row)
    {
        /** Plot the y axis */
        float tempLevel = min + static_cast<float>(row) / scale;
        std::cout << std::setw(8) << std::fixed << std::setprecision(3) << tempLevel << " | ";
        /** Plot each column */
        for (size_t i = 0; i < prediction.size(); i += step)
        {
            /** If it is actual data */
            if (i < temps.size())
            {
                if (std::abs(temps[i].tempValue - tempLevel) < 1 / (scale * 2))
                {
                    std::cout << pointChar;
                }
                else
                {
                    std::cout << " ";
                }
            }
            /** If it is prediction */
            else
            {
                if (std::abs(prediction[i] - tempLevel) < 1 / (scale * 2))
                {
                    std::cout << GREEN << predictChar << RESET;
                }
                else
                {
                    std::cout << " ";
                }
            }
        }
        std::cout << std::endl;
    }

    /** Plot the x axis */
    std::cout << "         +";
    for (size_t i = 0; i < prediction.size() / step + 5; ++i)
    {
        std::cout << '-';
    }
    std::cout << '>' << std::endl;

    /** Plot time range labels */
    std::string frontLabel = temps.front().timestamp.str;
    std::string backLabel = temps.back().timestamp.str;
    size_t padding = std::max(static_cast<int>(temps.size() / step - frontLabel.size() - backLabel.size() + 4), 1);

    std::cout << "         " << frontLabel
              << std::string(padding, ' ') << backLabel << std::endl;
}

std::vector<float> TempSet::computeEMA(const std::vector<Temp> &temps, int predictPoints)
{
    /** Set params of the EMA formula */
    int recentPoints = 168;
    float alpha = 0.2;
    float max = temps.front().tempValue;
    float min = max;
    for (const auto &t : temps)
    {
        max = std::max(max, t.tempValue);
        min = std::min(min, t.tempValue);
    }
    float tempRange = max - min;
    /** Adjust the noise factor by the range of data */
    float noiseFactor = 0.2f * tempRange;

    std::vector<float> ema;
    if (temps.empty())
    {
        return ema;
    }

    /** Initial the first element */
    ema.push_back(temps.front().tempValue);

    /** Compute the EMA value */
    for (int i = 1; i < temps.size(); ++i)
    {
        float nextEMA = alpha * temps[i].tempValue + (1 - alpha) * ema.back();
        ema.push_back(nextEMA);
    }

    /** Predict with EMA values */
    for (int i = 0; i < predictPoints; ++i)
    {
        /** Compute the recent trend, and limit its range */
        float trend = computeTrend(temps, recentPoints);
        trend = std::min(2.0f, std::max(-2.0f, trend));
        /** Add noise to avoid constant prediction*/
        float noise = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * noiseFactor;

        float predictedValue = ema.back() + trend + noise;

        float nextEMA = alpha * predictedValue + (1 - alpha) * ema.back();
        ema.push_back(nextEMA);
    }
    return ema;
}

float TempSet::computeTrend(const std::vector<Temp> &temps, int recentPoints)
{
    if (temps.size() < recentPoints || recentPoints < 2)
    {
        /** Not enough points */
        return 0.0f;
    }

    /** Least squares method */
    int begin = temps.size() - recentPoints;
    float sumX = 0.0f, sumY = 0.0f, sumXY = 0.0f, sumX2 = 0.0f;
    for (int i = 0; i < recentPoints; ++i)
    {
        int x = i;
        float y = temps[begin + i].tempValue;
        sumX += x;
        sumY += y;
        sumXY += x * y;
        sumX2 += x * x;
    }
    float slope = (recentPoints * sumXY - sumX * sumY) / (recentPoints * sumX2 - sumX * sumX);
    return slope;
}