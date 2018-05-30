#pragma once

#include "form.h"


#define MYTHOS_GROUND_SIZE	64
#define MYTHOS_CHUNK_SIZE	4
#define MYTHOS_AREA_SIZE	64


class MYTHOS_WORLD_API MythosGround {

	public:

		virtual void render(void) = 0;
};

class MYTHOS_WORLD_API MythosUnevenGround {

	private:

		mat4f					mTransformX;	// transform of (0,0)|(1,0)|(1,1) triangle
		mat4f					mTransformY;	// transform of (0,0)|(0,1)|(1,1) triangle

		mat4f					mInverseX;	// inverse transform of mTransformX
		mat4f					mInverseY;	// inverse transform of mTransformY

		MythosImageTexture*		mTextureX;
		MythosImageTexture*		mTextureY;

	public:

		MythosUnevenGround(MythosImage*, vec3f&, vec3f&, vec3f&, vec3f&, int);
		~MythosUnevenGround();

		void					render(void);
};


class MYTHOS_WORLD_API MythosChunk : public MythosDynamicContainerForm {
	
private:

	MythosGround*				mGround[MYTHOS_CHUNK_SIZE][MYTHOS_CHUNK_SIZE];
	float						mHeight[MYTHOS_CHUNK_SIZE][MYTHOS_CHUNK_SIZE];

public:

	void render(void);
};


class MYTHOS_WORLD_API MythosArea : public MythosContainerForm {

private:

	MythosChunk					mChunks[MYTHOS_AREA_SIZE][MYTHOS_AREA_SIZE];

public:

	int addChild(MythosForm*);



	void render(void);
	void renderChildren(void);
};


class MYTHOS_WORLD_API MythosWorld : public MythosContainerForm {
	// TODO
};