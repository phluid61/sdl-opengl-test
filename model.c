#include "model.h"

#include <GL/glew.h>

#define VERTEX_ATTRIB 0

static const GLfloat vertices_cube[] = {
	/* normal: 0,0,1 */
	/*012,023*/
	-1.0f, -1.0f, 1.0f,
	 1.0f, -1.0f, 1.0f,
	 1.0f,  1.0f, 1.0f,

	-1.0f, -1.0f, 1.0f,
	 1.0f, -1.0f, 1.0f,
	-1.0f,  1.0f, 1.0f,

	/* normal: 1,0,0 */
	/*156,162*/
	 1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,

	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,

	/* normal: 0,0,-1 */
	/*547,576*/
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	 1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,

	/* normal: -1,0,0 */
	/*403,437*/
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	/* normal: 0,1,0 */
	/*326,367*/
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,

	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	/* normal: 0,-1,0 */
	/*104,145*/
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,

	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
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
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[model]);
		glVertexAttribPointer(VERTEX_ATTRIB, 3/*tri*/, GL_FLOAT, GL_FALSE/*normalise?*/, 0/*stride*/, (void*)0/*offset*/);
		glDrawArrays(GL_TRIANGLES, 0, num_tris[model]);
		glDisableVertexAttribArray(0);
		return 0;
	}
	return -1;
}

/* vim: ts=4 sts=4 sw=4
 */
