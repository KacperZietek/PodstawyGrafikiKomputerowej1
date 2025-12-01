#ifndef PLAYER_H
#define PLAYER_H

#include "SpriteObject.h"
#include "BitmapHandler.h" // Potrzebne do definicji wskaünika
#include <allegro5/allegro.h>

class Player : public SpriteObject {
private:
    float speed;

public:
    Player(BitmapHandler* handler, float x, float y);

    // Teraz override zadzia≥a, bo dziedziczenie jest naprawione
    void update(float deltaTime) override;
};

#endif
