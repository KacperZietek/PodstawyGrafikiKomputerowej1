#include "Engine.h"
#include "PrimitiveRenderer.h"
#include "Point2D.h"
#include "LineSegment.h"
#include "PointShape.h"
#include "LineShape.h"
#include "Player.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>
#include <string>

class ComprehensiveDemo : public Engine {
private:
    PrimitiveRenderer renderer;

    // Podstawowe elementy graficzne
    std::vector<Point2D> points;
    std::vector<LineSegment> segments;

    // Shape objects do transformacji
    std::vector<std::shared_ptr<ShapeObject>> shapes;

    // Player
    Player player;

    // Bitmapy
    ALLEGRO_BITMAP* test_bitmap;
    ALLEGRO_BITMAP* animated_sprite;
    std::vector<ALLEGRO_BITMAP*> sprite_frames;

    // Stany i tryby
    bool use_incremental_algorithm;
    bool show_shapes;
    bool show_player;
    bool show_bitmaps;
    bool animate_shapes;
    bool animate_sprite;
    bool fill_primitives;

    // Transformacje
    float rotation_angle;
    float scale_factor;
    float translation_offset;

    // Animacje
    float accumulated_time;
    float sprite_animation_time;
    int current_sprite_frame;

    // Kolory
    ALLEGRO_COLOR current_color;
    int color_phase;

    // FPS
    float fps_time_accumulator;
    int fps_frame_count;
    int current_fps;

public:
    ComprehensiveDemo()
        : player(400, 300),
          test_bitmap(nullptr),
          animated_sprite(nullptr),
          use_incremental_algorithm(false),
          show_shapes(true),
          show_player(true),
          show_bitmaps(true),
          animate_shapes(false),
          animate_sprite(false),
          fill_primitives(false),
          rotation_angle(0.0f),
          scale_factor(1.0f),
          translation_offset(0.0f),
          accumulated_time(0.0f),
          sprite_animation_time(0.0f),
          current_sprite_frame(0),
          color_phase(0),
          fps_time_accumulator(0.0f),
          fps_frame_count(0),
          current_fps(0) {
    }

    virtual ~ComprehensiveDemo() {
        // Zwolnienie bitmap
        if (test_bitmap) {
            al_destroy_bitmap(test_bitmap);
        }
        if (animated_sprite) {
            al_destroy_bitmap(animated_sprite);
        }
        for (auto& frame : sprite_frames) {
            if (frame) {
                al_destroy_bitmap(frame);
            }
        }
    }

    virtual void update(float delta_time) override {
        accumulated_time += delta_time;

        // Liczenie FPS
        fps_time_accumulator += delta_time;
        fps_frame_count++;
        if (fps_time_accumulator >= 1.0f) {
            current_fps = fps_frame_count;
            fps_frame_count = 0;
            fps_time_accumulator = 0.0f;
        }

        // Animacja kolorów
        color_phase = static_cast<int>(accumulated_time * 100) % 360;
        current_color = al_map_rgb(
            (sin(color_phase * 3.14159f / 180.0f) + 1) * 127,
            (sin((color_phase + 120) * 3.14159f / 180.0f) + 1) * 127,
            (sin((color_phase + 240) * 3.14159f / 180.0f) + 1) * 127
        );

        // Animacja transformacji kształtów
        if (animate_shapes) {
            rotation_angle += delta_time * 30.0f;
            scale_factor = 1.0f + sin(accumulated_time) * 0.5f;
            translation_offset = sin(accumulated_time * 2.0f) * 50.0f;

            // Zastosuj transformacje do wszystkich kształtów
            for (auto& shape : shapes) {
                shape->rotate(delta_time * 30.0f, 400, 300);
                shape->scale(scale_factor, scale_factor, 400, 300);
                shape->translate(sin(accumulated_time) * 20.0f * delta_time,
                                cos(accumulated_time) * 20.0f * delta_time);
            }
        }

        // Animacja sprite'a
        if (animate_sprite && !sprite_frames.empty()) {
            sprite_animation_time += delta_time;
            if (sprite_animation_time >= 0.1f) { // 10 FPS animacji
                sprite_animation_time = 0.0f;
                current_sprite_frame = (current_sprite_frame + 1) % sprite_frames.size();
                animated_sprite = sprite_frames[current_sprite_frame];
            }
        }

        // Aktualizacja gracza
        player.update(delta_time);
    }

