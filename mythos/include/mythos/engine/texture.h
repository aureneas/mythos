#ifndef MYTHOS_TEXTURE_H
#define MYTHOS_TEXTURE_H

#include <vector>
#include <allegro5/allegro.h>
#include "graphics.h"
#include "../utility/matrix.h"


struct Texture {
    virtual bool in_bounds(int, int) { return false; }
    virtual void draw(Graphics*, int, int) = 0;
};


struct CompositeTexture: public Texture {
    struct TextureDisplacement {
        Point disp;
        std::unique_ptr<Texture> texture;
    };

    std::vector<TextureDisplacement> texture;

    bool in_bounds(int, int);
    void draw(Graphics*, int, int);
};


struct BitmapTexture: public Texture {
    ALLEGRO_BITMAP* bmp;
    BitmapTexture(ALLEGRO_BITMAP*);
    virtual bool in_bounds(int, int);
    virtual void draw(Graphics*, int, int);
};


struct TextTexture: public Texture {
	ALLEGRO_USTR* text;
	ALLEGRO_FONT* font;
	ALLEGRO_COLOR color;

	int line_width;
	int line_height;

	TextTexture(ALLEGRO_USTR*, ALLEGRO_FONT*, ALLEGRO_COLOR);
	virtual bool in_bounds(int, int);
	virtual void draw(Graphics*, int, int);
	virtual void evaluate_text();
};

struct MultilineTextTexture: public TextTexture {
	int line_num;

	MultilineTextTexture(ALLEGRO_USTR*, ALLEGRO_FONT*, ALLEGRO_COLOR, int, int);
	bool in_bounds(int, int);
	void draw(Graphics*, int, int);
	void evaluate_text();
};


struct TransformTexture: public Texture {
    Texture* texture;

    TransformTexture(Texture*);
    virtual void compose_transform(ALLEGRO_TRANSFORM*) = 0;
    bool in_bounds(int, int);
    void draw(Graphics*, int, int);
};

template <typename N>
struct ProportionalScaleTexture: public Texture {
	N*	v_cur;
	N*	v_max;
	N	v_cur_last;
	N	v_max_last;

	float width;
	float height;
	float perc;
	bool x_axis;	// True:	stretch on x-axis
					// False:	stretch on y-axis

	ProportionalScaleTexture(N* c, N* m, float w, float h, bool x) {
		v_cur = c;
		v_max = m;
		v_cur_last = *c;
		v_max_last = *m;

		width = w;
		height = h;
		x_axis = x;
	}

	void compose_transform(ALLEGRO_TRANSFORM* trans) {
		if (*v_cur != v_cur_last || *v_max != v_max_last) {
			v_cur_last = *v_cur;
			v_max_last = *v_max;
			perc = (float)v_cur_last / v_max_last;
		}

		if (x_axis)
			al_scale_transform(trans, width * perc, height);
		else
			al_scale_transform(trans, width, height * perc);
	}
};

#endif // MYTHOS_TEXTURE_H
