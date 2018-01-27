#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>
#include <stdint.h>

#include <time.h>
#include <sys/time.h>

#include "engine.h"
#include "entity.h"
#include "text.h"
#include "timing.h"

#define SCREEN_WIDTH   640
#define SCREEN_HEIGHT  480

static int scene_width;
static int scene_height;
static float scene_ratio;

static GLubyte black[]         = {0x00, 0x00, 0x00, 0xFF};
static GLubyte blue[]          = {0x00, 0x00, 0xAA, 0xFF};
static GLubyte green[]         = {0x00, 0xAA, 0x00, 0xFF};
static GLubyte cyan[]          = {0x00, 0xAA, 0xAA, 0xFF};
static GLubyte red[]           = {0xAA, 0x00, 0x00, 0xFF};
static GLubyte magenta[]       = {0xAA, 0x00, 0xAA, 0xFF};
static GLubyte brown[]         = {0xAA, 0x55, 0x00, 0xFF};
static GLubyte light_gray[]    = {0xAA, 0xAA, 0xAA, 0xFF};

static GLubyte gray[]          = {0x55, 0x55, 0x55, 0xFF};
static GLubyte light_blue[]    = {0x55, 0x55, 0xFF, 0xFF};
static GLubyte light_green[]   = {0x55, 0xFF, 0x55, 0xFF};
static GLubyte light_cyan[]    = {0x55, 0xFF, 0xFF, 0xFF};
static GLubyte light_red[]     = {0xFF, 0x55, 0x55, 0xFF};
static GLubyte light_magenta[] = {0xFF, 0x55, 0xFF, 0xFF};
static GLubyte yellow[]        = {0xFF, 0xFF, 0x55, 0xFF};
static GLubyte white[]         = {0xFF, 0xFF, 0xFF, 0xFF};

static GLfloat light_ambient[] = {0.01f, 0.01f, 0.02f, 1.0f};
static GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
static GLfloat light_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};

/* vertices of a 2-unit box around its origin */
static GLfloat v0[] = {-1.0f, -1.0f,  1.0f};
static GLfloat v1[] = { 1.0f, -1.0f,  1.0f};
static GLfloat v2[] = { 1.0f,  1.0f,  1.0f};
static GLfloat v3[] = {-1.0f,  1.0f,  1.0f};
static GLfloat v4[] = {-1.0f, -1.0f, -1.0f};
static GLfloat v5[] = { 1.0f, -1.0f, -1.0f};
static GLfloat v6[] = { 1.0f,  1.0f, -1.0f};
static GLfloat v7[] = {-1.0f,  1.0f, -1.0f};

/* light position */
static GLfloat l0[] = { 1.0f,  1.0f,  2.0f,  0.0f};

static Entity_t cube0;
static Entity_t cube1;
static Entity_t cube2;

static void shutdown(int status) {
	SDL_Quit();
	exit(status);
}

static void on_keydown(SDL_Keysym *keysym) {
	switch (keysym->sym) {
	case SDLK_ESCAPE:
		shutdown(0);
		break;
	default:
		break;
	}
}

static void handle_events() {
	SDL_Event event;
	SDL_KeyboardEvent *keyboard_event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			keyboard_event = (SDL_KeyboardEvent*)&(event);
			on_keydown(&(keyboard_event->keysym));
			break;
		case SDL_QUIT:
			shutdown(0);
			break;
		}
	}
}

static void paint_box(Entity_t box) {
	glPushMatrix();
	glTranslatef(box.pos.x, box.pos.y, box.pos.z);
	glRotatef(box.rot.x, 1.0, 0.0, 0.0);
	glRotatef(box.rot.y, 0.0, 1.0, 0.0);
	glRotatef(box.rot.z, 0.0, 0.0, 1.0);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glBegin(GL_TRIANGLES);

	glColor4ubv(red);
	glNormal3f( 0.0,  0.0,  1.0);

	glVertex3fv(v0);
	glVertex3fv(v1);
	glVertex3fv(v2);

	glVertex3fv(v0);
	glVertex3fv(v2);
	glVertex3fv(v3);

	glColor4ubv(magenta);
	glNormal3f( 1.0,  0.0,  0.0);

	glVertex3fv(v1);
	glVertex3fv(v5);
	glVertex3fv(v6);

	glVertex3fv(v1);
	glVertex3fv(v6);
	glVertex3fv(v2);

	glColor4ubv(brown);
	glNormal3f( 0.0,  0.0, -1.0);

	glVertex3fv(v5);
	glVertex3fv(v4);
	glVertex3fv(v7);

	glVertex3fv(v5);
	glVertex3fv(v7);
	glVertex3fv(v6);

	glColor4ubv(cyan);
	glNormal3f(-1.0,  0.0,  0.0);

	glVertex3fv(v4);
	glVertex3fv(v0);
	glVertex3fv(v3);

	glVertex3fv(v4);
	glVertex3fv(v3);
	glVertex3fv(v7);

	glColor4ubv(green);
	glNormal3f( 0.0,  1.0,  0.0);

	glVertex3fv(v3);
	glVertex3fv(v2);
	glVertex3fv(v6);

	glVertex3fv(v3);
	glVertex3fv(v6);
	glVertex3fv(v7);

	glColor4ubv(blue);
	glNormal3f( 0.0, -1.0,  0.0);

	glVertex3fv(v1);
	glVertex3fv(v0);
	glVertex3fv(v4);

	glVertex3fv(v1);
	glVertex3fv(v4);
	glVertex3fv(v5);

	glEnd();
	glPopMatrix();
}

