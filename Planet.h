#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Planet {
private:
    sf::CircleShape shape;
    sf::VertexArray trail;
    float orbitDistance;
    float rotationSpeed;
    float currentAngle;
    Planet* parentPlanet;
    bool showTrail;
    bool isVisible = true;
    std::string name;

public:
    // Constructor: Note default arguments for parent and hasTrail properties
    Planet(std::string name, float radius, float distance, float speed, sf::Color color, Planet* parent = nullptr, bool hasTrail = true);

    // Main lifecycle methods
    void update(float deltaTime, sf::Vector2f centerOffset);
    void draw(sf::RenderWindow& window);

    // Setters
    void setVisible(bool visible) { isVisible = visible; }
    void setParent(Planet* parent) { parentPlanet = parent; }

    // Getters for interactivity (allows main loop to access planetary data)
    sf::Vector2f getPosition() const;
    float getRadius() const;
    std::string getName() const { return name; }
    bool getIsVisible() const { return isVisible; }
};