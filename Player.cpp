#include "Player.h"
#include "PrimitiveRenderer.h"
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <cmath>

void Player::update(float dt) {
    // przyk�ad prostego ruchu automatycznego - brak
    (void)dt;
    // mo�esz doda� hamowanie, inercj� itp.
}

void Player::draw(PrimitiveRenderer* renderer) const {
    if (!renderer) return;
    // narysuj tr�jk�t reprezentuj�cy gracza zorientowany k�tem 'angle'
    float size = 12.0f;
    float rad = angle * 3.14159265358979323846f / 180.0f;
    float cosA = cos(rad);
    float sinA = sin(rad);

    // wierzcho�ki tr�jk�ta w lokalnych wsp�rz�dnych
    float vx1 = size, vy1 = 0;
    float vx2 = -size*0.5f, vy2 = size*0.6f;
    float vx3 = -size*0.5f, vy3 = -size*0.6f;

    auto rot = [&](float vx, float vy){
        float rx = vx * cosA - vy * sinA + x;
        float ry = vx * sinA + vy * cosA + y;
        return std::pair<float,float>(rx,ry);
    };

    auto p1 = rot(vx1, vy1);
    auto p2 = rot(vx2, vy2);
    auto p3 = rot(vx3, vy3);

    // u�yj renderer->drawLine lub bezpo�rednio Allegro
    renderer->drawLine(p1.first, p1.second, p2.first, p2.second);
    renderer->drawLine(p2.first, p2.second, p3.first, p3.second);
    renderer->drawLine(p3.first, p3.second, p1.first, p1.second);
}

void Player::handleEvent(const ALLEGRO_EVENT& ev) {
    if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_W: y -= 5.0f; break;
            case ALLEGRO_KEY_S: y += 5.0f; break;
            case ALLEGRO_KEY_A: x -= 5.0f; break;
            case ALLEGRO_KEY_D: x += 5.0f; break;
            case ALLEGRO_KEY_LEFT: angle -= 5.0f; break;
            case ALLEGRO_KEY_RIGHT: angle += 5.0f; break;
            default: break;
        }
    }
    // mo�esz doda� obs�ug� myszy (ALLEGRO_EVENT_MOUSE_AXES / BUTTON_DOWN)
}
