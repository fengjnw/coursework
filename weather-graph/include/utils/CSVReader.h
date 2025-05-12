#pragma once
#include <string>
#include <vector>
#include "models/Temp.h"

class CSVReader
{
public:
    CSVReader();
    static std::vector<Temp> readCSV(std::string file);
    static std::vector<std::string> tokenise(std::string cavLine, char separator);

private:
    /** Convert strings to objects in various country but the same timestamp */
    static std::vector<Temp> tokensToObjs(std::vector<std::string> tokens);
};
