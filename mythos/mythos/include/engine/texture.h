#ifndef MYTHOS_TEXTURE_H
#define MYTHOS_TEXTURE_H

#include <string>

#include "../../../_dependencies/GL/glew.h"

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
		virtual bool in_bounds(Vec2) { return false; }

		virtual void render() = 0;
	};


	struct ImageTexture : public Texture {
		Texture2D_Info* texture;

		ImageTexture(Texture2D_Info* = nullptr);

		void set_image(Texture2D_Info*);

		bool in_bounds(Vec2);
		
		void render();
	};

	struct TextTexture : public Texture {
		std::string* text;

		TextTexture(std::string* = nullptr);

		void set_text(std::string*);

		bool in_bounds(Vec2);

		void render();
	};

}

#endif