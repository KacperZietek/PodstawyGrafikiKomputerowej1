#ifndef PLAYER_H
#define PLAYER_H

#include "Point2D.h"
#include "PrimitiveRenderer.h"
#include <allegro5/allegro.h>

class Player {
private:
    Point2D position;
    float radius;
    float speed; // piksele na sekundê
    ALLEGRO_COLOR color;

public:
    Player(float x = 100, float y = 100, float radius = 20, float speed = 200.0f);

    void update(const ALLEGRO_KEYBOARD_STATE& key_state, float delta_time);

    void render(PrimitiveRenderer* renderer);

    // Gettery i settery
    Point2D getPosition() const { return position; }
    void setPosition(float x, float y) { position.setPosition(x, y); }
    float getRadius() const { return radius; }
    void setColor(ALLEGRO_COLOR c) { color = c; }
};

#endif
