#pragma once

#include "_important_stuff.h"

class MythosGroundTexture : public MythosTexture {

private:

	MythosImage* mImage;

public:

	void render(int = MYTHOS_MODELVIEW);
	void transform(int = MYTHOS_MODELVIEW);
};