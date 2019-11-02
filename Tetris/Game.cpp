#include "Game.h"
#include <algorithm>

bool is_rec_checked = false, is_rec_achieved = false;

auto absolute = [](int num)
{ return num < 0 ? num * (-1) : num; };

void Drawing::drawing_init(sf::RenderWindow &window)
{
    if (!textures.loadFromFile("Stuff/Textures.png", sf::IntRect(0, 0, 289, 37)))
    {
        perror("Error opening file 'Textures.png'");
        abort();
    }

    if (!main_font.loadFromFile("Stuff/unlearne.ttf"))
    {
        perror("Error opening file 'unlearne.ttf'");
        abort();
    }
    main_text.setFont(main_font);
    main_text.setCharacterSize(60);
    main_text.setFillColor(sf::Color::Black);
    main_text.setOutlineColor(sf::Color::White);
    main_text.setOutlineThickness(1.0f);

    if (!intro_font.loadFromFile("Stuff/pixelchunker.ttf"))
    {
        perror("Error opening file 'pixelchunker.ttf'");
        abort();
    }
    intro_text.setFont(intro_font);
    intro_text.setCharacterSize(110);
    intro_text.setFillColor(sf::Color::Black);
    intro_text.setOutlineColor(sf::Color::White);
    intro_text.setOutlineThickness(1.0f);

    size = window.getSize();
}
void Drawing::draw_block(sf::RenderWindow &window, fill color, std::pair<int, int> b_coords)
{
    sf::Sprite sprite;
    sprite.setTexture(textures);
    sprite.setTextureRect(sf::IntRect(36 * static_cast<int>(color), 0, 37, 37));
    sprite.move(sf::Vector2f(b_coords.first, b_coords.second));
    window.draw(sprite);
}

void Drawing::draw_game_data(sf::RenderWindow &window, int score)
{
    main_text.setString("SCORE:");

    float text_width = main_text.getLocalBounds().width,
        text_height = main_text.getLocalBounds().height;
    main_text.setPosition(size.x - text_width * 2.2f, size.y / 2.0f - text_height * 7.4f);
    window.draw(main_text);

    main_text.setString(std::to_string(score));
    main_text.setPosition(size.x - text_width, size.y / 2.0f - text_height * 7.4f);
    window.draw(main_text);

    main_text.setString("NEXT FIGURE:");
    main_text.setPosition(size.x - text_width * 2.2f, size.y / 2.0f - text_height * 5.0f);
    window.draw(main_text);

    main_text.setString("PAUSE: P");
    main_text.setPosition(size.x - text_width * 2.2f, size.y / 1.8f);
    window.draw(main_text);

    main_text.setString("EXIT: ESC");
    main_text.setPosition(size.x - text_width * 2.2f, size.y / 1.5f);
    window.draw(main_text);
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
            coords.second + j >= 0 && coords.second + j < width && item.get_cell_col(i, j) != fill::empty)
            draw_block(window, item.get_cell_col(i, j), std::make_pair(offset.first + (coords.second + j) * 36, offset.second + (coords.first + i) * 36));

    for (int i = 0; i < next.get_size(); i++)
        for (int j = 0; j < next.get_size(); j++)
            if (next.get_cell_col(i, j) != fill::empty)
            draw_block(window, next.get_cell_col(i, j), std::make_pair(offset.first * 10.5 + 36 * j, offset.second * 2.6 + i * 36));
}

void Drawing::draw_intro(sf::RenderWindow &window)
{
    intro_text.setString("TETRIS");

    float text_width = intro_text.getLocalBounds().width,
        text_height = intro_text.getLocalBounds().height;
    intro_text.setPosition(size.x / 2.0f - text_width / 2.0f - 10, size.y / 2.0f - text_height * 3.0f);
    window.draw(intro_text);

    main_text.setString("PRESS SPACE TO START");
    text_width = main_text.getLocalBounds().width;
    text_height = main_text.getLocalBounds().height;
    main_text.setPosition(size.x / 2.0f - text_width / 2.0f, size.y / 2.0f - text_height * 2.0f);
    window.draw(main_text);
}

