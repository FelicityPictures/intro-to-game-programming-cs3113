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
	projectionMatrix = glm::ortho(-widthRatio, widthRatio, -heightRatio, heightRatio, -1.0f, 1.0f);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	Mix_Music *bgm;
	bgm = Mix_LoadMUS("bgm.mp3");
	Mix_PlayMusic(bgm, -1);


	GLuint spriteSheet = LoadTexture(RESOURCE_FOLDER"sprites-01.png");
	GLuint textSheet = LoadTexture(RESOURCE_FOLDER"pixel_font.png");
	GLuint backgroundTexture1 = LoadTexture(RESOURCE_FOLDER"background1.png");
	GLuint backgroundTexture2 = LoadTexture(RESOURCE_FOLDER"background2.png");
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
	vector<Enemy> enemies;
	size_t score = 0;
	char scoreText[] = "Score 0000";
	float timeSurvived = 0.0f;
	float rocketTimer = 0.0f;
	Background backgrounds = Background(backgroundTexture1, backgroundTexture2);

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
				else if (event.key.keysym.scancode == SDL_SCANCODE_E) {
					enemies.push_back(Enemy(player.yPosition, 0.5f, 0.5f));
				}
			}
        }
		glClear(GL_COLOR_BUFFER_BIT);

		// TIMING and UPDATE
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		timeAccumulator += ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		while (timeAccumulator >= FIXED_TIMESTEP) {
			timeAccumulator -= FIXED_TIMESTEP;

			player.update(FIXED_TIMESTEP);
			if (player.timeDead <= 0.0f) {
				backgrounds.update(FIXED_TIMESTEP, timeSurvived);
				map.update(FIXED_TIMESTEP, timeSurvived);
				player.checkInelasticCollision(top);
				player.checkInelasticCollision(bottom);
				score += player.checkMap(map);
				for (size_t i = 0; i < enemies.size(); i++) {
					if (enemies[i].update(FIXED_TIMESTEP, player.yPosition, timeSurvived)) {
						swap(enemies[i], enemies[enemies.size() - 1]);
						enemies.pop_back();
					}
					else {
						player.collideWithRocket(enemies[i]);
					}
				}
				int numberOfValidRockets = floor(score / 100.0);
				if (enemies.size() < numberOfValidRockets && rocketTimer > 2.0f) {
					int random = rand() % 5;
					if (random > (5 - floorf(timeSurvived / 10.0f))) {
						enemies.push_back(Enemy(player.yPosition, 0.5f, 0.5f));
						rocketTimer = 0.0f;
						if (enemies.size() >= numberOfValidRockets) {
							rocketTimer = -10.0f;
						}
					}
				}
				rocketTimer += FIXED_TIMESTEP;
				timeSurvived += FIXED_TIMESTEP;
			}
			else {
				Mix_HaltMusic();
			}
			
		}

		// DRAWING
		backgrounds.draw(program);
		top.draw(program, spriteSheet);
		bottom.draw(program, spriteSheet);
		map.draw(program, spriteSheet);
		player.draw(program, spriteSheet);
		for (size_t i = 0; i < enemies.size(); i++) {
			enemies[i].draw(program, spriteSheet);
		}
		int temporaryScore = score;
		for (int i = 0; i < 4; i++) {
			scoreText[9 - i] = '0' + (temporaryScore % 10);
			temporaryScore = floor(temporaryScore / 10);
		}
		drawText(program, textSheet, scoreText, 10, -1.7f, 0.95f, 0.05f);

		glClearColor(205.0f / 255.0f, 205.0f / 255.0f, 205.0f / 255.0f, 1.0f);
		SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
