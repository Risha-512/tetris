#include "Game.h"

bool is_rec_checked = false, is_rec_achieved = false;

auto absolute = [](int num)
{ return num < 0 ? num * (-1) : num; };

void Drawing::draw_block(sf::RenderWindow &window, filling color, std::pair<int, int> b_coords)
{
   sf::Texture texture;
   if (!texture.loadFromFile("Stuff/Textures.png", sf::IntRect(0, 0, 289, 37)))
   {
      perror("Error opening file 'Textures.png'");
      abort();
   }
   sf::Sprite sprite;
   sprite.setTexture(texture);
   sprite.setTextureRect(sf::IntRect(36 * color, 0, 37, 37));
   sprite.move(sf::Vector2f(b_coords.first, b_coords.second));
   window.draw(sprite);
}

void Drawing::draw_game_data(sf::RenderWindow &window, int score)
{
   sf::Font font;
   if (!font.loadFromFile("Stuff/unlearne.ttf"))
   {
      perror("Error opening file 'unlearne.ttf'");
      abort();
   }
   sf::Text text;
   text.setFont(font);
   text.setString("SCORE:");
   text.setCharacterSize(60);
   text.setFillColor(sf::Color::Black);
   text.setOutlineColor(sf::Color::White);
   text.setOutlineThickness(1.0f);
   sf::Vector2u size = window.getSize();
   float text_width = text.getLocalBounds().width,
      text_height = text.getLocalBounds().height;
   text.setPosition(size.x - text_width * 2.2f, size.y / 2.0f - text_height * 7.4f);
   window.draw(text);

   text.setString(std::to_string(score));
   text.setPosition(size.x - text_width, size.y / 2.0f - text_height * 7.4f);
   window.draw(text);

   text.setString("NEXT FIGURE:");
   text.setPosition(size.x - text_width * 2.2f, size.y / 2.0f - text_height * 5.0f);
   window.draw(text);

   text.setString("PAUSE: P");
   text.setPosition(size.x - text_width * 2.2f, size.y / 1.8f);
   window.draw(text);

   text.setString("EXIT: ESC");
   text.setPosition(size.x - text_width * 2.2f, size.y / 1.5f);
   window.draw(text);
}

void Drawing::draw_field(sf::RenderWindow &window, const Field &area)
{
   for (int i = 0; i < height; i++)
      for (int j = 0; j < width; j++)
         draw_block(window, area.get_cell_col(i, j), std::make_pair(offset.first + j * 36, offset.second + i * 36));
}

void Drawing::draw_figure(sf::RenderWindow & window, const Figure &item, const Figure &next, std::pair<int, int> coords)
{
   for (int i = 0; i < item.get_size(); i++)
      for (int j = 0; j < item.get_size(); j++)
         if (coords.first + i >= 0 && coords.first + i < height &&
            coords.second + j >= 0 && coords.second + j < width && item.get_cell_col(i, j) != empty)
            draw_block(window, item.get_cell_col(i, j), std::make_pair(offset.first + (coords.second + j) * 36, offset.second + (coords.first + i) * 36));

   for (int i = 0; i < next.get_size(); i++)
      for (int j = 0; j < next.get_size(); j++)
         if (next.get_cell_col(i, j) != empty)
            draw_block(window, next.get_cell_col(i, j), std::make_pair(offset.first * 10.5 + 36 * j, offset.second * 2.6 + i * 36));
}

void Drawing::draw_intro(sf::RenderWindow &window)
{
   sf::Font font;
   if (!font.loadFromFile("Stuff/pixelchunker.ttf"))
   {
      perror("Error opening file 'pixelchunker.ttf'");
      abort();
   }
   sf::Text text;
   text.setFont(font);
   text.setString("TETRIS");
   text.setCharacterSize(120);
   text.setFillColor(sf::Color::Black);
   text.setOutlineColor(sf::Color::White);
   text.setOutlineThickness(1.0f);
   sf::Vector2u size = window.getSize();
   float text_width = text.getLocalBounds().width,
         text_height = text.getLocalBounds().height;
   text.setPosition(size.x / 2.0f - text_width / 2.0f, size.y / 2.0f - text_height * 3.0f);
   window.draw(text);

   if (!font.loadFromFile("Stuff/unlearne.ttf"))
   {
      perror("Error opening file 'unlearne.ttf'");
      abort();
   }
   text.setFont(font);
   text.setString("PRESS SPACE TO START");
   text.setCharacterSize(60);
   text_width = text.getLocalBounds().width;
   text_height = text.getLocalBounds().height;
   text.setPosition(size.x / 2.0f - text_width / 2.0f, size.y / 2.0f - text_height * 2.0f);
   window.draw(text);
}

