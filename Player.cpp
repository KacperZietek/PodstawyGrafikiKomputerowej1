#include "Player.h"
#include <iostream>

// Upewnij siê, ¿e te wymiary pasuj¹ do Twojego pliku!
const char* PLAYER_IMAGE = "walk.png";
const int FRAME_H = 128;
const int FRAME_W = 128;
const int TOTAL_FRAMES = 8;
const float ANIM_SPEED = 0.1f;

Player::Player(BitmapHandler* handler, float x, float y)
    : SpriteObject(handler->loadBitmap(PLAYER_IMAGE), x, y, FRAME_W, FRAME_H, TOTAL_FRAMES, ANIM_SPEED),
      speed(200.0f)
{
    // Skalowanie (jeœli postaæ jest za du¿a)
    scale(0.8f, 0, 0);

    if (!bitmap) {
        std::cerr << "[Player] Blad: Nie udalo sie wczytac bitmapy gracza!" << std::endl;
    }
}

void Player::update(float deltaTime) {
    // 1. Animacja
    SpriteObject::animate(deltaTime);

    // 2. Ruch i Odbicie lustrzane
    ALLEGRO_KEYBOARD_STATE keyState;
    al_get_keyboard_state(&keyState);

    float moveDist = speed * deltaTime;

    if (al_key_down(&keyState, ALLEGRO_KEY_W) || al_key_down(&keyState, ALLEGRO_KEY_UP)) {
        translate(0, -moveDist);
    }
    if (al_key_down(&keyState, ALLEGRO_KEY_S) || al_key_down(&keyState, ALLEGRO_KEY_DOWN)) {
        translate(0, moveDist);
    }

    // IDZIEMY W LEWO
    if (al_key_down(&keyState, ALLEGRO_KEY_A) || al_key_down(&keyState, ALLEGRO_KEY_LEFT)) {
        translate(-moveDist, 0);
        setFlip(true); // <-- Obróæ w lewo
    }
    // IDZIEMY W PRAWO
    if (al_key_down(&keyState, ALLEGRO_KEY_D) || al_key_down(&keyState, ALLEGRO_KEY_RIGHT)) {
        translate(moveDist, 0);
        setFlip(false); // <-- Obróæ w prawo (normalnie)
    }
}
