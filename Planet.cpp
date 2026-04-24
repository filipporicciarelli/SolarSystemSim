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
        trail.append(sf::Vertex(shape.getPosition(), shape.getFillColor()));

        // Se la coda è troppo lunga (es. 300 punti), tolgo il primo (il più vecchio)
        if (trail.getVertexCount() > 300) {
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

        // Ricalcolo la sfumatura per TUTTI i punti attualmente nella coda
        // Questo risolve il bug: la scia sfuma anche se ci sono solo 2 o 10 punti
        size_t count = trail.getVertexCount();

        for (size_t i = 0; i < count; ++i) {
            float progress = static_cast<float>(i) / static_cast<float>(count);
            sf::Color color = shape.getFillColor();

            // Alpha sfumato: il punto i=0 è il più vecchio (trasparente)
            // il punto i=count-1 è l'ultimo aggiunto (opaco)
            color.a = static_cast<unsigned char>(255.f * progress);
            trail[i].color = color;
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