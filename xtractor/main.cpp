
#include "xtractor/lib.h"
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    Database db;

    switch (argc)
    {
    case 2:
    {
        if (!is_digits(argv[1]))
        {
            std::cerr << "Error: Invalid x (number of outputs): " << argv[1] << std::endl;
            return 1;
        }
        int const x = std::atoi(argv[1]);
        db = parse_stream(std::cin, static_cast<std::size_t>(x));
    }
    break;
    case 3:
    {
        if (!is_digits(argv[1]))
        {
            std::cerr << "Error: Invalid x (number of outputs): " << argv[1] << std::endl;
            return 1;
        }

        int const x = std::atoi(argv[1]);
        std::ifstream input_file(argv[2]);
        if (input_file.is_open())
        {
            db = parse_stream(input_file, static_cast<std::size_t>(x));
            input_file.close();
        }
        else
        {
            std::cerr << "Error: Unable to open file " << argv[2] << std::endl;
            return 1;
        }
    }
    break;
    default:
    {
        std::cerr << "Usage: " << argv[0] << " x [file]" << std::endl;
        std::cerr << std::endl;
        std::cerr << "x:    Number of outputs" << std::endl;
        std::cerr << "file: Input file (optional, will read from stdin if not provided)" << std::endl;
        return 1;
    }
    }

    for (const auto &pair : db)
    {
        std::cout << pair.second << std::endl;
    }

    return 0;
}
