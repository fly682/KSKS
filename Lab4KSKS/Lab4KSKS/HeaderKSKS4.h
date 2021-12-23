#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics/RoundedRectangleShape.hpp>
#include <winsock2.h>
#include <iostream>
#include <cmath>
#include <time.h>
#include "GraphicsLib.h"
using namespace std;
using namespace sf;

const int w = 800;
const int h = 800;
RenderWindow window(VideoMode(w, h), "Lab4KSKS");
HANDLE hSerial;

class KSKS : public GraphicsLib {
private:
    int width = w;
    int height = h;
    char comms[50][50] = { "get width", "get height", "clear display", "draw pixel", "draw line", "draw rectangle", "draw rounded rectangle", "draw ellipse",
    "draw circle", "draw text", "fill rectangle", "fill rounded rectangle", "fill ellipse", "fill circle","play minesweeper","scroll text" };
    char fontArr[4][20] = { "Roboto-Black.ttf","comic.ttf","MAGNETOB.ttf","SHOWG.ttf" };
    char* colorBuf;
    int bgColor = -1;
    int renderColor;
    int colorPos;
    bool NaN = false;
    char textBuf[100] = { 0 };
    int textPos;

public:
    KSKS() : GraphicsLib(w, h) {};

    bool FinalResult(char* buff) {
        int id = CheckCommand(buff);
        int* numBuff = ParseCommand();
        return PrintCommand(id, numBuff);
    }

    int GetWidth() {
        return width;
    }

    int GetHeight() {
        return height;
    }

    int CheckCommand(char* buff) {
        char* command = strtok(buff, ":");
        for (int n = 0; n < 50; n++) {
            if (strncmp(command, comms[n], sizeof(comms[n])) == 0) {
                return n;
            }
        }
        return 404;
    }

    int* ParseCommand() {
        int* numAttr;
        numAttr = new int[10];
        char* attr = strtok(NULL, ":");
        char* attrBuf = strtok(attr, " ");
        int n = 0;
        NaN = false;
        textBuf[0] = '\0';
        while (attrBuf != NULL) {
            if (isdigit(attrBuf[0]) && NaN == false) {
                numAttr[n] = atoi(attrBuf);
            }
            else if ((strlen(attrBuf) == 7 || strlen(attrBuf) == 8) && attrBuf[0] == '#') {
                for (int k = 0; isxdigit(attrBuf[k + 1]); k++) {
                    if (k == 5) {
                        colorBuf = attrBuf + 1;
                        char f[12] = "0x------ff";
                        for (int i = 0; i < 6; i++) {
                            f[i + 2] = colorBuf[i];
                        }
                        sscanf(f, "%x", &renderColor);
                        colorPos = n;
                    }
                }
            }
            else {
                NaN = true;
                strcat(textBuf, attrBuf);
                strcat(textBuf, " ");
                textPos = n;
            }
            n++;
            attrBuf = strtok(NULL, " ");
        }
        return numAttr;
    }

