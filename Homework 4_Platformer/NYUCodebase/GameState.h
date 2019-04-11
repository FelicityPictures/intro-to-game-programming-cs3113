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
#include "glm/mat4x4.hpp"

class GameState {
public:
	Entity player;
	extern Entity enemies[NUMBER_OF_ALIENS];
	extern Entity bullets[MAX_BULLETS];
	int score;
	int currentNumberOfBullets, currentNumberOfAliens;

	void RenderGame(ShaderProgram &p);
	void UpdateGame(bool* keyDowns, float elapsed);
};