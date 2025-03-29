#define PL_LOG_IMPLEMENTATION
#include <pl_log.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-field-initializers"
#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>
#pragma clang diagnostic pop

#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define DMON_IMPL
#include <dmon.h>