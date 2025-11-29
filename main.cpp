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

// Klasa bazowa dla narysowanych kształtów
class DrawnShape {
public:
    virtual ~DrawnShape() = default;
    virtual void draw(PrimitiveRenderer* renderer, bool use_custom_circle, bool use_custom_ellipse, bool filled) const = 0;
};

// Klasa dla narysowanego okręgu
class DrawnCircle : public DrawnShape {
private:
    float center_x, center_y, radius;
    ALLEGRO_COLOR color;
public:
    DrawnCircle(float cx, float cy, float r, ALLEGRO_COLOR col)
        : center_x(cx), center_y(cy), radius(r), color(col) {}

    void draw(PrimitiveRenderer* renderer, bool use_custom_circle, bool use_custom_ellipse, bool filled) const override {
        renderer->setColor(color);
        if (use_custom_circle) {
            renderer->drawCircleBresenham(center_x, center_y, radius, filled);
        } else {
            renderer->drawCircle(center_x, center_y, radius, filled);
        }
    }
};

// Klasa dla narysowanej elipsy
class DrawnEllipse : public DrawnShape {
private:
    float center_x, center_y, rx, ry;
    ALLEGRO_COLOR color;
public:
    DrawnEllipse(float cx, float cy, float radius_x, float radius_y, ALLEGRO_COLOR col)
        : center_x(cx), center_y(cy), rx(radius_x), ry(radius_y), color(col) {}

    void draw(PrimitiveRenderer* renderer, bool use_custom_circle, bool use_custom_ellipse, bool filled) const override {
        renderer->setColor(color);
        if (use_custom_ellipse) {
            renderer->drawEllipseMidpoint(center_x, center_y, rx, ry, filled);
        } else {
            if (filled) {
                al_draw_filled_ellipse(center_x, center_y, rx, ry, color);
            } else {
                al_draw_ellipse(center_x, center_y, rx, ry, color, 2.0f);
            }
        }
    }
};

// Klasa dla narysowanego prostokąta
class DrawnRectangle : public DrawnShape {
private:
    float x1, y1, x2, y2;
    ALLEGRO_COLOR color;
public:
    DrawnRectangle(float x_start, float y_start, float x_end, float y_end, ALLEGRO_COLOR col)
        : x1(std::min(x_start, x_end)), y1(std::min(y_start, y_end)),
          x2(std::max(x_start, x_end)), y2(std::max(y_start, y_end)), color(col) {}

    void draw(PrimitiveRenderer* renderer, bool use_custom_circle, bool use_custom_ellipse, bool filled) const override {
        renderer->setColor(color);
        renderer->drawRectangle(x1, y1, x2, y2, filled);
    }
};

// Klasa dla narysowanego trójkąta
class DrawnTriangle : public DrawnShape {
private:
    float x1, y1, x2, y2, x3, y3;
    ALLEGRO_COLOR color;
public:
    DrawnTriangle(float x1_, float y1_, float x2_, float y2_, float x3_, float y3_, ALLEGRO_COLOR col)
        : x1(x1_), y1(y1_), x2(x2_), y2(y2_), x3(x3_), y3(y3_), color(col) {}

    void draw(PrimitiveRenderer* renderer, bool use_custom_circle, bool use_custom_ellipse, bool filled) const override {
        renderer->setColor(color);
        renderer->drawTriangle(x1, y1, x2, y2, x3, y3, filled);
    }
};

class ComprehensiveDemo : public Engine {
private:
    PrimitiveRenderer renderer;

    // Podstawowe elementy graficzne
    std::vector<Point2D> points;
    std::vector<LineSegment> segments;

    // Shape objects do transformacji
    std::vector<std::shared_ptr<ShapeObject>> shapes;

    // NOWE: Lista narysowanych kształtów
    std::vector<std::shared_ptr<DrawnShape>> drawn_shapes;

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

    // NOWE POLA do testowania algorytmów
    bool use_custom_circle_algorithm;
    bool use_custom_ellipse_algorithm;
    bool show_custom_primitives;

    // NOWE POLA do rysowania kształtów
    enum DrawingMode {
        DRAW_NONE,
        DRAW_CIRCLE,
        DRAW_ELLIPSE,
        DRAW_RECTANGLE,
        DRAW_TRIANGLE  // NOWE
    };

