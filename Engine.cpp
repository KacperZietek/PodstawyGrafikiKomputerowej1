#include "Engine.h"
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <string>

// Konstruktor
Engine::Engine() {}

// Destruktor
Engine::~Engine() {
    cleanup();
}

// Singleton
Engine& Engine::getInstance() {
    static Engine instance;
    return instance;
}

// Metoda pomocnicza do kolizji (AABB)
bool Engine::checkCollision(float x1, float y1, float w1, float h1,
                            float x2, float y2, float w2, float h2) {
    return x1 < x2 + w2 &&
           x1 + w1 > x2 &&
           y1 < y2 + h2 &&
           y1 + h1 > y2;
}

// Inicjalizacja
bool Engine::init(const char* title) {
    if (!al_init()) return false;
    if (!al_install_keyboard()) return false;
    if (!al_install_mouse()) return false;
    if (!al_init_primitives_addon()) return false;
    if (!al_init_image_addon()) return false;

    // Fonty
    al_init_font_addon();
    al_init_ttf_addon();

    timer = al_create_timer(1.0 / FPS);

    // Okno
    al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
    display = al_create_display(LOGICAL_WIDTH, LOGICAL_HEIGHT);
    if (!display) return false;
    al_set_window_title(display, title);

    // Bufor techniczny
    buffer = al_create_bitmap(LOGICAL_WIDTH, LOGICAL_HEIGHT);
    if (!buffer) return false;

    font = al_create_builtin_font();

    eventQueue = al_create_event_queue();
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_mouse_event_source());

    // Narzędzia
    renderer = new PrimitiveRenderer();
    bitmapHandler = new BitmapHandler();

    // --- OBIEKTY DEMO ---

    // 1. Gracz (środek)
    player = new Player(bitmapHandler, 400, 300);

    // 2. Trójkąt (lewy dół)
    float tx = 200, ty = 400;
    rotatingTriangle = new ShapeObject(tx, ty, al_map_rgb(0, 255, 0));
    rotatingTriangle->addVertex(tx + 0,   ty - 50);
    rotatingTriangle->addVertex(tx - 50,  ty + 50);
    rotatingTriangle->addVertex(tx + 50,  ty + 50);

    // 3. Kwadrat (prawy dół)
    float rx = 600, ry = 400;
    pulsingRect = new ShapeObject(rx, ry, al_map_rgb(255, 255, 0));
    pulsingRect->addVertex(rx - 40, ry - 40);
    pulsingRect->addVertex(rx + 40, ry - 40);
    pulsingRect->addVertex(rx + 40, ry + 40);
    pulsingRect->addVertex(rx - 40, ry + 40);

    // 4. Gwiazda (prawy górny róg)
    float sx = 650, sy = 150;
    star = new ShapeObject(sx, sy, al_map_rgb(255, 215, 0), false);
    star->addVertex(sx, sy - 40); star->addVertex(sx + 10, sy - 10);
    star->addVertex(sx + 40, sy - 10); star->addVertex(sx + 15, sy + 10);
    star->addVertex(sx + 25, sy + 40); star->addVertex(sx, sy + 20);
    star->addVertex(sx - 25, sy + 40); star->addVertex(sx - 15, sy + 10);
    star->addVertex(sx - 40, sy - 10); star->addVertex(sx - 10, sy - 10);

    updateScaleFactors();
    al_start_timer(timer);
    return true;
}

void Engine::run() {
    isRunning = true;
    lastFpsTime = al_get_time(); // Init czasu dla FPS

    while (isRunning) {
        processEvents();
    }
}

void Engine::updateScaleFactors() {
    if (!display) return;
    int windowWidth = al_get_display_width(display);
    int windowHeight = al_get_display_height(display);

    float scaleX = (float)windowWidth / LOGICAL_WIDTH;
    float scaleY = (float)windowHeight / LOGICAL_HEIGHT;
    scale = std::min(scaleX, scaleY);

    offsetX = (windowWidth - LOGICAL_WIDTH * scale) / 2.0f;
    offsetY = (windowHeight - LOGICAL_HEIGHT * scale) / 2.0f;
}

