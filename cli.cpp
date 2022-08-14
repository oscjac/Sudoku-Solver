#include "cli.h"

FileArg::FileArg(char *argv)
{
    std::string filename(argv);
    std::ifstream fin(filename.c_str());
    char n;
    int i = 0;
    while (fin >> n and i < 81)
    {
        int char_as_int = static_cast<int>(n);
        if (char_as_int < 48 || char_as_int > 57)
            throw std::runtime_error("Board input should only contain integers");
        data[i++] = char_as_int - 48;
    }
    if (i < 81 || !fin.eof())
    {
        throw std::runtime_error("Input stream must have exactly 81 integers");
    }
    fin.close();
}

StringArg::StringArg(char **argv)
{
    std::string flag(argv[1]);
    if (flag != "-r")
        throw cli_exception();
    std::string integers(argv[0]);
    if (integers.length() != 81)
        throw cli_exception("Board string must have exactly 81 integers");
    for (int i = 0; i < 81; ++i)
        data[i] = std::stoi(integers.substr(i, 1));
}

void Args::fill(int *target)
{
    std::copy(data, data + 81, target);
}

cli_exception::cli_exception() : _msg("Error in command line arguments provided"){};
cli_exception::cli_exception(std::string msg) : _msg(msg){};

const char *cli_exception::what() const noexcept
{
    return _msg.c_str();
};