    DrawingMode current_drawing_mode;
    bool drawing_active;
    Point2D drawing_start_point;
    Point2D drawing_current_point;
    ALLEGRO_COLOR drawing_color;

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
          // NOWE INICJALIZACJE
          use_custom_circle_algorithm(false),
          use_custom_ellipse_algorithm(false),
          show_custom_primitives(true),
          current_drawing_mode(DRAW_NONE),
          drawing_active(false),
          drawing_start_point(0, 0),
          drawing_current_point(0, 0),
          drawing_color(al_map_rgb(255, 255, 0)), // Żółty kolor
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

    // DODANA METODA: przekazuje zdarzenia do gracza (OVERRIDE)
    virtual void forwardEventToPlayer(const ALLEGRO_EVENT& ev) override {
        if (show_player) {
            player.handleEvent(ev);
        }
    }

    // NOWE METODY do rysowania kształtów
    void startDrawing(DrawingMode mode, int start_x, int start_y) {
        current_drawing_mode = mode;
        drawing_active = true;
        drawing_start_point.setPosition(start_x, start_y);
        drawing_current_point.setPosition(start_x, start_y);
    }

    void updateDrawing(int current_x, int current_y) {
        if (drawing_active) {
            drawing_current_point.setPosition(current_x, current_y);
        }
    }

    void finishDrawing() {
    if (drawing_active) {
        // Zapisz kształt do listy
        float x1 = drawing_start_point.getX();
        float y1 = drawing_start_point.getY();
        float x2 = drawing_current_point.getX();
        float y2 = drawing_current_point.getY();

        switch (current_drawing_mode) {
            case DRAW_CIRCLE: {
                float center_x = (x1 + x2) / 2;
                float center_y = (y1 + y2) / 2;
                float radius = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2)) / 2;
                drawn_shapes.push_back(std::make_shared<DrawnCircle>(center_x, center_y, radius, drawing_color));
                break;
            }

            case DRAW_ELLIPSE: {
                float center_x = (x1 + x2) / 2;
                float center_y = (y1 + y2) / 2;
                float rx = std::abs(x2 - x1) / 2;
                float ry = std::abs(y2 - y1) / 2;
                drawn_shapes.push_back(std::make_shared<DrawnEllipse>(center_x, center_y, rx, ry, drawing_color));
                break;
            }

            case DRAW_RECTANGLE: {
                drawn_shapes.push_back(std::make_shared<DrawnRectangle>(x1, y1, x2, y2, drawing_color));
                break;
            }

            case DRAW_TRIANGLE: {  // NOWE
                // Trójkąt: punkt startowy + punkt przeciągnięcia + trzeci punkt obliczony
                float x3 = x1 + (x2 - x1) * 0.5f;
                float y3 = y1 - std::abs(y2 - y1) * 0.8f; // Trzeci punkt nad podstawą
                drawn_shapes.push_back(std::make_shared<DrawnTriangle>(x1, y2, x2, y2, x3, y3, drawing_color)); // POPRAWIONE: użyj y3
                break;
            }

            case DRAW_NONE:
                break;
        }

