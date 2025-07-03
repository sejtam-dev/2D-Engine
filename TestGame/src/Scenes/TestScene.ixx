export module Game.Scenes:TestScene;

import Engine.Objects;
import Game.Components;

export class TestScene : public Scene {

    void Initialize() override {

        const auto testRenderer = GameObject::Create();
        testRenderer->AddComponent<TestComponent>();

    }

};