#ifndef SHAPEOBJECT_H
#define SHAPEOBJECT_H

#include "GameInterfaces.h"
#include <vector>
#include <cmath>

enum ShapeType {
    POLYGON,
    CIRCLE,
    ELLIPSE,
    LINE
};

class ShapeObject : public UpdatableObject, public DrawableObject, public TransformableObject {
protected:
    std::vector<Point2D> vertices;
    Point2D position;
    ALLEGRO_COLOR color;
    bool filled;

    ShapeType type;
    float radius;
    float radiusX, radiusY;

public:
    ShapeObject(float x, float y, ALLEGRO_COLOR col, bool fill = false)
        : position(x, y), color(col), filled(fill), type(POLYGON), radius(0), radiusX(0), radiusY(0) {}

    static ShapeObject* createCircle(float x, float y, float r, ALLEGRO_COLOR col) {
        ShapeObject* obj = new ShapeObject(x, y, col);
        obj->type = CIRCLE;
        obj->radius = r;
        return obj;
    }

    static ShapeObject* createEllipse(float x, float y, float rx, float ry, ALLEGRO_COLOR col) {
        ShapeObject* obj = new ShapeObject(x, y, col);
        obj->type = ELLIPSE;
        obj->radiusX = rx;
        obj->radiusY = ry;
        return obj;
    }

    //metoda tworząca linie
    static ShapeObject* createLine(float x1, float y1, float x2, float y2, ALLEGRO_COLOR col) {
        ShapeObject* obj = new ShapeObject(0, 0, col);
        obj->type = LINE;
        obj->addVertex(x1, y1);
        obj->addVertex(x2, y2);
        return obj;
    }

    void addVertex(float x, float y) {
        vertices.push_back(Point2D(x, y));
    }

    void setColor(ALLEGRO_COLOR c) { color = c; }

    void update(float deltaTime) override {}

    void draw(PrimitiveRenderer* renderer) override {
        if (type == POLYGON) {
            if (!vertices.empty()) renderer->drawPolygon(vertices, color);
        }
        else if (type == CIRCLE) {
            renderer->drawCircle((int)position.getX(), (int)position.getY(), (int)radius, color);
        }
        else if (type == ELLIPSE) {
            renderer->drawEllipse((int)position.getX(), (int)position.getY(), (int)radiusX, (int)radiusY, color);
        }
        //rysowanie linii algorytmem przyrostowym
        else if (type == LINE) {
            if (vertices.size() >= 2) {
                renderer->drawLineIncremental(vertices[0], vertices[1], color);
            }
        }
    }

    void translate(float dx, float dy) override {
        position.setX(position.getX() + dx);
        position.setY(position.getY() + dy);

        if (type == POLYGON || type == LINE) {
            for (auto& p : vertices) {
                p.setX(p.getX() + dx);
                p.setY(p.getY() + dy);
            }
        }
    }

    void rotate(float angle, float ox, float oy) override {
        if (type == POLYGON || type == LINE) {
            float s = std::sin(angle);
            float c = std::cos(angle);
            for (auto& p : vertices) {
                float px = p.getX() - ox;
                float py = p.getY() - oy;
                float xNew = px * c - py * s;
                float yNew = px * s + py * c;
                p.setX(xNew + ox);
                p.setY(yNew + oy);
            }
        }
    }

    void scale(float k, float ox, float oy) override {
        if (type == POLYGON || type == LINE) {
            for (auto& p : vertices) {
                float xNew = p.getX() * k + (1 - k) * ox;
                float yNew = p.getY() * k + (1 - k) * oy;
                p.setX(xNew);
                p.setY(yNew);
            }
        }
        else if (type == CIRCLE) {
            radius *= k;
        }
        else if (type == ELLIPSE) {
            radiusX *= k;
            radiusY *= k;
        }
    }

    Point2D getPosition() const { return position; }
};

#endif
