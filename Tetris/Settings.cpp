#include "Settings.h"

Figure::~Figure()
{ figure.clear(); }

void Figure::rotate()
{
   int size = figure.size();
   matrix rotated(size, std::vector<filling>(size));
   for (int i = 0; i < size; i++)
      for (int j = 0; j < size; j++)
         rotated[i][j] = figure[j][size - i - 1];
   figure = rotated;
}

void Figure::clear()
{
   for (int i = 0; i < get_size(); i++)
      for (int j = 0; j < get_size(); j++)
         figure[i][j] = empty;
}

int Figure::get_size() const
{ return figure.size(); }

int Figure::get_height() const
{
   int f_height = 0;
   for (int i = 0; i < figure.size(); i++)
      if (!empty_row(i))
         f_height++;
   return f_height;
}

int Figure::get_width() const
{
   int f_width = 0;
   for (int i = 0; i < figure.size(); i++)
      if (!empty_column(i))
         f_width++;
   return f_width;
}

filling Figure::get_cell_col(int row, int column) const
{
   if (row >= 0 && row < get_size() && column >= 0 && column < get_size())
      return figure[row][column];
   return empty;
}

bool Figure::empty_row(int number) const
{
   for (int i = 0; i < figure.size(); i++)
      if (figure[number][i] != empty)
         return false;
   return true;
}

bool Figure::empty_column(int number) const
{
   for (int i = 0; i < figure.size(); i++)
      if (figure[i][number] != empty)
         return false;
   return true;
}

bool Figure::empty_cell(int row, int column) const
{
   if (row >= 0 && row < get_size() && column >= 0 && column < get_size())
      return figure[row][column] == empty;
   return true;
}

/***************************************************************************/

Field::Field()
{
   for (int i = 0; i < height; i++)
   {
      for (int j = 0; j < width; j++)
         field[i][j] = empty;
   }
}

Field::~Field()
{ field.clear(); }

filling Field::get_cell_col(int row, int column) const
{
   if (row >= 0 && row < height && column >= 0 && column < width)
      return field[row][column];
   return empty;
}

void Field::set_cell_col(int row, int column, filling color)
{
   if (row >= 0 && row < height && column >= 0 && column < width)
      field[row][column] = color;
}

bool Field::full_row(int number)
{
   for (int i = 0; i < width; i++)
      if (field[number][i] == empty)
         return false;
   clear_row(number);
   return true;
}

filling Field::empty_cell(int row, int column) const
{
   if (row >= 0 && row < height && column >= 0 && column < width)
      return field[row][column];
   return outside;
}

void Field::clear_row(int number)
{
   for (int j = 0; j < width; j++)
      field[number][j] = empty;
   line_shift(number);
}

void Field::line_shift(int number)
{
   for (int i = number; i > 0; i--)
      for (int j = 0; j < width; j++)
      {
         field[i][j] = field[i - 1][j];
         field[i - 1][j] = empty;
      }
}

void Field::clear_field()
{
   for (int i = 0; i < height; i++)
      for (int j = 0; j < field.size(); j++)
         field[i][j] = empty;
}