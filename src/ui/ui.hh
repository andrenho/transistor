#ifndef UI_UI_HH_
#define UI_UI_HH_

#include <chrono>
#include <memory>
#include <optional>
#include <tuple>
#include <vector>

#include "scene.hh"
#include "uistate.hh"
#include "ui_commands.hh"
#include "SDL2/SDL.h"

#include "devices/deviceeditor.hh"
#include "gui/gui.hh"
#include "res/resourcemanager.hh"

using hr = std::chrono::high_resolution_clock;
using Duration = decltype(hr::now() - hr::now());

class UI {
public:
    ~UI();

    UI (const UI&) = delete;
    UI& operator=(const UI&) = delete;

    void operator<<(U::Command&& command) const;

    void update(Duration timestep);
    void render() const;

    [[nodiscard]] bool running() const { return running_; }

    [[nodiscard]] SDL_Renderer* ren() const { return ren_; }

private:
    UI();

    void recreate_devices();
    void execute_queue();

    void draw_image(Scene::Image const& image, DeviceEditor const* layer) const;

    void                                             drag_device(DeviceEditor* layer, int xrel, int yrel);
    std::tuple<DeviceEditor*, int, int>              find_device(int x, int y) const;
    std::vector<std::tuple<DeviceEditor*, int, int>> find_all_devices(int x, int y) const;

    bool running_ = true;

    SDL_Window*   window_;
    SDL_Renderer* ren_;
    SDL_Cursor    *move_cursor_, *delete_cursor_; // TODO

    GUI             gui_;
    UIState         state_;

    resource_idx_t  bg_;

    std::vector<std::unique_ptr<DeviceEditor>> device_editors_;
    std::optional<DeviceEditor*> dragging_;
    mutable std::queue<U::Command> commands_;

    mutable Duration frame_time_ = std::chrono::milliseconds(0);
    mutable size_t   frame_count_ = 0;
    size_t           total_frames_ = 0;

    friend UI const& ui();
};

UI const& ui();
void      ui_update(Duration timestep);

#endif