void Drawing::draw_ending(sf::RenderWindow & window, int score, int &record)
{
    intro_text.setString("GAME OVER");

    float text_width = intro_text.getLocalBounds().width,
        text_height = intro_text.getLocalBounds().height;
    intro_text.setPosition(size.x / 2.0f - text_width / 2.0f - 10, size.y / 2.0f - text_height * 3.0f);
    window.draw(intro_text);

    main_text.setString("PRESS SPACE TO RESTART");
    text_width = main_text.getLocalBounds().width;
    text_height = main_text.getLocalBounds().height;
    main_text.setPosition(size.x / 2.0f - text_width / 2.0f, size.y / 2.0f - text_height / 2.0f);
    window.draw(main_text);

    main_text.setString("SCORE:");
    main_text.setPosition(size.x / 2.0f - text_width / 2.0f, size.y / 2.0f - text_height * 2.5f);
    window.draw(main_text);

    main_text.setString(std::to_string(score));
    main_text.setPosition(size.x / 2.0f - text_width / 5.5f, size.y / 2.0f - text_height * 2.5f);
    window.draw(main_text);

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
        main_text.setString("YOU SET A RECORD!");
        text_width = main_text.getLocalBounds().width;
        main_text.setPosition(size.x / 2.0f - text_width / 1.5f, size.y / 2.0f - text_height * 4.5f);
        window.draw(main_text);
    }
    else
    {
        main_text.setString("RECORD: ");
        text_width = main_text.getLocalBounds().width;
        main_text.setPosition(size.x / 2.0f - text_width * 1.45f, size.y / 2.0f - text_height * 4.5f);
        window.draw(main_text);

        main_text.setString(std::to_string(record));
        main_text.setPosition(size.x / 2.0f - text_width / 2.0f, size.y / 2.0f - text_height * 4.5f);
        window.draw(main_text);
    }
}

/***********************************************************************************************************/

Game::Game()
{
    score = 0;
    //record = 0;
    matrix list_item = 
    { { fill::empty, fill::empty, fill::empty, fill::empty },
        { fill::green, fill::green, fill::green, fill::green },
        { fill::empty, fill::empty, fill::empty, fill::empty },
        { fill::empty, fill::empty, fill::empty, fill::empty } };
    figure_list.push_back(Figure(list_item, 'I'));       // figure I

    list_item =
    { { fill::empty, fill::cyan, fill::empty },
        { fill::cyan, fill::cyan, fill::cyan },
        { fill::empty, fill::empty, fill::empty } };
    figure_list.push_back(Figure(list_item, 'T'));      // figure T

    list_item =
    { { fill::empty, fill::blue, fill::empty },
        { fill::empty, fill::blue, fill::empty },
        { fill::blue, fill::blue, fill::empty } };
    figure_list.push_back(Figure(list_item, 'J'));      // figure J

    list_item =
    { { fill::empty, fill::red, fill::empty },
        { fill::empty, fill::red, fill::empty },
        { fill::empty, fill::red, fill::red } };
    figure_list.push_back(Figure(list_item, 'L'));      // figure L

    list_item =
    { { fill::purple, fill::purple},
        { fill::purple, fill::purple} };
    figure_list.push_back(Figure(list_item, 'O'));      // figure O

    list_item =
    { { fill::empty, fill::yellow, fill::yellow },
        { fill::yellow, fill::yellow, fill::empty },
        { fill::empty, fill::empty, fill::empty } };
    figure_list.push_back(Figure(list_item, 'S'));      // figure S

    list_item =
    { { fill::orange, fill::orange, fill::empty },
        { fill::empty, fill::orange, fill::orange },
        { fill::empty, fill::empty, fill::empty } };
    figure_list.push_back(Figure(list_item, 'Z'));      // figure Z

    list_item.clear();

    order.resize(0);
    history.resize(0);
    pool.resize(35);

    for (int i = 0; i < 7; i++)  // 7 figures
    {
        pool[i] = pool[i + 7] = pool[i + 14] = pool[i + 21] = pool[i + 28] = 
            figure_list[i].get_name();
    }

    char first_fig_name = figure_list[rand() % 4].get_name();

    history = { 'S', 'Z', 'S', first_fig_name };

    next = pick_figure();
    coords = std::make_pair(-1, -1);
}

Game::~Game()
{ figure_list.clear(); }

void Game::init(sf::RenderWindow &window)
{ drawing_init(window); }

void Game::intro(sf::RenderWindow &window)
{ draw_intro(window); }

void Game::ending(sf::RenderWindow &window)
{ draw_ending(window, score, record); }

void Game::draw(sf::RenderWindow &window)
{
    draw_field(window, area);
    draw_figure(window, item, next, coords);
    draw_game_data(window, score);
}

