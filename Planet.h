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
    // Costruttore: nota il valore di default per parent e hasTrail
    Planet(std::string name, float radius, float distance, float speed, sf::Color color, Planet* parent = nullptr, bool hasTrail = true);

    // Funzioni principali di ciclo vita
    void update(float deltaTime, sf::Vector2f centerOffset);
    void draw(sf::RenderWindow& window);

    //setter
    void setVisible(bool visible) { isVisible = visible; }
    void setParent(Planet* parent) { parentPlanet = parent; }

    // Getter per l'interattività (permettono al main di "leggere" i dati del pianeta)
    sf::Vector2f getPosition() const;
    float getRadius() const;
    std::string getName() const { return name; }
    bool getIsVisible() const { return isVisible; }
};