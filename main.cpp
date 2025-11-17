#include "Engine.h"
#include "PrimitiveRenderer.h"
#include "Point2D.h"
#include "LineSegment.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include "Player.h"


// ---------------------- MyGame ----------------------
class MyGame : public Engine {
private:
    PrimitiveRenderer renderer;
    Player player;
    std::vector<Point2D> points;
    std::vector<LineSegment> segments;
    bool use_incremental_algorithm;
    bool show_circle;
    bool show_ellipse;
    bool show_filled_polygon;
    bool test_filled[4] = {false, false, false, false};
    std::vector<Point2D> polygon_vertices;


    bool key_state[ALLEGRO_KEY_MAX] = {false};

public:
    MyGame() : use_incremental_algorithm(false), show_circle(true),
               show_ellipse(true), show_filled_polygon(false),
               player(512, 384, 20.0f, 200.0f) {
        // Przykładowy wielokąt
        polygon_vertices.push_back(Point2D(200, 200));
        polygon_vertices.push_back(Point2D(300, 150));
        polygon_vertices.push_back(Point2D(400, 200));
        polygon_vertices.push_back(Point2D(350, 300));
        polygon_vertices.push_back(Point2D(250, 300));
    }

    virtual void update(float delta_time) override {
        ALLEGRO_KEYBOARD_STATE keyState;
        al_get_keyboard_state(&keyState);
        player.update(keyState, delta_time);

    }

    virtual void render() override {
        clear_screen(al_map_rgb(0, 0, 0));

        // --- Twój istniejący render() bez zmian ---
        // 1. Biały kwadrat
        if (test_filled[0]) {
            renderer.setColor(255, 255, 0);
            renderer.drawRectangle(100, 100, 200, 200, true);
            renderer.setColor(255, 255, 255);
            renderer.drawRectangle(100, 100, 200, 200, false);
        } else {
            renderer.setColor(255, 255, 255);
            renderer.drawRectangle(100, 100, 200, 200, false);
        }

        // 2. Czerwone koło
        if (test_filled[2]) {
            renderer.setColor(255, 0, 255);
            renderer.drawCircle(400, 300, 50, true);
            renderer.setColor(255, 0, 0);
            renderer.drawCircle(400, 300, 50, false);
        } else {
            renderer.setColor(255, 0, 0);
            renderer.drawCircle(400, 300, 50, false);
        }

        // 3. Zielony trójkąt
        if (test_filled[3]) {
            renderer.setColor(0, 255, 255);
            renderer.drawTriangle(500, 100, 600, 100, 550, 200, true);
            renderer.setColor(0, 255, 0);
            renderer.drawTriangle(500, 100, 600, 100, 550, 200, false);
        } else {
            renderer.setColor(0, 255, 0);
            renderer.drawTriangle(500, 100, 600, 100, 550, 200, false);
        }

        // 4. Niebieskie tło (symulacja flood fill)
        if (test_filled[1]) {
            renderer.setColor(0, 0, 255);
            renderer.drawRectangle(0, 0, 1024, 768, true);
            // Ponownie rysujemy kształty na wierzchu
            if (test_filled[0]) {
                renderer.setColor(255, 255, 0);
                renderer.drawRectangle(100, 100, 200, 200, true);
                renderer.setColor(255, 255, 255);
                renderer.drawRectangle(100, 100, 200, 200, false);
            } else {
                renderer.setColor(255, 255, 255);
                renderer.drawRectangle(100, 100, 200, 200, false);
            }

            if (test_filled[2]) {
                renderer.setColor(255, 0, 255);
                renderer.drawCircle(400, 300, 50, true);
                renderer.setColor(255, 0, 0);
                renderer.drawCircle(400, 300, 50, false);
            } else {
                renderer.setColor(255, 0, 0);
                renderer.drawCircle(400, 300, 50, false);
            }

            if (test_filled[3]) {
                renderer.setColor(0, 255, 255);
                renderer.drawTriangle(500, 100, 600, 100, 550, 200, true);
                renderer.setColor(0, 255, 0);
                renderer.drawTriangle(500, 100, 600, 100, 550, 200, false);
            } else {
                renderer.setColor(0, 255, 0);
                renderer.drawTriangle(500, 100, 600, 100, 550, 200, false);
            }
        }

        // Punkty użytkownika
        renderer.setColor(255, 255, 0);
        for (const auto& p : points)
            p.draw(&renderer);

        // Odcinki użytkownika
        renderer.setColor(255, 0, 0);
        for (const auto& seg : segments) {
            LineAlgorithm algo = use_incremental_algorithm ? INCREMENTAL : DEFAULT;
            seg.draw(&renderer, algo);
        }

        // Łamana użytkownika
        renderer.setColor(0, 255, 0);
        renderer.drawPolyline(points, false);

        // Okrąg na środku ekranu
        if (show_circle) {
            renderer.setColor(0, 128, 255);
            renderer.drawCircleSymmetric(512, 384, 100, true);
        }

        // Elipsa
        if (show_ellipse) {
            renderer.setColor(255, 0, 255);
            renderer.drawEllipse(700, 384, 120, 80, false);
        }

        // Wielokąt
        if (show_filled_polygon) {
            renderer.setColor(255, 128, 0);
            renderer.fillPolygon(polygon_vertices);
        } else {
            renderer.setColor(0, 255, 255);
            renderer.drawPolygon(polygon_vertices);
        }

        // Tekst
        if (get_font()) {
            std::string alg = use_incremental_algorithm ? "INCREMENTAL" : "DEFAULT";
            al_draw_text(get_font(), al_map_rgb(255,255,255), 10, 10, 0, ("Algorytm: " + alg).c_str());
            al_draw_text(get_font(), al_map_rgb(255,255,255), 10, 30, 0,
                "LPM - dodaj punkt | PPM - czysc | R - obrot | Y - skalowanie | T - przesuniecie");
            al_draw_text(get_font(), al_map_rgb(255,255,255), 10, 50, 0,
                "O - okrag | E - elipsa | P - wielokat");
            al_draw_text(get_font(), al_map_rgb(255,255,255), 10, 70, 0,
                "1 - boundary fill kwadrat (zolty) | 2 - flood fill tło (niebieski)");
            al_draw_text(get_font(), al_map_rgb(255,255,255), 10, 90, 0,
                "3 - boundary fill koło (rozowy) | 4 - boundary fill trojkąt (cyjan)");

            std::string fill_status = "Wypelnienie: ";
            for (int i = 0; i < 4; i++) {
                fill_status += test_filled[i] ? "1" : "0";
                if (i < 3) fill_status += " ";
            }
            al_draw_text(get_font(), al_map_rgb(255,255,255), 10, 110, 0, fill_status.c_str());
        }

        // --- Rysujemy gracza ---
        player.render(&renderer);
    }

