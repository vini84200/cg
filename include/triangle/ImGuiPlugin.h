//
// Created by vini84200 on 6/1/23.
//

#ifndef TRIANGLE_IMGUIPLUGIN_H
#define TRIANGLE_IMGUIPLUGIN_H

#include "imgui.h"

class Window;

class ImGuiPlugin {



public:
    ImGuiPlugin();

    void render();

    void init();
    void shutdown();

    void setWindow(Window* window);

    bool demo_;
    Window* window_;
};


#endif //TRIANGLE_IMGUIPLUGIN_H
