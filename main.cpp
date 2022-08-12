#include "board.h"

int main()
{
    using namespace SudokuSolver;
    // Create input functions for board files
    std::ifstream fin("std_input.txt");
    std::ofstream fout("main_debug.txt");
    Board *board = new Board(fin);
    show_solve(board, fout);
    board->output_board(std::cout);
    delete board;
    return 0;
}