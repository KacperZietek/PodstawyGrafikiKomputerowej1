#include "Engine.h"
#include "PrimitiveRenderer.h"
#include "Point2D.h"
#include "LineSegment.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <vector>

class MyGame : public Engine {
private:
    PrimitiveRenderer renderer;
    std::vector<Point2D> points;
    std::vector<LineSegment> segments;
    bool use_incremental_algorithm;

public:
    virtual void update(float delta_time) override {
    }

    virtual void render() override {
        // Czyszczenie ekranu safd
        clear_screen(al_map_rgb(0, 0, 0));

        // Rysowanie wszystkich punktów
        renderer.setColor(255, 255, 0); // ¯ó³ty
        for (const auto& point : points) {
            point.draw(&renderer);
        }

        // Rysowanie odcinków z wybranym algorytmem
        renderer.setColor(255, 0, 0); // Czerwony
        for (const auto& segment : segments) {
            LineAlgorithm algo = use_incremental_algorithm ? INCREMENTAL : DEFAULT;
            segment.draw(&renderer, algo);
        }

        // Rysowanie linii lamanej
        renderer.setColor(0, 255, 0); // Zielony
        renderer.drawPolyline(points, false);

        // Informacje
        renderer.setColor(255, 255, 255);
        if (get_font()) {
            std::string algorithm_text = use_incremental_algorithm ?
                "Algorithm: INCREMENTAL" : "Algorithm: DEFAULT";
            al_draw_text(get_font(), al_map_rgb(255, 255, 255), 10, 10, 0,
                        algorithm_text.c_str());
            al_draw_text(get_font(), al_map_rgb(255, 255, 255), 10, 30, 0,
                        "Press SPACE to change algorithm, ESC to exit");
        }
    }

    virtual void on_key_press(int key) override {
        if (key == ALLEGRO_KEY_ESCAPE) {
            stop();
        }
        if (key == ALLEGRO_KEY_SPACE) {
            use_incremental_algorithm = !use_incremental_algorithm;
        }
    }

    virtual void on_mouse_click(int button, int x, int y) override {
        if (button == 1) { // Lewy przycisk - dodaj punkt
            points.push_back(Point2D(x, y));

            // Jezeli mamy przynajmniej 2 punkty, tworzymy odcinek
            if (points.size() >= 2) {
                segments.push_back(LineSegment(
                    points[points.size()-2],
                    points[points.size()-1]
                ));
            }
        }
        else if (button == 2) { // Prawy przycisk - wyczysc
            points.clear();
            segments.clear();
        }
    }

    void initialize_game() {
        use_incremental_algorithm = false;

        // Przykladowe punkty i odcinki
        points.push_back(Point2D(100, 100));
        points.push_back(Point2D(200, 150));
        points.push_back(Point2D(300, 80));
        points.push_back(Point2D(400, 200));

        segments.push_back(LineSegment(100, 300, 400, 350));
        segments.push_back(LineSegment(150, 400, 350, 250));
    }
};

int main() {
    MyGame game;

    // Inicjalizacja z podwójnym buforowaniem
    if (!game.initialize(1024, 768, WINDOWED, 60, true)) {
        return -1;
    }

    game.initialize_game();
    game.run();

    return 0;
}
