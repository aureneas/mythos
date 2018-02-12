#ifndef MYTHOS_OPTIONS_H
#define MYTHOS_OPTIONS_H

namespace mythos_engine {

	struct Options {
		int		width;			// width of screen
		int		height;			// height of screen
		double	resX;			// x-axis resolution
		double	resY;			// y-axis resolution

		bool	free_resize;	// can the screen be resized at will?
	};

	void load_options(void);

	Options& get_options(void);

}

#endif