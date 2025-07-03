module;
#include "Utils.h"

export module Game;

import Engine;

export class Game : public Engine {
public:
    Game() {
        window->title = "TestGame";
    }

protected:
    void OnInit() override {
        window->ChangeSizeEvent(ChangeSize);
    }

    void OnLoadContent() override {
    }

    void OnUnloadContent() override {
    }

    void OnUpdate() override {

    }

    void OnDraw() override {

    }

private:
    static void ChangeSize(const Window* window, const int width, const int height) {
        DEBUG_LOG("Size changed: {}x{}", width, height);
    }
};