        drawing_active = false;
        current_drawing_mode = DRAW_NONE;
    }
}

    void drawCurrentShape(PrimitiveRenderer* renderer) {
    if (!drawing_active || !renderer) return;

    float x1 = drawing_start_point.getX();
    float y1 = drawing_start_point.getY();
    float x2 = drawing_current_point.getX();
    float y2 = drawing_current_point.getY();

    renderer->setColor(drawing_color);

    switch (current_drawing_mode) {
        case DRAW_CIRCLE: {
            float center_x = (x1 + x2) / 2;
            float center_y = (y1 + y2) / 2;
            float radius = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2)) / 2;

            if (use_custom_circle_algorithm) {
                renderer->drawCircleBresenham(center_x, center_y, radius, fill_primitives);
            } else {
                renderer->drawCircle(center_x, center_y, radius, fill_primitives);
            }
            break;
        }

        case DRAW_ELLIPSE: {
            float center_x = (x1 + x2) / 2;
            float center_y = (y1 + y2) / 2;
            float rx = std::abs(x2 - x1) / 2;
            float ry = std::abs(y2 - y1) / 2;

            if (use_custom_ellipse_algorithm) {
                renderer->drawEllipseMidpoint(center_x, center_y, rx, ry, fill_primitives);
            } else {
                if (fill_primitives) {
                    al_draw_filled_ellipse(center_x, center_y, rx, ry, drawing_color);
                } else {
                    al_draw_ellipse(center_x, center_y, rx, ry, drawing_color, 2.0f);
                }
            }
            break;
        }

        case DRAW_RECTANGLE: {
            if (fill_primitives) {
                renderer->drawRectangle(
                    std::min(x1, x2), std::min(y1, y2),
                    std::max(x1, x2), std::max(y1, y2),
                    true
                );
            } else {
                renderer->drawRectangle(
                    std::min(x1, x2), std::min(y1, y2),
                    std::max(x1, x2), std::max(y1, y2),
                    false
                );
            }
            break;
        }

        case DRAW_TRIANGLE: {  // NOWE
            // Trójkąt: punkt startowy + punkt przeciągnięcia + trzeci punkt obliczony
            float x3 = x1 + (x2 - x1) * 0.5f;
            float y3 = y1 - std::abs(y2 - y1) * 0.8f; // Trzeci punkt nad podstawą

            if (fill_primitives) {
                renderer->drawTriangle(x1, y2, x2, y2, x3, y3, true); // POPRAWIONE: użyj y3
            } else {
                renderer->drawTriangle(x1, y2, x2, y2, x3, y3, false); // POPRAWIONE: użyj y3
            }
            break;
        }

        case DRAW_NONE:
            break;
    }
}

    void drawSavedShapes(PrimitiveRenderer* renderer) {
        for (const auto& shape : drawn_shapes) {
            shape->draw(renderer, use_custom_circle_algorithm, use_custom_ellipse_algorithm, fill_primitives);
        }
    }

    virtual void update(float delta_time) override {
        accumulated_time += delta_time;

        // NOWE: Aktualizuj rozdzielczość gracza
        player.setScreenSize(get_screen_width(), get_screen_height());

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

        // === RYSOWANIE ZAPISANYCH KSZTAŁTÓW ===
        drawSavedShapes(&renderer);

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

        // === RYSOWANIE AKTUALNEGO KSZTAŁTU (PODGLĄD) ===
        drawCurrentShape(&renderer);

        // === TEST NOWYCH ALGORYTMÓW ===
        if (show_custom_primitives) {
            renderer.setColor(255, 0, 255); // Magenta dla własnych algorytmów

            // Okrąg - własny vs biblioteczny algorytm
            if (use_custom_circle_algorithm) {
                renderer.drawCircleBresenham(200, 600, 30, fill_primitives);
            } else {
                renderer.drawCircle(200, 600, 30, fill_primitives);
            }

            // Elipsa - własny vs biblioteczny algorytm
            if (use_custom_ellipse_algorithm) {
                renderer.drawEllipseMidpoint(300, 600, 40, 25, fill_primitives);
            } else {
                if (fill_primitives) {
                    al_draw_filled_ellipse(300, 600, 40, 25, al_map_rgb(255, 0, 255));
                } else {
                    al_draw_ellipse(300, 600, 40, 25, al_map_rgb(255, 0, 255), 2.0f);
                }
            }
        }

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

            // NOWE: Informacje o algorytmach okręgu i elipsy
            std::string circle_algo = use_custom_circle_algorithm ?
                "Okrag: BRESENHAM" : "Okrag: BIBLIOTEKA";
            al_draw_text(get_font(), al_map_rgb(255, 200, 100), 10, y_offset, 0,
                        circle_algo.c_str());
            y_offset += 20;

            std::string ellipse_algo = use_custom_ellipse_algorithm ?
                "Elipsa: MIDPOINT" : "Elipsa: BIBLIOTEKA";
            al_draw_text(get_font(), al_map_rgb(255, 200, 100), 10, y_offset, 0,
                        ellipse_algo.c_str());
            y_offset += 20;

            std::string custom_primitives_text = show_custom_primitives ?
                "Wlasne algorytmy: WLACZONE" : "Wlasne algorytmy: WYLACZONA";
            al_draw_text(get_font(), al_map_rgb(255, 200, 100), 10, y_offset, 0,
                        custom_primitives_text.c_str());
            y_offset += 20;

            // NOWE: Informacje o trybie rysowania
            std::string drawing_mode_text;
            switch (current_drawing_mode) {
                case DRAW_NONE: drawing_mode_text = "Rysowanie: BRAK"; break;
                case DRAW_CIRCLE: drawing_mode_text = "Rysowanie: OKRAG (1)"; break;
                case DRAW_ELLIPSE: drawing_mode_text = "Rysowanie: ELIPSA (2)"; break;
                case DRAW_RECTANGLE: drawing_mode_text = "Rysowanie: PROSTOKAT (3)"; break;
                case DRAW_TRIANGLE: drawing_mode_text = "Rysowanie: TROJKAT (4)"; break;  // NOWE
            }
            al_draw_text(get_font(), al_map_rgb(200, 255, 200), 10, y_offset, 0,
                        drawing_mode_text.c_str());
            y_offset += 20;

            std::string drawing_status = drawing_active ?
                "Status: RYSOWANIE AKTYWNE" : "Status: OCZEKIWANIE";
            al_draw_text(get_font(), al_map_rgb(200, 255, 200), 10, y_offset, 0,
                        drawing_status.c_str());
            y_offset += 30;

            // Informacja o trybie wyswietlania
            std::string display_text = (get_display_mode() == FULLSCREEN) ?
                "Tryb: PELNY EKRAN (F11)" : "Tryb: OKNO (F11)";
            al_draw_text(get_font(), al_map_rgb(255, 200, 100), 10, y_offset, 0,
                        display_text.c_str());
            y_offset += 20;

            // Sterowanie
            al_draw_text(get_font(), al_map_rgb(255, 200, 100), 10, y_offset, 0,
                        "STEROWANIE:");
            y_offset += 20;

            al_draw_text(get_font(), al_map_rgb(200, 200, 200), 10, y_offset, 0,
                        "F11 - Pelny ekran | SPACE - Algorytm | F - Wypelnienie");
            y_offset += 20;

            al_draw_text(get_font(), al_map_rgb(200, 200, 200), 10, y_offset, 0,
                        "T - Animacja ksztaltow | Y - Animacja sprite | P - Gracz");
            y_offset += 20;

            al_draw_text(get_font(), al_map_rgb(200, 200, 200), 10, y_offset, 0,
                        "B - Bitmapy | C - Czysc | R - Reset | ESC - Wyjscie");
            y_offset += 20;

            // NOWE: Sterowanie algorytmami
            al_draw_text(get_font(), al_map_rgb(200, 200, 200), 10, y_offset, 0,
                        "O - Algorytm okregu | E - Algorytm elipsy | U - Wl/wyl wlasne");
            y_offset += 20;

            // NOWE: Sterowanie rysowaniem
            al_draw_text(get_font(), al_map_rgb(200, 200, 200), 10, y_offset, 0,
                        "1 - Okrag | 2 - Elipsa | 3 - Prostokat | 4 - Trojkat");  // NOWE
            y_offset += 20;

            al_draw_text(get_font(), al_map_rgb(200, 200, 200), 10, y_offset, 0,
                        "LPM - Rozpocznij rysowanie | PPM - Anuluj | SPM - Zakoncz");
            y_offset += 20;

            al_draw_text(get_font(), al_map_rgb(200, 200, 200), 10, y_offset, 0,
                        "WASD - Ruch | Strzalki - Rotacja");
            y_offset += 30;

            // Statystyki
            std::string stats = "Punkty: " + std::to_string(points.size()) +
                               " | Odcinki: " + std::to_string(segments.size()) +
                               " | Ksztalty: " + std::to_string(shapes.size()) +
                               " | Narysowane: " + std::to_string(drawn_shapes.size()) +
                               " | FPS: " + std::to_string(current_fps) +
                               " | Rozdzielczosc: " + std::to_string(get_screen_width()) +
                               "x" + std::to_string(get_screen_height());
            al_draw_text(get_font(), al_map_rgb(100, 255, 100), 10, y_offset, 0,
                        stats.c_str());
        }
    }

    virtual void on_key_press(int key) override {
        if (key == ALLEGRO_KEY_ESCAPE) {
            stop();
        }
        else if (key == ALLEGRO_KEY_F11) {
            toggle_fullscreen();
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
        else if (key == ALLEGRO_KEY_T) {
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
            drawn_shapes.clear(); // NOWE: wyczyść też narysowane kształty
            // Zachowaj tylko podstawowe ksztalty
            shapes.erase(std::remove_if(shapes.begin(), shapes.end(),
                [](const std::shared_ptr<ShapeObject>& shape) {
                    return dynamic_cast<PointShape*>(shape.get()) != nullptr;
                }), shapes.end());
        }
        else if (key == ALLEGRO_KEY_Y) {
            animate_sprite = !animate_sprite;
        }
        // NOWE KLAWISZE do algorytmów
        else if (key == ALLEGRO_KEY_O) { // O - przełącz algorytm okręgu
            use_custom_circle_algorithm = !use_custom_circle_algorithm;
        }
        else if (key == ALLEGRO_KEY_E) { // E - przełącz algorytm elipsy
            use_custom_ellipse_algorithm = !use_custom_ellipse_algorithm;
        }
        else if (key == ALLEGRO_KEY_U) { // U - pokaż/ukryj własne prymitywy
            show_custom_primitives = !show_custom_primitives;
        }
        // NOWE KLAWISZE do rysowania kształtów
        else if (key == ALLEGRO_KEY_1) { // 1 - okrąg
            if (current_drawing_mode == DRAW_CIRCLE) {
                current_drawing_mode = DRAW_NONE;
            } else {
                current_drawing_mode = DRAW_CIRCLE;
            }
        }
        else if (key == ALLEGRO_KEY_2) { // 2 - elipsa
            if (current_drawing_mode == DRAW_ELLIPSE) {
                current_drawing_mode = DRAW_NONE;
            } else {
                current_drawing_mode = DRAW_ELLIPSE;
            }
        }
        else if (key == ALLEGRO_KEY_3) { // 3 - prostokąt
            if (current_drawing_mode == DRAW_RECTANGLE) {
                current_drawing_mode = DRAW_NONE;
            } else {
                current_drawing_mode = DRAW_RECTANGLE;
            }
        }
        else if (key == ALLEGRO_KEY_4) { // 4 - trójkąt  // NOWE
            if (current_drawing_mode == DRAW_TRIANGLE) {
                current_drawing_mode = DRAW_NONE;
            } else {
                current_drawing_mode = DRAW_TRIANGLE;
            }
        }
    }

    virtual void on_key_release(int key) override {
        // Mozesz dodac obsluge zwolnienia klawiszy jesli potrzebna
    }

    virtual void on_mouse_click(int button, int x, int y) override {
        if (button == 1) { // Lewy przycisk
            if (current_drawing_mode == DRAW_NONE) {
                // Standardowe dodawanie punktów
                points.push_back(Point2D(x, y));

                if (points.size() >= 2) {
                    segments.push_back(LineSegment(
                        points[points.size()-2],
                        points[points.size()-1]
                    ));
                }

                auto newPoint = std::make_shared<PointShape>(x, y, 8.0f);
                shapes.push_back(newPoint);
            } else {
                if (!drawing_active) {
                    // Rozpocznij rysowanie kształtu
                    startDrawing(current_drawing_mode, x, y);
                } else {
                    // Zakończ rysowanie (puszczenie LPM)
                    finishDrawing();
                }
            }
        }
        else if (button == 2) { // Prawy przycisk
            if (drawing_active) {
                // Anuluj rysowanie
                drawing_active = false;
                current_drawing_mode = DRAW_NONE;
            } else {
                // Wyczyść punkty
                points.clear();
                segments.clear();
                drawn_shapes.clear();
                shapes.erase(std::remove_if(shapes.begin(), shapes.end(),
                    [](const std::shared_ptr<ShapeObject>& shape) {
                        return dynamic_cast<PointShape*>(shape.get()) != nullptr;
                    }), shapes.end());
            }
        }
        else if (button == 3) { // Środkowy przycisk - zakończ rysowanie
            finishDrawing();
        }
    }

    virtual void on_mouse_move(int x, int y) override {
        // Aktualizuj pozycję rysowania
        updateDrawing(x, y);
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
        use_custom_circle_algorithm = false;
        use_custom_ellipse_algorithm = false;
        show_custom_primitives = true;
        current_drawing_mode = DRAW_NONE;
        drawing_active = false;

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