void Drawing::draw_ending(sf::RenderWindow & window, int score, int &record)
{
   sf::Font font;
   if (!font.loadFromFile("Stuff/pixelchunker.ttf"))
   {
      perror("Error opening file 'pixelchunker.ttf'");
      abort();
   }
   sf::Text text;
   text.setFont(font);
   text.setString("GAME OVER");
   text.setCharacterSize(100);
   text.setFillColor(sf::Color::Black);
   text.setOutlineColor(sf::Color::White);
   text.setOutlineThickness(1.0f);
   sf::Vector2u size = window.getSize();
   float text_width = text.getLocalBounds().width,
         text_height = text.getLocalBounds().height;
   text.setPosition(size.x / 2.0f - text_width / 2.0f, size.y / 2.0f - text_height * 3.0f);
   window.draw(text);

   if (!font.loadFromFile("Stuff/unlearne.ttf"))
   {
      perror("Error opening file 'unlearne.ttf'");
      abort();
   }
   text.setFont(font);
   text.setCharacterSize(60);
   text.setString("PRESS SPACE TO RESTART");
   text_width = text.getLocalBounds().width;
   text_height = text.getLocalBounds().height;
   text.setPosition(size.x / 2.0f - text_width / 2.0f, size.y / 2.0f - text_height / 2.0f);
   window.draw(text);

   text.setString("SCORE:");
   text.setPosition(size.x / 2.0f - text_width / 2.0f, size.y / 2.0f - text_height * 2.5f);
   window.draw(text);

   text.setString(std::to_string(score));
   text.setPosition(size.x / 2.0f - text_width / 5.5f, size.y / 2.0f - text_height * 2.5f);
   window.draw(text);

   if (!is_rec_checked)
   {
       is_rec_checked = true;

       if (score > record)
       {
           record = score;
           is_rec_achieved = true;
       }
   }

   if (is_rec_achieved)
   {
       record = score;
       text.setString("YOU SET A RECORD!");
       text_width = text.getLocalBounds().width;
       text.setPosition(size.x / 2.0f - text_width / 1.5f, size.y / 2.0f - text_height * 4.5f);
       window.draw(text);
   }
   else
   {
       text.setString("RECORD: ");
       text_width = text.getLocalBounds().width;
       text.setPosition(size.x / 2.0f - text_width * 1.45f, size.y / 2.0f - text_height * 4.5f);
       window.draw(text);

       text.setString(std::to_string(record));
       text.setPosition(size.x / 2.0f - text_width / 2.0f, size.y / 2.0f - text_height * 4.5f);
       window.draw(text);
   }
}

/***********************************************************************************************************/

Game::Game()
{
   score = 0;
   //record = 0;
   matrix list_item = 
    { { empty, empty, empty, empty },
      { green, green, green, green },
      { empty, empty, empty, empty },
      { empty, empty, empty, empty } };
   figure_list.push_back(Figure(list_item));      // figure I

   list_item =
   { { empty, cyan, empty },
     { cyan, cyan, cyan },
     { empty, empty, empty } };
   figure_list.push_back(Figure(list_item));      // figure T

   list_item =
   { { empty, blue, empty },
     { empty, blue, empty },
     { blue, blue, empty } };
   figure_list.push_back(Figure(list_item));      // figure J

   list_item =
   { { empty, red, empty },
     { empty, red, empty },
     { empty, red, red } };
   figure_list.push_back(Figure(list_item));      // figure L

   list_item =
   { { purple, purple},
     { purple, purple} };
   figure_list.push_back(Figure(list_item));      // figure O

   list_item =
   { { empty, yellow, yellow },
     { yellow, yellow, empty },
     { empty, empty, empty } };
   figure_list.push_back(Figure(list_item));      // figure S

   list_item =
   { { orange, orange, empty },
     { empty, orange, orange },
     { empty, empty, empty } };
   figure_list.push_back(Figure(list_item));      // figure Z

   list_item.clear();
   next = figure_list[rand() % 7];
   coords = std::make_pair(-1, -1);
}

