#ifndef BITMAPOBJECT_H
#define BITMAPOBJECT_H

#include "GameInterfaces.h"
#include "Point2D.h"
#include <allegro5/allegro.h>

class BitmapObject : public DrawableObject, public TransformableObject, public UpdatableObject {
protected:
    ALLEGRO_BITMAP* bitmap;
    Point2D position;
    float angle;
    float scaleX, scaleY;
    int width, height;

public:
    BitmapObject(ALLEGRO_BITMAP* bmp, float x, float y)
        : bitmap(bmp), position(x, y), angle(0.0f), scaleX(1.0f), scaleY(1.0f) {
        if (bitmap) {
            width = al_get_bitmap_width(bitmap);
            height = al_get_bitmap_height(bitmap);
        } else {
            width = 0;
            height = 0;
        }
    }

    virtual ~BitmapObject() {}

    void update(float deltaTime) override {}

    void draw(PrimitiveRenderer* renderer) override {
        if (!bitmap) return;
        al_draw_scaled_rotated_bitmap(
            bitmap,
            width / 2.0f, height / 2.0f,
            position.getX(), position.getY(),
            scaleX, scaleY,
            angle,
            0
        );
    }

    void translate(float dx, float dy) override {
        position.setX(position.getX() + dx);
        position.setY(position.getY() + dy);
    }

    void rotate(float angle_radians, float ox, float oy) override {
        angle += angle_radians;
    }

    void scale(float k, float ox, float oy) override {
        scaleX *= k;
        scaleY *= k;
    }

    Point2D getPosition() const { return position; }

    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

#endif
