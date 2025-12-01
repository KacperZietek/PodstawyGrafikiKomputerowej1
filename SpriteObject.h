#ifndef SPRITEOBJECT_H
#define SPRITEOBJECT_H

#include "BitmapObject.h"

class SpriteObject : public BitmapObject, public AnimatedObject {
protected: // Zmienione na protected, ¿eby Player mia³ dostêp
    int frameWidth;
    int frameHeight;
    int currentFrame;
    int totalFrames;
    int cols;

    float timePerFrame;
    float timer;

    int renderFlags = 0;

public:
    SpriteObject(ALLEGRO_BITMAP* bmp, float x, float y, int fWidth, int fHeight, int tFrames, float animSpeed = 0.1f)
        : BitmapObject(bmp, x, y),
          frameWidth(fWidth), frameHeight(fHeight),
          currentFrame(0), totalFrames(tFrames),
          timePerFrame(animSpeed), timer(0.0f) {

        if (bitmap) {
            int bmpW = al_get_bitmap_width(bitmap);
            cols = (frameWidth > 0) ? bmpW / frameWidth : 1;
        }
        width = frameWidth;
        height = frameHeight;
    }

    void setFlip(bool flipHorizontal) {
        if (flipHorizontal) renderFlags = ALLEGRO_FLIP_HORIZONTAL;
        else renderFlags = 0;
    }

    void animate(float deltaTime) override {
        timer += deltaTime;
        if (timer >= timePerFrame) {
            timer = 0;
            currentFrame++;
            if (currentFrame >= totalFrames) {
                currentFrame = 0;
            }
        }
    }

    // Dziêki naprawie BitmapObject, teraz mo¿emy to bezpiecznie nadpisaæ
    void update(float deltaTime) override {
        animate(deltaTime);
    }

    void draw(PrimitiveRenderer* renderer) override {
        if (!bitmap) return;

        int safeCols = (cols > 0) ? cols : 1;
        int fx = (currentFrame % safeCols) * frameWidth;
        int fy = (currentFrame / safeCols) * frameHeight;

        al_draw_tinted_scaled_rotated_bitmap_region(
            bitmap,
            fx, fy, frameWidth, frameHeight,
            al_map_rgb(255, 255, 255),
            frameWidth / 2.0f, frameHeight / 2.0f,
            position.getX(), position.getY(),
            scaleX, scaleY,
            angle,
            renderFlags
        );
    }
};

#endif