Game::~Game()
{ figure_list.clear(); }

void Game::intro(sf::RenderWindow &window)
{ draw_intro(window); }

void Game::ending(sf::RenderWindow &window)
{ draw_ending(window, score, record); }

void Game::draw(sf::RenderWindow & window)
{
   draw_field(window, area);
   draw_figure(window, item, next, coords);
   draw_game_data(window, score);
}

void Game::set_record(int rec)
{ record = rec; }

int Game::get_record() const
{ return record; }

void Game::set_coords(moves mode)
{
   switch (mode)
   {
   case none:
      coords = std::make_pair(0, (width - item.get_width()) / 2);
      for (int i = 0; i < item.get_size(); i++)
         if (item.empty_row(i))
            coords.first--;
         else
            break;
      break;
   case down:
      coords.first++;
      break;
   case right:
      coords.second++;
      break;
   case left:
      coords.second--;
      break;
   default:
      break;
   }
}

void Game::add_figure()
{
   for (int i = 0; i < item.get_size(); i++)
      for (int j = 0; j < item.get_size(); j++)
         if (item.get_cell_col(i, j) != empty)
            area.set_cell_col(coords.first + i, coords.second + j, item.get_cell_col(i, j));
}

bool Game::collisions(moves mode) const
{
   switch (mode)
   {
   case right:
   {
      if (coords.second + item.get_size() < width || item.empty_column(width - coords.second - 1))
      {
         for (int i = 0; i < item.get_size(); i++)
            for (int j = 0; j < item.get_size(); j++)
               if (!item.empty_cell(i, j) && area.empty_cell(coords.first + i, coords.second + j + 1) != outside &&
                   area.empty_cell(coords.first + i, coords.second + j + 1) != empty)
                  return false;
         return true;
      }
      return false;
   }
   case left:
   {
      if (coords.second > 0 || item.empty_column(absolute(coords.second)))
      {
         for (int i = 0; i < item.get_size(); i++)
            for (int j = 0; j < item.get_size(); j++)
               if (!item.empty_cell(i, j) && area.empty_cell(coords.first + i, coords.second + j - 1) != outside &&
                   area.empty_cell(coords.first + i, coords.second + j - 1) != empty)
               return false;
         return true;
      }
      return false;
   }
   case down:
   {
      int check = item.get_size() - 1;
      while (item.empty_row(check))
         check--;
      if (coords.first + check + 1 >= height) return false;

      for (int i = 0; i < item.get_size(); i++)
         for (int j = 0; j < item.get_size(); j++)
            if (!item.empty_cell(i, j) && area.empty_cell(coords.first + i + 1, coords.second + j) != outside &&
                area.empty_cell(coords.first + i + 1, coords.second + j) != empty)
               return false;
      return true;
   }
   case rotate:
   {
      Figure check_item = item;
      check_item.rotate();

      for (int i = 0; i < item.get_size(); i++)
         for (int j = 0; j < item.get_size(); j++)
            if (!check_item.empty_cell(i, j) && area.empty_cell(coords.first + i, coords.second + j) != empty)
               return false;
      return true;
   }
   default:
      return false;
   }
}

bool Game::new_figure()
{
   item = next;
   next = figure_list[rand() % 7];
   set_coords(none);
   return collisions(down);
}

void Game::moving(moves type)
{
   if (collisions(type))
   {
      if (type == rotate)
         item.rotate();
      else
         set_coords(type);
   }
}

bool Game::falling()
{
   if (collisions(down))
   {
      set_coords(down);
      return true;
   }
   else
   {
      add_figure();
      score += 100;
      for (int i = height - 1; i >= 0; i--)
         if (area.full_row(i))
         {
            score += 500;
            i++;
         }
      item.clear();
   }
   return false;
}

void Game::restart()
{
   area.clear_field();
   item.clear();
   next = figure_list[rand() % 7];
   coords = std::make_pair(-1, -1);
   score = 0;
   is_rec_checked = false;
   is_rec_achieved = false;
}
