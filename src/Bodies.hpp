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


};
