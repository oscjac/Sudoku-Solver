#define BOOST_TEST_MODULE Board_Test_module
#include <boost/test/included/unit_test.hpp>
#include <string>
#include "board.h"

using namespace SudokuSolver;

BOOST_AUTO_TEST_SUITE(board_tests)
BOOST_AUTO_TEST_SUITE(board_constructor)
BOOST_AUTO_TEST_CASE(board_from_file)
{
    std::ifstream fin("std_input.txt");
    std::ifstream correct_fin("std_input.out.txt");
    std::ostringstream output;
    BOOST_ASSERT(fin.is_open());
    Board from_file(fin);
    from_file.output_board(output);
    std::stringstream correct;
    correct << correct_fin.rdbuf();
    BOOST_TEST(correct.str() == output.str(), "\n" + correct.str() + "\n!=\n" + output.str());
}

BOOST_AUTO_TEST_CASE(board_from_short_file)
{
    std::ifstream fin("short_input.txt");
    BOOST_ASSERT(fin.is_open());
    try
    {
        Board from_file(fin);
        BOOST_ERROR("Constructor should throw a runtime error due to short input");
    }
    catch (std::runtime_error e)
    {
        BOOST_TEST(true);
        return;
    }
    catch (std::exception e)
    {
        BOOST_ERROR("Error should be a runtime error");
    }
}

