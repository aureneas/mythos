#ifndef MYTHOS_WIDGET_H
#define MYTHOS_WIDGET_H

#include <list>
#include <memory>
#include <unordered_map>

#include "event.h"
#include "texture.h"
#include "../utility/matrix.h"

namespace mythos_engine {

	enum MYTHOS_UPDATE_RETURN : int {
		MYTHOS_UPDATE_CONTINUE = 0,
		MYTHOS_UPDATE_STOP = 1
	};


	struct Widget;
	typedef int(*WidgetFunction)(Widget*, Vec2);

	typedef std::unordered_map<MYTHOS_EVENT_TYPE, WidgetFunction> WidgetEvents;

	struct Widget {
		Vec2 crd;

		WidgetEvents events;
		WidgetFunction get_event(MYTHOS_EVENT_TYPE);
		void set_event(MYTHOS_EVENT_TYPE, WidgetFunction);

		Widget(Vec2);

		virtual void update_frame() {}

		/*
		*	Updates the Widget from an event.
		*	@param type		An enum representing the type of event.
		*	@param point	A Vec2 object representing the absolute coordinates of the parent of the current Widget.
		*/
		virtual int update_event(MYTHOS_EVENT_TYPE, Vec2);

		/*
		*	Returns true if the given coordinates are in the bounds of the Widget.
		*	@param point	A Vec2 object, with coordinates relative to the parent of the current Widget.
		*/
		virtual bool in_bounds(Vec2) { return false; }

		virtual void render() = 0;
	};


	typedef std::unique_ptr<Widget> WidgetPtr;

	typedef std::list<WidgetPtr> WidgetList;

	struct ContainerWidget: public Widget {
		WidgetList child;
		int length;

		ContainerWidget(Vec2);

		void update_frame();

		int update_event(MYTHOS_EVENT_TYPE, Vec2);

		bool in_bounds(Vec2);

		void render();

		void insert(Widget*, int = 0);

		void remove(Widget*);
	};


	struct TextureWidget : public Widget {
		Texture* texture;

		TextureWidget(Texture*, Vec2);

		bool in_bounds(Vec2);

		void render();
	};

}

#endif