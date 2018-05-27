#include "mythos_shader_sources.h"


const char* __mythosSolidShaderVertexSource = R"glsl(

#version 140

in vec3 position;

uniform mat4 gl_ModelViewProjectionMatrix;

void main() {

	gl_Position = gl_ModelViewProjectionMatrix * vec4(position, 1.0);
}

)glsl";

const char* __mythosSolidShaderFragmentSource = R"glsl(

#version 140

out vec4 fragColor;

uniform vec4 color;

void main() {

	fragColor = vec4(1.0, 1.0, 1.0, 1.0);
}

)glsl";


MYTHOS_CORE_API const char* __mythosGetSolidShaderVertexSource(void) {

	return __mythosSolidShaderVertexSource;
}

MYTHOS_CORE_API const char* __mythosGetSolidShaderFragmentSource(void) {

	return __mythosSolidShaderFragmentSource;
}