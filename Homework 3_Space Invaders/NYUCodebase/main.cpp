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

	//Textures
	/*ShaderProgram program;
	program.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	glUseProgram(program.programID);
	GLuint spriteSheet = LoadTexture(RESOURCE_FOLDER"Space Invaders Sprite Sheet.png");
	GLuint textSheet = LoadTexture(RESOURCE_FOLDER"Letters.png");
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

	projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);
	float lastFrameTicks = 0.0f;
	float ticks, elapsed;
	/*float elapsedForAliens = 0.0f;
	float elapsedForBullets = 0.0f;*/

	/*int score = 0;
	Entity laser = Entity(0.0f, -0.9f, 0.2f, 0.2f, 8);
	Entity aliens[NUMBER_OF_ALIENS];
	float alienVelocity = 0.075f;
	float leftmostAlien = -1.6f;
	Entity bullets[MAX_BULLETS];
	int currentNumberOfBullets = 0;
	int currentNumberOfAliens = NUMBER_OF_ALIENS;*/
	/*for (int i = 0; i < NUMBER_OF_ALIENS; i++) {
		float newX = -1.5f + (0.2f * (i % 10));
		float newY = 0.8f - (0.2f * floor(i / 10.0f));
		aliens[i] = Entity(newX, newY, 0.17f, 0.17f, (int)floor(i / SPRITE_COUNT_Y));
	}*/

    SDL_Event event;
    bool done = false;
	GameState state;
	state.program.SetProjectionMatrix(projectionMatrix);
	state.program.SetViewMatrix(viewMatrix);

	/*bool leftKeyDown, rightKeyDown, spaceKeyDown;
	leftKeyDown = rightKeyDown = spaceKeyDown = false;*/
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
			}
			else if (event.type == SDL_KEYDOWN) {
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
        }
		glClear(GL_COLOR_BUFFER_BIT);

		//DRAW WITH GAME STATE
		state.RenderGame();

		//TIMING
		ticks = (float)SDL_GetTicks() / 1000.0f;
		elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		/*elapsedForAliens += elapsed;
		elapsedForBullets += elapsed;*/

		//UPDATE GAME
		state.UpdateGame(elapsed);
		////DRAW THINGS
		//laser.Draw(program, spriteSheet);
		//for (int i = 0; i < currentNumberOfAliens; i++) {
		//	aliens[i].Draw(program, spriteSheet);
		//}
		//for (int i = 0; i < currentNumberOfBullets; i++) {
		//	bullets[i].Draw(program, spriteSheet);
		//}
		//char text[] = "Score 0000";
		//int temporaryScore = score;
		//for (int i = 0; i < 4; i++) {
		//	text[9-i] = '0' + (temporaryScore % 10);
		//	temporaryScore = floor(temporaryScore / 10);
		//}
		//drawText(program, textSheet, text, 10, -1.7f, 0.9f, 0.1f);
		////drawText(ShaderProgram &p, GLuint &texture, char* str, int strLength, float x, float y, float height) {

		//COLLISION AND CLEANUP CALCULATION
		/*for (int i = 0; i < currentNumberOfBullets; i++) {
			if (bullets[i].y > 1.0f) {
				bullets[i] = bullets[currentNumberOfBullets - 1];
				currentNumberOfBullets--;
			}
			else {
				for (int m = 0; m < currentNumberOfAliens; m++) {
					float XDistBetweenBulletAndAlien = (float)abs(bullets[i].x - aliens[m].x) - ((bullets[i].width + aliens[m].width) / 2);
					float YDistBetweenBulletAndAlien = (float)abs(bullets[i].y - aliens[m].y) - ((bullets[i].height + aliens[m].height) / 2);
					if (XDistBetweenBulletAndAlien < 0.0f && YDistBetweenBulletAndAlien < 0.0f) {
						bullets[i] = bullets[currentNumberOfBullets - 1];
						currentNumberOfBullets--;
						aliens[m] = aliens[currentNumberOfAliens - 1];
						currentNumberOfAliens--;
						score += 5;
						break;
					}
				}
			}
		}*/

		//CALCULATE MOVEMENT
		/*if (elapsedForAliens >= 1.0f) {
			if (leftmostAlien > -0.2f || leftmostAlien < -1.6f) {
				for (int i = 0; i < currentNumberOfAliens; i++) {
					aliens[i].y -= 0.2f;
				}
				alienVelocity *= -1;
			}
			for (int i = 0; i < currentNumberOfAliens; i++) {
				aliens[i].x += (alienVelocity * elapsedForAliens);
				if (aliens[i].spriteIndex % 2 == 0) {
					aliens[i].spriteIndex++;
				}
				else {
					aliens[i].spriteIndex--;
				}
			}
			leftmostAlien += (alienVelocity * elapsedForAliens);
			elapsedForAliens = 0.0f;
		}*/
		
		//Keyboard interactions
		/*if (leftKeyDown) {
			laser.x -= (1.0f * elapsed);
		}
		if (rightKeyDown) {
			laser.x += (1.0f * elapsed);
		}
		if (spaceKeyDown && elapsedForBullets >= 0.5f) {
			//create bullet
			if (currentNumberOfBullets < MAX_BULLETS) {
				bullets[currentNumberOfBullets] = Entity(laser.x, laser.y, 0.02f, 0.1f, 9);
				currentNumberOfBullets++;
			}
			elapsedForBullets = 0.0f;
		}*/
		/*for (int i = 0; i < currentNumberOfBullets; i++) {
			bullets[i].y += (2.5f * elapsed);
		}*/
		SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
