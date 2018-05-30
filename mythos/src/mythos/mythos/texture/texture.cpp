#include <SOIL/src/SOIL.h>
#include <mythos\texture\texture.h>
#include <mythos\shaders\shader.h>
#include <mythos\utility\stack.h>

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

MYTHOS_CORE_API const GLuint mythosGetSquareArr() { return __squareVAO; }
MYTHOS_CORE_API const GLuint mythosGetSquareBuf() { return __squareBuf; }


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




MythosImageTexture::MythosImageTexture(MythosImage* image, int flags) {

	mImage = image;
	setFlags(flags);
}

void MythosImageTexture::setImage(MythosImage* image) {

	mImage = image;
}

void MythosImageTexture::setFlags(int flags) {

	if ((flags & MYTHOS_ALIGN_HORIZONTAL_CENTER) == MYTHOS_ALIGN_HORIZONTAL_CENTER)
		mHorizontalAlignment = 0.5f;
	else if ((flags & MYTHOS_ALIGN_HORIZONTAL_LEFT) == MYTHOS_ALIGN_HORIZONTAL_LEFT)
		mHorizontalAlignment = 0.0f;
	else if ((flags & MYTHOS_ALIGN_HORIZONTAL_RIGHT) == MYTHOS_ALIGN_HORIZONTAL_RIGHT)
		mHorizontalAlignment = 1.0f;

	if ((flags & MYTHOS_ALIGN_VERTICAL_CENTER) == MYTHOS_ALIGN_VERTICAL_CENTER)
		mVerticalAlignment = 0.5f;
	else if ((flags & MYTHOS_ALIGN_VERTICAL_TOP) == MYTHOS_ALIGN_VERTICAL_TOP)
		mVerticalAlignment = 0.0f;
	else if ((flags & MYTHOS_ALIGN_VERTICAL_BOTTOM) == MYTHOS_ALIGN_VERTICAL_BOTTOM)
		mVerticalAlignment = 1.0f;
}

void MythosImageTexture::transform(int stack) {

	mythosScalef(mImage->width, mImage->height, 1.0f, stack);
	mythosTranslatef(mHorizontalAlignment, mVerticalAlignment, 0.0f, stack);
}

int MythosImageTexture::inBounds(const vec2f& pos, int stack) {

	return inBounds({ vec3f(pos), vec3f(0.0f, 0.0f, 1.0f) }, stack);
}

int MythosImageTexture::inBounds(const Line& line, int stack) {

	mythosPushMatrix(stack);
	transform(stack);

	Plane plane = { vec3f(0.0f, 0.0f, 1.0f), 0.0f };
	Line newLine = { vec3f(mythosUnproject(line.origin)), vec3f(mythosUnproject(line.direction)) };
	float t = plane.getIntersect(newLine);

	mythosPopMatrix(stack);

	if (t != INFINITY) {

		vec3f pos = newLine.getPoint(t);
		return __MYTHOS_WITHIN_BOUNDS(pos, 0.0f, 1.0f, 0.0f, 1.0f);
	}
	
	return MYTHOS_FALSE;
}

void MythosImageTexture::renderSetup(int stack) {

	mythosPushMatrix(stack);
	transform(stack);

	__imageShader->use();

	glBindTexture(GL_TEXTURE_2D, mImage->key);
	mythosSendMatrixToShader(__imageShaderMVPM, stack);

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

	glBindVertexArray(__squareVAO);
}

void MythosImageTexture::render(int stack) {

	renderSetup(stack);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	glBindVertexArray(0);
	glDisableVertexAttribArray(__imageShaderPos);
	mythosPopMatrix(stack);
}

float MythosImageTexture::getWidth() {

	if (mImage)
		return mImage->width;
	return 0.0f;
}

float MythosImageTexture::getHeight() {

	if (mImage)
		return mImage->height;
	return 0.0f;
}



template <>
void MythosTriangleImageTexture<0>::render(int stack) {

	renderSetup(stack);

	glDrawArrays(GL_TRIANGLES, 3, 3);

	glBindVertexArray(0);
	glDisableVertexAttribArray(__imageShaderPos);
	mythosPopMatrix(stack);
}

template <>
void MythosTriangleImageTexture<1>::render(int stack) {

	renderSetup(stack);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(0);
	glDisableVertexAttribArray(__imageShaderPos);
	mythosPopMatrix(stack);
}