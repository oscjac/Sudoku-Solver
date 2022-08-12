#include "board.h"

using namespace SudokuSolver;

void SudokuSolver::solve(Board *board)
{
    auto itr = board->begin();
    while (itr != board->end())
    {
        if (itr->constant)
        {
            ++itr;
            continue;
        }
        // Assign value to current square
        ++(itr->value);
        while (!(board->valid()) && itr->value < 10)
            ++(itr->value);
        if (itr->value == 10)
        {
            itr->value = 0;
            --itr;
            while (itr->constant)
                --itr;
            continue;
        }
        if (board->valid())
            ++itr;
    }
}

void SudokuSolver::show_solve(Board *board, std::ostream &out)
{
    auto itr = board->begin();
    while (itr != board->end())
    {
        if (itr->constant)
        {
            ++itr;
            continue;
        }
        // Assign value to current square
        ++(itr->value);
        board->output_board(out);
        while (!(board->valid()) && itr->value < 10)
        {
            ++(itr->value);
            board->output_board(out);
        }
        if (itr->value == 10)
        {
            itr->value = 0;
            board->output_board(out);
            --itr;
            while (itr->constant)
                --itr;

            continue;
        }
        if (board->valid())
            ++itr;
    }
}