// Obsługa Zdarzeń
void Engine::processEvents() {
    ALLEGRO_EVENT ev;
    al_wait_for_event(eventQueue, &ev);

    if (ev.type == ALLEGRO_EVENT_TIMER) {
        update();
        render();
    }
    else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        isRunning = false;
    }
    else if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
        al_acknowledge_resize(display);
        updateScaleFactors();
    }
    // --- KLAWIATURA ---
    else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
        if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) isRunning = false;
        if (ev.keyboard.keycode == ALLEGRO_KEY_F) toggleFullscreen();

        // Wybór narzędzi
        if (ev.keyboard.keycode == ALLEGRO_KEY_1) { currentMode = MODE_RECT; std::cout << "Tryb: Prostokat" << std::endl; }
        if (ev.keyboard.keycode == ALLEGRO_KEY_2) { currentMode = MODE_CIRCLE; std::cout << "Tryb: Kolo" << std::endl; }
        if (ev.keyboard.keycode == ALLEGRO_KEY_3) { currentMode = MODE_TRIANGLE; std::cout << "Tryb: Trojkat" << std::endl; }
        if (ev.keyboard.keycode == ALLEGRO_KEY_4) { currentMode = MODE_ELLIPSE; std::cout << "Tryb: Elipsa" << std::endl; }
        if (ev.keyboard.keycode == ALLEGRO_KEY_5) { currentMode = MODE_LINE; std::cout << "Tryb: Linia" << std::endl; }

        if (ev.keyboard.keycode == ALLEGRO_KEY_0) { currentMode = MODE_NONE; std::cout << "Tryb: Brak" << std::endl; }

        // Reset
        if (ev.keyboard.keycode == ALLEGRO_KEY_R) {
            for (auto* s : drawnShapes) delete s;
            drawnShapes.clear();
            std::cout << "Zresetowano ksztalty!" << std::endl;
        }
    }
    // --- MYSZ ---
    else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
        currentMousePos.setX((ev.mouse.x - offsetX) / scale);
        currentMousePos.setY((ev.mouse.y - offsetY) / scale);
    }
    else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        float gx = (ev.mouse.x - offsetX) / scale;
        float gy = (ev.mouse.y - offsetY) / scale;

        if (gx >= 0 && gx <= LOGICAL_WIDTH && gy >= 0 && gy <= LOGICAL_HEIGHT) {
            // LEWY PRZYCISK: Rysowanie
            if (ev.mouse.button == 1) {
                if (currentMode != MODE_NONE) {
                    if (!isDrawing) {
                        startDragPos.setX(gx);
                        startDragPos.setY(gy);
                        isDrawing = true;
                    } else {
                        createShapeFromInput();
                        isDrawing = false;
                    }
                }
            }
            // PRAWY PRZYCISK: Kolorowanie (STOP-KLATKA)
            else if (ev.mouse.button == 2) {
                al_set_target_bitmap(buffer);
                al_clear_to_color(al_map_rgb(50, 50, 50));

                ALLEGRO_TRANSFORM identity;
                al_identity_transform(&identity);
                al_use_transform(&identity);

                renderer->drawCircle(150, 100, 40, al_map_rgb(255, 255, 255));
                renderer->drawEllipse(650, 100, 60, 30, al_map_rgb(100, 100, 255));
                renderer->drawLineIncremental(Point2D(190, 100), Point2D(590, 100), al_map_rgb(100, 100, 100));

                if (rotatingTriangle) rotatingTriangle->draw(renderer);
                if (pulsingRect) pulsingRect->draw(renderer);
                if (star) star->draw(renderer);
                for (auto* s : drawnShapes) s->draw(renderer);
                if (player) player->draw(renderer);

                renderer->floodFill(gx, gy, al_map_rgb(255, 0, 0));

                if (font) {
                    al_identity_transform(&identity);
                    al_use_transform(&identity);
                    // Podczas pauzy też pokazujemy FPS
                    al_draw_textf(font, al_map_rgb(0, 255, 0), 780, 10, ALLEGRO_ALIGN_RIGHT, "FPS: %d", displayedFPS);
                    // ... (reszta UI)
                }

                al_set_target_backbuffer(display);
                al_clear_to_color(al_map_rgb(0, 0, 0));

                ALLEGRO_TRANSFORM trans;
                al_identity_transform(&trans);
                al_scale_transform(&trans, scale, scale);
                al_translate_transform(&trans, offsetX, offsetY);
                al_use_transform(&trans);

                al_draw_bitmap(buffer, 0, 0, 0);
                al_flip_display();

                al_rest(0.5);
            }
        }
    }
}

