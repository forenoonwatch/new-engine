
#if defined(VS_BUILD)

#include "noise.glh"

Layout(0) attribute vec2 xyPos;
Layout(1) attribute vec4 adjacent;

layout (std140) uniform ShaderData {
	mat4 jointTransforms[MAX_JOINTS];
	vec3 lightPosition;
	float ambientLight;
	vec3 lightColor;
	vec3 cameraPosition;
	float time;
};

void main() {
	vec4 position = vec4(xyPos.x,
			10 * cnoise(0.01 * (xyPos + cameraPosition.xz) + 0.1 * vec2(time)),
			xyPos.y, 1.0);

	gl_Position = position;
}

#elif defined(FS_BUILD)

DeclareFragOutput(0, vec4);

void main() {
	SetFragOutput(0, vec4(0.0, 0.0, 1.0, 1.0));
}

#endif
