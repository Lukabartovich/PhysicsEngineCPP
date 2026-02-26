#include "common.hpp"
#include "Engine.hpp"

int main(int argc, char* argv[]) {
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;   


	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("SDL_Init Error: %s", SDL_GetError());
		return 1;
	} 

	if (!SDL_CreateWindowAndRenderer("SDL3 Test Window", 600, 600, 0, &window, &renderer)) {
		SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
		SDL_Quit();
		return 1;
	}


	Engine engine(renderer, window);
	engine.init();
	engine.render();


	return 0;
}
