#include "Player.h"
#include "PrimitiveRenderer.h"
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <cmath>

void Player::update(float dt) {
    updateMovement(dt);

    // NOWE: Dynamiczne ograniczenia ekranu - dostosowują się do rozdzielczości
    // Używamy większych wartości aby gracz mógł wyjść poza widoczny obszar
    const float screen_margin = 50.0f; // Margines poza ekranem

    x = std::max(-screen_margin, std::min(x, screen_width + screen_margin));
    y = std::max(-screen_margin, std::min(y, screen_height + screen_margin));
}

void Player::updateMovement(float dt) {
    // Reset velocity
    velocity_x = 0;
    velocity_y = 0;

    // Sprawdź które klawisze są wciśnięte i ustaw velocity
    if (key_states[0]) { // W - do góry
        velocity_y = -speed;
    }
    if (key_states[1]) { // S - do dołu
        velocity_y = speed;
    }
    if (key_states[2]) { // A - w lewo
        velocity_x = -speed;
    }
    if (key_states[3]) { // D - w prawo
        velocity_x = speed;
    }

    // Normalizacja prędkości dla ruchu po skosie
    if (velocity_x != 0 && velocity_y != 0) {
        velocity_x *= 0.7071f; // 1/sqrt(2)
        velocity_y *= 0.7071f;
    }

    // Aktualizacja pozycji
    x += velocity_x * dt;
    y += velocity_y * dt;

    // Aktualizacja kątu jeśli się porusza
    if (velocity_x != 0 || velocity_y != 0) {
        angle = atan2(velocity_y, velocity_x) * 180.0f / 3.14159f;
    }
}

void Player::draw(PrimitiveRenderer* renderer) const {
    if (!renderer) return;

    // narysuj trójkąt reprezentujący gracza zorientowany kątem 'angle'
    float size = 12.0f;
    float rad = angle * 3.14159265358979323846f / 180.0f;
    float cosA = cos(rad);
    float sinA = sin(rad);

    // wierzchołki trójkąta w lokalnych współrzędnych
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

    // Ustaw kolor gracza (zielony)
    renderer->setColor(0, 255, 0);

    // Rysuj trójkąt
    renderer->drawLine(p1.first, p1.second, p2.first, p2.second);
    renderer->drawLine(p2.first, p2.second, p3.first, p3.second);
    renderer->drawLine(p3.first, p3.second, p1.first, p1.second);

    // Opcjonalnie: narysuj celownik/kierunek (żółty)
    renderer->setColor(255, 255, 0);
    float end_x = x + cos(rad) * 20.0f;
    float end_y = y + sin(rad) * 20.0f;
    renderer->drawLine(x, y, end_x, end_y);
}

void Player::handleEvent(const ALLEGRO_EVENT& ev) {
    if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_W: key_states[0] = true; break;
            case ALLEGRO_KEY_S: key_states[1] = true; break;
            case ALLEGRO_KEY_A: key_states[2] = true; break;
            case ALLEGRO_KEY_D: key_states[3] = true; break;
            case ALLEGRO_KEY_LEFT: angle -= 5.0f; break;
            case ALLEGRO_KEY_RIGHT: angle += 5.0f; break;
            default: break;
        }
    }
    else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
        switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_W: key_states[0] = false; break;
            case ALLEGRO_KEY_S: key_states[1] = false; break;
            case ALLEGRO_KEY_A: key_states[2] = false; break;
            case ALLEGRO_KEY_D: key_states[3] = false; break;
            default: break;
        }
    }
}
