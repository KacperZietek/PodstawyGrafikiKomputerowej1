#ifndef PLAYER_H
#define PLAYER_H

#include "UpdatableObject.h"
#include "DrawableObject.h"
#include "TransformableObject.h"
#include "PrimitiveRenderer.h"
#include <allegro5/allegro.h>

class Player : public UpdatableObject, public DrawableObject, public TransformableObject {
private:
    float x, y;
    float angle; // degrees
    float speed; // units per second

    // Nowe pola do p³ynnego sterowania
    float velocity_x, velocity_y;
    bool key_states[4]; // 0=W, 1=S, 2=A, 3=D

    // NOWE: Rozdzielczoœæ ekranu dla ograniczeñ
    float screen_width, screen_height;

public:
    Player(float x = 0, float y = 0)
        : x(x), y(y), angle(0), speed(200.0f), velocity_x(0), velocity_y(0),
          screen_width(1024.0f), screen_height(768.0f) { // Domyœlna rozdzielczoœæ
        // Inicjalizacja stanów klawiszy
        for (int i = 0; i < 4; i++) {
            key_states[i] = false;
        }
    }

    virtual ~Player() = default;

    // UpdatableObject
    void update(float dt) override;

    // DrawableObject
    void draw(PrimitiveRenderer* renderer) const override;

    // TransformableObject
    void translate(float dx, float dy) override { x += dx; y += dy; }
    void rotate(float angle_deg, float cx = 0.0f, float cy = 0.0f) override { angle += angle_deg; }
    void scale(float sx, float sy, float cx = 0.0f, float cy = 0.0f) override {
        (void)sx; (void)sy; (void)cx; (void)cy; /* opcjonalnie: zmiana rozmiaru */
    }

    // Input handler: uzyj w glownym loopie
    void handleEvent(const ALLEGRO_EVENT& ev);

    // getters
    float getX() const { return x; }
    float getY() const { return y; }
    float getAngle() const { return angle; }

    // setters
    void setPosition(float new_x, float new_y) { x = new_x; y = new_y; }
    void setSpeed(float new_speed) { speed = new_speed; }

    // NOWA METODA: Ustaw rozdzielczoœæ ekranu
    void setScreenSize(float width, float height) {
        screen_width = width;
        screen_height = height;
    }

private:
    void updateMovement(float dt);
};

#endif // PLAYER_H
