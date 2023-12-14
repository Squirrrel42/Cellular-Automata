#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>

#define WHITE sf::Color::White
#define BLACK sf::Color::Black
#define GRAY sf::Color(153, 153, 153)

class Life {
    bool* grid;
    int const height, width;

public:
    Life() : height(25), width(25) {
        // конструктор по умолчанию
        grid = new bool[height * width];
        clear();
    }

    Life(int height, int width) : height(height), width(width) {
        // конструктор с заданной шириной и длиной
        grid = new bool[height * width];
        clear();
    }

    Life(Life& other) : height(other.get_height()), width(other.get_width()) {
        // конструктор копирования
        grid = new bool[height * width];
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                (*this)(x, y) = other(x, y);
            }
        }
    }

    ~Life() {
        // деструктор
        delete[] grid;
    }

    int get_height() const { return height; } // возвращает высоту

    int get_width() const { return width; } // возвращает ширину

    bool& operator()(int x, int y) {
        // доступ к элементу с координатами x, y
        while (x >= width) x -= width;
        while (y >= height) y -= height;
        while (x < 0) x += width;
        while (y < 0) y += height;
        return grid[x + y * width];
    }

    void clear() {
        // делает все клеточки мертвыми
        for (int idx = 0; idx < height * width; ++idx) {
            grid[idx] = false;
        }
    }

    void random() {
        // делает все клеточки случайными
        srand(time(NULL));
        for (int idx = 0; idx < height * width; ++idx) {
            if (rand() % 2 == 0)
                grid[idx] = false;
            else
                grid[idx] = true;
        }
    }

    void iteration() {
        // итерация
        int neighbours;

        Life prev(*this);   

        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                // пробегаем по каждой клеточке
                neighbours = 0;
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        // считаем живых соседей
                        if (prev(x + i, y + j))
                            ++neighbours;
                    }
                }

                if (prev(x, y))
                    --neighbours; // если клетка живая, она тоже посчитается за соседа, поэтому мы ее вычитам

                // определяем новое состояние клеточки
                if (prev(x, y)) {
                    if (neighbours == 2 || neighbours == 3)
                        (*this)(x, y) = true;
                    else
                        (*this)(x, y) = false;
                }
                else {
                    if (neighbours == 3)
                        (*this)(x, y) = true;
                    else
                        (*this)(x, y) = false;
                }
            }
        }
    }
};

int main()
{
    const int CELL_SIZE = 10;
    const sf::Vector2f CELL_VECTOR(CELL_SIZE, CELL_SIZE);
    const int GRID_WIDTH = 150;
    const int GRID_HEIGHT = 80;
    const int N_CELLS = GRID_WIDTH * GRID_HEIGHT;
    
    Life life(GRID_HEIGHT, GRID_WIDTH);

    life.random();
    
    int delay = 50;

    bool isPlaying = true;

    sf::Font font;
    font.loadFromFile("./fonts/arial.ttf");

    sf::Text textPause("Press space to pause.", font);
    textPause.setCharacterSize(15);
    textPause.setPosition(10, CELL_SIZE * GRID_HEIGHT + 5);
    textPause.setFillColor(BLACK);

    sf::Text textPlay("Press space to play.", font);
    textPlay.setCharacterSize(15);
    textPlay.setPosition(10, CELL_SIZE * GRID_HEIGHT + 5);
    textPlay.setFillColor(BLACK);

    sf::Text textToggle("Click on cell to toggle live/dead cell.", font);
    textToggle.setCharacterSize(15);
    textToggle.setPosition(10, CELL_SIZE * GRID_HEIGHT + 25);
    textToggle.setFillColor(BLACK);

    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * GRID_WIDTH, CELL_SIZE * GRID_HEIGHT + 50), "Game of Life");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Space)
                    isPlaying = !isPlaying;
                break;

            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    int x = double(event.mouseButton.x) / CELL_SIZE;
                    int y = double(event.mouseButton.y) / CELL_SIZE;
                    if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT)
                        if (life(x, y))
                            life(x, y) = false;
                        else
                            life(x, y) = true;
                }
                break;
            }
        }

        window.clear(WHITE);
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            for (int y = 0; y < GRID_HEIGHT; y++)
            {
                sf::RectangleShape cell;
                cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                cell.setSize(CELL_VECTOR);
                cell.setOutlineThickness(1);
                cell.setOutlineColor(GRAY);
                if (life(x, y))
                    cell.setFillColor(BLACK);
                else
                    cell.setFillColor(WHITE);
                window.draw(cell);
            }
        }

        

        window.draw(textToggle);
        if (isPlaying)
            window.draw(textPause);
        else
            window.draw(textPlay);

        window.display();

        if (isPlaying)
            life.iteration();

        sf::sleep(sf::milliseconds(delay));
    }
}
