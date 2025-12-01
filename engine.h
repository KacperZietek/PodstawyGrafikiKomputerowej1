#ifndef ENGINE_H
#define ENGINE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <vector>
#include "PrimitiveRenderer.h"
#include "BitmapHandler.h"
#include "Player.h"
#include "ShapeObject.h"

// Tryby rysowania
enum EditorMode {
    MODE_NONE,
    MODE_RECT,
    MODE_CIRCLE,
    MODE_TRIANGLE,
    MODE_ELLIPSE,
    MODE_LINE
};

class Engine {
private:
    Engine();

    ALLEGRO_DISPLAY* display = nullptr;
    ALLEGRO_EVENT_QUEUE* eventQueue = nullptr;
    ALLEGRO_TIMER* timer = nullptr;
    ALLEGRO_FONT* font = nullptr;

    bool isRunning = false;
    const float FPS = 144.0f;

    const int LOGICAL_WIDTH = 800;
    const int LOGICAL_HEIGHT = 600;

    float scale = 1.0f;
    float offsetX = 0.0f;
    float offsetY = 0.0f;

    ALLEGRO_BITMAP* buffer = nullptr;

    PrimitiveRenderer* renderer = nullptr;
    BitmapHandler* bitmapHandler = nullptr;

    // Obiekty
    Player* player = nullptr;
    ShapeObject* rotatingTriangle = nullptr;
    ShapeObject* pulsingRect = nullptr;
    ShapeObject* star = nullptr;

    // Edytor
    std::vector<ShapeObject*> drawnShapes;
    EditorMode currentMode = MODE_NONE;
    bool isDrawing = false;
    Point2D startDragPos;
    Point2D currentMousePos;

    float globalTime = 0.0f;
    bool isFullscreen = false;

    // --- NOWOŚĆ: Zmienne do licznika FPS ---
    double lastFpsTime = 0.0; // Kiedy ostatnio liczyliśmy
    int frameCount = 0;       // Ile klatek w tej sekundzie
    int displayedFPS = 0;     // Wartość do wyświetlenia
    // ----------------------------------------

    void processEvents();
    void update();
    void render();
    void toggleFullscreen();
    void updateScaleFactors();

    bool checkCollision(float x1, float y1, float w1, float h1,
                        float x2, float y2, float w2, float h2);

    void createShapeFromInput();
    void renderPreviewShape();

public:
    Engine(const Engine&) = delete;
    void operator=(const Engine&) = delete;
    ~Engine();

    static Engine& getInstance();

    bool init(const char* title);
    void run();
    void cleanup();

    PrimitiveRenderer* getRenderer() { return renderer; }
    BitmapHandler* getBitmapHandler() { return bitmapHandler; }

    int getLogicalWidth() const { return LOGICAL_WIDTH; }
    int getLogicalHeight() const { return LOGICAL_HEIGHT; }
};

#endif
