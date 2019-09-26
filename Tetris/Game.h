#pragma once
#ifndef GAME_H

#include "Settings.h"
#include <SFML/Graphics.hpp>

enum moves {rotate = 24, down, right, left, none};

class Drawing
{
private:
   const std::pair<int, int> offset = std::make_pair(50, 100);

   void draw_block(sf::RenderWindow &window, filling color, std::pair<int, int> b_coords);

protected:
   void draw_game_data(sf::RenderWindow &window, int score);
   void draw_field(sf::RenderWindow &window, const Field &area);
   void draw_figure(sf::RenderWindow &window, const Figure &item, const Figure &next, std::pair<int, int> coords);

   void draw_intro(sf::RenderWindow &window);
   void draw_ending(sf::RenderWindow &window, int score, int &record);
};

class Game : private Drawing
{
private:
   Field area;                            // game field
   Figure item,                           // current figure
          next;                           // next figure
   std::vector<Figure> figure_list;       // list with all figure types
   std::pair<int, int> coords;            // coordinates of top left block of figure item matrix
   int score, record;

   void set_coords(moves mode);           // change coordinates after changing figure item position
   void add_figure();                     // add figure item elements to field

   bool collisions(moves mode) const;     // checking about figure item moving

public:
   Game();
   ~Game();

   void intro(sf::RenderWindow &window);
   void ending(sf::RenderWindow &window);
   void draw(sf::RenderWindow &window);

   void set_record(int rec);
   int get_record() const;

   bool new_figure();                     // set new value to figure item
   void moving(moves type);               // move figure item
   bool falling();                        // change figure item coords according with item falling

   void restart();                        // play new game after old game ending
};

#endif // GAME_H