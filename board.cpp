#include "board.h"

using namespace SudokuSolver;

// Input is a stream of 81 digits representing starting board values
Board::Board(std::istream &input) : board_boxes(new Box[9]), root(nullptr)
{
    char n;
    int k[81];
    int i = 0;
    while (input >> n and i < 81)
    {
        k[i++] = static_cast<int>(n) - 48; // NOT SAFE. Should replace with safe way to read in input
    }
    if (i < 81 || !input.eof())
    {
        throw std::runtime_error("Input stream must have exactly 81 integers");
    }
    for (int i = 0; i < 3; ++i)
    {
        int grid_i = i * 3;
        board_boxes[grid_i] = Box(k + grid_i * 9),
        board_boxes[grid_i + 1] = Box(k + grid_i * 9 + 3),
        board_boxes[grid_i + 2] = Box(k + grid_i * 9 + 6);
    }
}

square &Board::get_square(int row, int col)
{
    if (row > 8 || col > 8 || col < 0 || row < 0)
    {
        throw std::logic_error("row and col arguments must be between 0 and 8");
    }
    int grid_row = row / 3;
    int grid_i = grid_row * 3 + col / 3, square_i = 3 * (row % 3) + (col % 3);
    return board_boxes[grid_i][square_i];
}

void Board::set_square(int row, int col, int value)
{
    if (value < 0 || value > 9)
        throw std::runtime_error("Value must be between 0 and 9");
    auto sqr = get_square(row, col);
    if (sqr.constant)
        throw std::runtime_error("Cannot change the value of a constant square");
    sqr.value = value;
}

void Board::output_board(std::ostream &out)
{
    // Top row
    out << "\n  ";
    for (unsigned int i = 1; i < 10; ++i)
        out << " " << i;
    out << std::endl
        << "  ";
    for (int i = 0; i < 19; ++i)
        out << "_";
    out << "\n";
    // Board data
    for (int i = 0; i < 81; ++i)
    {
        if (i % 9 == 0)
            out << i / 9 + 1 << " ";
        out << "|" << get_square(i / 9, i % 9).value;
        if (i % 9 == 8)
            out << "|\n";
    }
    out << "  ";
    // Bottom row
    for (int i = 0; i < 19; ++i)
        out << "-";
    out << std::endl;
}

bool Board::row_valid(int row)
{
    bool seen[9] = {false};
    for (int i = 0; i < 9; ++i)
    {
        int n = get_square(row, i).value;
        if (n > 9)
            return false;
        if (!n) // Square with 0 is valid
            continue;
        if (seen[n - 1])
            return false;
        seen[n - 1] = true;
    }
    return true;
}
bool Board::col_valid(int col)
{
    bool seen[9] = {false};
    for (int i = 0; i < 9; ++i)
    {
        int n = get_square(i, col).value;
        if (n > 9)
            return false;
        if (!n) // Square with 0 is valid
            continue;
        if (seen[n - 1])
            return false;
        seen[n - 1] = true;
    }
    return true;
}

bool Board::solved()
{
    if (!valid())
        return false;
    auto itr = begin();
    while (itr != end())
        if (itr->value == 0)
            return false;
    return true;
}

void Board::populate_root()
{
    root = new Node;
    root->prev = nullptr;
    root->ptr = &get_square(0, 0);
    Node *prev = new Node;
    root->next = prev;
    prev->prev = root;
    prev->ptr = &get_square(0, 1);
    for (int i = 2; i < 81; ++i)
    {
        Node *node = new Node;
        node->prev = prev;
        prev->next = node;
        node->ptr = &get_square(i / 9, i % 9);
        node->next = nullptr;
        prev = node;
    }
}

Board::Iterator Board::begin()
{
    if (root)
        return Iterator(root);
    populate_root();
    return Iterator(root);
};

bool Board::valid()
{
    for (int i = 0; i < 9; ++i)
    {
        bool _row_valid = row_valid(i);
        bool _col_valid = col_valid(i);
        bool _grid_valid = board_boxes[i].valid();
        if (!_row_valid or !_col_valid or !_grid_valid)
            return false;
    }
    return true;
}

Board::Iterator Board::end()
{
    if (!root)
        populate_root();
    Node *node = root;
    while (node->next)
        node = node->next;
    if (node->ptr)
    {
        Node *end = new Node;
        node->next = end;
        end->ptr = nullptr;
        end->prev = node;
        end->next = nullptr;
        return Iterator(end);
    }
    return Iterator(node);
}

void Board::delete_nodes()
{
    Node *node = root;
    while (node->next)
    {
        node = node->next;
        delete node->prev;
    }
    delete node;
}

Board::~Board()
{
    delete_nodes();
    delete[] board_boxes;
}

Box::Box() : solved(false), squares(new square[9])
{
    square sqr;
    sqr.value = 0;
    sqr.constant = false;
    for (int i = 0; i < 9; ++i)
        squares[i] = sqr;
}

Box::Box(int *values) : solved(false), squares(new square[9])
{
    for (int i = 0; i < 3; ++i)
    {
        int k = i * 9,
            j = i * 3;
        int n = values[k], n1 = values[k + 1], n2 = values[k + 2];
        if (n < 0 || n1 < 0 || n2 < 0)
            throw std::runtime_error("Values must be postive integers");
        if (n > 9 || n1 > 9 || n2 > 9)
            throw std::runtime_error("Values must be between 0 and 9");
        square sqr;
        sqr.value = n;
        sqr.constant = n;
        squares[j] = sqr;
        sqr.value = n1;
        sqr.constant = n1;
        squares[j + 1] = sqr;
        sqr.value = n2;
        sqr.constant = n2;
        squares[j + 2] = sqr;
    }
}

Box &Box::operator=(const Box &rhs)
{
    std::copy(rhs.squares, rhs.squares + 9, squares);
    return *this;
}

// Checks number array to verify that no more than one number.
bool Box::valid()
{
    bool seen[9] = {false};
    for (int i = 0; i < 9; ++i)
    {
        int n = squares[i].value;
        if (n > 9)
            return false;
        if (!n) // Box may have 0
            continue;
        if (seen[n - 1])
            return false;
        seen[n - 1] = true;
    }
    return true;
}

square &Box::operator[](int i)
{
    return squares[i];
}

Box::~Box()
{
    delete[] squares;
}

Board::Iterator::Iterator(Node *root)
{
    current = root;
    return;
}

Board::Iterator &Board::Iterator::operator++()
{
    current = current->next;
    return *this;
}

Board::Iterator Board::Iterator::operator++(int)
{
    auto tmp = *this;
    ++*this;
    return tmp;
}

Board::Iterator &Board::Iterator::operator--()
{
    current = current->prev;
    return *this;
}

Board::Iterator Board::Iterator::operator--(int)
{
    auto tmp = *this;
    --*this;
    return tmp;
}

square &Board::Iterator::operator*() const
{
    if (current->ptr)
        return *(current->ptr);
    throw std::runtime_error("Cannot dereference a nullptr");
}

square *Board::Iterator::operator->() const
{
    if (current->ptr)
        return current->ptr;
    throw std::runtime_error("Cannot dereference a nullptr");
}

bool Board::Iterator::operator==(const Board::Iterator &rhs) const
{
    return current == rhs.current;
}

bool Board::Iterator::operator!=(const Board::Iterator &rhs) const
{
    return !(*this == rhs);
}