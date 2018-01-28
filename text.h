#ifndef __TEXT_H__
#define __TEXT_H__

#include <stdint.h>

extern const uint64_t Text__font_width;
extern const uint64_t Text__font_height;

void Text__generate_textures();

int Text__use_texture(char c);

#endif

/* vim: ts=4 sts=4 sw=4
 */
