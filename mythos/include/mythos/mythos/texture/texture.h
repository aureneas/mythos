#pragma once

#include "../_important_stuff.h"
#include "../widget.h"
#include "../utility/stack.h"
#include "../utility/geometry.h"


#define MYTHOS_ALIGN_HORIZONTAL_LEFT	1
#define MYTHOS_ALIGN_HORIZONTAL_CENTER	3
#define MYTHOS_ALIGN_HORIZONTAL_RIGHT	2

#define MYTHOS_ALIGN_VERTICAL_TOP		4
#define MYTHOS_ALIGN_VERTICAL_CENTER	12
#define MYTHOS_ALIGN_VERTICAL_BOTTOM	8


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



class MYTHOS_CORE_API MythosTexture {

	public:

		virtual int				inBounds(const vec2f&, int = MYTHOS_MODELVIEW) { return MYTHOS_FALSE; }
		virtual int				inBounds(const Line&, int = MYTHOS_MODELVIEW) { return MYTHOS_FALSE; }

		virtual void			render(int = MYTHOS_MODELVIEW) = 0;
		virtual void			transform(int = MYTHOS_MODELVIEW) {}

		virtual float			getWidth(void) { return 0.0f; }
		virtual float			getHeight(void) { return 0.0f; }
};


class MYTHOS_CORE_API MythosImageTexture : public MythosTexture {

	private:

		MythosImage*			mImage;
		
		float					mHorizontalAlignment;
		float					mVerticalAlignment;

	protected:

		void					renderSetup(int);

	public:

		MythosImageTexture(MythosImage*, int = 0);

		int						inBounds(const vec2f&, int = MYTHOS_MODELVIEW);
		int						inBounds(const Line&, int = MYTHOS_MODELVIEW);

		virtual void			render(int = MYTHOS_MODELVIEW);
		virtual void			transform(int = MYTHOS_MODELVIEW);

		void					setImage(MythosImage*);

		void					setFlags(int);

		float					getWidth(void);
		float					getHeight(void);
};

template <int INCLUDES_X1_Y0>
class MYTHOS_CORE_API MythosTriangleImageTexture : public MythosImageTexture {

	public:

		MythosTriangleImageTexture(MythosImage* image, int flags = 0) : MythosImageTexture(image, flags) {}

		void					render(int = MYTHOS_MODELVIEW);
};


void						__mythosTextureInit(void);

MYTHOS_CORE_API	MythosImage mythosLoadImage(const char*);


MYTHOS_CORE_API const GLuint mythosGetSquareArr(void);
MYTHOS_CORE_API const GLuint mythosGetSquareBuf(void);