    int PrintCommand(int id, int* numAttr) {
        switch (id) {
        case 0:
            cout << "Распознана команда запроса ширины" << endl;
            cout << "Width = " << GetWidth() << endl;
            return true;
        case 1:
            cout << "Распознана команда запроса высоты" << endl;
            cout << "Height = " << GetHeight() << endl;
            return true;
        case 2:
            cout << "Распознана команда очистки экрана" << endl;
            if (colorPos == 0 && NaN == false && !(abs(numAttr[1]) <= SHRT_MAX)) {
                cout << "Color = #" << colorBuf << endl;
                bgColor = renderColor;
                fillScreen(renderColor);
                return true;
            }
            else if (textBuf[0] == '\n') {
                cout << "Пользователь не ввёл цвет, экран будет очищен цветом по умолчанию (белый)." << endl;
            }
            else {
                cout << "Неправильное количество или значение параметров" << endl;
                return false;
            }
        case 3:
            cout << "Распознана команда отрисовки пикселя" << endl;
            if (abs(numAttr[0]) <= width && abs(numAttr[1]) <= height && colorBuf != NULL && colorPos == 2 && NaN == false && !(abs(numAttr[3]) <= SHRT_MAX)) {
                cout << "X = " << numAttr[0] << ", Y = " << numAttr[1] << ", color = #" << colorBuf << endl;
                drawPixel(numAttr[0], numAttr[1], renderColor);
                return true;
            }
            else {
                cout << "Неправильное количество или значение параметров" << endl;
                return false;
            }
        case 4:
            cout << "Распознана команда отрисовки линии" << endl;
            if (abs(numAttr[0]) <= width && abs(numAttr[1]) <= height && abs(numAttr[2]) <= width && abs(numAttr[3]) <= height && colorBuf != NULL && colorPos == 4 && NaN == false && !(abs(numAttr[5]) <= SHRT_MAX)) {
                cout << "X1 = " << numAttr[0] << ", Y1 = " << numAttr[1] << ", X2 = " << numAttr[2] << ", Y2 = " <<
                    numAttr[3] << ", color = #" << colorBuf << endl;
                drawLine(numAttr[0], numAttr[1], numAttr[2], numAttr[3], renderColor);
                return true;
            }
            else {
                cout << "Неправильное количество или значение параметров" << endl;
                return false;
            }
        case 5:
            cout << "Распознана команда отрисовки прямоугольника" << endl;
            if (abs(numAttr[0]) <= width && abs(numAttr[1]) <= height && abs(numAttr[2]) <= width && abs(numAttr[3]) <= height && colorBuf != NULL && colorPos == 4 && NaN == false && !(abs(numAttr[5]) <= SHRT_MAX)) {
                cout << "X0 = " << numAttr[0] << ", Y0 = " << numAttr[1] << ", width = " << numAttr[2] << ", height = " <<
                    numAttr[3] << ", color = #" << colorBuf << endl;
                drawRect(numAttr[0], numAttr[1], numAttr[2], numAttr[3], renderColor);
                return true;
            }
            else {
                cout << "Неправильное количество или значение параметров" << endl;
                return false;
            }
        case 6:
            cout << "Распознана команда отрисовки скругленного прямоугольника" << endl;
            if (abs(numAttr[0]) <= width && abs(numAttr[1]) <= height && abs(numAttr[2]) <= width && abs(numAttr[3]) <= height && abs(numAttr[4]) <= height / 2 && colorBuf != NULL && colorPos == 5 && NaN == false && !(abs(numAttr[6]) <= SHRT_MAX)) {
                cout << "X0 = " << numAttr[0] << ", Y0 = " << numAttr[1] << ", width = " << numAttr[2] << ", height = " <<
                    numAttr[3] << ", radius = " << numAttr[4] << ", color = #" << colorBuf << endl;
                drawRoundRect(numAttr[0], numAttr[1], numAttr[2], numAttr[3], numAttr[4], renderColor);
                return true;
            }
            else {
                cout << "Неправильное количество или значение параметров" << endl;
                return false;
            }
        case 7:
            cout << "Распознана команда отрисовки эллипса" << endl;
            if (abs(numAttr[0]) <= width && abs(numAttr[1]) <= height && abs(numAttr[2]) <= width / 2 && abs(numAttr[3]) <= height / 2 && colorBuf != NULL && colorPos == 4 && NaN == false && !(abs(numAttr[5]) <= SHRT_MAX)) {
                cout << "X0 = " << numAttr[0] << ", Y0 = " << numAttr[1] << ", radiusX = " << numAttr[2] << ", radiusY = " <<
                    numAttr[3] << ", color = #" << colorBuf << endl;
                drawEllipse(numAttr[0], numAttr[1], numAttr[2], numAttr[3], renderColor);
                return true;
            }
            else {
                cout << "Неправильное количество или значение параметров" << endl;
                return false;
            }
        case 8:
            cout << "Распознана команда отрисовки окружности" << endl;
            if (abs(numAttr[0]) <= width && abs(numAttr[1]) <= height && abs(numAttr[2]) <= height / 2 && colorBuf != NULL && colorPos == 3 && NaN == false && !(abs(numAttr[4]) <= SHRT_MAX)) {
                cout << "X0 = " << numAttr[0] << ", Y0 = " << numAttr[1] << ", radius = " << numAttr[2] << ", color = #" <<
                    colorBuf << endl;
                drawEllipse(numAttr[0], numAttr[1], numAttr[2], numAttr[2], renderColor);
                return true;
            }
            else {
                cout << "Неправильное количество или значение параметров" << endl;
                return false;
            }
        case 9:
            cout << "Распознана команда отрисовки текста" << endl;
            if (abs(numAttr[0]) <= width && abs(numAttr[1]) <= height && colorBuf != NULL && colorPos == 2 && abs(numAttr[3]) <= 4 && abs(numAttr[4]) <= SHRT_MAX && (textPos >= 5 || NaN == false)) {
                cout << "X0 = " << numAttr[0] << ", Y0 = " << numAttr[1] << ", color = #" << colorBuf << ", font number = " <<
                    numAttr[3] << ", font size = " << numAttr[4] << ", text = " << textBuf << endl;
                drawText(numAttr[0], numAttr[1], textBuf, renderColor, numAttr[3], numAttr[4]);
                return true;
            }
            else {
                cout << "Неправильное количество или значение параметров" << endl;
                return false;
            }
        case 10:
            cout << "Распознана команда отрисовки заполненного прямоугольника" << endl;
            if (abs(numAttr[0]) <= width && abs(numAttr[1]) <= height && abs(numAttr[2]) <= width && abs(numAttr[3]) <= height && colorBuf != NULL && colorPos == 4 && NaN == false && !(abs(numAttr[5]) <= SHRT_MAX)) {
                cout << "X0 = " << numAttr[0] << ", Y0 = " << numAttr[1] << ", width = " << numAttr[2] << ", height = " <<
                    numAttr[3] << ", color = #" << colorBuf << endl;
                fillRect(numAttr[0], numAttr[1], numAttr[2], numAttr[3], renderColor);
                return true;
            }
            else {
                cout << "Неправильное количество или значение параметров" << endl;
                return false;
            }
        case 11:
            cout << "Распознана команда отрисовки заполненного скругленного прямоугольника" << endl;
            if (abs(numAttr[0]) <= width && abs(numAttr[1]) <= height && abs(numAttr[2]) <= width && abs(numAttr[3]) <= height && abs(numAttr[4]) <= height / 2 && colorBuf != NULL && colorPos == 5 && NaN == false && !(abs(numAttr[6]) <= SHRT_MAX)) {
                cout << "X0 = " << numAttr[0] << ", Y0 = " << numAttr[1] << ", width = " << numAttr[2] << ", height = " <<
                    numAttr[3] << ", radius = " << numAttr[4] << ", color = #" << colorBuf << endl;
                fillRoundRect(numAttr[0], numAttr[1], numAttr[2], numAttr[3], numAttr[4], renderColor);
                return true;
            }
            else {
                cout << "Неправильное количество или значение параметров" << endl;
                return false;
            }
        case 12:
            cout << "Распознана команда отрисовки заполненного эллипса" << endl;
            if (abs(numAttr[0]) <= width && abs(numAttr[1]) <= height && abs(numAttr[2]) <= width / 2 && abs(numAttr[3]) <= height / 2 && colorPos == 4 && NaN == false && !(abs(numAttr[5]) <= SHRT_MAX)) {
                cout << "X0 = " << numAttr[0] << ", Y0 = " << numAttr[1] << ", radiusX = " << numAttr[2] << ", radiusY = " <<
                    numAttr[3] << ", color = #" << colorBuf << endl;
                fillEllipse(numAttr[0], numAttr[1], numAttr[2], numAttr[3], renderColor);
                return true;
            }
            else {
                cout << "Неправильное количество или значение параметров" << endl;
                return false;
            }
        case 13:
            cout << "Распознана команда отрисовки заполненной окружности" << endl;
            if (abs(numAttr[0]) <= width && abs(numAttr[1]) <= height && abs(numAttr[2]) <= height / 2 && colorBuf != NULL && colorPos == 3 && NaN == false && !(abs(numAttr[4]) <= SHRT_MAX)) {
                cout << "X0 = " << numAttr[0] << ", Y0 = " << numAttr[1] << ", radius = " << numAttr[2] << ", color = #" <<
                    colorBuf << endl;
                fillEllipse(numAttr[0], numAttr[1], numAttr[2], numAttr[2], renderColor);
                return true;
            }
            else {
                cout << "Неправильное количество или значение параметров" << endl;
                return false;
            }
        case 14:
            cout << "Распознана команда запуска игры 'Сапёр'" << endl;
            if (numAttr[0] > 5 && numAttr[0] < 12) {
                launchMinesweeper(numAttr[0]);
            } else launchMinesweeper(10);
                
                return true;
        case 15:
            cout << "Распознана команда отрисовки бегущей строки" << endl;
            if (abs(numAttr[1]) <= height && colorBuf != NULL && colorPos == 2 && abs(numAttr[3]) <= 4 && abs(numAttr[4]) <= SHRT_MAX && (textPos >= 5 || NaN == false)) {
                cout << "Scroll speed = " << numAttr[0] << " << numAttr[0] << " << ", Y0 = " << numAttr[2] << ", color = #" << colorBuf << ", font number = " <<
                    numAttr[3] << ", font size = " << numAttr[4] << ", text = " << textBuf << endl;
                drawScrollingText(numAttr[0], numAttr[1], textBuf, renderColor, numAttr[3], numAttr[4]);
                return true;
            }
            else {
                cout << "Неправильное количество или значение параметров" << endl;
                return false;
            }
        default:
            cout << "Команда не распознана" << endl;
            return false;
        }
    }


