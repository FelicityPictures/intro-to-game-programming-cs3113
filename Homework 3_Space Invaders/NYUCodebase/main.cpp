#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "helper.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define NUMBER_OF_ALIENS 30
#define MAX_BULLETS 10
#define LETTER_COUNT_X 16
#define LETTER_COUNT_Y 16
#define SPRITE_COUNT_X 2
#define SPRITE_COUNT_Y 5

SDL_Window* displayWindow;
glm::mat4 projectionMatrix = glm::mat4(1.0f);
glm::mat4 viewMatrix = glm::mat4(1.0f);

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Felicity's Homework #2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

	glViewport(0, 0, 640, 360);

	projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);
	float lastFrameTicks = 0.0f;
	float ticks, elapsed;

    SDL_Event event;
    bool done = false;
	int mode = GameMode::STATE_MAIN_MENU;
	GameState state;
	state.program.SetProjectionMatrix(projectionMatrix);
	state.program.SetViewMatrix(viewMatrix);

    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
			}
			switch (mode) {
				case GameMode::STATE_GAME_LEVEL:
					if (event.type == SDL_KEYDOWN) {
						if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
							state.keyDowns[0] = true;
						}
						if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
							state.keyDowns[1] = true;
						}
						if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
							state.keyDowns[2] = true;
						}
					}
					else if (event.type == SDL_KEYUP) {
						if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
							state.keyDowns[0] = false;
						}
						if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
							state.keyDowns[1] = false;
						}
						if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
							state.keyDowns[2] = false;
						}
					}
				break;
				case GameMode::STATE_MAIN_MENU:
					if (event.type == SDL_KEYUP && event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
						mode = GameMode::STATE_GAME_LEVEL;
					}
				break;
			}
        }
		glClear(GL_COLOR_BUFFER_BIT);


		//DRAW WITH GAME STATE
		state.RenderGame(mode);

		if (mode == GameMode::STATE_GAME_LEVEL) {
			//TIMING
			ticks = (float)SDL_GetTicks() / 1000.0f;
			elapsed = ticks - lastFrameTicks;
			lastFrameTicks = ticks;
			//UPDATE GAME
			state.UpdateGame(elapsed);
		}

		SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
