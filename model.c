#include "model.h"

#ifdef USE_GLEW
#  include <GL/glew.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#endif

/* if defined manually render all the points and stuff */
#define MANUAL_RENDER

#define VERTEX_ATTRIB 0
#define NORMAL_ATTRIB 1
#define TEXTURE_ATTRIB 2

static const GLfloat vertices_cube[] = {
	/* normal: 0,0,1 */
	/*012,023*/
	 0.0f, 0.0f,    0.0f,  0.0f,  1.0f,   -1.0f, -1.0f, 1.0f,
	 0.0f, 1.0f,    0.0f,  0.0f,  1.0f,    1.0f, -1.0f, 1.0f,
	 1.0f, 1.0f,    0.0f,  0.0f,  1.0f,    1.0f,  1.0f, 1.0f,

	 0.0f, 0.0f,    0.0f,  0.0f,  1.0f,   -1.0f, -1.0f, 1.0f,
	 1.0f, 1.0f,    0.0f,  0.0f,  1.0f,    1.0f,  1.0f, 1.0f,
	 1.0f, 0.0f,    0.0f,  0.0f,  1.0f,   -1.0f,  1.0f, 1.0f,

	/* normal: 1,0,0 */
	/*156,162*/
	 0.0f, 0.0f,    1.0f,  0.0f,  0.0f,    1.0f, -1.0f,  1.0f,
	 0.0f, 1.0f,    1.0f,  0.0f,  0.0f,    1.0f, -1.0f, -1.0f,
	 1.0f, 1.0f,    1.0f,  0.0f,  0.0f,    1.0f,  1.0f, -1.0f,

	 0.0f, 0.0f,    1.0f,  0.0f,  0.0f,    1.0f, -1.0f,  1.0f,
	 1.0f, 1.0f,    1.0f,  0.0f,  0.0f,    1.0f,  1.0f, -1.0f,
	 1.0f, 0.0f,    1.0f,  0.0f,  0.0f,    1.0f,  1.0f,  1.0f,

	/* normal: 0,0,-1 */
	/*547,576*/
	 0.0f, 0.0f,    0.0f,  0.0f, -1.0f,    1.0f, -1.0f, -1.0f,
	 0.0f, 1.0f,    0.0f,  0.0f, -1.0f,   -1.0f, -1.0f, -1.0f,
	 1.0f, 1.0f,    0.0f,  0.0f, -1.0f,   -1.0f,  1.0f, -1.0f,

	 0.0f, 0.0f,    0.0f,  0.0f, -1.0f,    1.0f, -1.0f, -1.0f,
	 1.0f, 1.0f,    0.0f,  0.0f, -1.0f,   -1.0f,  1.0f, -1.0f,
	 1.0f, 0.0f,    0.0f,  0.0f, -1.0f,    1.0f,  1.0f, -1.0f,

	/* normal: -1,0,0 */
	/*403,437*/
	 0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,   -1.0f, -1.0f, -1.0f,
	 0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,   -1.0f, -1.0f,  1.0f,
	 1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,   -1.0f,  1.0f,  1.0f,

	 0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,   -1.0f, -1.0f, -1.0f,
	 1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,   -1.0f,  1.0f,  1.0f,
	 1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,   -1.0f,  1.0f, -1.0f,

	/* normal: 0,1,0 */
	/*326,367*/
	 0.0f, 0.0f,    0.0f,  1.0f,  0.0f,   -1.0f,  1.0f,  1.0f,
	 0.0f, 1.0f,    0.0f,  1.0f,  0.0f,    1.0f,  1.0f,  1.0f,
	 1.0f, 1.0f,    0.0f,  1.0f,  0.0f,    1.0f,  1.0f, -1.0f,

	 0.0f, 0.0f,    0.0f,  1.0f,  0.0f,   -1.0f,  1.0f,  1.0f,
	 1.0f, 1.0f,    0.0f,  1.0f,  0.0f,    1.0f,  1.0f, -1.0f,
	 1.0f, 0.0f,    0.0f,  1.0f,  0.0f,   -1.0f,  1.0f, -1.0f,

	/* normal: 0,-1,0 */
	/*104,145*/
	 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    1.0f, -1.0f,  1.0f,
	 0.0f, 1.0f,    0.0f, -1.0f,  0.0f,   -1.0f, -1.0f,  1.0f,
	 1.0f, 1.0f,    0.0f, -1.0f,  0.0f,   -1.0f, -1.0f, -1.0f,

	 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    1.0f, -1.0f,  1.0f,
	 1.0f, 1.0f,    0.0f, -1.0f,  0.0f,   -1.0f, -1.0f, -1.0f,
	 1.0f, 0.0f,    0.0f, -1.0f,  0.0f,    1.0f, -1.0f, -1.0f,
};

