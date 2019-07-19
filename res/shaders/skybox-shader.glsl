#include "common.glh"

varying vec3 texCoord0;

#if defined(VS_BUILD)
Layout(0) attribute vec3 position;
Layout(4) attribute mat4 mvp;

layout (std140) uniform ShaderData {
	mat4 jointTransforms[MAX_JOINTS];
	vec3 lightPosition;
	float ambientLight;
	vec3 lightColor;
	vec3 cameraPosition;
};

void main() {
	vec4 pos = vec4(position, 1.0) * mvp;
    gl_Position = pos.xyww;
    texCoord0 = position;
}

#elif defined(FS_BUILD)
uniform samplerCube cubemap;

DeclareFragOutput(0, vec4);

void main() {
	SetFragOutput(0, texture2D(cubemap, texCoord0));
}
#endif
