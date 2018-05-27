#pragma once

#include "../mythos_important_stuff.h"
#include "../mythos_widget.h"
#include "../utility/matrix.h"


#define MYTHOS_POS 0
#define MYTHOS_TEX 1

#define __MYTHOS_WITHIN_BOUNDS(pos, xMin, xMax, yMin, yMax) ((pos).x >= (xMin) && (pos).x < (xMax) && (pos).y >= (yMin) && (pos).y < (yMax))


class MYTHOS_CORE_API MythosColor {

private:

	float			mColor[4];

public:

	const float&	r;
	const float&	g;
	const float&	b;
	const float&	a;
	
	MythosColor();
	
	MythosColor(float, float, float, float);

	MythosColor& operator=(MythosColor&);
};

struct MYTHOS_CORE_API MythosImage {
	
	GLuint	key;

	float	width;
	float	height;
};


class MYTHOS_CORE_API MythosImageWidget : public MythosWidget {

	private:

		MythosImage*		mImage;

	public:

		MythosImageWidget(MythosImage*, vec2f);

		int					inBounds(vec2f&);

		void				render(void);

		void				setImage(MythosImage*);
};


void						__mythosTextureInit(void);

MYTHOS_CORE_API	MythosImage mythosLoadImage(const char*);