#include "Player.h"

Player::Player(float x, float y, float radius, float speed)
    : position(x, y), radius(radius), speed(speed), color(al_map_rgb(0, 255, 0)) {}

void Player::update(const ALLEGRO_KEYBOARD_STATE& key_state, float delta_time) {
    float dx = 0, dy = 0;
    if (al_key_down(&key_state, ALLEGRO_KEY_W)) dy -= 1;
    if (al_key_down(&key_state, ALLEGRO_KEY_S)) dy += 1;
    if (al_key_down(&key_state, ALLEGRO_KEY_A)) dx -= 1;
    if (al_key_down(&key_state, ALLEGRO_KEY_D)) dx += 1;

    if (dx != 0 || dy != 0) {
        float len = std::sqrt(dx*dx + dy*dy);
        dx /= len;
        dy /= len;
    }

    position.translate(dx * speed * delta_time, dy * speed * delta_time);
}


void Player::render(PrimitiveRenderer* renderer) {
    if (!renderer) return;
    renderer->setColor(color);
    renderer->drawCircle(position.getX(), position.getY(), radius, true);
    renderer->setColor(al_map_rgb(255, 255, 255));
    renderer->drawCircle(position.getX(), position.getY(), radius, false); // kontur
}
