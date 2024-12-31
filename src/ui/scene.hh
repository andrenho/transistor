#ifndef SCENE_HH
#define SCENE_HH

#include <optional>
#include <queue>

#include "layers/board/circuit_atlas.hh"
#include "resources/resource.hh"

struct Pen {
    bool semitransparent = false;
};

class Scene {
public:
    struct Image {
        std::variant<Resource, CSprite> image;
        int      x, y;
        Pen      pen;
    };

    void add(std::variant<Resource, CSprite> image, int x, int y, Pen const& pen={}) {
        images_.emplace(image, x, y, pen);
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
