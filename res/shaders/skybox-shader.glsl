#include "common.glh"

varying vec3 texCoord0;

#if defined(VS_BUILD)
Layout(0) attribute vec3 position;
Layout(4) attribute mat4 mvp;

void main() {
	vec4 pos = vec4(position, 1.0) * mvp;
    gl_Position = pos.xyww;
    texCoord0 = position;
}

#elif defined(FS_BUILD)

#define LOWER_LIMIT 0.0
#define UPPER_LIMIT 0.5

layout (std140) uniform ShaderData {
	mat4 jointTransforms[MAX_JOINTS];
	vec3 lightPosition;
	float ambientLight;
	vec3 lightColor;
	vec3 cameraPosition;
	float time;
	vec3 fogColor;
};

uniform samplerCube cubemap;

DeclareFragOutput(0, vec4);

void main() {
	vec4 skyColor = texture2D(cubemap, texCoord0);
	float fact = (texCoord0.y - LOWER_LIMIT) / (UPPER_LIMIT - LOWER_LIMIT);
	fact = clamp(fact, 0.0, 1.0);

	SetFragOutput(0, mix(vec4(fogColor, 1.0), skyColor, fact));
}
#endif
