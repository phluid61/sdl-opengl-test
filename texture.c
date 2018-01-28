#include "texture.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <unistd.h>
#include <fcntl.h>

static GLuint textures[__NUM_TEXTURES__];

static uint8_t* load_texture(const char *fn, uint64_t nbytes) {
	uint8_t *pixels, *ptr;
	int fd;
	ssize_t this_read, total = 0;

	pixels = malloc(sizeof(uint8_t) * nbytes);
	ptr = pixels;
	fd = open(fn, O_RDONLY);
	while (total < nbytes) {
		this_read = read(fd, (void*)ptr, (size_t)(nbytes - total));
		if (this_read < (uint8_t)1) {
			/* TODO */
			fprintf(stderr, "something went wrong reading texture '%s' [%ld]\n", fn, this_read);
			break;
		} else {
			total += this_read;
			ptr = (uint8_t*)((uint64_t)ptr + (uint64_t)this_read);
		}
	}
	close(fd);

	return pixels;
}

void Texture__generate_textures() {
	uint8_t *pixels;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(__NUM_TEXTURES__, textures);

	pixels = load_texture("textures/box-64x64-RGBA.data", (uint64_t)(64 * 64 * 4));
	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_BOX]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	free(pixels);

	pixels = load_texture("textures/steelbox-64x64-RGBA.data", (uint64_t)(64 * 64 * 4));
	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_STEELBOX]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	free(pixels);

	pixels = load_texture("textures/cardboard-64x64-RGBA.data", (uint64_t)(64 * 64 * 4));
	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_CARDBOARDBOX]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	free(pixels);
}

int Texture__use_texture(Texture_t texture) {
	if (texture >= 0 && texture < __NUM_TEXTURES__) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[texture]);
		return 0;
	}
	return -1;
}

/* vim: ts=4 sts=4 sw=4
 */