    virtual void on_key_press(int key) override {
        key_state[key] = true;

        if (key == ALLEGRO_KEY_ESCAPE) stop();
        if (key == ALLEGRO_KEY_SPACE) use_incremental_algorithm = !use_incremental_algorithm;
        if (key == ALLEGRO_KEY_O) show_circle = !show_circle;
        if (key == ALLEGRO_KEY_E) show_ellipse = !show_ellipse;
        if (key == ALLEGRO_KEY_P) show_filled_polygon = !show_filled_polygon;

        // WYPEŁNIANIE OBSZARU
        if (key == ALLEGRO_KEY_1) test_filled[0] = !test_filled[0];
        if (key == ALLEGRO_KEY_2) test_filled[1] = !test_filled[1];
        if (key == ALLEGRO_KEY_3) test_filled[2] = !test_filled[2];
        if (key == ALLEGRO_KEY_4) test_filled[3] = !test_filled[3];
        if (key == ALLEGRO_KEY_5) for (int i = 0; i < 4; i++) test_filled[i] = false;

        if (key == ALLEGRO_KEY_R) {
            float cx = 512.0f, cy = 384.0f;
            for (auto& p : points) p.rotate(15.0f, cx, cy);
            for (auto& v : polygon_vertices) v.rotate(15.0f, cx, cy);
        }

        if (key == ALLEGRO_KEY_Y) {
            float cx = 512.0f, cy = 384.0f;
            for (auto& p : points) { p.translate(-cx, -cy); p.scale(1.1f, 1.1f); p.translate(cx, cy); }
            for (auto& v : polygon_vertices) { v.translate(-cx, -cy); v.scale(1.1f, 1.1f); v.translate(cx, cy); }
        }

        if (key == ALLEGRO_KEY_T) {
            for (auto& p : points) p.translate(10.0f, 10.0f);
            for (auto& v : polygon_vertices) v.translate(10.0f, 10.0f);
        }
    }

    virtual void on_key_release(int key) override {
        key_state[key] = false;
    }

    virtual void on_mouse_click(int button, int x, int y) override {
        if (button == 1) {
            points.push_back(Point2D(x, y));
            if (points.size() >= 2) segments.push_back(LineSegment(points[points.size()-2], points.back()));
        } else if (button == 2) {
            points.clear();
            segments.clear();
        }
    }

    void initialize_game() {
        use_incremental_algorithm = false;
        show_circle = true;
        show_ellipse = true;
        show_filled_polygon = false;
        for (int i = 0; i < 4; i++) test_filled[i] = false;
    }
};

// ---------------------- main ----------------------
int main() {
    MyGame game;
    if (!game.initialize(1024, 768, WINDOWED, 60, true)) return -1;
    game.initialize_game();
    game.run();
    return 0;
}
