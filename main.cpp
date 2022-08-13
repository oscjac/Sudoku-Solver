#include "board.h"

void get_filename(std::ifstream &fin, std::string &filename)
{
    std::cout << "Enter board file name: ";
    std::cin >> filename;
    fin.open(filename.c_str());
}

int main()
{
    using namespace SudokuSolver;
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