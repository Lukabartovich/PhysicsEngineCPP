#include "Engine.hpp"

void Engine::init(){
	lastTime = std::chrono::steady_clock::now();

	circleTex = createCircleTexture(renderer, 64);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(20.0f, 580.0f);

	int w, h;
	SDL_GetWindowSize(window, &w, &h);


	for (size_t i=0; i<2; i++){
		Body* b = new Body(dist(gen), dist(gen), 20, std::pair<int, int>(w, h));

		//Body* b = new Body(300, 60, 20, std::pair<int, int>(w, h));

		bodies.push_back(b);
	}

	Spring* spring = new Spring(bodies[0], bodies[1], 50.0f, 150.0f);
	springs.push_back(spring);
}

void Engine::render() {
	bool running = true;
	SDL_Event event;

	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}
		}

		// Delta Time
		auto now = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsed = now - lastTime;
		dt = elapsed.count();
		lastTime = now;


		for (auto* b : bodies) {
			b->applyForce(gravity*static_cast<float>(b->mass));
			b->update(dt);
		}

		for (auto* s : springs){
			s->update();
		}

		SDL_SetRenderDrawColor(renderer, 100, 50, 150, 255);
		SDL_RenderClear(renderer);

		for (auto* b : bodies) {
			b->render(renderer, circleTex);
		}

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(circleTex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


void Engine::step(){

}

void Engine::update(){

}

SDL_Texture* Engine::createCircleTexture(SDL_Renderer* renderer, int diameter) {
	SDL_Surface* surf = SDL_CreateSurface(diameter, diameter, SDL_PIXELFORMAT_RGBA32);

	Uint32* pixels = (Uint32*)surf->pixels;
	int r = diameter / 2;

	for (int y = 0; y < diameter; y++) {
		for (int x = 0; x < diameter; x++) {
			int dx = x - r;
			int dy = y - r;
			if (dx*dx + dy*dy <= r*r) {
				pixels[y * diameter + x] = 0xFFFFFFFF; // white pixel
			} else {
				pixels[y * diameter + x] = 0x00000000; // transparent
			}
		}
	}

	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_DestroySurface(surf);
	return tex;
}

