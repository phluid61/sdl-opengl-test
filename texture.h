#ifndef __TEXTURE_H__
#define __TEXTURE_H__

typedef enum Texture {
	TEXTURE_BOX = 0,
	TEXTURE_STEELBOX,
	TEXTURE_CARDBOARDBOX,

	__NUM_TEXTURES__
} Texture_t;

void Texture__generate_textures();

int Texture__use_texture(Texture_t texture);

#endif

/* vim: ts=4 sts=4 sw=4
 */
