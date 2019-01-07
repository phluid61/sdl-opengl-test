#ifdef USE_GLEW
#  include <GL/glew.h>
#  include <SDL.h>
#else
#  include <SDL.h>
#  include <GL/gl.h>
#  include <GL/glu.h>
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <time.h>
#include <sys/time.h>

#include <unistd.h>

#include "engine.h"
#include "entity.h"
#include "model.h"
/*#include "shader.h"*/
#include "text.h"
#include "texture.h"
#include "timing.h"
#include "flags.h"

/* 0=none, 1=version, 2=+extensions */
#define DIAGNOSTICS 1

#define SCREEN_WIDTH   640
#define SCREEN_HEIGHT  480

static int scene_width;
static int scene_height;
static float scene_ratio;

/*static GLubyte black[]         = {0x00, 0x00, 0x00, 0xFF};*/
static GLubyte blue[]          = {0x00, 0x00, 0xAA, 0xFF};
static GLubyte green[]         = {0x00, 0xAA, 0x00, 0xFF};
/*static GLubyte cyan[]          = {0x00, 0xAA, 0xAA, 0xFF};*/
static GLubyte red[]           = {0xAA, 0x00, 0x00, 0xFF};
/*static GLubyte magenta[]       = {0xAA, 0x00, 0xAA, 0xFF};*/
/*static GLubyte brown[]         = {0xAA, 0x55, 0x00, 0xFF};*/
static GLubyte light_gray[]    = {0xAA, 0xAA, 0xAA, 0xFF};

static GLubyte gray[]          = {0x55, 0x55, 0x55, 0xFF};
/*static GLubyte light_blue[]    = {0x55, 0x55, 0xFF, 0xFF};*/
/*static GLubyte light_green[]   = {0x55, 0xFF, 0x55, 0xFF};*/
/*static GLubyte light_cyan[]    = {0x55, 0xFF, 0xFF, 0xFF};*/
/*static GLubyte light_red[]     = {0xFF, 0x55, 0x55, 0xFF};*/
/*static GLubyte light_magenta[] = {0xFF, 0x55, 0xFF, 0xFF};*/
/*static GLubyte yellow[]        = {0xFF, 0xFF, 0x55, 0xFF};*/
static GLubyte white[]         = {0xFF, 0xFF, 0xFF, 0xFF};

static GLfloat light_ambient[] = {0.01f, 0.01f, 0.02f, 1.0f};
static GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
static GLfloat light_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};

/* light position */
static GLfloat l0[] = { 1.0f,  1.0f,  2.0f,  0.0f};

static Entity_t ent0;
static Entity_t ent1;
static Entity_t ent2;

/* Runtime/debug flags */
static flag_t runflags = FLAG_C(0);
#define RUNFLAG_TEXTURES FLAG_C(0x1)
#define RUNFLAG_LIGHTING FLAG_C(0x2)
#define RUNFLAG_PYRAMIDS FLAG_C(0x4)

static void shutdown(int status) {
	SDL_Quit();
	exit(status);
}

