#ifndef MYTHOS_VOLUME_H
#define MYTHOS_VOLUME_H

#include <map>
#include <memory>
#include <vector>

#include "../utility/matrix.h"
#include "form.h"

struct ContainerVolume;

typedef std::shared_ptr<Form>				FormPtr;

struct Volume {
	Vec3				crd;
	Vec3				dim;
	FormPtr				form;
	ContainerVolume*	parent;

	virtual bool intersect(Volume*);	// volume intersection
};

typedef std::unique_ptr<Volume>				VolumePtr;

typedef std::multimap<int, VolumePtr>		ContVolumeInnerList;
typedef std::map<int, ContVolumeInnerList>	ContVolumeList;

struct ContainerVolume: public Volume {
	Vec2				angle1;
	Vec2				angle2;
	int					max_dim;
	ContVolumeList		vols;

	bool intersect(Volume*);
	void set_angle(const Vec2&);
	bool set_crd(Volume*, const Vec3&);
};


#endif // MYTHOS_VOLUME_H
