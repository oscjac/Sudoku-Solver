#include "board.h"

void get_filename(std::ifstream &fin, std::string &filename)
{
    std::cout << "Enter board file name: ";
    std::cin >> filename;
    fin.open(filename.c_str());
}

int main(int argc, char **argv)
{
    using namespace SudokuSolver;
    if (argc > 1)
    {
        try
        {
            auto arguments = factory<2, 3>(argc, argv);
            if (!arguments)
                throw cli_exception();
            Board board(*arguments);
            solve(&board);
            std::cout << board.as_string();
        }
        catch (const std::exception &e)
        {
            std::cout << e.what();
            return 1;
        }
        return 0;
    }
    else
    {
        while (true)
        {
            std::ifstream fin;
            std::string filename;
            get_filename(fin, filename);
            while (!fin.is_open())
            {
                std::cout << "Could not open file " << filename.c_str() << std::endl;
                filename = "";
                get_filename(fin, filename);
            }
            Board *board = new Board(fin);
            solve(board);
            if (board->solved())
                board->output_board(std::cout);
            delete board;
        }
        return 0;
    }
}