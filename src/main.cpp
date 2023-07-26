#include "Window.h"
#include "loadShader/LoadShaders.h"
#include <iostream>

int main() {
    try {
        Window window;
        window.run();
    } catch (const std::exception &e) {
        printf("Exception: %s\n", e.what());
    }
    return 0;
}
