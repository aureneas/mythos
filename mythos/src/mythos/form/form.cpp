#include <algorithm>

#include "../../../include/mythos/form/form.h"

int Form::execute(ALLEGRO_EVENT_TYPE type, void* extra) {
	FormEventMap::iterator it = events.find(type);
	if (it != events.end())
		return it->second(this, extra);
	return 0;
}
