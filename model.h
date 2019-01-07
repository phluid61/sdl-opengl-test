#ifndef __MODEL_H__
#define __MODEL_H__

typedef enum Model {
	MODEL_CUBE    = 0,
	MODEL_PYRAMID = 1,

	__NUM_MODELS__
} Model_t;

void Model__generate_models();

int Model__paint_model(Model_t model);

#endif

/* vim: ts=4 sts=4 sw=4
 */
