#include "utils/CSVReader.h"
#include <fstream>
#include <iostream>

CSVReader::CSVReader()
{
}

std::vector<Temp> CSVReader::readCSV(std::string file)
{
    std::vector<Temp> temps;
    std::ifstream csvFile{file};
    std::string line;
    if (csvFile.is_open())
    {
        while (std::getline(csvFile, line))
        {
            try
            {
                std::vector<Temp> objs = tokensToObjs(tokenise(line, ','));
                for (int i = 0; i < objs.size(); ++i)
                {
                    temps.push_back(objs[i]);
                };
            }
            catch (const std::exception &e)
            {
                std::cerr << "CSVReader::readCSV -> " << e.what() << std::endl;
            }
        }
    }
    std::cout << "CSVReader::readCSV read " << temps.size() << " temperatures" << std::endl;
    return temps;
}

std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator)
{
    std::vector<std::string> tokens;
    signed int start, end;
    std::string token;
    start = csvLine.find_first_not_of(separator, 0);
    do
    {
        end = csvLine.find_first_of(separator, start);
        if (start == csvLine.length() || start == end)
            break;
        if (end >= 0)
            token = csvLine.substr(start, end - start);
        else
            token = csvLine.substr(start, csvLine.length() - start);
        tokens.push_back(token);
        start = end + 1;
    } while (end > 0);

    return tokens;
}

std::vector<Temp> CSVReader::tokensToObjs(std::vector<std::string> tokens)
{
    std::vector<Temp> objs;
    if (tokens.size() != 29)
    {
        throw std::invalid_argument("CSVReader::tokensToObjs -> Bad line ");
    }

    Timestamp timestamp = Temp::stringToTimestamp(tokens[0]);

    for (int i = 1; i < 29; ++i)
    {
        float tempValue;
        std::string country = Temp::indexToCountry(i - 1);
        try
        {
            tempValue = std::stof(tokens[i]);
        }
        catch (const std::exception &e)
        {
            std::cerr << "CSVReader::stringsToOBJs -> " << e.what() << " in -> " << tokens[i] << std::endl;
        }
        Temp obj{timestamp, country, tempValue};
        objs.push_back(obj);
    }
    return objs;
}