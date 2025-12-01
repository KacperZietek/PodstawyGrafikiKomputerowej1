#include "Engine.h"

int main() {
    // Teraz podajemy tylko tytu³ okna
    if (Engine::getInstance().init("Projekt GK - Final Demo")) {
        Engine::getInstance().run();
    }

    return 0;
}
