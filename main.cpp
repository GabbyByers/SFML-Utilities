#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
#include <vector>
#include <SFML/Graphics.hpp>
#include <json/json.h>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <algorithm>
#include <typeinfo>

class Game {
public:
    int screenWidth = 1600;
    int screenHeight = 900;

    sf::Font CourierPrime_Regular;

    Game() {
        srand(time(nullptr)); //seed
        CourierPrime_Regular.loadFromFile("../Fonts/CourierPrime-Regular.ttf");
    }
};
Game game;

class Mouse {
public:
    int x = 0;
    int y = 0;
    int prev_x = 0;
    int prev_y = 0;
    int rel_x = 0;
    int rel_y = 0;
    bool isOverScreen = false;

    void setMouseProperties(sf::Vector2i vect) {
        checkMouseOverScreen(vect);
        setMousePosition(vect);
        getRelativeMouseMovement(vect);
    }

    void checkMouseOverScreen(sf::Vector2i vect) {
        isOverScreen = false;
        if (0 < vect.x && vect.x < game.screenWidth) {
            if (0 < vect.y && vect.y < game.screenHeight) {
                isOverScreen = true;
            }
        }
    }

    void setMousePosition(sf::Vector2i vect) {
        if (0 < vect.x && vect.x < game.screenWidth) {
            x = vect.x;
        }
        if (0 < vect.y && vect.y < game.screenHeight) {
            y = vect.y;
        }
    }

    void getRelativeMouseMovement(sf::Vector2i vect) {
        rel_x = vect.x - prev_x;
        rel_y = vect.y - prev_y;
        prev_x = vect.x;
        prev_y = vect.y;
    }
};
Mouse mouse;

class box {
public:
    double x = 1.0;
    double y = 1.0;
    double w = 1.0;
    double h = 1.0;



    sf::Color color = sf::Color(255, 255, 255);

    int index = -1;

    box(std::vector<sf::Vertex>& boxes) {
        push(boxes);
    }

    void push(std::vector<sf::Vertex>& boxes) {
        sf::Vertex a = sf::Vertex(sf::Vector2f(x    , y), color);
        sf::Vertex b = sf::Vertex(sf::Vector2f(x + w, y), color);
        sf::Vertex c = sf::Vertex(sf::Vector2f(x + w, y + h), color);
        sf::Vertex d = sf::Vertex(sf::Vector2f(x    , y + h), color);
        index = boxes.size();
        boxes.push_back(a);
        boxes.push_back(b);
        boxes.push_back(b);
        boxes.push_back(c);
        boxes.push_back(c);
        boxes.push_back(d);
        boxes.push_back(d);
        boxes.push_back(a);
    }

    void setColor(sf::Color newColor, std::vector<sf::Vertex>& boxes) {
        color = newColor;
        for (int i = index; i < index + 8; i++) {
            sf::Vertex& vertex = boxes[i];
            vertex.color = color;
        }
    }

    void setDim(double x, double y, double w, double h, std::vector<sf::Vertex>& boxes) {
        boxes[index + 0].position = sf::Vector2f(x, y);
        boxes[index + 1].position = sf::Vector2f(x + w, y);
        boxes[index + 2].position = sf::Vector2f(x + w, y);
        boxes[index + 3].position = sf::Vector2f(x + w, y + h);
        boxes[index + 4].position = sf::Vector2f(x + w, y + h);
        boxes[index + 5].position = sf::Vector2f(x, y + h);
        boxes[index + 6].position = sf::Vector2f(x, y + h);
        boxes[index + 7].position = sf::Vector2f(x, y);
    }

    void invisible(std::vector<sf::Vertex>& boxes) {
        for (int i = index; i < index + 8; i++) {
            sf::Vertex& vertex = boxes[i];
            vertex.color.a = 0;
        }
    }

    void visible(std::vector<sf::Vertex>& boxes) {
        for (int i = index; i < index + 8; i++) {
            sf::Vertex& vertex = boxes[i];
            vertex.color.a = 255;
        }
    }
};

class rect {
public:
    double x = 1.0;
    double y = 1.0;
    double w = 1.0;
    double h = 1.0;