    virtual void render() override {
        // Czyszczenie ekranu - STAŁE TŁO (czarne)
        clear_screen(al_map_rgb(0, 0, 0));

        // === RYSOWANIE PUNKTOW Z ANIMACJA ===
        renderer.setColor(255, 255, 0);
        for (size_t i = 0; i < points.size(); ++i) {
            // Animacja punktow - pulsowanie
            float pulse = (sin(accumulated_time * 3.0f + i) + 1) * 2.0f + 3.0f;
            points[i].draw(&renderer, pulse);
        }

        // === RYSOWANIE ODCINKOW Z ROZNYMI ALGORYTMAMI ===
        renderer.setColor(255, 0, 0);
        for (const auto& segment : segments) {
            LineAlgorithm algo = use_incremental_algorithm ? INCREMENTAL : DEFAULT;
            segment.draw(&renderer, algo);
        }

        // === RYSOWANIE LINII LAMANEJ ===
        if (points.size() >= 2) {
            renderer.setColor(0, 255, 0);
            renderer.drawPolyline(points, false);
        }

        // === RYSOWANIE SHAPE OBJECTS Z TRANSFORMACJAMI ===
        if (show_shapes) {
            for (const auto& shape : shapes) {
                shape->draw(&renderer);
            }
        }

        // === RYSOWANIE GRACZA ===
        if (show_player) {
            player.draw(&renderer);
        }

        // === RYSOWANIE PRYMITYWOW Z WYPELNIENIEM ===
        renderer.setColor(current_color);

        // Prostokaty - wypelnione i niewypelnione
        renderer.drawRectangle(50, 50, 150, 100, fill_primitives);
        renderer.drawRectangle(170, 50, 270, 100, !fill_primitives);

        // Kola - wypelnione i niewypelnione
        renderer.drawCircle(350, 75, 25, fill_primitives);
        renderer.drawCircle(410, 75, 25, !fill_primitives);

        // Trojkaty - wypelnione i niewypelnione
        renderer.drawTriangle(500, 50, 550, 100, 450, 100, fill_primitives);
        renderer.drawTriangle(600, 50, 650, 100, 550, 100, !fill_primitives);

        // === RYSOWANIE BITMAP ===
        if (show_bitmaps) {
            // Statyczna bitmapa z transformacjami
            if (test_bitmap) {
                float bitmap_x = 700 + sin(accumulated_time) * 20.0f;
                float bitmap_y = 100 + cos(accumulated_time) * 20.0f;
                float bitmap_angle = accumulated_time * 30.0f;

                al_draw_rotated_bitmap(test_bitmap,
                                      al_get_bitmap_width(test_bitmap) / 2,
                                      al_get_bitmap_height(test_bitmap) / 2,
                                      bitmap_x, bitmap_y,
                                      bitmap_angle * 3.14159f / 180.0f, 0);
            }

            // Animowana bitmapa (sprite)
            if (animated_sprite) {
                float sprite_x = 700 + sin(accumulated_time * 2.0f) * 100.0f;
                float sprite_y = 250 + cos(accumulated_time * 2.0f) * 50.0f;

                al_draw_bitmap(animated_sprite, sprite_x, sprite_y, 0);
            }

            // Multipleksowanie bitmapy (efekt grid)
            if (test_bitmap) {
                for (int i = 0; i < 3; ++i) {
                    for (int j = 0; j < 2; ++j) {
                        al_draw_scaled_bitmap(test_bitmap,
                                            0, 0,
                                            al_get_bitmap_width(test_bitmap),
                                            al_get_bitmap_height(test_bitmap),
                                            750 + i * 40, 350 + j * 40,
                                            30, 30, 0);
                    }
                }
            }
        }

        // === WYSWIETLANIE INFORMACJI I INTERFEJSU ===
        renderer.setColor(255, 255, 255);
        if (get_font()) {
            int y_offset = 10;

            // Informacje o trybach
            std::string algorithm_text = use_incremental_algorithm ?
                "Algorytm: INCREMENTAL" : "Algorytm: DEFAULT";
            al_draw_text(get_font(), al_map_rgb(255, 255, 255), 10, y_offset, 0,
                        algorithm_text.c_str());
            y_offset += 20;

            std::string fill_text = fill_primitives ?
                "Wypelnienie: WLACZONE" : "Wypelnienie: WYLACZONE";
            al_draw_text(get_font(), al_map_rgb(255, 255, 255), 10, y_offset, 0,
                        fill_text.c_str());
            y_offset += 20;

            std::string anim_text = animate_shapes ?
                "Animacja ksztaltow: WLACZONA" : "Animacja ksztaltow: WYLACZONA";
            al_draw_text(get_font(), al_map_rgb(255, 255, 255), 10, y_offset, 0,
                        anim_text.c_str());
            y_offset += 20;

            std::string sprite_text = animate_sprite ?
                "Animacja sprite: WLACZONA" : "Animacja sprite: WYLACZONA";
            al_draw_text(get_font(), al_map_rgb(255, 255, 255), 10, y_offset, 0,
                        sprite_text.c_str());
            y_offset += 30;

            // Sterowanie
            al_draw_text(get_font(), al_map_rgb(255, 200, 100), 10, y_offset, 0,
                        "STEROWANIE:");
            y_offset += 20;

            al_draw_text(get_font(), al_map_rgb(200, 200, 200), 10, y_offset, 0,
                        "SPACE - Algorytm | F - Wypelnienie | T - Animacja ksztaltow");
            y_offset += 20;

            al_draw_text(get_font(), al_map_rgb(200, 200, 200), 10, y_offset, 0,
                        "Y - Animacja sprite | P - Gracz | B - Bitmapy");
            y_offset += 20;

            al_draw_text(get_font(), al_map_rgb(200, 200, 200), 10, y_offset, 0,
                        "C - Czysc | R - Reset | ESC - Wyjscie");
            y_offset += 20;

            al_draw_text(get_font(), al_map_rgb(200, 200, 200), 10, y_offset, 0,
                        "WASD - Ruch | Strzalki - Rotacja");
            y_offset += 20;

            al_draw_text(get_font(), al_map_rgb(200, 200, 200), 10, y_offset, 0,
                        "LPM - Dodaj punkt | PPM - Czysc punkty");
            y_offset += 30;

            // Statystyki
            std::string stats = "Punkty: " + std::to_string(points.size()) +
                               " | Odcinki: " + std::to_string(segments.size()) +
                               " | Ksztalty: " + std::to_string(shapes.size()) +
                               " | FPS: " + std::to_string(current_fps);
            al_draw_text(get_font(), al_map_rgb(100, 255, 100), 10, y_offset, 0,
                        stats.c_str());
        }
    }

