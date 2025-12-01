#include "Engine.h"

int main() {
    if (Engine::getInstance().init("Projekt GK - Final Demo")) {
        Engine::getInstance().run();
    }

    return 0;
}
