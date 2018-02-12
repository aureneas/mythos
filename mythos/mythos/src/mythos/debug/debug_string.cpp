#include "../../../include/mythos/debug/debug_string.h"

void RenderBitmapString(float x, float y, void *font, char *string)
{
	char *c;
	::glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) {
		::glutBitmapCharacter(font, *c);
	}
	::glRasterPos2f(x + 1, y);
	for (c = string; *c != '\0'; c++) {
		::glutBitmapCharacter(font, *c);
	}
}

void ShowText(const char* text, int x, int y, bool reset_projection)
{
	int* pFont = (int*)GLUT_BITMAP_TIMES_ROMAN_24;

	if (reset_projection) {
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, 320, 0, 240);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glScalef(1, -1, 1);
		glTranslatef(0, -240, 0);
	}

	// Display the frame count in 2D text
	pFont = (int*)GLUT_BITMAP_8_BY_13;
	// Display control instructions
	char txt[256];
	strcpy(txt, text);
	RenderBitmapString(x, y, pFont, txt);

	if (reset_projection) {
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		//glMatrixMode(GL_MODELVIEW);
	}
}