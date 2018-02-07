#ifndef MYTHOS_FORMENGINE_H
#define MYTHOS_FORMENGINE_H

#include "../engine/state.h"
#include "volume.h"

namespace engine {


struct VolumeWidget: public Widget {
	Volume*		vol;
	Texture**	texture;

	bool in_bounds(int, int);
	void draw(Graphics*, int, int);
};

struct ContVolumeLayer: public Layer {
	ContainerVolume		cont;
	Point				center;
	float				zoom;

	void update_frame();

	void set_center(const Point&);
	void adjust_center(const Point&);
	void set_zoom(float);
};


}


#endif // MYTHOS_AREALAYER_H