static const GLfloat vertices_pyramid[] = {
	/* front */
	 0.5f, 1.0f,    0.0f,  0.31623f,  0.94868f,    0.0f,  1.0f,  0.0f,
	 0.0f, 0.0f,    0.0f,  0.31623f,  0.94868f,   -1.0f, -1.0f,  1.0f,
	 1.0f, 0.0f,    0.0f,  0.31623f,  0.94868f,    1.0f, -1.0f,  1.0f,

	/* right */
	 0.5f, 1.0f,    0.94868f,  0.31623f,  0.0f,    0.0f,  1.0f,  0.0f,
	 0.0f, 0.0f,    0.94868f,  0.31623f,  0.0f,    1.0f, -1.0f,  1.0f,
	 1.0f, 0.0f,    0.94868f,  0.31623f,  0.0f,    1.0f, -1.0f, -1.0f,

	/* back */
	 0.5f, 1.0f,    0.0f,  0.31623f, -0.94868f,    0.0f,  1.0f,  0.0f,
	 0.0f, 0.0f,    0.0f,  0.31623f, -0.94868f,    1.0f, -1.0f, -1.0f,
	 1.0f, 0.0f,    0.0f,  0.31623f, -0.94868f,   -1.0f, -1.0f, -1.0f,

	/* left */
	 0.5f, 1.0f,   -0.94868f,  0.31623f,  0.0f,    0.0f,  1.0f,  0.0f,
	 0.0f, 0.0f,   -0.94868f,  0.31623f,  0.0f,   -1.0f, -1.0f, -1.0f,
	 1.0f, 0.0f,   -0.94868f,  0.31623f,  0.0f,   -1.0f, -1.0f,  1.0f,

	/* base */
	 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    1.0f, -1.0f,  1.0f,
	 0.0f, 1.0f,    0.0f, -1.0f,  0.0f,   -1.0f, -1.0f,  1.0f,
	 1.0f, 1.0f,    0.0f, -1.0f,  0.0f,   -1.0f, -1.0f, -1.0f,

	 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,    1.0f, -1.0f,  1.0f,
	 1.0f, 1.0f,    0.0f, -1.0f,  0.0f,   -1.0f, -1.0f, -1.0f,
	 1.0f, 0.0f,    0.0f, -1.0f,  0.0f,    1.0f, -1.0f, -1.0f,
};

static const GLint num_verts[] = {
	36, /* cube */
	18, /* pyramid */
};

#ifndef MANUAL_RENDER

static GLuint vertex_buffers[__NUM_MODELS__];

void Model__generate_models() {
	glGenBuffers(2, vertex_buffers);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[MODEL_CUBE]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cube), vertices_cube, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[MODEL_PYRAMID]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_pyramid), vertices_pyramid, GL_STATIC_DRAW);
}

int Model__paint_model(Model_t model) {
	if (model >= 0 && model < __NUM_MODELS__) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
			glEnableVertexAttribArray( NORMAL_ATTRIB);
			glEnableVertexAttribArray(TEXTURE_ATTRIB);
			glEnableVertexAttribArray( VERTEX_ATTRIB);
#  if 1
				glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[model]);
				/* FIXME: need to assign the attribs in the shaders */
				glVertexAttribPointer(TEXTURE_ATTRIB, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, (void*)0);
				glVertexAttribPointer( NORMAL_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, (void*)(sizeof(GLfloat)*2));
				glVertexAttribPointer( VERTEX_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, (void*)(sizeof(GLfloat)*5));
#  elif 0
				glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[model]);
				glTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat)*6, (GLvoid*)0);
				glNormalPointer(     GL_FLOAT, sizeof(GLfloat)*5, (GLvoid*)(sizeof(GLfloat)*2));
				glVertexPointer(  3, GL_FLOAT, sizeof(GLfloat)*5, (GLvoid*)(sizeof(GLfloat)*5));
#  else
				glInterleavedArrays(GL_T2F_N3F_V3F, 0, (void*)vertices_cube);
#  endif
				glDrawArrays(GL_TRIANGLES, 0, num_verts[model]);
			glDisableVertexAttribArray( VERTEX_ATTRIB);
			glDisableVertexAttribArray(TEXTURE_ATTRIB);
			glDisableVertexAttribArray( NORMAL_ATTRIB);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		return 0;
	}
	return -1;
}

#else

static GLfloat* vertex_arrays[__NUM_MODELS__];

void Model__generate_models() {
	vertex_arrays[MODEL_CUBE]    = (GLfloat*)vertices_cube;
	vertex_arrays[MODEL_PYRAMID] = (GLfloat*)vertices_pyramid;
}

int Model__paint_model(Model_t model) {
	GLint n;
	GLfloat *ptr;
	if (model >= 0 && model < __NUM_MODELS__) {
		glBegin(GL_TRIANGLES);
		ptr = vertex_arrays[model];
		for (n = num_verts[model]; n > 0; n--) {
			glTexCoord2fv(ptr); ptr += 2;
			glNormal3fv(ptr); ptr += 3;
			glVertex3fv(ptr); ptr += 3;
		}
		glEnd();
		return 0;
	}
	return -1;
}

#endif

/* vim: ts=4 sts=4 sw=4
 */