static void on_keydown(SDL_Keysym *keysym) {
	switch (keysym->sym) {
	case SDLK_ESCAPE:
	case SDLK_q:
		shutdown(0);
		break;
	case SDLK_s:
		FLAG_TOGGLE(runflags, RUNFLAG_PYRAMIDS);
		break;
	case SDLK_t:
		FLAG_TOGGLE(runflags, RUNFLAG_TEXTURES);
		break;
	case SDLK_l:
		FLAG_TOGGLE(runflags, RUNFLAG_LIGHTING);
		break;
	case SDLK_p:
		Engine__toggle_pause();
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

static void paint_entity_texture(Entity_t box, Texture_t texture) {
	glPushMatrix();
	glTranslatef(box.pos.x, box.pos.y, box.pos.z);
	glRotatef(box.rot.x, 1.0, 0.0, 0.0);
	glRotatef(box.rot.y, 0.0, 1.0, 0.0);
	glRotatef(box.rot.z, 0.0, 0.0, 1.0);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColor4ubv(white);

	glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		Texture__use_texture(texture);
		if (FLAG_TEST(runflags, RUNFLAG_PYRAMIDS)) {
			Model__paint_model(MODEL_PYRAMID);
		} else {
			Model__paint_model(MODEL_CUBE);
		}
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

static void paint_entity_colour(Entity_t box, GLubyte *colour) {
	glPushMatrix();
	glTranslatef(box.pos.x, box.pos.y, box.pos.z);
	glRotatef(box.rot.x, 1.0, 0.0, 0.0);
	glRotatef(box.rot.y, 0.0, 1.0, 0.0);
	glRotatef(box.rot.z, 0.0, 0.0, 1.0);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColor4ubv(colour);

	if (FLAG_TEST(runflags, RUNFLAG_PYRAMIDS)) {
		Model__paint_model(MODEL_PYRAMID);
	} else {
		Model__paint_model(MODEL_CUBE);
	}
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

static float paint_debug_flag(bool flag_set, char c, float x, float w, float y, float h) {
	if (flag_set) {
		glColor4ubv(white);
	} else {
		glColor4ubv(gray);
	}
	return paint_debug_char(c, x, w, y, h);
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
			glVertex3f(w*10.0f,  h+h, 0.0f);
			glVertex3f(   0.0f,  h+h, 0.0f);
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
			/* begin HACK*/
			x = w * 9;
			fps = scene_height;
			while (fps > 0) {
				x = paint_debug_char('0' + (char)(fps % 10), x, w, h, h);
				fps /= 10;
			}
			x = paint_debug_char('X', x, w, h, h);
			fps = scene_width;
			while (fps > 0) {
				x = paint_debug_char('0' + (char)(fps % 10), x, w, h, h);
				fps /= 10;
			}
			/* end HACK, begin HACK2 */
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glColor4ubv(light_gray);
			x = w * 5;
			x = paint_debug_char(':', x, w, h*3, h);
			x = paint_debug_char('S', x, w, h*3, h);
			x = paint_debug_char('G', x, w, h*3, h);
			x = paint_debug_char('A', x, w, h*3, h);
			x = paint_debug_char('L', x, w, h*3, h);
			x = paint_debug_char('F', x, w, h*3, h);
			x = w * 10;
			x = paint_debug_flag(Engine__paused(), 'P', x, w, h*3, h);
			x = paint_debug_flag(FLAG_TEST(runflags, RUNFLAG_PYRAMIDS), 'S', x, w, h*3, h);
			x = paint_debug_flag(FLAG_TEST(runflags, RUNFLAG_LIGHTING), 'L', x, w, h*3, h);
			x = paint_debug_flag(FLAG_TEST(runflags, RUNFLAG_TEXTURES), 'T', x, w, h*3, h);
			/* end HACK */
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

	if (FLAG_TEST(runflags, RUNFLAG_LIGHTING)) {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0f);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0f);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 2.0f);
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
		glLightfv(GL_LIGHT0, GL_POSITION, l0);
	}

	/* draw boxes -- NB: back-to-front */

	if (FLAG_TEST(runflags, RUNFLAG_TEXTURES)) {
		paint_entity_texture(ent2, TEXTURE_STEELBOX);
		paint_entity_texture(ent1, TEXTURE_CARDBOARDBOX);
		paint_entity_texture(ent0, TEXTURE_BOX);
	} else {
		paint_entity_colour(ent2, red);
		paint_entity_colour(ent1, blue);
		paint_entity_colour(ent0, green);
	}

	if (FLAG_TEST(runflags, RUNFLAG_LIGHTING)) {
		glDisable(GL_LIGHTING);
	}

	/*** draw overlay ***/

	paint_debug_fps(NANOSECOND_C(1000000000) / (this_paint_start - last_paint_start));
	last_paint_start = this_paint_start;

	SDL_GL_SwapWindow(window);
}

#define S_FLAG_D 0x01
#define S_FLAG_X 0x02
#define S_FLAG_P 0x04
static int parse_s(const char *str) {
	int64_t w = UINT64_C(0);
	int64_t h = UINT64_C(0);
	flag_t flags;
	char *ptr = (char*)str;
	char c;

	while ((c = *ptr) != (char)0) {
		if (!flags) {
			/* parsing first digit */
			if (c >= '1' && c <= '9') {
				w = (w * 10) + (int64_t)(c - '0');
				FLAG_SET(flags, S_FLAG_D);
			} else {
				return -1;
			}
		} else if (FLAG_TEST(flags, S_FLAG_D)) {
			/* parsing rest of first number */
			if (c >= '0' && c <= '9') {
				w = (w * 10) + (int64_t)(c - '0');
			} else if (c == 'p') {
				FLAG_SET(flags, S_FLAG_P);
			} else if (c == 'x') {
				FLAG_CLEAR(flags, S_FLAG_D);
				FLAG_SET(flags, S_FLAG_X);
			} else {
				return -1;
			}
		} else if (FLAG_TEST(flags, S_FLAG_P)) {
			/* was '#p', there should be nothing here! */
			return -1;
		} else if (FLAG_TEST(flags, S_FLAG_X)) {
			/* was '#x', now reading height */
			if (FLAG_TEST_NONE(flags, S_FLAG_D) && c >= '1' && c <= '9') {
				h = (h * 10) + (int64_t)(c - '0');
				FLAG_SET(flags, S_FLAG_D);
			} else if (c >= '0' && c <= '9') {
				h = (h * 10) + (int64_t)(c - '0');
			} else {
				return -1;
			}
		} else {
			/* ??? */
			return -2;
		}
		ptr++;
	}

	if (FLAG_TEST(flags, S_FLAG_P)) {
		/* gonna assume these are 16:9 */
		scene_height = w;
		scene_width = w * 16 / 9;
	} else if (h == UINT64_C(0)) {
		/* old-fashioned 4:3 */
		scene_width = w;
		scene_height = w * 3 / 4;
	}

	return 0;
}

static void show_help() {
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "    -s <size>   '#' or '#x#' or '#p'\n");
	fprintf(stderr, "    -f          fullscreen\n");
	fprintf(stderr, "    -w          windowed (default)\n");
	fprintf(stderr, "    -b          borderless window\n");
}

