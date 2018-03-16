#ifndef __SHADER_H__
#define __SHADER_H__

typedef enum Shader {
	SHADER_DEFAULT = 0,

	__NUM_SHADERS__
} Shader_t;

void Shader__generate_shaders();

int Shader__paint_shader(Shader_t shader);

#endif

/* vim: ts=4 sts=4 sw=4
 */
