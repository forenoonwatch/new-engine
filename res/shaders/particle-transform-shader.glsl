#include "common.glh"

#if defined(VS_BUILD)

Layout(0) attribute float type;
Layout(1) attribute vec3 position;
Layout(2) attribute vec3 velocity;
Layout(3) attribute float age;
Layout(4) attribute vec4 transScale;

varying float type0;
varying vec3 position0;
varying vec3 velocity0;
varying float age0;
varying vec4 transScale0;

void main() {
	type0 = type;
	position0 = position;
	velocity0 = velocity;
	age0 = age;
	transScale0 = transScale;
}

#elif defined(GS_BUILD)

#define PARTICLE_TYPE_EMITTER	0.0
#define PARTICLE_TYPE_OBJECT	1.0

#define LAUNCH_FREQUENCY		0.1
#define PARTICLE_LIFE_TIME		5.0

layout (points) in;
layout (points) out;
layout (max_vertices = 30) out;

in float type0[];
in vec3 position0[];
in vec3 velocity0[];
in float age0[];
in vec4 transScale0[];

out float type1;
out vec3 position1;
out vec3 velocity1;
out float age1;
out vec4 transScale1;

layout (std140) uniform ShaderData {
	float deltaTime;
};

void main() {
	float age = age0[0] + deltaTime;

	if (type0[0] == PARTICLE_TYPE_EMITTER) {
		if (age >= LAUNCH_FREQUENCY) {
			type1 = PARTICLE_TYPE_EMITTER;
			position1 = position0[0];
			velocity1 = velocity0[0];
			age1 = 0.0;
			transScale1 = transScale0[0];

			EmitVertex();
			EndPrimitive();

			type1 = PARTICLE_TYPE_OBJECT;
			position1 = position0[0];
			velocity1 = velocity0[0];
			age1 = 0.0;
			transScale1 = transScale0[0];

			EmitVertex();
			EndPrimitive();
		}
		else {
			type1 = PARTICLE_TYPE_EMITTER;
			position1 = position0[0];
			velocity1 = velocity0[0];
			age1 = age;
			transScale1 = transScale0[0];

			EmitVertex();
			EndPrimitive();
		}
	}
	else {
		if (age < PARTICLE_LIFE_TIME) {
			type1 = PARTICLE_TYPE_OBJECT;
			position1 = mad(velocity0[0], deltaTime, position0[0]);
			velocity1 = velocity0[0]; // TODO: acceleration
			age1 = age;
			transScale1 = transScale0[0];

			EmitVertex();
			EndPrimitive();
		}
	}
}

#elif defined(FS_BUILD)

void main() {

}

#endif
