#include "Planet.h"
#include <cmath>

Planet::Planet(float radius, float distance, float speed, sf::Color color, Planet* parent, bool hasTrail)
    : orbitDistance(distance), rotationSpeed(speed), currentAngle(0.f), parentPlanet(parent), showTrail(hasTrail)
{
    shape.setRadius(radius);
    shape.setOrigin({ radius, radius });
    shape.setFillColor(color);

    if (showTrail) {
        trail.setPrimitiveType(sf::PrimitiveType::LineStrip);
    }
}

void Planet::update(float deltaTime, sf::Vector2f centerOffset) {
    // Se è in pausa o il tempo è fermo, non aggiorna nulla (risolve bug sparizione scia)
    if (deltaTime <= 0.f) return;

    // AGGIORNAMENTO POSIZIONE
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

    // LOGICA DELLA SCIA (Solo se in movimento)
    if (showTrail) {
        // Aggiungo il nuovo punto
        trail.append(sf::Vertex(shape.getPosition(), shape.getFillColor()));

        // Gestione lunghezza massima (es. 300 punti)
        if (trail.getVertexCount() > 300) {
            // Rimuovo il punto più vecchio spostando gli altri (Shift)
            for (size_t i = 0; i < trail.getVertexCount() - 1; i++) {
                trail[i] = trail[i + 1];
            }
            trail.resize(trail.getVertexCount() - 1);
        }

        // Ricalcolo della sfumatura Alpha
        size_t count = trail.getVertexCount();
        for (size_t i = 0; i < count; ++i) {
            float progress = static_cast<float>(i) / static_cast<float>(count);
            sf::Color color = shape.getFillColor();
            color.a = static_cast<unsigned char>(255.f * progress);
            trail[i].color = color;
        }
    }
}

void Planet::draw(sf::RenderWindow& window) {
    // Disegna la scia (se esiste)
    if (showTrail && trail.getVertexCount() > 0) {
        window.draw(trail);
    }

    // Disegna il pianeta
    window.draw(shape);

    // Effetto speciale: Anelli di Saturno
    // Uso una condizione specifica per "identificare" Saturno basandoci sulle sue proprietà
    if (orbitDistance > 550.f && shape.getRadius() > 20.f && shape.getRadius() < 35.f) {
        sf::CircleShape ring(shape.getRadius() * 1.8f);
        ring.setOrigin({ ring.getRadius(), ring.getRadius() });
        ring.setPosition(shape.getPosition());
        ring.setFillColor(sf::Color::Transparent);
        ring.setOutlineThickness(2.f);
        ring.setOutlineColor(sf::Color(200, 200, 150, 150));
        ring.setScale({ 1.f, 0.4f });
        window.draw(ring);
    }
}

sf::Vector2f Planet::getPosition() const {
    return shape.getPosition();
}

float Planet::getRadius() const {
    return shape.getRadius();
}

sf::Color Planet::getColor() const {
    return shape.getFillColor();
}