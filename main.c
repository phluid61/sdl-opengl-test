#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>
#include <stdint.h>

#include <time.h>
#include <sys/time.h>

static uint64_t time_now() {
	struct timespec ts;
	if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
		fprintf(stderr, "unable to check realtime clock\n");
		exit(-2);
	}
	return ((uint64_t)(ts.tv_sec) * UINT64_C(1000000000)) + (uint64_t)(ts.tv_nsec);
}

#define SCREEN_WIDTH   640
#define SCREEN_HEIGHT  480

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

static GLboolean animate = GL_TRUE;
static float rot_y = 0.0f;
static float rot_z = 0.0f;

static void shutdown(int status) {
	SDL_Quit();
	exit(status);
}

static void on_keydown(SDL_Keysym *keysym) {
	switch (keysym->sym) {
	case SDLK_ESCAPE:
		shutdown(0);
		break;
	case SDLK_SPACE:
		animate = !animate;
		break;
	default:
		break;
	}
}

static void events() {
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

static void tick(uint64_t nanoseconds) {
	/* ns / 10_000_000 = 100 degrees per second */
	float increment = (float)nanoseconds / 10000000.0f;

	if (animate) {
		/* FIXME: tied to framerate */
		rot_y += (increment);
		if (rot_y >= 360.0f) {
			rot_y -= 360.0f;
		}

		/*rot_z += (increment * 0.333333f);*/
		rot_z += (increment / 3.1f);
		if (rot_z >= 360.0f) {
			rot_z -= 360.0f;
		}
	}
}

static void paint(SDL_Window *window) {
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

	/* draw box */

	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);
	glRotatef(rot_y, 0.0, 1.0, 0.0);
	glRotatef(rot_z, 0.0, 0.0, 1.0);

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

	SDL_GL_SwapWindow(window);
}

int main(int argc, char **argv) {
	SDL_Window       *window   = NULL;
	SDL_Renderer     *renderer = NULL;
	SDL_RendererInfo  info;

	int width = SCREEN_WIDTH;
	int height = SCREEN_HEIGHT;

	uint64_t timestamp0;
	uint64_t timestamp1;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		exit(-1);
	}

	window = SDL_CreateWindow("SDL Test",
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				width, height,
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

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)width / (float)height, 1.0, 1024.0);

	/*glLightModel(GL_LIGHT_MODEL_AMBIENT, light_ambient);*/

	timestamp0 = time_now();
	for (;;) {
		events();

		timestamp1 = time_now();
		tick(timestamp1 - timestamp0);
		timestamp0 = timestamp1;

		paint(window);
	}

	return 0;
}

/* vim: ts=4 sts=4 sw=4
 */
