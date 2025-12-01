#include "Player.h"
#include <iostream>

const char* PLAYER_IMAGE = "walk.png";
const int FRAME_H = 128;
const int FRAME_W = 128;
const int TOTAL_FRAMES = 8;
const float ANIM_SPEED = 0.1f;

Player::Player(BitmapHandler* handler, float x, float y)
    : SpriteObject(handler->loadBitmap(PLAYER_IMAGE), x, y, FRAME_W, FRAME_H, TOTAL_FRAMES, ANIM_SPEED),
      speed(200.0f)
{
    //skalowanie
    scale(0.8f, 0, 0);

    if (!bitmap) {
        std::cerr << "[Player] Blad: Nie udalo sie wczytac bitmapy gracza!" << std::endl;
    }
}

void Player::update(float deltaTime) {
    //animacja
    SpriteObject::animate(deltaTime);

    //ruch i Odbicie lustrzane
    ALLEGRO_KEYBOARD_STATE keyState;
    al_get_keyboard_state(&keyState);

    float moveDist = speed * deltaTime;

    if (al_key_down(&keyState, ALLEGRO_KEY_W) || al_key_down(&keyState, ALLEGRO_KEY_UP)) {
        translate(0, -moveDist);
    }
    if (al_key_down(&keyState, ALLEGRO_KEY_S) || al_key_down(&keyState, ALLEGRO_KEY_DOWN)) {
        translate(0, moveDist);
    }

    //ruch w lewo
    if (al_key_down(&keyState, ALLEGRO_KEY_A) || al_key_down(&keyState, ALLEGRO_KEY_LEFT)) {
        translate(-moveDist, 0);
        setFlip(true);
    }
    //ruch w prawo
    if (al_key_down(&keyState, ALLEGRO_KEY_D) || al_key_down(&keyState, ALLEGRO_KEY_RIGHT)) {
        translate(moveDist, 0);
        setFlip(false);
}}
