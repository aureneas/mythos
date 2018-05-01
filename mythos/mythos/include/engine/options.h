#ifndef MYTHOS_OPTIONS_H
#define MYTHOS_OPTIONS_H

namespace mythos_engine {

	struct Options {
		const char*		title = "Mythos Debug";		// title of window
		
		int				width = 640;				// width of window
		int				height = 480;				// height of window
		double			resX = 1.0;					// x-axis resolution
		double			resY = 1.0;					// y-axis resolution

		bool			free_resize = false;		// can the screen be resized at will?
	};

	void load_options(void);

	Options& get_options(void);

}

#endif