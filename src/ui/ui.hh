#ifndef UI_UI_HH_
#define UI_UI_HH_

#include <chrono>
#include <memory>
#include <tuple>
#include <vector>

#include "uiinterface.hh"
#include "ui_layer.hh"
#include "engine/sandbox/sandbox.hh"
#include "resources/resourcemanager.hh"

using hr = std::chrono::high_resolution_clock;
using Duration = decltype(hr::now() - hr::now());

class UI : public UI_Interface {
public:
    UI();
    ~UI();
    UI (const UI&) = delete;
    UI& operator=(const UI&) = delete;

    void set_sandbox(Sandbox& sandbox);

    void draw_image(UILayer const* layer, Resource const& res, int x, int y, DrawProperties const& dp) const override;

    void update([[maybe_unused]] Duration timestep);
    void render();

    [[nodiscard]] bool running() const { return running_; }

private:
    void init_imgui();
    std::tuple<class UILayer*, int, int> find_layer(int x, int y) const;

    bool running_ = true;

    struct SDL_Window*   window_;
    struct SDL_Renderer* ren_;

    ResourceManager resource_manager_;
    Resource        bg_;

    std::vector<std::unique_ptr<UILayer>> layers;
};

#endif
