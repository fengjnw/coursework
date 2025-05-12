#include <iostream>
#include "WeatherMain.h"

int main()
{
    std::cout << "Wait for loading data..." << std::endl;
    WeatherMain app{};
    app.init();
    return 0;
}