#include<iostream>
#include<vector>
#include<random>
#include<SFML/Graphics.hpp>

std::random_device rng;
//std::mt19937_64 dev(rng());

std::vector<sf::Vector2f> pos;
std::vector<sf::Vector2f> vel;
std::vector<sf::Color> color;

float size = 15000;
float zoom;

sf::RenderWindow window(sf::VideoMode(1600, 900), "particleSim");

sf::RectangleShape p;
sf::Vector2i mpos;

bool mpressed;
bool rpressed;

float sqrt2(sf::Vector2f dif)
{
    return sqrt((dif.x * dif.x) + (dif.y * dif.y));
}
sf::Vector2f normalise(sf::Vector2f dif)
{
    float sqroot = sqrt2(dif);
    return sf::Vector2f(dif.x / sqroot, dif.y / sqroot);
}

void update(unsigned int t)
{
    float tsize = size / 4;
    for (unsigned int i = tsize * (t - 1); i < tsize * t; i++)
    {
        if (mpressed)
        {
            sf::Vector2f dif = pos[i] - sf::Vector2f(mpos);
            sf::Vector2f normalised = normalise(dif);
            vel[i] -= sf::Vector2f(normalised.x  / 5, normalised.y / 5);
        }
        if (rpressed)
        {
            sf::Vector2f dif = pos[i] - sf::Vector2f(mpos);
            sf::Vector2f normalised = normalise(dif);
            vel[i] += sf::Vector2f(normalised.x / 5, normalised.y / 5);
        }
        float colorChange = abs(vel[i].x) + abs(vel[i].y);
        color[i].b = 155;
        /*if (colorChange * 30 > 255)
        {
            color[i].r = 155;
            color[i].b = 0;
        }
        else
        {
            color[i].r = colorChange * 30;
            color[i].b = 255 - color[i].r;
        }*/
        color[i].b = 155;
        if (colorChange * 30 > 255)
        {
            color[i].r = 255;
            color[i].b = 63;
            color[i].g = 63;
        }
        else if (colorChange * 30 >= 128)
        {
            color[i].r = colorChange * 30;
            color[i].b = 255 - color[i].r * 0, 75;
            color[i].g = 255 - color[i].r * 0, 75;
        }
        else
        {
            color[i].r = colorChange * 30;
            color[i].b = 158 + color[i].r;
            color[i].g = 158 + color[i].r;
        }

        vel[i].x *= 0.99; //decreasing speed
        vel[i].y *= 0.99;
        pos[i] += sf::Vector2f(vel[i].x * 3, vel[i].y * 3);

    }
}

sf::Thread t1(&update, 1);
sf::Thread t2(&update, 2);
sf::Thread t3(&update, 3);
sf::Thread t4(&update, 4);

int main()
{
    window.setMouseCursorVisible(false);
    for (unsigned int i = 0; i < size; i++)
    {
        std::uniform_int_distribution<int> x(0, window.getSize().x);
        std::uniform_int_distribution<int> y(0, window.getSize().y);
        pos.push_back(sf::Vector2f(x(rng), y(rng)));
        vel.push_back(sf::Vector2f(0, 0));
        color.push_back(sf::Color::Green);
    }
    zoom = 1;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::MouseMoved)
            {
                mpos = sf::Mouse::getPosition(window);
                mpos = sf::Vector2i((mpos.x / zoom), (mpos.y / zoom));
            }
            if (event.type == sf::Event::MouseWheelMoved)
            {
                zoom += event.mouseWheel.delta * 0.01;
                mpos = sf::Mouse::getPosition(window);
                mpos = sf::Vector2i((mpos.x / zoom), (mpos.y / zoom));
            }
        }
        window.clear(sf::Color::Black);

        rpressed = sf::Mouse::isButtonPressed(sf::Mouse::Right);
        mpressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

        t1.launch();
        t2.launch();
        t3.launch();
        t4.launch();

        p.setSize(sf::Vector2f(zoom, zoom));
        for (unsigned int i = 0; i < pos.size(); i++)
        {
            p.setFillColor(color[i]);
            p.setPosition(pos[i].x * zoom, pos[i].y * zoom);
            window.draw(p);
        }
        p.setSize(sf::Vector2f(4, 4));
        p.setFillColor(sf::Color(255, 255, 255));
        p.setPosition(mpos.x * zoom, mpos.y * zoom);
        window.draw(p);
        window.display();
        
    }

    return 0;
}