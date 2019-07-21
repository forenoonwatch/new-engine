#include "common.glh"

#if defined(VS_BUILD)

Layout(0) attribute vec3 position;
Layout(1) attribute vec3 velocity;
Layout(2) attribute float age;

varying vec3 position0;
varying vec3 velocity0;
varying float age0;

/*layout (std140) uniform ShaderData {

};*/

void main() {
	position0 = position;
	velocity0 = velocity;
	age0 = age;
}

#elif defined(GS_BUILD)

layout (points) in;
layout (points) out;
layout (max_vertices = 30) out;

in vec3 position0[];
in vec3 velocity0[];
in float age0[];

layout (std140) uniform ShaderData {

};

void main() {
	
}

#elif defined(FS_BUILD)

void main() {

}

#endif
