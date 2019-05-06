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
	float velocityX;
	float velocityY;

	Entity();
	Entity(float x, float y, int spriteIndex);

	void draw(ShaderProgram &p) const;
	void draw(ShaderProgram &p, const GLuint &texture) const;
	virtual void update();
};

class Player : public Entity {
public:
	bool gravityDown = true;

	Player();

	void update();
};