    sf::Color color = sf::Color(255, 255, 255);

    int index = -1;

    rect(std::vector<sf::Vertex>& rectangles) {
        push(rectangles);
    }

    void push(std::vector<sf::Vertex>& rectangles) {
        index = rectangles.size();
        rectangles.push_back(sf::Vertex(sf::Vector2f(x    , y), color));
        rectangles.push_back(sf::Vertex(sf::Vector2f(x + w, y), color));
        rectangles.push_back(sf::Vertex(sf::Vector2f(x + w, y + h), color));
        rectangles.push_back(sf::Vertex(sf::Vector2f(x    , y + h), color));
    }

    void setColor(sf::Color newColor, std::vector<sf::Vertex>& rectangles) {
        color = newColor;
        for (int i = index; i < index + 4; i++) {
            sf::Vertex& vertex = rectangles[i];
            vertex.color = color;
        }
    }

    void setDim(double x, double y, double w, double h, std::vector<sf::Vertex>& boxes) {
        boxes[index + 0].position = sf::Vector2f(x    , y);
        boxes[index + 1].position = sf::Vector2f(x + w, y);
        boxes[index + 2].position = sf::Vector2f(x + w, y + h);
        boxes[index + 3].position = sf::Vector2f(x    , y + h);
    }

    void invisible(std::vector<sf::Vertex>& boxes) {
        for (int i = index; i < index + 4; i++) {
            sf::Vertex& vertex = boxes[i];
            vertex.color.a = 0;
        }
    }

    void visible(std::vector<sf::Vertex>& boxes) {
        for (int i = index; i < index + 4; i++) {
            sf::Vertex& vertex = boxes[i];
            vertex.color.a = 255;
        }
    }
};

class widget {
public:
    double x = 0.0, y = 0.0;
    double w = 0.0, h = 0.0;

    widget() {}


};

int main()
{
    sf::RenderWindow window(sf::VideoMode(game.screenWidth, game.screenHeight), "Hello SFML", sf::Style::Close);
    sf::Clock clock;

    std::vector<int> stupid_vector;

    stupid_vector.push_back(33);
    stupid_vector.push_back(23);
    stupid_vector.push_back(55);
    stupid_vector.push_back(3);
    stupid_vector.push_back(-10);

    std::cout << &stupid_vector << std::endl;



    std::vector<sf::Vertex> boxes;
    std::vector<sf::Vertex> rectangles;

    rect goofy_ahh_rect(rectangles);
    rect goofy_ahh_rect_(rectangles);
    rect goofy_ahh_rect__(rectangles);
    goofy_ahh_rect.setDim(300, 300, 40, 90, rectangles);
    goofy_ahh_rect.setColor(sf::Color(255, 255, 0), rectangles);
    goofy_ahh_rect_.setDim(350, 300, 40, 90, rectangles);
    goofy_ahh_rect_.setColor(sf::Color(0, 255, 255), rectangles);
    goofy_ahh_rect__.setDim(400, 300, 40, 90, rectangles);
    goofy_ahh_rect__.setColor(sf::Color(255, 0, 255), rectangles);
    //goofy_ahh_rect_.invisible(rectangles);


    box silly_test_box(boxes);
    box silly_test_box_2(boxes);
    silly_test_box.setColor(sf::Color(255, 0, 255), boxes);
    silly_test_box_2.setColor(sf::Color(0, 255, 255), boxes);
    silly_test_box.setDim(100, 100, 50, 50, boxes);
    silly_test_box_2.setDim(120, 120, 50, 50, boxes);

    silly_test_box.invisible(boxes);
    silly_test_box.visible(boxes);

    while (window.isOpen()) {
        mouse.setMouseProperties(sf::Mouse::getPosition(window));

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        goofy_ahh_rect_.setDim(350, 300, mouse.x - 350, mouse.y - 300, rectangles);

        window.clear(sf::Color(0, 0, 0));
        window.draw(&boxes[0], boxes.size(), sf::Lines);
        window.draw(&rectangles[0], rectangles.size(), sf::Quads);
        window.display();
    }
    return 0;
}
