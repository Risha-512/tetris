#pragma once
#ifndef SETTINGS_H

#include <vector>

enum class fill { green, cyan, blue, red, purple, yellow, orange, empty, outside };

typedef std::vector<std::vector<fill>> matrix;

constexpr int height = 15;
constexpr int width = 10;

class Figure
{
private:
    matrix figure;
    char name;

public:
    Figure() {}
    explicit Figure(matrix shape, char symb) : figure(shape), name(symb) {}
    ~Figure();

    void rotate();
    void clear();

    int get_size() const;
    int get_height() const;
    int get_width() const;
    char get_name() const;

    fill get_cell_col(int row, int column) const;

    bool empty_row(int number) const;
    bool empty_column(int number) const;
    bool empty_cell(int row, int column) const;
};

class Field
{
private:
    matrix field{height, std::vector<fill>(width)};

    void line_shift(int number);
    void clear_row(int number);

public:
    Field();
    ~Field();

    void clear_field();

    fill get_cell_col(int row, int column) const;
    fill empty_cell(int row, int column) const;

    bool full_row(int number);

    void set_cell_col(int row, int column, fill color);
};

#endif // !SETTINGS_H