    virtual void on_key_press(int key) override {
        if (key == ALLEGRO_KEY_ESCAPE) {
            stop();
        }
        else if (key == ALLEGRO_KEY_SPACE) {
            use_incremental_algorithm = !use_incremental_algorithm;
        }
        else if (key == ALLEGRO_KEY_P) {
            show_player = !show_player;
        }
        else if (key == ALLEGRO_KEY_B) {
            show_bitmaps = !show_bitmaps;
        }
        else if (key == ALLEGRO_KEY_F) {
            fill_primitives = !fill_primitives;
        }
        else if (key == ALLEGRO_KEY_T) {  // ZMIENIONE z A na T
            animate_shapes = !animate_shapes;
        }
        else if (key == ALLEGRO_KEY_R) {
            // Reset animacji
            rotation_angle = 0.0f;
            scale_factor = 1.0f;
            translation_offset = 0.0f;
            reset_shapes();
        }
        else if (key == ALLEGRO_KEY_C) {
            points.clear();
            segments.clear();
            // Zachowaj tylko podstawowe ksztalty
            shapes.erase(std::remove_if(shapes.begin(), shapes.end(),
                [](const std::shared_ptr<ShapeObject>& shape) {
                    // Usun tylko te stworzone przez klikniecia
                    PointShape* ps = dynamic_cast<PointShape*>(shape.get());
                    // Sprawdz czy to PointShape i czy ma wieksza wspolrzedna x niz 200
                    if (ps) {
                        // Uzyj metody Point2D przez publiczny interfejs PointShape
                        return true; // Tymczasowo usuwamy wszystkie PointShape
                    }
                    return false;
                }), shapes.end());
        }
        else if (key == ALLEGRO_KEY_Y) {  // ZMIENIONE z 1 na Y
            animate_sprite = !animate_sprite;
        }

        // Przekaz input do gracza
        ALLEGRO_EVENT ev;
        ev.type = ALLEGRO_EVENT_KEY_DOWN;
        ev.keyboard.keycode = key;
        player.handleEvent(ev);
    }

    virtual void on_key_release(int key) override {
        // Mozesz dodac obsluge zwolnienia klawiszy jesli potrzebna
    }

