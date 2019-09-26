#pragma once
#ifndef SETTINGS_H

#include <vector>

enum filling { green = 0, cyan, blue, red, purple, yellow, orange, empty, outside };

typedef std::vector<std::vector<filling>> matrix;

constexpr int height = 15;
constexpr int width = 10;

class Figure
{
private:
   matrix figure;

public:
   Figure() {}
   explicit Figure(matrix shape) : figure(shape) {}
   ~Figure();

   void rotate();
   void clear();

   int get_size() const;
   int get_height() const;
   int get_width() const;

   filling get_cell_col(int row, int column) const;

   bool empty_row(int number) const;
   bool empty_column(int number) const;
   bool empty_cell(int row, int column) const;
};

class Field
{
private:
   matrix field{height, std::vector<filling>(width)};

   void line_shift(int number);
   void clear_row(int number);

public:
   Field();
   ~Field();

   void clear_field();

   filling get_cell_col(int row, int column) const;
   filling empty_cell(int row, int column) const;

   bool full_row(int number);

   void set_cell_col(int row, int column, filling color);
};

#endif // !SETTINGS_H