// Logika tworzenia kształtów
void Engine::createShapeFromInput() {
    float x1 = startDragPos.getX();
    float y1 = startDragPos.getY();
    float x2 = currentMousePos.getX();
    float y2 = currentMousePos.getY();
    ALLEGRO_COLOR col = al_map_rgb(0, 255, 255);

    ShapeObject* newShape = nullptr;

    if (currentMode == MODE_RECT) {
        newShape = new ShapeObject(0, 0, col);
        newShape->addVertex(x1, y1); newShape->addVertex(x2, y1);
        newShape->addVertex(x2, y2); newShape->addVertex(x1, y2);
    }
    else if (currentMode == MODE_TRIANGLE) {
        newShape = new ShapeObject(0, 0, col);
        newShape->addVertex(x1, y1); newShape->addVertex(x2, y2);
        newShape->addVertex(x1 - (x2-x1), y2);
    }
    else if (currentMode == MODE_CIRCLE) {
        float r = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
        newShape = ShapeObject::createCircle(x1, y1, r, col);
    }
    else if (currentMode == MODE_ELLIPSE) {
        float rx = std::abs(x2 - x1);
        float ry = std::abs(y2 - y1);
        newShape = ShapeObject::createEllipse(x1, y1, rx, ry, col);
    }
    else if (currentMode == MODE_LINE) {
        newShape = ShapeObject::createLine(x1, y1, x2, y2, col);
    }

    if (newShape) drawnShapes.push_back(newShape);
}

// Podgląd (Duch)
void Engine::renderPreviewShape() {
    float x1 = startDragPos.getX();
    float y1 = startDragPos.getY();
    float x2 = currentMousePos.getX();
    float y2 = currentMousePos.getY();
    ALLEGRO_COLOR col = al_map_rgba(0, 255, 255, 100);

    if (currentMode == MODE_RECT) al_draw_rectangle(x1, y1, x2, y2, col, 1.0f);
    else if (currentMode == MODE_CIRCLE) {
        float r = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
        al_draw_circle(x1, y1, r, col, 1.0f);
    }
    else if (currentMode == MODE_ELLIPSE) {
         float rx = std::abs(x2 - x1); float ry = std::abs(y2 - y1);
         al_draw_ellipse(x1, y1, rx, ry, col, 1.0f);
    }
    else if (currentMode == MODE_TRIANGLE) {
        al_draw_triangle(x1, y1, x2, y2, x1 - (x2-x1), y2, col, 1.0f);
    }
    else if (currentMode == MODE_LINE) {
        al_draw_line(x1, y1, x2, y2, col, 1.0f);
    }
}

void Engine::update() {
    float deltaTime = 1.0f / FPS;
    globalTime += deltaTime;

    if (player) player->update(deltaTime);
    if (rotatingTriangle) rotatingTriangle->rotate(0.05f, 200, 400);
    if (star) star->rotate(-0.02f, 650, 150);

    if (pulsingRect) {
        float scaleSpeed = 1.01f;
        if ((int)(globalTime * 2) % 2 == 0) pulsingRect->scale(scaleSpeed, 600, 400);
        else pulsingRect->scale(1.0f/scaleSpeed, 600, 400);

        ALLEGRO_MOUSE_STATE ms;
        al_get_mouse_state(&ms);
        float mx = (ms.x - offsetX) / scale;
        float my = (ms.y - offsetY) / scale;

        if (checkCollision(mx, my, 1, 1, 560, 360, 80, 80)) pulsingRect->setColor(al_map_rgb(255, 0, 0));
        else pulsingRect->setColor(al_map_rgb(255, 255, 0));
    }
}

