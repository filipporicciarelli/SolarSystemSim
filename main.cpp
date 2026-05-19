#include <SFML/Graphics.hpp>
#include "Planet.h"
#include <optional>
#include <vector>
#include <cstdint> // Required for uint8_t

struct Star {
    sf::Vector2f pos;
    uint8_t brightness;
};

int main() {
    // WINDOW CONFIGURATION
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode(); // Get the current monitor resolution    
    sf::RenderWindow window(desktop, "Solar System Sim", sf::State::Fullscreen); // Create the window in Fullscreen mode
    window.setFramerateLimit(144);

    sf::Clock clock;

    float screenW = static_cast<float>(desktop.size.x);
    float screenH = static_cast<float>(desktop.size.y);
    sf::Vector2f screenCenter(screenW / 2.f, screenH / 2.f);

    // [ZOOM] Camera/View Initialization
    sf::View view(window.getDefaultView());
    Planet* followedPlanet = nullptr;
    float targetZoom = 1.0f;
    float currentZoom = 1.0f;

    // [FONT] Font and Text Loading
    sf::Font font;
    if (!font.openFromFile("comic.ttf")) {
        // If font loading fails, the program will continue without rendering text
    }
    sf::Text planetNameText(font, "");
    planetNameText.setCharacterSize(25);
    planetNameText.setFillColor(sf::Color::White);
    planetNameText.setOutlineColor(sf::Color::Black);
    planetNameText.setOutlineThickness(2.f);

    // STATE AND CONTROL VARIABLES
    float timeScale = 1.0f; // 1.0 represents standard speed
    bool isPaused = false;
    bool showHUD = false;

    // HUD Setup
    sf::RectangleShape hudPanel({ 300.f, 200.f });
    hudPanel.setFillColor(sf::Color(0, 0, 0, 180));
    hudPanel.setOutlineThickness(2.f);
    hudPanel.setOutlineColor(sf::Color::White);

    sf::CircleShape helpButton(20.f);
    helpButton.setFillColor(sf::Color(50, 50, 50, 200));
    helpButton.setOutlineThickness(2.f);
    helpButton.setOutlineColor(sf::Color::White);

    sf::Text helpButtonText(font, "?");
    helpButtonText.setCharacterSize(25);

    sf::Text commandsText(font, "COMANDI:\n- Spazio: Pausa\n- Frecce <- ->: Velocita'\n- Click: Zoom Pianeta\n- Backspace: Reset");
    commandsText.setCharacterSize(18);

    // HUD Positioning (Top-right corner)
    float margin = 20.f;
    helpButton.setPosition({ screenW - 50.f - margin, margin });
    helpButtonText.setPosition({ screenW - 35.f - margin, margin + 2.f });
    hudPanel.setPosition({ screenW - 300.f - margin, margin + 60.f });
    commandsText.setPosition({ screenW - 290.f - margin, margin + 70.f });

    std::vector<Planet> planets;

    // PLANETS INITIALIZATION
    planets.emplace_back("Sole", 40.f, 0.f, 0.f, sf::Color::Yellow, nullptr, false);

    // Inner planets
    planets.emplace_back("Mercurio", 6.f, 100.f, 1.6f, sf::Color(169, 169, 169));
    planets.emplace_back("Venere", 12.f, 160.f, 1.2f, sf::Color(255, 223, 196));
    planets.emplace_back("Terra", 13.f, 230.f, 1.0f, sf::Color::Blue);
    planets.emplace_back("Marte", 10.f, 310.f, 0.8f, sf::Color::Red);

    // Outer planets
    planets.emplace_back("Giove", 30.f, 450.f, 0.5f, sf::Color(210, 180, 140));
    planets.emplace_back("Saturno", 25.f, 600.f, 0.4f, sf::Color(238, 232, 170));
    planets.emplace_back("Urano", 18.f, 750.f, 0.3f, sf::Color(173, 216, 230));
    planets.emplace_back("Nettuno", 18.f, 900.f, 0.25f, sf::Color(65, 105, 225));

    Planet* earthPtr = nullptr;
    for (auto& p : planets) {
        if (p.getName() == "Terra") earthPtr = &p;
    }

    std::vector<Planet> moons;

    // Moons
    moons.emplace_back("Luna", 5.f, 60.f, 2.5f, sf::Color(150, 150, 150), earthPtr, false);
    moons[0].setVisible(false);

    // STARFIELD GENERATION
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

        // EVENT HANDLING (Keyboard/Mouse Input)
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                // Exit
                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
                // Pause Toggle
                if (keyPressed->code == sf::Keyboard::Key::Space) {
                    isPaused = !isPaused;
                }
                // Time scale control (Right/Left Arrow Keys)
                if (keyPressed->code == sf::Keyboard::Key::Right) timeScale += 0.2f;
                if (keyPressed->code == sf::Keyboard::Key::Left)  timeScale -= 0.2f;
                if (timeScale < 0.f) timeScale = 0.f;

                // [ZOOM] Reset view via Backspace key
                if (keyPressed->code == sf::Keyboard::Key::Backspace) {
                    followedPlanet = nullptr;
                    targetZoom = 1.0f;
                }
            }

            // MOUSE CLICK HANDLING (Selection & Target Tracking)
            if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePosHUD = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getDefaultView());

                    sf::Vector2f btnDiff = mousePosHUD - (helpButton.getPosition() + sf::Vector2f(20.f, 20.f));
                    if (std::sqrt(btnDiff.x * btnDiff.x + btnDiff.y * btnDiff.y) < 20.f) {
                        showHUD = !showHUD; // Toggle HUD panel visibility
                    }
                    else {
                        // [HUD - NEW] If the "?" button wasn't clicked, process world-space click logic (planetary zoom)
                        sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
                        bool clickedAnything = false;
                        for (auto& p : planets) {
                            sf::Vector2f diff = worldPos - p.getPosition();
                            if (std::sqrt(diff.x * diff.x + diff.y * diff.y) < p.getRadius() + 15.f) {
                                followedPlanet = &p; targetZoom = 0.15f; clickedAnything = true;
                                if (p.getName() == "Terra") moons[0].setVisible(true);
                                break;
                            }
                        }
                        if (!clickedAnything) { followedPlanet = nullptr; targetZoom = 1.0f; moons[0].setVisible(false); }
                    }
                }
            }
        }

        // TIME STEP LOGICA (Always restart clock to prevent lag spikes after unpausing)
        float dt;
        if (isPaused) {
            // Continuously flush the clock while paused
            clock.restart();
            dt = 0.f;
        }
        else {
            // Calculate delta time normally when unpaused
            dt = clock.restart().asSeconds() * timeScale;
        }

        // POSITION UPDATES
        for (auto& p : planets) p.update(dt, screenCenter);
        for (auto& p : moons) p.update(dt, screenCenter);

        // CAMERA ZOOM LOGIC
        if (followedPlanet) view.setCenter(followedPlanet->getPosition());
        else view.setCenter(screenCenter);
        currentZoom += (targetZoom - currentZoom) * 0.05f;
        view.setSize({ screenW * currentZoom, screenH * currentZoom });

        // RENDERING
        window.clear(sf::Color(5, 5, 15));

        window.setView(view);

        // Background: Starfield
        for (const auto& star : stars) {
            sf::Vertex point(star.pos, sf::Color(255, 255, 255, star.brightness));
            window.draw(&point, 1, sf::PrimitiveType::Points);
        }

        // Space entities
        for (auto& p : planets) p.draw(window);
        for (auto& p : moons) p.draw(window);

        if (followedPlanet) {
            planetNameText.setString(followedPlanet->getName());
            planetNameText.setPosition({ followedPlanet->getPosition().x - 20.f, followedPlanet->getPosition().y - followedPlanet->getRadius() - 40.f });
            window.draw(planetNameText);
        }

        // Render HUD elements using the default static view to prevent scaling/shifting
        window.setView(window.getDefaultView());
        window.draw(helpButton);
        window.draw(helpButtonText);

        if (showHUD) {
            window.draw(hudPanel);
            window.draw(commandsText);
        }

        window.display();
    }
    return 0;
}