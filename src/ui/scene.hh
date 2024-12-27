#ifndef SCENE_HH
#define SCENE_HH

#include <deque>

#include "resources/resource.hh"

struct Pen {
    bool semitransparent = false;
};

class Scene {
public:
    class ImageContext {
    public:
        virtual ~ImageContext() = default;
        virtual int x() const = 0;
        virtual int y() const = 0;
        virtual float zoom() const = 0;
    };

    struct Image {
        ImageContext const* context;
        Resource            resource;
        int                 x, y;
        Pen const&          pen;
    };

    void add(ImageContext const* context, Resource resource, int x, int y, Pen pen={}) {
        images_.emplace_back(context, resource, x, y, pen);
    }

    [[nodiscard]] std::deque<Image> const& images() const { return images_; }

    Resource bg;

private:
    std::deque<Image> images_;
};

#endif //SCENE_HH
