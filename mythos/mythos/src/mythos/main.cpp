#include "../../include/mythos/mythos.h"

#include "../../include/mythos/engine/state.h"
#include "../../include/mythos/engine/texture.h"
#include "../../include/mythos/engine/functions/mouse_handlers.h"

#include "../../include/mythos/debug/debug.h"

using namespace mythos_engine;

int main(int argc, char* argv[]) {
	debug_init();
	mythos_init(&argc, argv);

	Texture2D_Info tbg = load_texture("mythos/lib/checkerboard.png");
	ImageTexture img_bg(&tbg);
	TextureWidget* tex_bg = new TextureWidget(&img_bg, Vec2(0, 0));
	push_layer(tex_bg);

	Texture2D_Info t1 = load_texture("mythos/lib/test1.png");
	ImageTexture img1(&t1);
	TextureWidget* tex_widg = new TextureWidget(&img1, Vec2(250, 250));
	tex_widg->set_event(MYTHOS_EVENT_MOUSE_BUTTON_DOWN, mouse_button_down_draggable);
	tex_widg->set_event(MYTHOS_EVENT_MOUSE_BUTTON_UP, mouse_button_up_draggable);
	//tex_widg->set_event(MYTHOS_EVENT_MOUSE_MOTION_PRESSED, mouse_motion_pressed_draggable);
	push_layer(tex_widg);

	mythos_run();
	debug_close();
}