#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "helper.h"
#include "Entity.h"
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
float pixelWidth = 640;
float pixelHeight = 360;

int main(int argc, char *argv[]){
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Felicity's Final Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

	glViewport(0, 0, pixelWidth, pixelHeight);
	//Textures
	ShaderProgram program;
	float widthRatio = pixelWidth / pixelHeight;
	float heightRatio = 1.0f;
	program.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	//program.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	projectionMatrix = glm::ortho(-widthRatio, widthRatio, -heightRatio, heightRatio, -1.0f, 1.0f);

	GLuint spriteSheet = LoadTexture(RESOURCE_FOLDER"sprites-01.png");
	GLuint textSheet = LoadTexture(RESOURCE_FOLDER"letters.png");
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(program.programID);

	float lastFrameTicks = 0.0f;
	float timeAccumulator = 0.0f;
	Player player = Player();
	InelasticBox top = InelasticBox(0.0f, heightRatio - 0.05f, widthRatio * 2.0f, 0.1f);
	InelasticBox bottom = InelasticBox(0.0f, (-heightRatio) + 0.05f, widthRatio * 2.0f, 0.1f);
	bottom.spriteIndex = 7;
	Map map;
	size_t score = 0;
	char scoreText[] = "Score 0000";

    SDL_Event event;
    bool done = false;

	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);

	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	while (!done) {
        while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					player.changeDirection();
					if (player.gravityDown) {
						bottom.spriteIndex = 7;
						top.spriteIndex = 6;
					}
					else {
						bottom.spriteIndex = 6;
						top.spriteIndex = 7;
					}
				}
			}
        }
		glClear(GL_COLOR_BUFFER_BIT);

		//TIMING
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		timeAccumulator += ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		while (timeAccumulator >= FIXED_TIMESTEP) {
			timeAccumulator -= FIXED_TIMESTEP;
			player.update(FIXED_TIMESTEP);
			map.update(FIXED_TIMESTEP);
			player.checkInelasticCollision(top);
			player.checkInelasticCollision(bottom);
			score += player.checkMap(map);
		}

		top.draw(program, spriteSheet);
		bottom.draw(program, spriteSheet);
		map.draw(program, spriteSheet);
		player.draw(program, spriteSheet);
		int temporaryScore = score;
		for (int i = 0; i < 4; i++) {
			scoreText[9 - i] = '0' + (temporaryScore % 10);
			temporaryScore = floor(temporaryScore / 10);
		}
		drawText(program, textSheet, scoreText, 10, -1.7f, 0.9f, 0.1f);

		glClearColor(205.0f / 255.0f, 205.0f / 255.0f, 205.0f / 255.0f, 1.0f);
		SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
