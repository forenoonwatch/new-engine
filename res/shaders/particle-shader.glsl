#include "common.glh"

#if defined(VS_BUILD)

Layout(0) attribute vec3 position;
Layout(1) attribute vec3 velocity;
Layout(2) attribute float timeToLive;

varying vec3 position0;
varying vec3 velocity0;
varying float timeToLive0;

/*layout (std140) uniform ShaderData {

};*/

void main() {
	position0 = position;
	velocity0 = velocity;
	timeToLive0 = timeToLive;
}

#elif defined(GS_BUILD)

layout (points) in;
layout (points) out;
layout (max_vertices = 30) out;

in vec3 position0[];
in vec3 velocity0[];
in float timeToLive0[];

out vec3 position1;
out vec3 velocity1;
out float timeToLive1;

layout (std140) uniform ParticleData {
	vec4 accDT; // vec3 acceleration xyz, deltaTime w
};

void main() {
	float ttl = timeToLive0[0] - accDT.w;

	if (ttl > 0) {
		position1 = mad(velocity0[0], accDT.w, position0[0]);
		velocity1 = mad(accDT.xyz, accDT.w, velocity0[0]);
		timeToLive1 = ttl;
		
		EmitVertex();
		EndPrimitive();
	}
}

#elif defined(FS_BUILD)

void main() {

}

#endif