BOOST_AUTO_TEST_CASE(board_from_long_file)
{
    std::ifstream fin("long_input.txt");
    BOOST_ASSERT(fin.is_open());
    try
    {
        Board from_file(fin);
        BOOST_ERROR("COnstructor should throw a runtime error due to long input");
    }
    catch (std::runtime_error e)
    {
        BOOST_TEST(true);
        return;
    }
    catch (std::exception e)
    {
        BOOST_ERROR("Error should be a runtime error");
    }
}
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(set_square_method)
BOOST_AUTO_TEST_CASE(set_negative_square)
{
    std::ifstream fin("std_input.txt");
    Board *board = new Board(fin);
    try
    {
        board->set_square(0, 3, -1);
        delete board;
        BOOST_ERROR("set_square should throw a runtime error");
    }
    catch (std::runtime_error e)
    {
        delete board;
        BOOST_TEST(true);
        return;
    }
    catch (std::exception)
    {
        delete board;
        BOOST_ERROR("set_square should throw a runtime error");
        return;
    }
}
BOOST_AUTO_TEST_CASE(set_square_with10)
{
    std::ifstream fin("std_input.txt");
    Board *board = new Board(fin);
    try
    {
        board->set_square(0, 3, 10);
        delete board;
        BOOST_ERROR("set_square should throw a runtime error");
    }
    catch (std::runtime_error e)
    {
        delete board;
        BOOST_TEST(true);
        return;
    }
    catch (std::exception)
    {
        delete board;
        BOOST_ERROR("set_square should throw a runtime error");
        return;
    }
}
BOOST_AUTO_TEST_CASE(set_square_invalid_row)
{
    std::ifstream fin("std_input.txt");
    Board *board = new Board(fin);
    try
    {
        board->set_square(9, 1, 5);
        delete board;
        BOOST_ERROR("set_square should throw a runtime error");
    }
    catch (std::logic_error e)
    {
        delete board;
        BOOST_TEST(true);
        return;
    }
    catch (std::exception)
    {
        delete board;
        BOOST_ERROR("set_square should throw a runtime error");
        return;
    }
}
BOOST_AUTO_TEST_CASE(set_square_invalid_col)
{
    std::ifstream fin("std_input.txt");
    Board *board = new Board(fin);
    try
    {
        board->set_square(4, 11, 5);
        delete board;
        BOOST_ERROR("set_square should throw a runtime error");
    }
    catch (std::logic_error e)
    {
        delete board;
        BOOST_TEST(true);
        return;
    }
    catch (std::exception)
    {
        delete board;
        BOOST_ERROR("set_square should throw a runtime error");
        return;
    }
}
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(board_iterator)
BOOST_AUTO_TEST_CASE(iterator_ctor)
{
    std::ifstream fin("std_input.txt");
    Board *board = new Board(fin);
    try
    {
        board->begin();
        BOOST_TEST(true);
        delete board;
    }
    catch (std::exception e)
    {
        BOOST_TEST(false, e.what());
        delete board;
    }
}
BOOST_AUTO_TEST_CASE(iterator_begin)
{
    std::ifstream fin("std_input.txt");
    Board *board = new Board(fin);
    auto itr = board->begin();
    BOOST_TEST(itr->value == 0, "First square is empty");
    delete board;
}
BOOST_AUTO_TEST_CASE(iterator_end)
{
    std::ifstream fin("std_input.txt");
    std::ostringstream actual;
    Board *board = new Board(fin);
    auto itr = board->begin();
    while (itr != board->end())
        actual << (itr++)->value;
    BOOST_TEST(actual.str() == "095078342000041060467039501080400000506000290000090805804900003000050020051360000");
    delete board;
}
BOOST_AUTO_TEST_CASE(postfix_increment)
{
    std::ifstream fin("std_input.txt");
    Board *board = new Board(fin);
    auto itr = board->begin();
    BOOST_TEST((itr++)->value == 0, "First value is 0");
    BOOST_TEST((itr++)->value == 9, "Second value is 9");
    BOOST_TEST((itr++)->value == 5, "Third value is 5");
    delete board;
}
BOOST_AUTO_TEST_CASE(prefix_increment)
{
    std::ifstream fin("std_input.txt");
    Board *board = new Board(fin);
    auto itr = board->begin();
    BOOST_TEST((++itr)->value == 9, "Second value is 9");
    BOOST_TEST((++itr)->value == 5, "Third value is 9");
    delete board;
}
BOOST_AUTO_TEST_CASE(postfix_decrement)
{
    std::ifstream fin("std_input.txt");
    Board *board = new Board(fin);
    auto itr = board->begin();
    ++itr;
    BOOST_TEST((itr--)->value == 9);
    delete board;
}
BOOST_AUTO_TEST_CASE(prefix_decrement)
{
    std::ifstream fin("std_input.txt");
    Board *board = new Board(fin);
    auto itr = board->begin();
    ++itr;
    BOOST_TEST((--itr)->value == 0);
    BOOST_TEST(itr->value == 0);
    delete board;
}
BOOST_AUTO_TEST_CASE(dereference_nullptr)
{
    std::ifstream fin("std_input.txt");
    Board *board = new Board(fin);
    auto itr = board->end();
    try
    {
        *itr;
        delete board;
        BOOST_ERROR("Should throw a runtime error");
    }
    catch (std::runtime_error e)
    {
        delete board;
        BOOST_TEST(true, "Cannot dereference a nullptr");
    }
    catch (std::exception e)
    {
        BOOST_ERROR("Should throw a runtime error");
        delete board;
    }
}
BOOST_AUTO_TEST_CASE(arrow_operator_nullptr)
{
    std::ifstream fin("std_input.txt");
    Board *board = new Board(fin);
    auto itr = board->end();
    try
    {
        int n = itr->value;
        delete board;
        BOOST_ERROR("Should throw a runtime error");
    }
    catch (std::runtime_error)
    {
        delete board;
        BOOST_TEST(true, "Should throw a runtime error");
    }
    catch (std::exception e)
    {
        delete board;
        std::cout << e.what();
        BOOST_ERROR("Should throw a runtime error");
    }
}
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(grid_tests)
BOOST_AUTO_TEST_SUITE(grid_constructor)
BOOST_AUTO_TEST_CASE(grid_std_ctor)
{
    Box *box = new Box();
    for (int i = 0; i < 9; ++i)
        BOOST_TEST(box->operator[](i).value == 0);
    delete box;
}
BOOST_AUTO_TEST_CASE(grid_undefined_ctor)
{
    int *values = new int[8];
    try
    {
        Box *box = new Box(values);
        delete[] values;
        delete box;
        BOOST_ERROR("Constructor should throw a runtime error");
    }
    catch (std::runtime_error e)
    {
        delete[] values;
        BOOST_TEST(true);
        return;
    }
    catch (std::exception e)
    {
        delete[] values;
        BOOST_ERROR("Constructor should throw a runtime error\n");
    }
}
BOOST_AUTO_TEST_CASE(grid_negative_value_ctor)
{
    int *values = new int[9];
    for (int i = 0; i < 9; ++i)
        values[i] = -1;
    try
    {
        Box *box = new Box(values);
        delete[] values;
        delete box;
        BOOST_ERROR("Constructor should throw a run_time error");
    }
    catch (std::runtime_error e)
    {
        delete[] values;
        BOOST_TEST(true);
    }
    catch (std::exception e)
    {
        delete[] values;
        BOOST_ERROR("Constructor should throw a run_time error");
    }
}
BOOST_AUTO_TEST_CASE(grid_invalid_value_ctor)
{
    int *values = new int[9];
    for (int i = 0; i < 9; ++i)
        values[i] = 10;
    try
    {
        Box *box = new Box(values);
        delete[] values;
        delete box;
        BOOST_ERROR("Constructor should throw a run_time error");
    }
    catch (std::runtime_error e)
    {
        delete[] values;
        BOOST_TEST(true);
    }
    catch (std::exception e)
    {
        delete[] values;
        BOOST_ERROR("Constructor should throw a run_time error");
    }
}
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(grid_is_valid_method)
BOOST_AUTO_TEST_CASE(valid_grid)
{
    Box *box = new Box();
    for (int i = 1; i < 10; ++i)
        box->operator[](i - 1).value = i;
    bool valid = box->valid();
    delete box;
    BOOST_TEST(valid, "Grid is valid");
}
BOOST_AUTO_TEST_CASE(duplicate_9_grid)
{
    Box *box = new Box();
    for (int i = 1; i < 10; ++i)
        box->operator[](i - 1).value = i;
    box->operator[](0).value = 9;
    bool valid = box->valid();
    delete box;
    BOOST_TEST(!valid, "Grid number 9 is in index 0 and 8");
}
BOOST_AUTO_TEST_CASE(grid_with_0)
{
    Box *box = new Box();
    bool valid = box->valid();
    delete box;
    BOOST_TEST(valid, "Grid only has zeroes");
}
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()