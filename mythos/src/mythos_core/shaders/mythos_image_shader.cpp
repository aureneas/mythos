#include "mythos_shader_sources.h"


const char* __mythosImageShaderVertexSource = R"glsl(

#version 120

in vec2 position;

uniform mat4 modelViewProjectionMatrix;

out vec2 fragTexCoord;

void main() {

	gl_Position = modelViewProjectionMatrix * vec4(position, 0.0, 1.0);
	fragTexCoord = position;
}

)glsl";

const char* __mythosImageShaderFragmentSource = R"glsl(

#version 120

in vec2 fragTexCoord;

out vec4 fragColor;

uniform sampler2D texSampler;

void main() {

	fragColor = texture(texSampler, fragTexCoord);
}

)glsl";


const char* __mythosGetImageShaderVertexSource(void) {

	return __mythosImageShaderVertexSource;
}

const char* __mythosGetImageShaderFragmentSource(void) {

	return __mythosImageShaderFragmentSource;
}