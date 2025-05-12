#include "WeatherMain.h"
#include <iostream>
#include <sstream>
#include "models/Temp.h"

WeatherMain::WeatherMain()
{
}

void WeatherMain::init()
{
    int input;
    while (true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}

void WeatherMain::printMenu()
{
    std::cout << "===========================================" << std::endl;
    std::cout << "1. Get candlestick data" << std::endl;
    std::cout << "2. Draw a candlestick plot" << std::endl;
    std::cout << "3. Filter data and draw a plot" << std::endl;
    std::cout << "4. Make prediction and draw a plot" << std::endl;
    std::cout << "===========================================" << std::endl;
}

int WeatherMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << "Type in 1-5" << std::endl;
    std::getline(std::cin, line);
    try
    {
        userOption = std::stoi(line);
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void WeatherMain::processUserOption(int userOption)
{
    if (userOption < 1 || userOption > 5) // bad input
    {
        std::cout << "Invalid choice. Choose 1-5" << std::endl;
    }
    if (userOption == 1)
    {
        candlestickData();
    }
    if (userOption == 2)
    {
        candlestickPlot();
    }
    if (userOption == 3)
    {
        filterPlot();
    }
    if (userOption == 4)
    {
        predictPlot();
    }
    else if (userOption == 5)
    {
        std::cout << "Exiting the application. Goodbye!" << std::endl;
        exit(0); // Exit the application
    }
}

/** Convert user input into params, and get candlestick data */
std::vector<Candlestick> WeatherMain::inputToCandlesticks(std::string input)
{
    std::vector<Candlestick> candlesticks;

    std::istringstream inputStream(input);
    std::string timeAStr, timeBStr, country, period;
    std::getline(inputStream, timeAStr, ',');
    std::getline(inputStream, timeBStr, ',');
    std::getline(inputStream, country, ',');
    std::getline(inputStream, period, ',');
    /** A lamda function to remove spaces before and after string */
    auto trim = [](std::string &s)
    {
        s.erase(0, s.find_first_not_of(" \t\""));
        s.erase(s.find_last_not_of(" \t\"") + 1);
    };
    trim(timeAStr);
    trim(timeBStr);
    trim(country);
    trim(period);
    try
    {
        int timeA = std::stoi(timeAStr);
        int timeB = std::stoi(timeBStr);

        candlesticks = tempSet.getCandlesticks(timeA, timeB, country, period);
    }
    catch (const std::exception &e)
    {
        std::cerr << "WeatherMain::inputToCandlesticks -> " << e.what() << std::endl;
    }
    if (candlesticks.size() < 1)
    {
        std::cerr << "WeatherMain::inputToCandlesticks -> No data" << std::endl;
    }
    return candlesticks;
}

void WeatherMain::candlestickData()
{
    std::cout << "1. Get candlestick data" << std::endl;
    std::cout << "Input time range, country, and period you want. Example: 1980010100, 1990123123, \"AT\", \"month\"" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    /** Print out in a table */
    std::vector<Candlestick> candlesticks = inputToCandlesticks(input);
    std::cout << std::endl
              << candlesticks.size() << " rows of data in candlesticks"
              << std::endl
              << std::endl;
    TempSet::printCandlesticks(candlesticks);
    std::cout << std::endl;
}

void WeatherMain::candlestickPlot()
{
    std::cout << "2. Plot candlesticks" << std::endl;
    std::cout << "Input time range, country, and period you want. Example: 1987010100, 1990123123, \"GB\", \"month\"" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    /** Call to handle user input */
    std::vector<Candlestick> candlesticks = inputToCandlesticks(input);
    std::cout << std::endl
              << candlesticks.size() << " rows of data in candlesticks"
              << std::endl
              << std::endl;
    TempSet::plotCandlesticks(candlesticks, 140);
    std::cout << std::endl;
}
void WeatherMain::filterPlot()
{
    std::cout << "3. Filter data and plot" << std::endl;
    std::cout << "Which kind of filtering you want?" << std::endl;
    std::cout << "1. By time range, temparature range and country, and create a line plot of the trend of temparature changes over time" << std::endl;
    std::cout << "2. By time range and temparature range, and create a bar plot displaying the  distribution of selected temparature among countries" << std::endl;
    std::string input;
    std::getline(std::cin, input);
    /** Two type of filterings for selecting */
    if (input == "1")
    {
        filterPlotLine();
    }
    else if (input == "2")
    {
        filterPlotBar();
    }
    else
    {
        std::cerr << "WeatherMain::filterPlot -> Wrong input" << std::endl;
    }
}

void WeatherMain::filterPlotLine()
{
    std::cout << "Input time range, temparature range, and country you want. Example: 1990010100, 1990123100, 0.0, 25.5, \"FR\"" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<Temp> temps;
    std::vector<Temp> timeRange;

    /** Convert user input into params */
    std::istringstream inputStream(input);
    std::string timeAStr, timeBStr, tempAStr, tempBStr, country;
    std::getline(inputStream, timeAStr, ',');
    std::getline(inputStream, timeBStr, ',');
    std::getline(inputStream, tempAStr, ',');
    std::getline(inputStream, tempBStr, ',');
    std::getline(inputStream, country, ',');
    /** A lamda function to remove spaces before and after string */
    auto trim = [](std::string &s)
    {
        s.erase(0, s.find_first_not_of(" \t\""));
        s.erase(s.find_last_not_of(" \t\"") + 1);
    };
    trim(timeAStr);
    trim(timeBStr);
    trim(tempAStr);
    trim(tempBStr);
    trim(country);
    try
    {
        int timeA = std::stoi(timeAStr);
        int timeB = std::stoi(timeBStr);
        float tempA = std::stof(tempAStr);
        float tempB = std::stof(tempBStr);

        temps = tempSet.getTemps(timeA, timeB, tempA, tempB, country);
        timeRange = tempSet.getTemps(timeA, timeB, country);
    }
    catch (const std::exception &e)
    {
        std::cerr << "WeatherMain::filterPlotLine -> " << e.what() << std::endl;
    }
    if (temps.size() < 1)
    {
        std::cerr << "WeatherMain::filterPlotLine -> No data" << std::endl;
        return;
    }
    std::cout << std::endl
              << temps.size() << " rows of data retrieved"
              << std::endl
              << std::endl;
    TempSet::plotLine(temps, timeRange, 140);
    std::cout << std::endl;
}

void WeatherMain::filterPlotBar()
{
    std::cout << "Input time and temparature range you want. Example: 2000010100, 2010010100, 25, 50" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<Temp> temps;

    /** Convert user input into params */
    std::istringstream inputStream(input);
    std::string timeAStr, timeBStr, tempAStr, tempBStr;
    std::getline(inputStream, timeAStr, ',');
    std::getline(inputStream, timeBStr, ',');
    std::getline(inputStream, tempAStr, ',');
    std::getline(inputStream, tempBStr, ',');
    /** A lamda function to remove spaces before and after string */
    auto trim = [](std::string &s)
    {
        s.erase(0, s.find_first_not_of(" \t\""));
        s.erase(s.find_last_not_of(" \t\"") + 1);
    };
    trim(timeAStr);
    trim(timeBStr);
    trim(tempAStr);
    trim(tempBStr);
    try
    {
        int timeA = std::stoi(timeAStr);
        int timeB = std::stoi(timeBStr);
        float tempA = std::stof(tempAStr);
        float tempB = std::stof(tempBStr);

        temps = tempSet.getTemps(timeA, timeB, tempA, tempB);
    }
    catch (const std::exception &e)
    {
        std::cerr << "WeatherMain::filterPlotLine -> " << e.what() << std::endl;
    }
    if (temps.size() < 1)
    {
        std::cerr << "WeatherMain::filterPlotLine -> No data" << std::endl;
        return;
    }
    std::cout << std::endl
              << temps.size() << " rows of data retrieved"
              << std::endl
              << std::endl;
    TempSet::plotBar(temps, 140);
    std::cout << std::endl;
}

void WeatherMain::predictPlot()
{
    std::cout << "4. Make prediction and plot" << std::endl;
    std::cout << "Input time range and country you want. For accuracy, the range should not smaller than 7 days and larger the 2 years. Example: 2010010100, 2010060100, \"GR\"" << std::endl;

    std::string input;
    std::getline(std::cin, input);

    std::vector<Temp> temps;

    /** Convert user input into params */
    std::istringstream inputStream(input);
    std::string timeAStr, timeBStr, country;
    std::getline(inputStream, timeAStr, ',');
    std::getline(inputStream, timeBStr, ',');
    std::getline(inputStream, country, ',');
    /** A lamda function to remove spaces before and after string */
    auto trim = [](std::string &s)
    {
        s.erase(0, s.find_first_not_of(" \t\""));
        s.erase(s.find_last_not_of(" \t\"") + 1);
    };
    trim(timeAStr);
    trim(timeBStr);
    trim(country);
    try
    {
        int timeA = std::stoi(timeAStr);
        int timeB = std::stoi(timeBStr);

        temps = tempSet.getTemps(timeA, timeB, country);
    }
    catch (const std::exception &e)
    {
        std::cerr << "WeatherMain::filterPlotLine -> " << e.what() << std::endl;
    }
    if (temps.size() < 1)
    {
        std::cerr << "WeatherMain::filterPlotLine -> No data" << std::endl;
        return;
    }
    std::cout << std::endl
              << temps.size() << " rows of data retrieved"
              << std::endl
              << std::endl;
    int predictPoints = temps.size() / 4;
    std::vector<float> prediction = TempSet::computeEMA(temps, predictPoints);
    TempSet::plotPredict(temps, prediction, 140);
    std::cout << std::endl;
}