#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengl.h>
#include <vector>
#include <deque>
#include <math.h>
#include "Entity.h"
#include "helper.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "stb_image.h"
#include <SDL_mixer.h>

enum GameMode { STATE_MAIN_MENU,
	STATE_SINGLE_PLAYER_PLAY, STATE_SINGLE_PLAYER_GAME_OVER,
	STATE_TWO_PLAYER_PLAY, STATE_TWO_PLAYER_GAME_OVER};

class GameState {
public:
	ShaderProgram program;
	ShaderProgram untexturedProgram;
	Mix_Music *bgm;

	GLuint titleScreen;
	Button titleScreenButtons[3];

	GLuint gameOverScreen;
	Button gameOverButtons[3];

	GLuint spriteSheet, textSheet;
	Background backgrounds;

	Player player[2];
	InelasticBox top, bottom;
	Map map;
	std::vector<Enemy> enemies;
	size_t P1score = 0;
	size_t P2score = 0;
	float timeSurvived = 0.0f;
	float rocketTimer = 0.0f;

	GameState();
	void RenderGame(int mode);
	bool UpdateGame(float elapsed);
	void ProcessInput(float elapsed);

	void reset();
};