#include <sstream>
#include <iostream>
#include "cli.h"

namespace SudokuSolver
{
    struct square
    {
        int value;
        bool constant;
    };

    // Represents the nine boxes in the board
    class Box
    {
    private:
        bool solved;
        square *squares;

    public:
        Box();
        // Allocates memory for and assigns values to squares.
        Box(int *values);

        // Assignment operator
        Box &operator=(const Box &rhs);

        // Returns true if box the box contains at most one of each number
        bool valid();

        // Returns the square at the given index. Ordered from left to right. Up to down
        square &operator[](int i);

        // Frees all alocated memory for Box squares
        ~Box();
    };

    class Board
    {
    private:
        struct Node
        {
            square *ptr;
            Node *prev;
            Node *next;
        };
        // Pointer to array containing board data. Default to 9x9 board
        Box *board_boxes;

        // Gets box containing row and col
        Box *get_box(int row, int col);

        // Gets box containing with index i
        Box *get_box(int i);

        // Gets corresponding square in box. Row and Col start at index 0
        square &get_square(int row, int col);

        // Root node for Iterator to traverse
        Node *root;

        // Checks if the specified row is valid
        bool row_valid(int row);

        // Checks if the specified column is valid
        bool col_valid(int col);

        // Creates linked list of nodes for root
        void populate_root();

        // Goes through and deletes all the nodes linked to the root Node
        void delete_nodes();

    public:
        class Iterator
        {
        private:
            friend class Board;
            Node *current;

            // Creates an iterator at given square coordinate.
            Iterator(Node *root);

        public:
            Iterator &operator++();
            Iterator operator++(int);
            Iterator &operator--();
            Iterator operator--(int);
            square &operator*() const;
            square *operator->() const;
            bool operator==(const Iterator &rhs) const;
            bool operator!=(const Iterator &rhs) const;
        };

        // Reads input and creates board, allocates memory
        Board(std::istream &input);

        // Creates board from command line arguments
        Board(Args &arguments);

        // Sets square to new value if empty. Overwrites current value if not empty. If square is constant, does nothing.
        void set_square(int row, int col, int value);

        // Returns true if board is solved.
        bool solved();

        // Represent board in standard output stream
        void output_board(std::ostream &out);

        // Representation of the board as a string of integers. Ends with a \n
        std::string as_string();

        // Returns true if the board is valid.
        bool valid();

        // Returns an iterator pointing to the first square
        Iterator begin();

        // Returns an iterator pointing to the last square + 1
        Iterator end();

        // Deletes all data stored in memory.
        ~Board();
    };

    // Implements backtracking algorithm as described in:
    // https://en.wikipedia.org/wiki/Sudoku_solving_algorithms#Backtracking
    void solve(Board *board);

    // Same as solve, but shows each step algorithm takes in solving the puzzle
    void show_solve(Board *board, std::ostream &out);
}