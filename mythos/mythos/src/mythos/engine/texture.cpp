#include <GL/glew.h>
#include "../../../SOIL/Simple OpenGL Image Library/src/SOIL.h"

#include "../../../include/mythos/engine/texture.h"

//#include "../../../include/mythos/engine/state.h"
//#include "../../../include/mythos/debug/debug.h"

namespace mythos_engine {

	GLuint squareVBO;
	GLuint squareVAO;
	GLuint imageShader;

	GLint texCoordIndex;

	void init_texture() {
		/*
		glGenVertexArrays(1, &squareVAO);
		glBindVertexArray(squareVAO);

		float squareVertices[] = {
			0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 	1.0f, 1.0f, 0.0f,
			0.0f, 1.0f,		0.0f, 1.0f, 0.0f
		};

		glGenBuffers(1, &squareVBO);
		glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);
		*/

		const char* imageShaderVertexSource = R"glsl(
			#version 150 core

			in vec2 position;
			in vec2 vertTexCoord;

			out vec2 fragTexCoord;

			uniform mat4 gl_ModelViewProjectionMatrix;

			void main() {
				gl_Position = gl_ModelViewProjectionMatrix * vec4(position, 0.0, 1.0);
				fragTexCoord = vertTexCoord;
			}
		)glsl";

		const char* imageShaderFragmentSource = R"glsl(
			#version 150 core

			in vec2 fragTexCoord;

			uniform sampler2D texSampler;

			void main() {
				gl_FragColor = texture(texSampler, fragTexCoord);
			}
		)glsl";
		
		GLuint imageShaderVertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(imageShaderVertex, 1, &imageShaderVertexSource, NULL);
		glCompileShader(imageShaderVertex);
		GLuint imageShaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(imageShaderFragment, 1, &imageShaderFragmentSource, NULL);
		glCompileShader(imageShaderFragment);

		imageShader = glCreateProgram();
		glAttachShader(imageShader, imageShaderVertex);
		glAttachShader(imageShader, imageShaderFragment);
		glLinkProgram(imageShader);
		glUseProgram(imageShader);
		glDeleteShader(imageShaderVertex);
		glDeleteShader(imageShaderFragment);

		/*
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, squareVBO);
		glInterleavedArrays(GL_T2F_V3F, 0, squareVertices);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		*/

		texCoordIndex = glGetAttribLocation(imageShader, "vertTexCoord");

		/*
		GLint posAttr = glGetAttribLocation(imageShader, "position");
		glEnableVertexAttribArray(posAttr);
		//glVertexPointer(2, GL_DOUBLE, byte_offset, 0);
		GLint texAttr = glGetAttribLocation(imageShader, "vertTexCoord");
		glEnableVertexAttribArray(texAttr);
		//glTexCoordPointer(2, GL_DOUBLE, byte_offset, (void*)byte_offset);
		*/
	}

	Texture2D_Info load_texture(const char* path) {
		Texture2D_Info data;
		glGenTextures(1, &data.key);
		glBindTexture(GL_TEXTURE_2D, data.key);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		unsigned char* image = SOIL_load_image(path, &data.width, &data.height, 0, SOIL_LOAD_RGBA);
		data.widthD = (double)data.width;
		data.heightD = (double)data.height;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.width, data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);

		return data;
	}

	void render_square() {
		// TODO figure out how to do glDrawArrays
		//glDrawArrays(GL_QUADS, 0, 4);

		glBegin(GL_QUADS);
		glVertex2d(0.0, 0.0);
		glVertexAttrib2d(texCoordIndex, 1.0, 0.0);
		glVertex2d(1.0, 0.0);
		glVertexAttrib2d(texCoordIndex, 1.0, 1.0);
		glVertex2d(1.0, 1.0);
		glVertexAttrib2d(texCoordIndex, 0.0, 1.0);
		glVertex2d(0.0, 1.0);
		glVertexAttrib2d(texCoordIndex, 0.0, 0.0);
		glEnd();
	}


	ImageTexture::ImageTexture(Texture2D_Info* t) {
		texture = t;
	}
	
	void ImageTexture::set_image(Texture2D_Info* t) {
		texture = t;
	}
	
	bool ImageTexture::in_bounds(Vec2 point) {
		return (point.x < texture->width
			&& point.y < texture->height
			&& point.x >= 0
			&& point.y >= 0);
	}

	void ImageTexture::render() {
		glPushMatrix();
		glScaled(texture->widthD, texture->heightD, 1.0);
		glBindTexture(GL_TEXTURE_2D, texture->key);
		
		render_square();

		glPopMatrix();
	}

}