#include <SOIL/src/SOIL.h>
#include "mythos_texture.h"
#include "../shaders/mythos_shader.h"
#include "../shaders/mythos_shader_sources.h"
#include "../utility/mythos_stack.h"

#include <iostream>


const float __squarePosArr[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f
};



GLuint				__squareVAO;
GLuint				__squareBuf;
	// [0]	position coordinates
	// [1]	texture coordinates


MythosShader*		__solidShader;
GLint				__solidShaderPos;
GLint				__solidShaderColor;

MythosShader*		__imageShader;
GLuint				__imageShaderPos;
GLuint				__imageShaderTex;
GLuint				__imageShaderMVPM;


void __mythosTextureInit() {

	// Create vertex array buffers

	glGenVertexArrays(1, &__squareVAO);
	glBindVertexArray(__squareVAO);

	glGenBuffers(1, &__squareBuf);
	glBindBuffer(GL_ARRAY_BUFFER, __squareBuf);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), __squarePosArr, GL_STATIC_DRAW);


	// Load shaders and the shader data

	__mythosShaderInit();

	__solidShader = mythosGetSolidShader();
	__solidShaderPos = __solidShader->getAttribLocation("position");
	__solidShaderColor = __solidShader->getUniformLocation("color");

	__imageShader = mythosGetImageShader();
	__imageShaderPos = __imageShader->getAttribLocation("position");
	__imageShaderTex = __imageShader->getAttribLocation("vertTexCoord");
	__imageShaderMVPM = __imageShader->getUniformLocation("modelViewProjectionMatrix");
}

void __renderSquare() {

	glBindVertexArray(__squareVAO);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
}


MythosColor::MythosColor() : MythosColor(0.0f, 0.0f, 0.0f, 0.0f) {}

MythosColor::MythosColor(float cr, float cg, float cb, float ca) : r(mColor[0]), g(mColor[1]), b(mColor[2]), a(mColor[3]) {

	mColor[0] = cr;
	mColor[1] = cg;
	mColor[2] = cb;
	mColor[3] = ca;
}

MythosColor& MythosColor::operator=(MythosColor& color) {

	for (int i = 3; i >= 0; --i)
		mColor[i] = color.mColor[i];
	return *this;
}


MYTHOS_CORE_API MythosImage mythosLoadImage(const char* path) {

	MythosImage data;
	glGenTextures(1, &data.key);
	glBindTexture(GL_TEXTURE_2D, data.key);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int w = 0, h = 0;
	unsigned char* image = SOIL_load_image(path, &w, &h, 0, SOIL_LOAD_RGBA);
	data.width = (float)w;
	data.height = (float)h;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.width, data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	return data;
}


MythosImageWidget::MythosImageWidget(MythosImage* image, vec2f pos) : MythosWidget(pos) {

	mImage = image;
}

void MythosImageWidget::setImage(MythosImage* image) {

	mImage = image;
}

int MythosImageWidget::inBounds(vec2f& pos) {

	return __MYTHOS_WITHIN_BOUNDS(pos, 0.0f, mImage->width, 0.0f, mImage->height);
}

void MythosImageWidget::render() {

#ifdef MYTHOS_STACK
	mythosPushMatrix();
	mythosTranslatef(mPos.x, mPos.y, 0.0f);
	mythosScalef(mImage->width, mImage->height, 1);
#else
	glPushMatrix();
	glScalef(mImage->widthf, mImage->heightf, 1.0);
#endif

	__imageShader->use();

	glBindTexture(GL_TEXTURE_2D, mImage->key);
	mythosSendMatrixToShader(__imageShaderMVPM);
	
	glEnableVertexAttribArray(__imageShaderPos);
	glBindBuffer(GL_ARRAY_BUFFER, __squareBuf);
	glVertexAttribPointer(
		__imageShaderPos,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	__renderSquare();

	glDisableVertexAttribArray(__imageShaderPos);

#ifdef MYTHOS_STACK
	mythosPopMatrix();
#else
	glPopMatrix();
#endif
}