void Engine::render() {
    // --- OBLICZANIE FPS ---
    frameCount++;
    double currentTime = al_get_time();
    if (currentTime - lastFpsTime >= 1.0) {
        displayedFPS = frameCount;
        frameCount = 0;
        lastFpsTime = currentTime;
    }
    // ----------------------

    // 1. Rysowanie na buforze
    al_set_target_bitmap(buffer);
    al_clear_to_color(al_map_rgb(50, 50, 50));

    ALLEGRO_TRANSFORM identity;
    al_identity_transform(&identity);
    al_use_transform(&identity);

    // Tło
    renderer->drawCircle(150, 100, 40, al_map_rgb(255, 255, 255));
    renderer->drawEllipse(650, 100, 60, 30, al_map_rgb(100, 100, 255));
    renderer->drawLineIncremental(Point2D(190, 100), Point2D(590, 100), al_map_rgb(100, 100, 100));

    // Obiekty
    if (rotatingTriangle) rotatingTriangle->draw(renderer);
    if (pulsingRect) pulsingRect->draw(renderer);
    if (star) star->draw(renderer);
    for (auto* s : drawnShapes) s->draw(renderer);
    if (player) player->draw(renderer);
    if (isDrawing) renderPreviewShape();

    // UI
    if (font) {
        al_identity_transform(&identity);
        al_use_transform(&identity);

        al_draw_text(font, al_map_rgb(255, 255, 255), 10, 10, 0, "Silnik 2D - FINAL DEMO");
        al_draw_text(font, al_map_rgb(200, 200, 200), 10, 25, 0, "1-5: Rysuj | PPM: Wypelnij | R: Reset");
        al_draw_text(font, al_map_rgb(200, 200, 200), 10, 40, 0, "LPM: Rysuj | F: Fullscreen");

        std::string modeText = "Tryb: ";
        if (currentMode == MODE_NONE) modeText += "GRA";
        else if (currentMode == MODE_RECT) modeText += "Rysuj PROSTOKAT";
        else if (currentMode == MODE_CIRCLE) modeText += "Rysuj KOLO";
        else if (currentMode == MODE_TRIANGLE) modeText += "Rysuj TROJKAT";
        else if (currentMode == MODE_ELLIPSE) modeText += "Rysuj ELIPSE";
        else if (currentMode == MODE_LINE) modeText += "Rysuj LINIE";

        al_draw_text(font, al_map_rgb(0, 255, 255), 10, 55, 0, modeText.c_str());

        // --- WYŚWIETLANIE FPS ---
        al_draw_textf(font, al_map_rgb(0, 255, 0), 780, 10, ALLEGRO_ALIGN_RIGHT, "FPS: %d", displayedFPS);
        // -----------------------

        ALLEGRO_MOUSE_STATE ms;
        al_get_mouse_state(&ms);
        float mx = (ms.x - offsetX) / scale;
        float my = (ms.y - offsetY) / scale;
        renderer->drawPoint(mx, my, al_map_rgb(255, 0, 0));
    }

    // 2. Skalowanie na ekran
    al_set_target_backbuffer(display);
    al_clear_to_color(al_map_rgb(0, 0, 0));

    ALLEGRO_TRANSFORM trans;
    al_identity_transform(&trans);
    al_scale_transform(&trans, scale, scale);
    al_translate_transform(&trans, offsetX, offsetY);
    al_use_transform(&trans);

    al_draw_bitmap(buffer, 0, 0, 0);
    al_flip_display();
}

void Engine::toggleFullscreen() {
    isFullscreen = !isFullscreen;
    al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, isFullscreen);
    updateScaleFactors();
}

void Engine::cleanup() {
    if (player) delete player;
    if (rotatingTriangle) delete rotatingTriangle;
    if (pulsingRect) delete pulsingRect;
    if (star) delete star;
    for (auto* s : drawnShapes) delete s;
    drawnShapes.clear();

    if (renderer) delete renderer;
    if (bitmapHandler) delete bitmapHandler;

    if (font) al_destroy_font(font);
    if (buffer) al_destroy_bitmap(buffer);

    if (eventQueue) al_destroy_event_queue(eventQueue);
    if (timer) al_destroy_timer(timer);
    if (display) al_destroy_display(display);
}
