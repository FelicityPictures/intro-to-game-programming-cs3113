#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "ShaderProgram.h"
#include "helper.h"
#include "Entity.h"
#include "GameState.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define LEVEL_HEIGHT 10
#define TILE_SIZE 0.15f
#define FIXED_TIMESTEP 0.0166666f

SDL_Window* displayWindow;
glm::mat4 projectionMatrix = glm::mat4(1.0f);
glm::mat4 viewMatrix = glm::mat4(1.0f);

int main(int argc, char *argv[]){
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Felicity's Final Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

	glViewport(0, 0, 640, 360);
	projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	Mix_Music *bgm;
	bgm = Mix_LoadMUS("bgm.mp3");
	Mix_PlayMusic(bgm, -1);


	float lastFrameTicks = 0.0f;
	float timeAccumulator = 0.0f;

	GameState state = GameState();
	int mode = GameMode::STATE_MAIN_MENU;
	//int mode = GameMode::STATE_SINGLE_PLAYER_PLAY;
	//int mode = GameMode::STATE_SINGLE_PLAYER_GAME_OVER;
	state.program.SetProjectionMatrix(projectionMatrix);
	state.program.SetViewMatrix(viewMatrix);
	state.untexturedProgram.SetProjectionMatrix(projectionMatrix);
	state.untexturedProgram.SetViewMatrix(viewMatrix);

    SDL_Event event;
    bool done = false;

	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	while (!done) {
        while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN) {
				float unitX = (((float)event.button.x / 640.0f) * 3.554f) - 1.777f;
				float unitY = (((float)(360 - event.button.y) / 360.0f) * 2.0f) - 1.0f;
				if (mode == GameMode::STATE_MAIN_MENU) {
					if (state.titleScreenButtons[0].clicked(unitX, unitY)) {
						mode = GameMode::STATE_SINGLE_PLAYER_PLAY;
					}
					else if (state.titleScreenButtons[1].clicked(unitX, unitY)) {
						//MULTIPLAYER
					}
					else if (state.titleScreenButtons[2].clicked(unitX, unitY)) {
						done = true;
					}
				}
				else if (mode == GameMode::STATE_SINGLE_PLAYER_GAME_OVER) {
					if (state.gameOverButtons[0].clicked(unitX, unitY)) {
						state.reset();
						mode = GameMode::STATE_SINGLE_PLAYER_PLAY;
					}
					else if (state.gameOverButtons[1].clicked(unitX, unitY)) {
						state.reset();
						mode = GameMode::STATE_MAIN_MENU;
					}
					else if (state.gameOverButtons[2].clicked(unitX, unitY)) {
						done = true;
					}
				}
			}
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE && state.player.timeDead <= 0.0f) {
					state.player.changeDirection();
					if (state.player.gravityDown) {
						state.bottom.spriteIndex = 7;
						state.top.spriteIndex = 6;
					}
					else {
						state.bottom.spriteIndex = 6;
						state.top.spriteIndex = 7;
					}
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_E) {
					state.enemies.push_back(Enemy(state.player.yPosition, 0.5f, 0.5f));
				}
			}
        }
		glClear(GL_COLOR_BUFFER_BIT);

		// TIMING and UPDATE
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		if (mode == GameMode::STATE_SINGLE_PLAYER_PLAY) {
			timeAccumulator += ticks - lastFrameTicks;
			while (timeAccumulator >= FIXED_TIMESTEP) {
				if (state.UpdateGame(FIXED_TIMESTEP)) {
					mode = GameMode::STATE_SINGLE_PLAYER_GAME_OVER;
				}
				timeAccumulator -= FIXED_TIMESTEP;
			}
		}
		lastFrameTicks = ticks;

		// DRAWING
		state.RenderGame(mode);

		glClearColor(205.0f / 255.0f, 205.0f / 255.0f, 205.0f / 255.0f, 1.0f);
		SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
