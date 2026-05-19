#include "Planet.h"
#include <cmath>

Planet::Planet(std::string n, float radius, float distance, float speed, sf::Color color, Planet* parent, bool hasTrail)
    : name(n), orbitDistance(distance), rotationSpeed(speed), currentAngle(0.f), parentPlanet(parent), showTrail(hasTrail)
{
    shape.setRadius(radius);
    shape.setOrigin({ radius, radius });
    shape.setFillColor(color);

    if (showTrail) {
        trail.setPrimitiveType(sf::PrimitiveType::LineStrip);
    }
}

void Planet::update(float deltaTime, sf::Vector2f centerOffset) {
    // Return early if simulation is paused or time is stopped (fixes trail disappearance bug)
    if (deltaTime <= 0.f) return;

    // POSITION UPDATE
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

    // PLANETARY TRAIL LOGIC (Only processed when moving)
    if (showTrail) {
        // Append the new position vertex
        trail.append(sf::Vertex(shape.getPosition(), shape.getFillColor()));

        // Cap maximum trail length (e.g., 300 points)
        if (trail.getVertexCount() > 300) {
            // Shift vertices left to remove the oldest point
            for (size_t i = 0; i < trail.getVertexCount() - 1; i++) {
                trail[i] = trail[i + 1];
            }
            trail.resize(trail.getVertexCount() - 1);
        }

        // Recalculate alpha gradient for fading effect
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
    // Check visibility state
    if (!isVisible) return;

    // Draw orbital trail if enabled and populated
    if (showTrail && trail.getVertexCount() > 0) {
        window.draw(trail);
    }

    // Draw planetary body
    window.draw(shape);

    // Visual effect: Saturn's Rings
    // Target Saturn specifically by checking its string identifier
    if (name == "Saturno") {
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