static float paint_debug_char(char c, float x, float w, float y, float h) {
	Text__use_texture(c);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,     y,     0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + w, y,     0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + w, y + h, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,     y + h, 0.0f);
	glEnd();
	return x - w;
}

static void paint_debug_fps(uint64_t fps) {
	float w = 2.0f * (float)Text__font_width;
	float h = 2.0f * (float)Text__font_height;
	float x = w * 9;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-0.0f, (float)scene_width, 0.0f, (float)scene_height);

	/*glPushMatrix();*/
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
			glVertex3f(   0.0f, 0.0f, 0.0f);
			glVertex3f(w*10.0f, 0.0f, 0.0f);
			glVertex3f(w*10.0f,    h, 0.0f);
			glVertex3f(   0.0f,    h, 0.0f);
		glEnd();
		glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			x = paint_debug_char('S', x, w, 0.0f, h);
			x = paint_debug_char('P', x, w, 0.0f, h);
			x = paint_debug_char('F', x, w, 0.0f, h);
			while (fps > 0) {
				x = paint_debug_char('0' + (char)(fps % 10), x, w, 0.0f, h);
				fps /= 10;
			}
		glDisable(GL_TEXTURE_2D);
	/*glPopMatrix();*/
}

static nanosecond_t last_paint_start = 0;
static nanosecond_t this_paint_start;
static void paint(SDL_Window *window) {
	if (last_paint_start == 0)
		last_paint_start = time_now();
	this_paint_start = time_now();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, scene_ratio, 1.0, 1024.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	/* install lights */

	glLoadIdentity();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 2.0f);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, l0);

	/* draw boxes -- NB: back-to-front */

	paint_box(cube2);
	paint_box(cube1);
	paint_box(cube0);

	glDisable(GL_LIGHTING);

	/*** draw overlay ***/

	paint_debug_fps(NANOSECOND_C(1000000000) / (this_paint_start - last_paint_start));
	last_paint_start = this_paint_start;

	SDL_GL_SwapWindow(window);
}

int main(int argc, char **argv) {
	SDL_Window       *window   = NULL;
	SDL_Renderer     *renderer = NULL;
	SDL_RendererInfo  info;

	scene_width = SCREEN_WIDTH;
	scene_height = SCREEN_HEIGHT;
	scene_ratio = (float)scene_width / (float)scene_height;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		exit(-1);
	}

	window = SDL_CreateWindow("SDL Test",
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				scene_width, scene_height,
				SDL_WINDOW_OPENGL);
	if (window == NULL) {
		fprintf(stderr, "Unable to create window: %s\n", SDL_GetError());
		exit(-1);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		fprintf(stderr, "Unable to create renderer: %s\n", SDL_GetError());
		exit(-1);
	}

	if (SDL_GetRendererInfo(renderer, &info) < 0) {
		fprintf(stderr, "Unable to get renderer info: %s\n", SDL_GetError());
		exit(-1);
	}
	if ((info.flags & SDL_RENDERER_ACCELERATED) == 0) {
		fprintf(stderr, "Unable to acquire hardware-accelerated renderer\n");
		exit(-1);
	}

	/* set up OpenGL */

	glShadeModel(GL_SMOOTH);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	glClearColor(0, 0, 0, 0);

	glViewport(0, 0, scene_width, scene_height);

	Text__generate_textures();

	/* set up engine, entities, etc. */

	Entity__set3f(&cube0, ENTITY_POSITION,  0.0f, -0.5f, -5.0f);
	Entity__set3f(&cube0, ENTITY_ROTATION,  0.0f,  0.0f,  0.0f);
	Engine__register_entity(&cube0);

	Entity__set3f(&cube1, ENTITY_POSITION, -4.5f,  0.0f, -9.0f);
	Entity__set3f(&cube1, ENTITY_ROTATION,180.0f,180.0f,  0.0f);
	Engine__register_entity(&cube1);

	Entity__set3f(&cube2, ENTITY_POSITION,  6.0f,  0.5f,-12.0f);
	Entity__set3f(&cube2, ENTITY_ROTATION,270.0f, 90.0f, 60.0f);
	Engine__register_entity(&cube2);

	for (;;) {
		handle_events();
		Engine__maybe_tick();
		paint(window);
	}

	return 0;
}

/* vim: ts=4 sts=4 sw=4
 */
