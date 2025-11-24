#ifndef PLAYER_H
#define PLAYER_H

#include "UpdatableObject.h"
#include "DrawableObject.h"
#include "TransformableObject.h"
#include "PrimitiveRenderer.h"
#include <allegro5/allegro.h>

class Player : public UpdatableObject, public DrawableObject, public TransformableObject {
private:
    float x,y;
    float angle; // degrees
    float speed; // units per second
public:
    Player(float x=0, float y=0) : x(x), y(y), angle(0), speed(100.0f) {}
    virtual ~Player() = default;

    // UpdatableObject
    void update(float dt) override;

    // DrawableObject
    void draw(PrimitiveRenderer* renderer) const override;

    // TransformableObject
    void translate(float dx, float dy) override { x += dx; y += dy; }
    void rotate(float angle_deg, float cx=0.0f, float cy=0.0f) override { angle += angle_deg; }
    void scale(float sx, float sy, float cx=0.0f, float cy=0.0f) override { (void)sx; (void)sy; (void)cx; (void)cy; /* opcjonalnie: zmiana rozmiaru */ }

    // Input handler: uzyj w glownym loopie
    void handleEvent(const ALLEGRO_EVENT& ev);

    // getters
    float getX() const { return x; }
    float getY() const { return y; }
    float getAngle() const { return angle; }
};

#endif // PLAYER_H