    virtual void on_mouse_click(int button, int x, int y) override {
        if (button == 1) { // Lewy przycisk - dodaj punkt
            points.push_back(Point2D(x, y));

            // Jesli mamy przynajmniej 2 punkty, tworzymy odcinek
            if (points.size() >= 2) {
                segments.push_back(LineSegment(
                    points[points.size()-2],
                    points[points.size()-1]
                ));
            }

            // Dodaj rowniez PointShape w tym miejscu
            auto newPoint = std::make_shared<PointShape>(x, y, 8.0f);
            shapes.push_back(newPoint);
        }
        else if (button == 2) { // Prawy przycisk - wyczysc punkty
            points.clear();
            segments.clear();
            shapes.erase(std::remove_if(shapes.begin(), shapes.end(),
                [](const std::shared_ptr<ShapeObject>& shape) {
                    return dynamic_cast<PointShape*>(shape.get()) != nullptr;
                }), shapes.end());
        }
    }

    virtual void on_mouse_move(int x, int y) override {
        // Sledzenie pozycji myszy - mozesz dodac interakcje
    }

    void initialize_game() {
        // Inicjalizacja stanow
        use_incremental_algorithm = false;
        show_shapes = true;
        show_player = true;
        show_bitmaps = true;
        animate_shapes = false;
        animate_sprite = false;
        fill_primitives = false;

        // Przykladowe punkty i odcinki
        create_sample_geometry();

        // Inicjalizacja ksztaltow
        initialize_shapes();

        // Inicjalizacja bitmap
        initialize_bitmaps();
    }

private:
    void create_sample_geometry() {
        // Punkty tworzace gwiazde
        for (int i = 0; i < 8; ++i) {
            float angle = i * 3.14159f / 4.0f;
            points.push_back(Point2D(
                200 + cos(angle) * 80.0f,
                400 + sin(angle) * 80.0f
            ));
        }

        // Odcinki tworzace prosty ksztalt
        segments.push_back(LineSegment(100, 300, 300, 300));
        segments.push_back(LineSegment(300, 300, 200, 200));
        segments.push_back(LineSegment(200, 200, 100, 300));
    }

    void initialize_shapes() {
        // PointShape - czerwony punkt
        auto point1 = std::make_shared<PointShape>(100, 500, 12.0f);
        shapes.push_back(point1);

        // LineShape - niebieska linia
        auto line1 = std::make_shared<LineShape>(150, 500, 250, 550);
        shapes.push_back(line1);

        // Ksztalt z wieloma punktami
        auto point2 = std::make_shared<PointShape>(300, 500, 8.0f);
        shapes.push_back(point2);

        auto line2 = std::make_shared<LineShape>(350, 450, 450, 500);
        shapes.push_back(line2);
    }

    void reset_shapes() {
        shapes.clear();
        initialize_shapes();
    }

    void initialize_bitmaps() {
        // Tworzenie testowej bitmapy (czerwone kolo na niebieskim tle)
        test_bitmap = al_create_bitmap(64, 64);
        if (test_bitmap) {
            al_set_target_bitmap(test_bitmap);
            al_clear_to_color(al_map_rgb(0, 0, 255));
            al_draw_filled_circle(32, 32, 25, al_map_rgb(255, 0, 0));
            al_set_target_backbuffer(al_get_current_display());
        }

        // Tworzenie prostych klatek animacji sprite'a
        for (int i = 0; i < 4; ++i) {
            ALLEGRO_BITMAP* frame = al_create_bitmap(32, 32);
            if (frame) {
                al_set_target_bitmap(frame);
                al_clear_to_color(al_map_rgba(0, 0, 0, 0)); // Przezroczyste tlo

                // Rysowanie roznych ksztaltow dla kazdej klatki
                ALLEGRO_COLOR frame_color = al_map_rgb(
                    (i * 60) % 255,
                    (i * 90) % 255,
                    (i * 120) % 255
                );

                switch (i) {
                    case 0: al_draw_filled_rectangle(4, 4, 28, 28, frame_color); break;
                    case 1: al_draw_filled_circle(16, 16, 12, frame_color); break;
                    case 2: al_draw_filled_triangle(16, 4, 4, 28, 28, 28, frame_color); break;
                    case 3: al_draw_filled_ellipse(16, 16, 10, 14, frame_color); break;
                }

                sprite_frames.push_back(frame);
            }
        }

        if (!sprite_frames.empty()) {
            animated_sprite = sprite_frames[0];
        }

        al_set_target_backbuffer(al_get_current_display());
    }
};

int main() {
    ComprehensiveDemo game;

    // Inicjalizacja z podwojnym buforowaniem
    if (!game.initialize(1024, 768, WINDOWED, 60, true)) {
        return -1;
    }

    game.initialize_game();
    game.run();

    return 0;
}
