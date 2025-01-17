#ifndef SCENE_HH
#define SCENE_HH

#include <optional>
#include <queue>

#include "engine/geometry/direction.hh"
#include "res/resourcemanager.hh"

struct Pen {
    bool semitransparent = false;
    Direction rotation = Direction::N;
};

class Scene {
public:
    struct Image {
        ResourceId  resource;
        int         x, y;
        Pen         pen;
    };

    void add(ResourceId const& resource, int x, int y, Pen const& pen={}) {
        images_.emplace(resource, x, y, pen);
    }

    std::optional<Image> next_image() {
        if (!images_.empty()) {
            Image image = std::move(images_.front());
            images_.pop();
            return image;
        }
        return {};
    }

private:
    std::queue<Image> images_;
};

#endif //SCENE_HH
