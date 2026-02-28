#pragma once
#include "common.hpp"
#include <chrono>

#include "Bodies.hpp"

class Engine{
	public:
		SDL_Renderer* renderer;
		SDL_Window* window;
		
		SDL_Texture* circleTex = nullptr;
		float dt = 0.0f;

		glm::vec2 gravity {0, 150};

		Engine(SDL_Renderer * renderer, SDL_Window * window) : renderer(renderer), window(window){}

		std::vector<Body *> bodies;
		std::vector<Spring *> springs;

		void init();

		void render();

		void step();

		void update();

		SDL_Texture* createCircleTexture(SDL_Renderer* renderer, int diameter);

		void drawPolygon(const std::vector<glm::vec2>& points, const SDL_FColor color);

	private:
		std::chrono::steady_clock::time_point lastTime;
};
