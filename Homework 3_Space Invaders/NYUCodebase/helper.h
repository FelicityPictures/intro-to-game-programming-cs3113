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
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "stb_image.h"
#define NUMBER_OF_ENEMIES 30
#define MAX_BULLETS 10

GLuint LoadTexture(const char *filePath);
void drawText(ShaderProgram &p, GLuint &texture, char* str, int strLength, float x, float y, float height);
enum GameMode { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };

class Entity {
public:
	float x;
	float y;
	float width;
	float height;
	int spriteIndex;
	float velocityX;
	float velocityY;
	Entity();
	Entity(const float& x, const float& y, const float& width, const float& height, int spriteIndex);

	void Draw(ShaderProgram &p, const GLuint &texture) const;
};

class GameState {
public:
	ShaderProgram program;
	GLuint spriteSheet, textSheet;

	Entity player = Entity(0.0f, -0.9f, 0.2f, 0.2f, 8);
	Entity enemies[NUMBER_OF_ENEMIES];
	Entity bullets[MAX_BULLETS];
	int score = 0;
	int currentNumberOfAliens = NUMBER_OF_ENEMIES;
	int currentNumberOfBullets = 0;
	float leftmostAlien = -1.6f;
	float elapsedForAliens = 0.0f;
	float elapsedForBullets = 0.0f;
	bool keyDowns[3];
	//left, right, space

	GameState();
	void RenderGame(int mode);
	void UpdateGame(float elapsed);
	void ProcessInput(float elapsed);
};