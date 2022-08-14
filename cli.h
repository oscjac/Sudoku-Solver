#include <fstream>
#include <map>
class Args
{
protected:
    int data[81];

public:
    // Copies argument data to target container
    virtual void fill(int *target);
};

class StringArg : public Args
{
public:
    StringArg(char **argv);
};

class FileArg : public Args
{
public:
    FileArg(char *argv);
};

class cli_exception : public std::exception
{
    std::string _msg;

public:
    cli_exception();
    cli_exception(std::string msg);
    virtual const char *what() const noexcept override;
};

// Creates an Args object based on provided command line arguments
template <int MIN_ARGC, int MAX_ARGC>
std::unique_ptr<Args> factory(int argc, char **argv)
{
    if (argc < MIN_ARGC)
        throw std::invalid_argument("Minimum number of command line arguments is" + std::to_string(MIN_ARGC));
    if (argc > MAX_ARGC)
        throw std::invalid_argument("Maximum number of command line arguments is" + std::to_string(MAX_ARGC));
    static std::map<int,
                    std::function<std::unique_ptr<Args>()>>
        mapping{{0, [argv]()
                 { return std::make_unique<FileArg>(argv[1]); }},
                {1, [argv]()
                 { return std::make_unique<StringArg>(argv + 1); }}};
    auto itr = mapping.find(argc - MIN_ARGC);
    if (itr != mapping.end())
        return itr->second();
    return nullptr;
}