void Game::set_record(int rec)
{ record = rec; }

int Game::get_record() const
{ return record; }

int Game::get_score() const
{ return score; }

void Game::set_coords(moves mode)
{
    switch (mode)
    {
    case moves::none:
        coords = std::make_pair(0, (width - item.get_width()) / 2);
        for (int i = 0; i < item.get_size(); i++)
            if (item.empty_row(i))
            coords.first--;
            else
            break;
        break;
    case moves::down:
        coords.first++;
        break;
    case moves::right:
        coords.second++;
        break;
    case moves::left:
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
        if (item.get_cell_col(i, j) != fill::empty)
            area.set_cell_col(coords.first + i, coords.second + j, item.get_cell_col(i, j));
}

Figure Game::pick_figure()
{
    char name;

    for (int i = 0, j = 0; i < 6; i++)
    {
        j = rand() % 35;
        name = pool[j];

        if (i == 5 || std::find(history.begin(), history.end(), name) == history.end())
            break;
        if (order.size())
            pool[j] = order[0];
    }

    auto it1 = std::find(order.begin(), order.end(), name);
    if (it1 != order.end()) 
        order.erase(it1);
    order.push_back(name);

    std::rotate(history.begin(), history.begin() + 1, history.end());
    history[3] = name;

    auto it2 = std::find_if(figure_list.begin(), figure_list.end(), [name](Figure fig) { return fig.get_name() == name; });
    auto index = std::distance(figure_list.begin(), it2);

    return figure_list[index];
}

bool Game::collisions(moves mode) const
{
    switch (mode)
    {
    case moves::right:
    {
        if (coords.second + item.get_size() < width || item.empty_column(width - coords.second - 1))
        {
            for (int i = 0; i < item.get_size(); i++)
            for (int j = 0; j < item.get_size(); j++)
                if (!item.empty_cell(i, j) && area.empty_cell(coords.first + i, coords.second + j + 1) != fill::outside &&
                    area.empty_cell(coords.first + i, coords.second + j + 1) != fill::empty)
                    return false;
            return true;
        }
        return false;
    }
    case moves::left:
    {
        if (coords.second > 0 || item.empty_column(absolute(coords.second)))
        {
            for (int i = 0; i < item.get_size(); i++)
            for (int j = 0; j < item.get_size(); j++)
                if (!item.empty_cell(i, j) && area.empty_cell(coords.first + i, coords.second + j - 1) != fill::outside &&
                    area.empty_cell(coords.first + i, coords.second + j - 1) != fill::empty)
                return false;
            return true;
        }
        return false;
    }
    case moves::down:
    {
        int check = item.get_size() - 1;
        while (item.empty_row(check))
            check--;
        if (coords.first + check + 1 >= height) return false;

        for (int i = 0; i < item.get_size(); i++)
            for (int j = 0; j < item.get_size(); j++)
            if (!item.empty_cell(i, j) && area.empty_cell(coords.first + i + 1, coords.second + j) != fill::outside &&
                area.empty_cell(coords.first + i + 1, coords.second + j) != fill::empty)
                return false;
        return true;
    }
    case moves::rotate:
    {
        Figure check_item = item;
        check_item.rotate();

        for (int i = 0; i < item.get_size(); i++)
            for (int j = 0; j < item.get_size(); j++)
            if (!check_item.empty_cell(i, j) && area.empty_cell(coords.first + i, coords.second + j) != fill::empty)
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
    next = pick_figure();
    set_coords(moves::none);
    return collisions(moves::down);
}

void Game::moving(moves type)
{
    if (collisions(type))
    {
        if (type == moves::rotate)
            item.rotate();
        else
            set_coords(type);
    }
}

bool Game::falling()
{
    if (collisions(moves::down))
    {
        set_coords(moves::down);
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
    order.resize(0);

    for (int i = 0; i < 7; i++)  // 7 figures
    {
        pool[i] = pool[i + 7] = pool[i + 14] = pool[i + 21] = pool[i + 28] =
            figure_list[i].get_name();
    }

    char first_fig_name = figure_list[rand() % 4].get_name();
    history = { 'S', 'Z', 'S', first_fig_name };
    next = pick_figure();

    coords = std::make_pair(-1, -1);
    score = 0;
    is_rec_checked = false;
    is_rec_achieved = false;
}
