#include "Planet.h"
#include <cmath>

Planet::Planet(float radius, float distance, float speed, sf::Color color, Planet* parent, bool hasTrail)
    : orbitDistance(distance), rotationSpeed(speed), currentAngle(0.f), parentPlanet(parent), showTrail(hasTrail)
{
    shape.setRadius(radius);
    shape.setOrigin({ radius, radius });
    shape.setFillColor(color);
    trail.setPrimitiveType(sf::PrimitiveType::LineStrip);
}

void Planet::update(float deltaTime, sf::Vector2f centerOffset) {
    currentAngle += rotationSpeed * deltaTime;

    float x = std::cos(currentAngle) * orbitDistance;
    float y = std::sin(currentAngle) * orbitDistance;

    sf::Vector2f newPos(x, y);

    if (parentPlanet != nullptr) {
        newPos += parentPlanet->getPosition();
    }
    else {
        newPos += centerOffset;
    }

    shape.setPosition(newPos);

    // scia
    if (showTrail) {
        trail.append(sf::Vertex(newPos, shape.getFillColor()));

        // Se la coda è troppo lunga (es. 150 punti), tolgo il primo (il più vecchio)
        if (trail.getVertexCount() > 150) {
            // Spostiamo tutti i punti indietro di uno per "mangiare la coda"
            for (size_t i = 0; i < trail.getVertexCount() - 1; i++) {
                trail[i] = trail[i + 1];

                // Effetto dissolvenza: più il punto è vecchio, più è trasparente
                float progress = (float)i / trail.getVertexCount();
                sf::Color color = shape.getFillColor();
                color.a = static_cast<unsigned char>(255.f * progress);
                trail[i].color = color;
            }

            // Ridimensioniamo l'array per rimuovere l'ultimo duplicato
            trail.resize(trail.getVertexCount() - 1);
        }
    }
    
}

void Planet::draw(sf::RenderWindow& window) {

    if (showTrail){
        window.draw(trail);
    }

    window.draw(shape);
}

sf::Vector2f Planet::getPosition() const {
    return shape.getPosition();
}