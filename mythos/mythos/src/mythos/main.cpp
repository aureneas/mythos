#include "../../include/mythos/mythos.h"

#include "../../include/mythos/engine/state.h"
#include "../../include/mythos/engine/texture.h"

int main(int argc, char* argv[]) {
	mythos_engine::mythos_init(&argc, argv);

	mythos_engine::Texture2D_Info t1 = mythos_engine::load_texture("mythos/lib/test1.png");
	mythos_engine::ImageTexture img1(&t1);
	mythos_engine::push_layer(new mythos_engine::TextureWidget(&img1, Vec2(0, 0)));

	mythos_engine::mythos_run();
}