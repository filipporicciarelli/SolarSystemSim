#include <SFML/Graphics.hpp>
#include "Planet.h"
#include <optional>
#include <vector>
#include <cstdint> //per usare uint8_t

struct Star {
    sf::Vector2f pos;
    uint8_t brightness;
};

int main() {
    // CONFIGURAZIONE FINESTRA
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode(); // Ottengo la risoluzione del monitor attuale    
    sf::RenderWindow window(desktop, "Solar System Sim", sf::State::Fullscreen); //creo la finestra in Fullscreen
    window.setFramerateLimit(144);

    sf::Clock clock;
    sf::Vector2f screenCenter(
        static_cast<float>(desktop.size.x) / 2.f,
        static_cast<float>(desktop.size.y) / 2.f
    );

    // VARIABILI DI STATO E CONTROLLO
    float timeScale = 1.0f; // 1.0 è velocità normale
    bool isPaused = false;

    // INIZIALIZZAZIONE PIANETI
    Planet sun(40.f, 0.f, 0.f, sf::Color::Yellow, nullptr, false);

    // pianeti interni
    Planet mercury(6.f, 100.f, 1.6f, sf::Color(169, 169, 169));
    Planet venus(12.f, 160.f, 1.2f, sf::Color(255, 223, 196));
    Planet earth(13.f, 230.f, 1.0f, sf::Color::Blue);
    Planet mars(10.f, 310.f, 0.8f, sf::Color::Red);

    // pianeti esterni
    Planet jupiter(30.f, 450.f, 0.5f, sf::Color(210, 180, 140));
    Planet saturn(25.f, 600.f, 0.4f, sf::Color(238, 232, 170));
    Planet uranus(18.f, 750.f, 0.3f, sf::Color(173, 216, 230));
    Planet neptune(18.f, 900.f, 0.25f, sf::Color(65, 105, 225));

    // lune
    Planet moon(5.f, 60.f, 2.5f, sf::Color(150, 150, 150), &earth, false);

    // GENERAZIONE STELLE
    std::vector<Star> stars;
    for (int i = 0; i < 1000; ++i) {
        stars.push_back({
            sf::Vector2f(
                static_cast<float>(rand() % desktop.size.x),
                static_cast<float>(rand() % desktop.size.y)
            ),
            static_cast<uint8_t>(rand() % 200 + 55)
            });
    }

    // --- GAME LOOP ---
    while (window.isOpen()) {

        // GESTIONE EVENTI (Input Tastiera/Mouse)
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                // Esci
                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
                // Pausa
                if (keyPressed->code == sf::Keyboard::Key::Space) {
                    isPaused = !isPaused;
                }
                // Velocità tempo (Frecce destra/sinistra)
                if (keyPressed->code == sf::Keyboard::Key::Right) timeScale += 0.2f;
                if (keyPressed->code == sf::Keyboard::Key::Left)  timeScale -= 0.2f;
                if (timeScale < 0.f) timeScale = 0.f;
            }
        }

        // LOGICA DEL TEMPO (Riavvio sempre l'orologio per evitare scatti dopo la pausa)
        float realDt = clock.restart().asSeconds();
        float dt = isPaused ? 0.f : realDt * timeScale;

        // UPDATE POSIZIONI
        sun.update(dt, screenCenter);
        mercury.update(dt, screenCenter);
        venus.update(dt, screenCenter);
        earth.update(dt, screenCenter);
        mars.update(dt, screenCenter);
        jupiter.update(dt, screenCenter);
        saturn.update(dt, screenCenter);
        uranus.update(dt, screenCenter);
        neptune.update(dt, screenCenter);
        moon.update(dt, screenCenter);

        // DISEGNO
        window.clear(sf::Color(5, 5, 15));

        // Sfondo: Stelle
        for (const auto& star : stars) {
            sf::Vertex point(star.pos, sf::Color(255, 255, 255, star.brightness));
            window.draw(&point, 1, sf::PrimitiveType::Points);
        }

        // Entità spaziali
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