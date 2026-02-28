#pragma once
#include "common.hpp"


class Body{
	public:
		glm::vec2 pos;
		float radius = 20.0f;
		int mass = radius;
		float friction = 5.0f;

		std::pair<float, float> windowSize;

		glm::vec2 vel{0, 0};
		glm::vec2 acc{0, 0};

		float elasticity = 0.4f;

		Body(int x, int y, int radius, std::pair<int, int> size) : radius(radius){
			pos.x = x;
			pos.y = y;

			windowSize.first = static_cast<float>(size.first);
			windowSize.second = static_cast<float>(size.second);
		}

		void update(float dt);

		void applyForce(glm::vec2 force);

		void render(SDL_Renderer* renderer, SDL_Texture* circleTex);

		void edgeCheck();

		bool onGround(){
			return (pos.y>windowSize.first-radius-1);
		}
};

class Spring{
	public:
		Body* body1;
		Body* body2;

		float stiffness;

		float restLength;

		Spring(Body* body1, Body* body2, float stiffness, float restLength): body1(body1), body2(body2),
		stiffness(stiffness), restLength(restLength){

		}

		void update();

		void render(SDL_Renderer* renderer);

		void RenderThickLine(SDL_Renderer *renderer, float x1, float y1, float x2, float y2,
				float thickness){
			float dx = x2 - x1;
			float dy = y2 - y1;
			float len = SDL_sqrtf(dx*dx + dy*dy);
			if (len == 0) return;

			dx /= len;
			dy /= len;

			float px = -dy * (thickness / 2.0f);
			float py =  dx * (thickness / 2.0f);

			Uint8 r, g, b, a;
			SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

			SDL_FColor col = {
				r / 255.0f,
				g / 255.0f,
				b / 255.0f,
				a / 255.0f
			};

			SDL_Vertex verts[4] = {
				{ {x1 + px, y1 + py}, col, {0,0} },
				{ {x1 - px, y1 - py}, col, {0,1} },
				{ {x2 + px, y2 + py}, col, {1,0} },
				{ {x2 - px, y2 - py}, col, {1,1} }
			};

			int indices[6] = {0,1,2, 2,1,3};

			SDL_RenderGeometry(renderer, NULL, verts, 4, indices, 6);
		}
};
