#include "resources.hh"

#include "images/bg.jpg.h"

ps::res::idx_t
    rs_bg;

void load_resources()
{
    rs_bg = ps::res::add_image(ui_resources_images_bg_jpg, ui_resources_images_bg_jpg_sz);
}
