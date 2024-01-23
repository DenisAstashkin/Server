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

    void write(std::string msg)
    {
        std::ofstream out;
        try
        {
            out.open(path + name, std::ios::out);
            if (out.is_open())
            {
                out << msg << "\n";
            }
            out.close();
        }
        catch (std::exception e)
        {
            out.close();
        }
        out.close();
    }

    ~Log()
    {
        path = "";
        name = "";
    }
};
