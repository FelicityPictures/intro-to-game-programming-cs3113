#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengl.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include <math.h>
#include "helper.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "stb_image.h"

class Entity {
public:
	float xPosition;
	float yPosition;
	float hitboxWidth;
	float hitboxHeight;
	int spriteIndex;
	float xVelocity = 0.0f;
	float yVelocity = 0.0f;

	Entity();
	Entity(float x, float y, int spriteIndex);

	void draw(ShaderProgram &p, const GLuint &texture) const;
	virtual void update(float timeElapsed);
};

class InelasticBox : public Entity {
public:
	InelasticBox(float x, float y, float width, float height);

	void draw(ShaderProgram &p, const GLuint &texture) const;
};

void insertInfinityShapeCoins(std::deque<std::vector<int>>& map);
void insertTopCoins(std::deque<std::vector<int>>& map);
void insertBottomCoins(std::deque<std::vector<int>>& map);
void insertMiddleCoins(std::deque<std::vector<int>>& map);

class Map {
public:
	std::deque<std::vector<int>> mapObjects;
	float xPositionOfHead;
	float speed;

	Map();
	void draw(ShaderProgram &p, const GLuint &texture) const;

	void update(float timeElapsed);

private:
	void insertNewPartIntoMap();
};

class Player : public Entity {
public:
	bool gravityDown = true;

	Player();
	void draw(ShaderProgram &p, const GLuint &texture) const;

	void update(float timeElapsed);
	void changeDirection();
	void checkInelasticCollision(const InelasticBox& box);
	size_t checkMap(Map& map);
};