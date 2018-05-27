#version 330 core

in vec2 fragTexCoord;

uniform sampler2D texSampler;

void main() {

	gl_FragColor = texture(texSampler, fragTexCoord);
}