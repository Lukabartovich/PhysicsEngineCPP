#include "Engine.hpp"
#include <SDL3/SDL_render.h>

void Engine::init(){
	lastTime = std::chrono::steady_clock::now();

	circleTex = createCircleTexture(renderer, 64);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(20.0f, 580.0f);

	int w, h;
	SDL_GetWindowSize(window, &w, &h);


	//for (size_t i=0; i<2; i++){
	//Body* b = new Body(dist(gen), dist(gen), 20, std::pair<int, int>(w, h));
	//
	////Body* b = new Body(300, 60, 20, std::pair<int, int>(w, h));
	//
	//bodies.push_back(b);
	//}

	bodies.push_back(new Body(0, 0, 20, std::pair<int, int>(w, h)));
	bodies.push_back(new Body(150, 0, 20, std::pair<int, int>(w, h)));
	bodies.push_back(new Body(0, 150, 20, std::pair<int, int>(w, h)));
	bodies.push_back(new Body(150, 150, 20, std::pair<int, int>(w, h)));

	float stiff = 1500.0f;

	springs.push_back(new Spring(bodies[0], bodies[1], stiff, 150.0f));
	springs.push_back(new Spring(bodies[1], bodies[2], stiff, 150.0f));
	springs.push_back(new Spring(bodies[2], bodies[3], stiff, 150.0f));
	springs.push_back(new Spring(bodies[3], bodies[0], stiff, 150.0f));

	springs.push_back(new Spring(bodies[0], bodies[2], stiff, 150.0f));
	springs.push_back(new Spring(bodies[1], bodies[3], stiff, 150.0f));

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

		if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_LMASK) {
		for (auto* b : bodies) {
			b->render(renderer, circleTex);
		}

		for (auto* s : springs){
			s->render(renderer);
		}	
		}else{
			drawPolygon(std::vector<glm::vec2>{
				{bodies[0]->pos.x-bodies[0]->radius, bodies[0]->pos.y-bodies[0]->radius},
				{bodies[1]->pos.x+bodies[1]->radius, bodies[1]->pos.y-bodies[1]->radius},
				{bodies[3]->pos.x+bodies[3]->radius, bodies[3]->pos.y+bodies[3]->radius},
				{bodies[2]->pos.x-bodies[2]->radius, bodies[2]->pos.y+bodies[2]->radius},},
				SDL_FColor{1, 1, 1, 1});
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

void Engine::drawPolygon(const std::vector<glm::vec2>& points, const SDL_FColor color){
	std::vector<SDL_Vertex> verts(points.size());

	float minX = +FLT_MAX, minY = +FLT_MAX;
	float maxX = -FLT_MAX, maxY = -FLT_MAX;
	for (auto& p : points){
		minX = std::min(minX, p.x);
		minY = std::min(minY, p.y);
		maxX = std::max(maxX, p.x);
		maxY = std::max(maxY, p.y);
	}
	float w = maxX - minX;
	float h = maxY - minY;


	for (size_t i=0; i<points.size(); i++){
		float u = (points[i].x - minX) / w;
		float v = (points[i].y - minY) / h;

		verts[i].position = {points[i].x, points[i].y};
		verts[i].tex_coord = {u, v};
		verts[i].color = color;
	}

	std::vector<int> indices;
	for (int i = 1; i < points.size() - 1; i++){
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 1);
	}

	SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), indices.data(), indices.size());
}
