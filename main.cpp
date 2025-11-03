#include "Engine.h"
#include "PrimitiveRenderer.h"
#include "Point2D.h"
#include "LineSegment.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include <string>
#include <cmath>

class MyGame : public Engine {
private:
    PrimitiveRenderer renderer;
    std::vector<Point2D> points;
    std::vector<LineSegment> segments;
    bool use_incremental_algorithm;
    bool show_circle;

public:
    MyGame() : use_incremental_algorithm(false), show_circle(true) {}

    virtual void update(float delta_time) override {}

    virtual void render() override {
        clear_screen(al_map_rgb(0, 0, 0));

        // Punkty
        renderer.setColor(255, 255, 0);
        for (const auto& p : points)
            p.draw(&renderer);

        // Odcinki
        renderer.setColor(255, 0, 0);
        for (const auto& seg : segments) {
            LineAlgorithm algo = use_incremental_algorithm ? INCREMENTAL : DEFAULT;
            seg.draw(&renderer, algo);
        }

        // Łamana
        renderer.setColor(0, 255, 0);
        renderer.drawPolyline(points, false);

        // Okrąg na środku ekranu
        if (show_circle) {
            renderer.setColor(0, 128, 255);
            renderer.drawCircleSymmetric(512, 384, 100, true);
        }


        if (get_font()) {
            std::string alg = use_incremental_algorithm ? "INCREMENTAL" : "DEFAULT";
            al_draw_text(get_font(), al_map_rgb(255,255,255), 10, 10, 0, ("Algorytm: " + alg).c_str());
            al_draw_text(get_font(), al_map_rgb(255,255,255), 10, 30, 0,
                "LPM - dodaj punkt | PPM - czysc | R - obrot | S - skalowanie | T - przesuniecie | O - okrag");
        }
    }

    virtual void on_key_press(int key) override {
        if (key == ALLEGRO_KEY_ESCAPE)
            stop();

        if (key == ALLEGRO_KEY_SPACE)
            use_incremental_algorithm = !use_incremental_algorithm;


        if (key == ALLEGRO_KEY_O)
            show_circle = !show_circle;

        if (key == ALLEGRO_KEY_R) {
            float cx = 512.0f, cy = 384.0f;
            for (auto& p : points)
                p.rotate(15.0f, cx, cy);
        }

        if (key == ALLEGRO_KEY_S) {
            float cx = 512.0f, cy = 384.0f;
            for (auto& p : points) {
                p.translate(-cx, -cy);
                p.scale(1.1f, 1.1f);
                p.translate(cx, cy);
            }
        }


        if (key == ALLEGRO_KEY_T) {
            for (auto& p : points)
                p.translate(10.0f, 10.0f);
        }
    }

    virtual void on_mouse_click(int button, int x, int y) override {
        if (button == 1) {
            points.push_back(Point2D(x, y));
            if (points.size() >= 2)
                segments.push_back(LineSegment(points[points.size() - 2], points.back()));
        }
        else if (button == 2) {
            points.clear();
            segments.clear();
        }
    }

    void initialize_game() {
        use_incremental_algorithm = false;
        show_circle = true;
    }
};

int main() {
    MyGame game;
    if (!game.initialize(1024, 768, WINDOWED, 60, true))
        return -1;

    game.initialize_game();
    game.run();
    return 0;
}