    void fillScreen(uint_least32_t color) {
        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
            }

            window.clear(Color(color));
            window.display();

            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                break;
            }
        }
    }

    void drawPixel(int_least16_t x0, int_least16_t y0, uint_least32_t color) {
        RectangleShape pixel(Vector2f(1, 1));
        pixel.setFillColor(Color(color));
        pixel.setOutlineThickness(10);
        pixel.setOutlineColor(Color(color));
        pixel.move(x0, y0);

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
            }

            window.draw(pixel);
            window.display();

            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                break;
            }
        }
    }

    void drawLine(int_least16_t x0, int_least16_t y0, int_least16_t x1, int_least16_t y1, uint_least32_t color) {
        Vertex line[] = { Vertex(Vector2f(x0, y0)), Vertex(Vector2f(x1, y1)) };
        line[0].color = Color(color);
        line[1].color = Color(color);

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
            }

            window.draw(line, 2, Lines);
            window.display();

            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                break;
            }
        }
    }

    void drawRect(int_least16_t x0, int_least16_t y0, int_least16_t w, int_least16_t h, uint_least32_t color) {
        RectangleShape rectangle(Vector2f(w, h));
        if (bgColor != -1) {
            rectangle.setFillColor(Color(bgColor));
        }
        rectangle.setOutlineThickness(5);
        rectangle.setOutlineColor(Color(color));
        rectangle.move(x0 - w / 2, y0 - h / 2);

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
            }

            window.draw(rectangle);
            window.display();

            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                break;
            }
        }
    }

    void fillRect(int_least16_t x0, int_least16_t y0, int_least16_t w, int_least16_t h, uint_least32_t color) {
        RectangleShape filledRectangle(Vector2f(w, h));
        filledRectangle.setFillColor(Color(color));
        filledRectangle.move(x0 - w / 2, y0 - h / 2);

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
            }

            window.draw(filledRectangle);
            window.display();

            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                break;
            }
        }
    }

    void drawEllipse(int_least16_t x0, int_least16_t y0, int_least16_t r_x, int_least16_t r_y, uint_least32_t color) {
        CircleShape ellipse(r_x);
        float sub = (float)(r_y) / (float)(r_x);
        if (bgColor != -1) {
            ellipse.setFillColor(Color(bgColor));
        }
        ellipse.setOutlineThickness(5);
        ellipse.setOutlineColor(Color(color));
        ellipse.setScale(1.f, sub);
        ellipse.move(x0 - r_x, y0 - r_y);

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
            }

            window.draw(ellipse);
            window.display();

            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                break;
            }
        }
    }

    void fillEllipse(int_least16_t x0, int_least16_t y0, int_least16_t r_x, int_least16_t r_y, uint_least32_t color) {
        CircleShape filledEllipse(r_x);
        float sub = (float)(r_y) / (float)(r_x);
        filledEllipse.setFillColor(Color(color));
        filledEllipse.setScale(1.f, sub);
        filledEllipse.move(x0 - r_x, y0 - r_y);

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
            }

            window.draw(filledEllipse);
            window.display();

            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                break;
            }
        }
    }

    void drawRoundRect(int_least16_t x0, int_least16_t y0, int_least16_t w, int_least16_t h, int_least16_t r, uint_least32_t сolor) {
        RoundedRectangleShape roundedRectangle(Vector2f(w, h), r, 100);
        if (bgColor != -1) {
            roundedRectangle.setFillColor(Color(bgColor));
        }
        roundedRectangle.setOutlineThickness(5);
        roundedRectangle.setOutlineColor(Color(сolor));
        roundedRectangle.move(x0 - w / 2, y0 - h / 2);

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
            }

            window.draw(roundedRectangle);
            window.display();

            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                break;
            }
        }
    }

    void fillRoundRect(int_least16_t x0, int_least16_t y0, int_least16_t w, int_least16_t h, int_least16_t r, uint_least32_t сolor) {
        RoundedRectangleShape filledRoundedRectangle(Vector2f(w, h), r, 100);
        filledRoundedRectangle.setFillColor(Color(сolor));
        filledRoundedRectangle.move(x0 - w / 2, y0 - h / 2);

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
            }

            window.draw(filledRoundedRectangle);
            window.display();

            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                break;
            }
        }
    }

    int_least16_t drawChar(int_least16_t x, int_least16_t y, char c, uint_least32_t color, uint_least16_t bg, uint_least8_t size) {
        return 0;
    }

    int_least16_t drawChar(int_least16_t x, int_least16_t y, unsigned char c, uint_least32_t color, uint_least16_t bg, uint_least8_t size) {
        return 0;
    }

    int_least16_t drawText(int_least16_t x, int_least16_t y, const char* s, uint_least32_t color, uint_least16_t bg, uint_least8_t size) {
        Font font;
        if (!font.loadFromFile(fontArr[bg - 1])) {
            cout << "Font upload error" << endl;
        }
        Text text;
        text.setFont(font);
        text.setString(s);
        text.setCharacterSize(size);
        text.setPosition(x, y);
        text.setFillColor(Color(color));
        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
            }
            window.draw(text);
            window.display();
            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                break;
            }
        }
        return 0;
    }

    int_least16_t drawScrollingText(int scroll_speed, int_least16_t y, const char* s, uint_least32_t color, uint_least16_t bg, uint_least8_t size) {
        Font font;
        if (!font.loadFromFile(fontArr[bg - 1])) {
            cout << "Font upload error" << endl;
        }
        Clock clock;
        Text text;
        text.setFont(font);
        text.setString(s);
        text.setCharacterSize(size);
        text.setPosition(0, y);
        text.setFillColor(Color(color));
        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
            }
            if ((clock.getElapsedTime().asSeconds() * scroll_speed) > GetWidth()) {
                clock.restart(); 
            }
            text.setPosition(clock.getElapsedTime().asSeconds() * scroll_speed, y);
            window.clear();
            window.draw(text);
            window.display();
            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                break;
            }
        }
        return 0;
    }

    void launchMinesweeper(int ns) {

            srand(time(0));
            bool end = false;
            int w = 64;
            const int ts = 12;
            int grid[12][12];
            int sgrid[12][12]; //for showing

            Texture t;
            t.loadFromFile("images/tiles.jpg");
            Sprite s(t);
            Texture o;
            o.loadFromFile("images/game.png");
            Sprite k(o);

            for (int i = 0; i < ns; i++)
                for (int j = 0; j < ns; j++)
                {
                    sgrid[i][j] = 10;
                    if (rand() % 5 == 0)  grid[i][j] = 9;
                    else grid[i][j] = 0;
                }

            for (int i = 0; i <= ns; i++)
                for (int j = 0; j <= ns; j++)
                {
                    int n = 0;
                    if (grid[i][j] == 9) continue;
                    if (grid[i + 1][j] == 9) n++;
                    if (grid[i][j + 1] == 9) n++;
                    if (grid[i - 1][j] == 9) n++;
                    if (grid[i][j - 1] == 9) n++;
                    if (grid[i + 1][j + 1] == 9) n++;
                    if (grid[i - 1][j - 1] == 9) n++;
                    if (grid[i - 1][j + 1] == 9) n++;
                    if (grid[i + 1][j - 1] == 9) n++;
                    grid[i][j] = n;
                }

            while (window.isOpen())
            {
                Vector2i pos = Mouse::getPosition(window);
                int x = pos.x / w;
                int y = pos.y / w;
                Event e;
                while (window.pollEvent(e))
                {
                    if (e.type == Event::Closed)
                        window.close();
                    if (e.type == Event::MouseButtonPressed)
                        if (e.key.code == Mouse::Left) sgrid[x][y] = grid[x][y];
                        else if (e.key.code == Mouse::Right) {
                            if (sgrid[x][y] == 10) {
                                sgrid[x][y] = 11;
                            }
                            else if (sgrid[x][y] == 11) {
                                sgrid[x][y] = 10;
                            }
                        }
                }

                window.clear(Color::White);
                for (int i = 0; i < ns; i++)
                    for (int j = 0; j < ns; j++)
                    {
                        if (sgrid[x][y] == 9) {
                            sgrid[i][j] = grid[i][j];
                            end = true;
                        }
                        if (end == true) {
                            k.setPosition(w, 350);
                            window.draw(k);
                        }
                        s.setTextureRect(IntRect(sgrid[i][j] * w, 0, w, w));
                        s.setPosition(i * w, j * w);
                        window.draw(s);
                    }
                window.display();
                if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                    break;
                }
            }        
    }
};