#include "common.glh"

varying vec2 texCoord0;

#if defined(VS_BUILD)
Layout(0) attribute vec4 vertex; // <posX, posY, texU, texV>
Layout(1) attribute vec4 positions;
Layout(2) attribute vec4 sizes;

layout (std140) uniform ShaderData {
	mat4 projection;
	vec3 textColor;
};

void main() {
	vec4 v = mad(vertex, sizes, positions);
	gl_Position = vec4(v.xy, 0.0, 1.0) * projection;
	texCoord0 = v.zw;
}

#elif defined(FS_BUILD)
DeclareFragOutput(0, vec4);

uniform sampler2D font;

layout (std140) uniform ShaderData {
	mat4 projection;
	vec3 textColor;
};

void main() {
	vec4 color = texture2D(font, texCoord0);

	SetFragOutput(0, vec4(color.x * textColor, color.x));
}
#endif
