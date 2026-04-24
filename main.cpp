#include <SFML/Graphics.hpp>
#include "Planet.h"
#include <optional>

int main() {
    sf::RenderWindow window(sf::VideoMode({ 1600, 900 }), "Solar System Sim");
    window.setFramerateLimit(144);
    sf::Clock clock;
    sf::Vector2f screenCenter(800.f, 450.f);

    //sole
    Planet sun(40.f, 0.f, 0.f, sf::Color::Yellow, nullptr, false);

    //pianeti interni
    Planet mercury(6.f, 80.f, 1.6f, sf::Color(169, 169, 169));
    Planet venus(12.f, 130.f, 1.2f, sf::Color(255, 223, 196));
    Planet earth(13.f, 200.f, 1.0f, sf::Color::Blue);
    Planet mars(10.f, 260.f, 0.8f, sf::Color::Red);

    //pianeti esterni
    Planet jupiter(30.f, 380.f, 0.5f, sf::Color(210, 180, 140));
    Planet saturn(25.f, 500.f, 0.4f, sf::Color(238, 232, 170));
    Planet uranus(18.f, 620.f, 0.3f, sf::Color(173, 216, 230));
    Planet neptune(18.f, 720.f, 0.25f, sf::Color(65, 105, 225));

    //lune
    Planet moon(5.f, 60.f, 2.5f, sf::Color(150, 150, 150), &earth, false);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        float dt = clock.restart().asSeconds();

        sun.update(dt, screenCenter);
        mercury.update(dt, screenCenter);
        venus.update(dt, screenCenter);
        earth.update(dt, screenCenter);
        mars.update(dt, screenCenter);
        jupiter.update(dt, screenCenter);
        saturn.update(dt, screenCenter);
        uranus.update(dt, screenCenter);
        neptune.update(dt, screenCenter);
        moon.update(dt);

        window.clear(sf::Color(5, 5, 15));
        sun.draw(window);
        mercury.draw(window);
        venus.draw(window);
        earth.draw(window);
        moon.draw(window);
        mars.draw(window);
        jupiter.draw(window);
        saturn.draw(window);
        uranus.draw(window);
        neptune.draw(window);

        window.display();
    }
    return 0;
}