#if DIAGNOSTICS
static void show_opengl_info() {
#  if DIAGNOSTICS > 1
	GLubyte *str;
	GLubyte *ptr;
	uint8_t is_blank = 0;
#  endif

	fprintf(stderr, "OpenGL Version %s\n", glGetString(GL_VERSION));
	fprintf(stderr, "(%s/%s)\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
	fprintf(stderr, "Shader language %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
#  if DIAGNOSTICS > 1
	fprintf(stderr, "Extensions:\n");
	str = (GLubyte*)glGetString(GL_EXTENSIONS);
	if (str != (GLubyte*)0) {
		fprintf(stderr, "  ");
		ptr = str;
		while (*ptr != (GLubyte)0) {
			if (*ptr == (GLubyte)' ') {
				if (!is_blank) {
					fprintf(stderr, "\n  ");
					is_blank = 1;
				}
			} else {
				fprintf(stderr, "%c", *ptr);
				is_blank = 0;
			}
			ptr++;
		}
		fprintf(stderr, "\n");
	}
#  endif
}
#else
#  define show_opengl_info() {}
#endif

#define OPTFLAG_S  0x10
#define OPTFLAG_F  0x01
#define OPTFLAG_W  0x02
#define OPTFLAG_B  0x04
#define OPTFLAG_FW 0x03
#define OPTFLAG_WB 0x06
#define OPTFLAG_FS 0x11
int main(int argc, char **argv) {
	SDL_Window       *window   = NULL;
	SDL_Renderer     *renderer = NULL;
	SDL_RendererInfo  info;
	SDL_DisplayMode   displaymode;

	int i;
	flag_t optflags = FLAG_C(0);

#ifdef USE_GLEW
	GLenum glew_err;
#endif

	scene_width = SCREEN_WIDTH;
	scene_height = SCREEN_HEIGHT;

	while ((i = getopt(argc, argv, "+s:fwb?")) != -1) {
		switch (i) {
		case (int)'s':
			if (parse_s(optarg) < 0) {
				fprintf(stderr, "size parameter (-s) should be:\n");
				fprintf(stderr, "  '#'    width (height is 3/4 of that)\n");
				fprintf(stderr, "  '#x#'  width x height\n");
				fprintf(stderr, "  '#p'    e.g. 1080p or 720p\n");
				fprintf(stderr, "\n");
				exit(-2);
			}
			FLAG_SET(optflags, OPTFLAG_S);
			break;
		case (int)'f':
			FLAG_SET(optflags, OPTFLAG_F);
			break;
		case (int)'w':
			FLAG_SET(optflags, OPTFLAG_W);
			break;
		case (int)'b':
			FLAG_SET(optflags, OPTFLAG_WB);
			break;
		default:
			show_help();
			exit(-2);
		}
	}
	if (optind < argc) {
		fprintf(stderr, "unrecognised command-line options");
		show_help();
		exit(-2);
	}
	if (FLAG_TEST_ALL(optflags, OPTFLAG_FW)) {
		fprintf(stderr, "-f and -w/-b options are mutually exclusive\n");
		exit(-2);
	}

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		exit(-1);
	}

	if (FLAG_TEST_MASK(optflags, OPTFLAG_FS, OPTFLAG_F)) {
		/* FIXME: always display 0 */
		/* FIXME: error? */
		if (SDL_GetDesktopDisplayMode(0, &displaymode) == 0) {
			scene_width = displaymode.w;
			scene_height = displaymode.h;
		}
	}
	scene_ratio = (float)scene_width / (float)scene_height;

	window = SDL_CreateWindow("SDL Test",
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				scene_width, scene_height,
				SDL_WINDOW_OPENGL | (
					(optflags & OPTFLAG_F)
					? SDL_WINDOW_FULLSCREEN
					: (
						(optflags & OPTFLAG_B)
						? SDL_WINDOW_BORDERLESS
						: 0
					)
				));
	if (window == NULL) {
		fprintf(stderr, "Unable to create window: %s\n", SDL_GetError());
		exit(-1);
	}

#ifdef USE_GLEW
	glew_err = glewInit();
	if (glew_err != GLEW_OK) {
		fprintf(stderr, "Unable to initialize GLEW: %s\n", glewGetErrorString(glew_err));
	}
	fprintf(stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

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

	/* diagnostics */

	show_opengl_info();

	/* set up OpenGL */

	glShadeModel(GL_SMOOTH);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	glClearColor(0, 0, 0, 0);

	glViewport(0, 0, scene_width, scene_height);

	Text__generate_textures();
	Texture__generate_textures();
/*	Shader__generate_shaders();*/
	Model__generate_models();

	/* set up engine, entities, etc. */

	Entity__set3f(&ent0, ENTITY_POSITION,  0.0f,  0.0f, -4.0f);
	Entity__set3f(&ent0, ENTITY_ROTATION,  0.0f,  0.0f,  0.0f);
	Engine__register_entity(&ent0);

	Entity__set3f(&ent1, ENTITY_POSITION, -4.5f,  0.0f, -6.5f);
	Entity__set3f(&ent1, ENTITY_ROTATION,180.0f,180.0f,  0.0f);
	Engine__register_entity(&ent1);

	Entity__set3f(&ent2, ENTITY_POSITION,  5.5f,  0.0f, -7.5f);
	Entity__set3f(&ent2, ENTITY_ROTATION,270.0f, 90.0f, 60.0f);
	Engine__register_entity(&ent2);

	FLAG_SET(runflags, RUNFLAG_TEXTURES);
	FLAG_SET(runflags, RUNFLAG_LIGHTING);

	for (;;) {
		handle_events();
		Engine__maybe_tick();
		paint(window);
	}

	return 0;
}

/* vim: ts=4 sts=4 sw=4
 */
