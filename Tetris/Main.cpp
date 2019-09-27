#include "Game.h"
#include <chrono>
#include <thread>
#include <ctime>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <ShlObj.h>

// TODO: figure selection function,
//       clockwise rotation

namespace fs = std::experimental::filesystem;

std::wstring get_path()
{
    TCHAR documents[MAX_PATH];

    HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, documents);
    if (result == S_OK)
    {
        std::wstringstream path;
        path << documents;
        return path.str();
    }
    return L"";
}

int main()
{
    srand(time(NULL));
    Game game;

    // opening pictures

    sf::RenderWindow window(sf::VideoMode(800, 700), "Tetris", sf::Style::Close);
    sf::Image icon;
    if (!icon.loadFromFile("Stuff/Icon.png"))
    {
        perror("Error opening file 'Icon.png'");
        abort();
    }
    window.setActive();
    window.setIcon(37, 37, icon.getPixelsPtr());
    window.setVerticalSyncEnabled(true);

    sf::Color back_color(227, 228, 255, 255);

    sf::Texture texture;
    if (!texture.loadFromFile("Stuff/Theme_texture.png", sf::IntRect(0, 0, 800, 700)))
    {
        perror("Error opening file 'Theme_texture.png'");
        abort();
    }
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 800, 700));

    game.init(window);

    // set variables

    bool start = false, fall = false,
         end = false, pause = false, move = true;

    std::chrono::high_resolution_clock::time_point t1, t2;
    double speed = 0.6f;

    // opening file

    fs::path file_path = get_path().append(L"\\Tetris");

    if (!fs::exists(file_path))
        fs::create_directories(file_path);

    file_path.append(L"Record.bin");

    int rec = 0;
    std::ifstream fin(file_path, std::ios::binary);
    if (fin.is_open())
    {
        fin >> rec;
        game.set_record(rec);
    }
    fin.close();

    // game start

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
            {
                pause = !pause;
                if (pause)
                    t1 = std::chrono::high_resolution_clock::now();
            }
            if (event.type == sf::Event::LostFocus)
            {
                pause = true;
                t1 = std::chrono::high_resolution_clock::now();
            }
            if (event.type == sf::Event::GainedFocus)
                pause = false;
        }

        window.clear(back_color);
        window.draw(sprite);

        if (!start)
        {
            game.intro(window);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                start = !start;
            t1 = std::chrono::high_resolution_clock::now();
        }
        else
        {
            if (!end)
            {
                if (!pause)
                {
                    if (!fall)
                    {
                        if (game.new_figure())
                            fall = !fall;
                        else
                            end = !end;
                    }
                    else
                    {
                        if (event.type == sf::Event::KeyReleased)
                            move = true;
                        else
                        {
                            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                                game.moving(right);
                            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                                game.moving(left);
                            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                                game.moving(down);
                            else if (move && sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                                game.moving(rotate);
                            move = false;

                            std::this_thread::sleep_for(std::chrono::milliseconds(70));
                        }

                        rec = game.get_record();

                        if (rec >= 50000)
                            speed = 0.35f;
                        else if (rec >= 40000)
                            speed = 0.4f;
                        else if (rec >= 30000)
                            speed = 0.45f;
                        else if (rec >= 20000)
                            speed = 0.5f;
                        else if (game.get_record() >= 10000)
                            speed = 0.55f;

                        t2 = std::chrono::high_resolution_clock::now();

                        if (std::chrono::duration<double>(t2 - t1).count() >= speed)
                        {
                            if (!game.falling())
                                fall = !fall;
                            t1 = std::chrono::high_resolution_clock::now();
                        }
                    }
                }
                game.draw(window);
            }
            else
            {
                game.ending(window);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                {
                    game.restart();
                    end = false;
                    fall = false;
                    move = true;
                }
            }
        }

        window.display();
    }

    // game ending and closing file

    std::ofstream fout(file_path, std::ios::binary);
    fout << game.get_record();
    fout.close();

    return 0;
}