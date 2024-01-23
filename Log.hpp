#pragma once
#include <fstream>

class Log
{
private:
    std::string path;
    std::string name;
public:
    Log(std::string path="unknown", std::string name="unknown")
    {
        this->path = path;
        this->name = name;
    }



    ~Log()
    {
        path = "";
        name = "";
    }
};
