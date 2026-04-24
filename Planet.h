#pragma once
#include <SFML/Graphics.hpp>

class Planet {
public:
    // Costruttore: raggio, distanza dal centro, velocità, colore, puntatore al padre
    Planet(float radius, float distance, float speed, sf::Color color, Planet* parent = nullptr, bool hasTrail = true);

    void update(float deltaTime, sf::Vector2f centerOffset = { 0.f, 0.f });
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;

private:
    sf::CircleShape shape;
    float orbitDistance;
    float rotationSpeed;
    float currentAngle;
    Planet* parentPlanet;
    bool showTrail;

    sf::VertexArray trail;
};