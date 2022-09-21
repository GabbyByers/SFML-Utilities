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
    double xpos = 1.0, ypos = 1.0;
    double width = 1.0, height = 1.0;

    sf::Color color = sf::Color(255, 255, 255);

    int index = -1;

    box(std::vector<sf::Vertex>& boxes) {
        push(boxes);
    }

    void push(std::vector<sf::Vertex>& boxes) {
        sf::Vertex a = sf::Vertex(sf::Vector2f(xpos, ypos), color);
        sf::Vertex b = sf::Vertex(sf::Vector2f(xpos + width, ypos), color);
        sf::Vertex c = sf::Vertex(sf::Vector2f(xpos + width, ypos + height), color);
        sf::Vertex d = sf::Vertex(sf::Vector2f(xpos, ypos + height), color);
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
        xpos = x;
        ypos = y;
        width = w;
        height = h;
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
    double xpos = 1.0, ypos = 1.0;
    double width = 1.0, height = 1.0;

    sf::Color color = sf::Color(255, 255, 255);

    int index = -1;

    rect(std::vector<sf::Vertex>& rectangles) {
        push(rectangles);
    }

    void push(std::vector<sf::Vertex>& rectangles) {
        index = rectangles.size();
        rectangles.push_back(sf::Vertex(sf::Vector2f(xpos, ypos)));
        rectangles.push_back(sf::Vertex(sf::Vector2f(xpos + width, ypos)));
        rectangles.push_back(sf::Vertex(sf::Vector2f(xpos + width, ypos + height)));
        rectangles.push_back(sf::Vertex(sf::Vector2f(xpos, ypos + height)));
    }

    void setColor(sf::Color newColor, std::vector<sf::Vertex>& rectangles) {
        color = newColor;
        for (int i = index; i < index + 4; i++) {
            sf::Vertex& vertex = rectangles[i];
            vertex.color = color;
        }
    }

    void setDim(double x, double y, double w, double h, std::vector<sf::Vertex>& boxes) {
        xpos = x;
        ypos = y;
        width = w;
        height = h;
        boxes[index + 0].position = sf::Vector2f(x, y);
        boxes[index + 1].position = sf::Vector2f(x + w, y);
        boxes[index + 2].position = sf::Vector2f(x + w, y + h);
        boxes[index + 3].position = sf::Vector2f(x, y + h);
    }

    void setTextureDim(double x, double y, double w, double h, std::vector<sf::Vertex>& boxes) {
        boxes[index + 0].texCoords = sf::Vector2f(x, y);
        boxes[index + 1].texCoords = sf::Vector2f(x + w, y);
        boxes[index + 2].texCoords = sf::Vector2f(x + w, y + h);
        boxes[index + 3].texCoords = sf::Vector2f(x, y + h);
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

int main()
{
    sf::RenderWindow window(sf::VideoMode(game.screenWidth, game.screenHeight), "Hello SFML", sf::Style::Close);
    sf::Clock clock;

    std::vector<sf::Vertex> boxes;
    std::vector<sf::Vertex> rectangles;
    sf::Texture rectTexture;
    if (!rectTexture.loadFromFile("images/sprites.png")) { std::cout << "nope" << std::endl; }

    //rainbow
    std::vector<rect> rect_instances;
    for (int i = 0; i < 255; i++) {
        rect rect(rectangles);
        //rect.setColor(sf::Color(255 - i, 0, i), rectangles);
        rect.setDim(i * 5.843 + 40, i * 2 + 40, 30, 30, rectangles);
        rect_instances.push_back(rect);
    }
    rect& rect = rect_instances[150];
    rect.setDim(rect.xpos, rect.ypos - 50, 30, 30, rectangles);

    std::vector<box> box_instances;
    for (int i = 0; i < 255; i++) {
        box box(boxes);
        box.setColor(sf::Color(255 - i, 0, i), boxes);
        box.setDim(i * 5.843 + 40, i * 2 + 200, 30, 30, boxes);
        box_instances.push_back(box);
    }
    box& box = box_instances[150];
    box.setDim(box.xpos, box.ypos - 50, 30, 30, boxes);

    while (window.isOpen()) {
        mouse.setMouseProperties(sf::Mouse::getPosition(window));

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color(0, 0, 0));
        if (boxes.size() > 0) { window.draw(&boxes[0], boxes.size(), sf::Lines); }
        if (rectangles.size() > 0) { window.draw(&rectangles[0], rectangles.size(), sf::Quads, &rectTexture); }
        window.display();
    }
    return 0;
}