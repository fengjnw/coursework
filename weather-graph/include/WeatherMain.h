#pragma once
#include "models/Temp.h"
#include "models/TempSet.h"

class WeatherMain
{
public:
    WeatherMain();
    void init();

private:
    void printMenu();
    void candlestickData();
    void candlestickPlot();
    void filterPlot();
    void filterPlotLine();
    void filterPlotBar();
    void predictPlot();
    int getUserOption();
    void processUserOption(int userOption);
    std::vector<Candlestick> inputToCandlesticks(std::string input);

    TempSet tempSet{"../data/weather_data_EU_1980-2019_temp_only.csv"};
};