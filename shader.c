#include "shader.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <GL/glew.h>

#include <unistd.h>
#include <fcntl.h>

static GLuint programs[__NUM_SHADERS__];

static const char *vertex_shader_src =
	"#version 330 core\n"
	"layout(location = 0) in vec3 vertexPosition_modelspace;\n"
	"void main() {\n"
	"  gl_Position.xyz = vertexPosition_modelspace;\n"
	"  gl_Position.w = 1.0;\n"
	"}\n"
;

static const char *fragment_shader_src =
	"#version 330 core\n"
	"out vec3 color;\n"
	"void main() {\n"
	"  color = vec3(1,0,0);\n"
	"}\n"
;

int compile_shader(GLuint shader, const char *src) {
	GLint result = GL_FALSE;
	int log_length;
	char *log;

	/* compile */
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	/* test */
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
	if (log_length > 0) {
		log = (char*)malloc(sizeof(char) * log_length);
		glGetShaderInfoLog(shader, log_length, NULL, log);
		fprintf(stderr, "error compiling shader: %s\n", log);
	}
	return (result == GL_FALSE) ? -1 : 0;
}

#define SHADER_VERTEX   0x01
#define SHADER_FRAGMENT 0x02
void Shader__generate_shaders() {
	GLuint vertex_shader;
	GLuint fragment_shader;

	uint8_t flags = UINT8_C(0);

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	/* TODO: load from external files */
	programs[SHADER_DEFAULT] = glCreateProgram();
	if (compile_shader(vertex_shader, vertex_shader_src)) {
		glAttachShader(programs[SHADER_DEFAULT], vertex_shader);
		flags |= SHADER_VERTEX;
	}
	if (compile_shader(fragment_shader, fragment_shader_src)) {
		glAttachShader(programs[SHADER_DEFAULT], fragment_shader);
		flags |= SHADER_FRAGMENT;
	}
	glLinkProgram(programs[SHADER_DEFAULT]);

	/* TODO: test program */

	if (flags & SHADER_VERTEX) {
		glDetachShader(programs[SHADER_DEFAULT], vertex_shader);
	}
	if (flags & SHADER_FRAGMENT) {
		glDetachShader(programs[SHADER_DEFAULT], fragment_shader);
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

int Shader__use_shader(Shader_t shader) {
	if (shader >= 0 && shader < __NUM_SHADERS__) {
		glUseProgram(programs[shader]);
		return 0;
	}
	return -1;
}

/* vim: ts=4 sts=4 sw=4
 */
