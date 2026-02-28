#include "Bodies.hpp"
#include <SDL3/SDL_render.h>
#include <glm/geometric.hpp>

void Body::update(float dt){
	if (onGround()) {
		vel *= (1.0f - friction * dt);
	}

	vel += acc * dt;
	pos += vel * dt;
	acc *= 0;	

	edgeCheck();
}

void Body::applyForce(glm::vec2 force){
	glm::vec2 f = force / static_cast<float>(mass);
	acc += f;
}

void Body::render(SDL_Renderer* renderer, SDL_Texture* circleTex) {
	SDL_SetTextureColorMod(circleTex, 255, 255, 255);

	SDL_FRect dst;
	dst.x = pos.x - radius;
	dst.y = pos.y - radius;
	dst.w = radius * 2;
	dst.h = radius * 2;

	SDL_RenderTexture(renderer, circleTex, NULL, &dst);
}

void Body::edgeCheck(){
	if (pos.y > windowSize.second-radius){
		pos.y = windowSize.second-(radius);
		vel.y = -vel.y;
		vel.y *= elasticity;
	}

	if (pos.y < radius){
		pos.y = radius;
		vel.y = -vel.y;
		vel.y *= elasticity;
	}

	if (pos.x > windowSize.first-radius){
		pos.x = windowSize.first - radius;
		vel.x = -vel.x;
		vel.x *= elasticity;
	}

	if (pos.x < radius){
		pos.x = radius;
		vel.x = -vel.x;
		vel.x *= elasticity;
	}
}


void Spring::update(){
	glm::vec2 force1, force2;

	force1 = body1->pos-body2->pos;
	float length1 = glm::length(force1);
	float x1 = length1-restLength;

	force1 = glm::normalize(force1) * (-1*stiffness*x1);

	body1->applyForce(force1);


	force2 = body2->pos-body1->pos;
	float length2 = glm::length(force2);
	float x2 = length2-restLength;

	force2 = glm::normalize(force2) * (-1*stiffness*x2);

	body2->applyForce(force2);
}

void Spring::render(SDL_Renderer* renderer){
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	RenderThickLine(renderer, body1->pos.x, body1->pos.y, body2->pos.x, body2->pos.y, 10);
}
