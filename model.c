#include "model.h"

#ifdef USE_GLEW
#  include <GL/glew.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#endif

#define VERTEX_ATTRIB 0
#define NORMAL_ATTRIB 1
#define TEXTURE_ATTRIB 2

static const GLfloat vertices_cube[] = {
	/* normal: 0,0,1 */
	/*012,023*/
	 0.0f,  0.0f,  1.0f,   0.0f, 0.0f,   -1.0f, -1.0f, 1.0f,
	 0.0f,  0.0f,  1.0f,   0.0f, 1.0f,    1.0f, -1.0f, 1.0f,
	 0.0f,  0.0f,  1.0f,   1.0f, 1.0f,    1.0f,  1.0f, 1.0f,

	 0.0f,  0.0f,  1.0f,   0.0f, 0.0f,   -1.0f, -1.0f, 1.0f,
	 0.0f,  0.0f,  1.0f,   1.0f, 1.0f,    1.0f,  1.0f, 1.0f,
	 0.0f,  0.0f,  1.0f,   1.0f, 0.0f,   -1.0f,  1.0f, 1.0f,

	/* normal: 1,0,0 */
	/*156,162*/
	 1.0f,  0.0f,  0.0f,   0.0f, 0.0f,    1.0f, -1.0f,  1.0f,
	 1.0f,  0.0f,  0.0f,   0.0f, 1.0f,    1.0f, -1.0f, -1.0f,
	 1.0f,  0.0f,  0.0f,   1.0f, 1.0f,    1.0f,  1.0f, -1.0f,

	 1.0f,  0.0f,  0.0f,   0.0f, 0.0f,    1.0f, -1.0f,  1.0f,
	 1.0f,  0.0f,  0.0f,   1.0f, 1.0f,    1.0f,  1.0f, -1.0f,
	 1.0f,  0.0f,  0.0f,   1.0f, 0.0f,    1.0f,  1.0f,  1.0f,

	/* normal: 0,0,-1 */
	/*547,576*/
	 0.0f,  0.0f, -1.0f,   0.0f, 0.0f,    1.0f, -1.0f, -1.0f,
	 0.0f,  0.0f, -1.0f,   0.0f, 1.0f,   -1.0f, -1.0f, -1.0f,
	 0.0f,  0.0f, -1.0f,   1.0f, 1.0f,   -1.0f,  1.0f, -1.0f,

	 0.0f,  0.0f, -1.0f,   0.0f, 0.0f,    1.0f, -1.0f, -1.0f,
	 0.0f,  0.0f, -1.0f,   1.0f, 1.0f,   -1.0f,  1.0f, -1.0f,
	 0.0f,  0.0f, -1.0f,   1.0f, 0.0f,    1.0f,  1.0f, -1.0f,

	/* normal: -1,0,0 */
	/*403,437*/
	-1.0f,  0.0f,  0.0f,   0.0f, 0.0f,   -1.0f, -1.0f, -1.0f,
	-1.0f,  0.0f,  0.0f,   0.0f, 1.0f,   -1.0f, -1.0f,  1.0f,
	-1.0f,  0.0f,  0.0f,   1.0f, 1.0f,   -1.0f,  1.0f,  1.0f,

	-1.0f,  0.0f,  0.0f,   0.0f, 0.0f,   -1.0f, -1.0f, -1.0f,
	-1.0f,  0.0f,  0.0f,   1.0f, 1.0f,   -1.0f,  1.0f,  1.0f,
	-1.0f,  0.0f,  0.0f,   1.0f, 0.0f,   -1.0f,  1.0f, -1.0f,

	/* normal: 0,1,0 */
	/*326,367*/
	 0.0f,  1.0f,  0.0f,   0.0f, 0.0f,   -1.0f,  1.0f,  1.0f,
	 0.0f,  1.0f,  0.0f,   0.0f, 1.0f,    1.0f,  1.0f,  1.0f,
	 0.0f,  1.0f,  0.0f,   1.0f, 1.0f,    1.0f,  1.0f, -1.0f,

	 0.0f,  1.0f,  0.0f,   0.0f, 0.0f,   -1.0f,  1.0f,  1.0f,
	 0.0f,  1.0f,  0.0f,   1.0f, 1.0f,    1.0f,  1.0f, -1.0f,
	 0.0f,  1.0f,  0.0f,   1.0f, 0.0f,   -1.0f,  1.0f, -1.0f,

	/* normal: 0,-1,0 */
	/*104,145*/
	 0.0f, -1.0f,  0.0f,   0.0f, 0.0f,    1.0f, -1.0f,  1.0f,
	 0.0f, -1.0f,  0.0f,   0.0f, 1.0f,   -1.0f, -1.0f,  1.0f,
	 0.0f, -1.0f,  0.0f,   1.0f, 1.0f,   -1.0f, -1.0f, -1.0f,

	 0.0f, -1.0f,  0.0f,   0.0f, 0.0f,    1.0f, -1.0f,  1.0f,
	 0.0f, -1.0f,  0.0f,   1.0f, 1.0f,   -1.0f, -1.0f, -1.0f,
	 0.0f, -1.0f,  0.0f,   1.0f, 0.0f,    1.0f, -1.0f, -1.0f,
};

static const GLint num_tris[] = {
	36, /* cube */
};

static GLuint vertex_buffers[__NUM_MODELS__];

void Model__generate_models() {
	glGenBuffers(1, vertex_buffers);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[MODEL_CUBE]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cube), vertices_cube, GL_STATIC_DRAW);
}

int Model__paint_model(Model_t model) {
	if (model >= 0 && model < __NUM_MODELS__) {
#if 0
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[model]);
		/* FIXME: need to assign the attribs in the shaders */
		glVertexAttribPointer(NORMAL_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, (void*)0);
		glVertexAttribPointer(TEXTURE_ATTRIB, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, (void*)3);
		glVertexAttribPointer(VERTEX_ATTRIB, 3/*tri*/, GL_FLOAT, GL_FALSE/*normalise?*/, sizeof(GLfloat)*5/*stride*/, (void*)5/*offset*/);
		glDrawArrays(GL_TRIANGLES, 0, num_tris[model]);
		glDisableVertexAttribArray(0);
#else
		/*glEnableVertexAttribArray(0);*/
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[model]);
		glNormalPointer(GL_FLOAT, sizeof(GLfloat)*5, (GLvoid*)0);
		glTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat)*6, (GLvoid*)(sizeof(GLfloat)*3));
		glVertexPointer(3, GL_FLOAT, sizeof(GLfloat)*5, (GLvoid*)(sizeof(GLfloat)*5));
		glDrawArrays(GL_TRIANGLES, 0, num_tris[model]);
		/*glDisableVertexAttribArray(0);*/
#endif
		return 0;
	}
	return -1;
}

/* vim: ts=4 sts=4 sw=4
 */
