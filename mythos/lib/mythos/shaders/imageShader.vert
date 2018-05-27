#version 330 core

in vec3 position;
in vec2 vertTexCoord;

out vec2 fragTexCoord;

void main() {

	gl_Position = vec4(position, 1.0);
	fragTexCoord = vertTexCoord;
}