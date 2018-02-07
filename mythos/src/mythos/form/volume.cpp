#include <cmath>
#include "../../../include/mythos/form/volume.h"

#define CLOSE_ENOUGH_TO_ZERO 0.01

bool Volume::intersect(Volume* vol) {
	if (vol == this)
		return false;
	if (ContainerVolume* cv = dynamic_cast<ContainerVolume*>(vol)) {
		if (cv->dim.length() < CLOSE_ENOUGH_TO_ZERO)
			return cv->intersect(this);
	}

	// TODO implement intersection algorithm for multiple shapes
	bool int_detect = true;
	if (crd.x <= vol->crd.x) {
		if (crd.x + dim.x <= vol->crd.x)
			int_detect = false;
	} else if (vol->crd.x + vol->dim.x <= crd.x)
		int_detect = false;

	if (int_detect) {
		if (crd.y <= vol->crd.y) {
			if (crd.y + dim.y <= vol->crd.y)
				int_detect = false;
		} else if (vol->crd.y + vol->dim.y <= crd.y)
			int_detect = false;
	}

	if (int_detect) {
		if (crd.z <= vol->crd.z) {
			if (crd.z + dim.z <= vol->crd.z)
				int_detect = false;
		} else if (vol->crd.z + vol->dim.z <= crd.z)
			int_detect = false;
	}

	if (int_detect && form != nullptr) {
		return (form->execute(MYTHOS_EVENT_COLLISION_FORM, vol->form) != 0);
	}
	return int_detect;
}


bool ContainerVolume::intersect(Volume* vol) {
	if (vol == this)
		return false;
	if (dim.length() > CLOSE_ENOUGH_TO_ZERO)
		return Volume::intersect(vol);

	Vec3 crd_old = vol->crd;
	Vec2 corn1 = crd_old.reduce() - Vec2(max_dim, max_dim);
	Vec2 corn2 = (crd_old + vol->dim).reduce();

	int out_key1 = corn1 * angle1;
	int out_key2 = corn2 * angle1;
	int inn_key1 = corn1 * angle2;
	int inn_key2 = corn2 * angle2;

	ContVolumeList::iterator out_it1 = vols.upper_bound(out_key1);
	ContVolumeList::iterator out_it2 = vols.lower_bound(out_key2);
	for (ContVolumeList::iterator out_it = out_it1; out_it != out_it2; ++out_it) {
		ContVolumeInnerList::iterator inn_it1 = out_it->second.upper_bound(inn_key1);
		ContVolumeInnerList::iterator inn_it2 = out_it->second.lower_bound(inn_key2);
		for (ContVolumeInnerList::iterator inn_it = inn_it1; inn_it != inn_it2; ++inn_it) {
			vol->crd -= inn_it->second->crd;
			bool int_detect = inn_it->second->intersect(vol);
			vol->crd = crd_old;
			if (int_detect)
				return false;
		}
	}
	return true;
}

void ContainerVolume::set_angle(const Vec2& a) {
	angle1 = a;
	angle2 = Vec2(a.y, -a.x);
}

/**
 *	Attempts to set the coordinates of the volume in the container.
 *	@return if the volume was set successfully
 */
bool ContainerVolume::set_crd(Volume* vol, const Vec3& crd) {
	Vec3 crd_old = vol->crd;
	vol->crd = crd;
	Vec3 dim_old = dim;
	dim = Vec3(0,0,0);

	bool ret = !intersect(vol);
	dim = dim_old;
	if (ret) {
		// if no intersection detected, the crd is set

		// remove [[vol]] from [[vol->parent]]
		int out_key = crd_old * angle1;
		int inn_key = crd_old * angle2;
		ContVolumeList::iterator out_it = vol->parent->vols.find(out_key);
		if (out_it != vol->parent->vols.end()) {
			std::pair<ContVolumeInnerList::iterator, ContVolumeInnerList::iterator> inn_its = out_it->second.equal_range(inn_key);
			for (ContVolumeInnerList::iterator inn_it = inn_its.first; inn_it != inn_its.second; ++inn_it) {
				if (vol == *(inn_it.second)) {
					inn_it->second.release();
					out_it->second.erase(inn_it);
					if (out_it->second.empty() && (vol->parent != this || (crd * angle1) != out_key))
						vol->parent->vols.erase(out_it);
					break;
				}
			}
		}

		// set [[vol->parent]] to [[this]]
		vol->parent = this;

		// put [[vol]] in the proper place in [[vols]]
		out_key = vol->crd * angle1;
		inn_key = vol->crd * angle2;
		out_it = vols.find(out_key);
		if (out_it == vols.end())
			out_it = vols.emplace(out_key, ContVolumeInnerList()).first;
		ContVolumeInnerList::iterator inn_it = out_it->second.find(inn_key);
		if (inn_it == out_it->second.end())
			inn_it = out_it->second.emplace(inn_key, VolumePtr(vol));

	} else {
		// if intersection detected, reset [[vol->crd]] to the old one
		vol->crd = crd_old;
	}
	return ret;
}
