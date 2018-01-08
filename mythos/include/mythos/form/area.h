#ifndef MYTHOS_AREA_H
#define MYTHOS_AREA_H

#include <map>
#include "form.h"


struct Tile {
	Texture*	texture;
	int			elevation;
};

struct Area: public Container {
	Tile** tile;
	unsigned size;				// size of tile array
	unsigned width;				// width of tiles
};


typedef std::multimap<int, Form*> AWInnerFormList;
typedef std::map<int, AWInnerFormList> AWFormList;
typedef std::multimap<int, Tile*> AWInnerTileList;
typedef std::map<int, AWInnerTileList> AWTileList;

struct AreaWrapper {
	Point angle;

	Area* area;
	Area* prox[8];

	AWFormList form;
	AWTileList tile;

	AreaWrapper();
	AreaWrapper(Area*);
	void set_area(Area*);
	void push(Area*, int, int);
	void reset();
};


#endif // MYTHOS_AREA_H
