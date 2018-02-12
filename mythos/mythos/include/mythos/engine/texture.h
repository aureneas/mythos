#ifndef MYTHOS_TEXTURE_H
#define MYTHOS_TEXTURE_H

#include <GL/glut.h>

#include "../utility/matrix.h"

namespace mythos_engine {

	struct Texture2D_Info {
		GLuint key;
		int width;
		int height;
		double widthD;
		double heightD;
	};

	void init_texture();

	Texture2D_Info load_texture(const char*);


	struct Texture {
		bool in_bounds(Vec2) { return false; }

		virtual void render() = 0;
	};


	struct ImageTexture : public Texture {
		Texture2D_Info* texture;

		ImageTexture(Texture2D_Info* = nullptr);

		void set_image(Texture2D_Info*);

		bool in_bounds(Vec2);
		
		void render();
	};

}

#endif