#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
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
#define SPRITE_COUNT_X 2
#define SPRITE_COUNT_Y 5
#define LETTER_COUNT_X 16
#define LETTER_COUNT_Y 16

SDL_Window* displayWindow;

glm::mat4 projectionMatrix = glm::mat4(1.0f);
glm::mat4 modelMatrix = glm::mat4(1.0f);
glm::mat4 viewMatrix = glm::mat4(1.0f);
glm::mat4 identityMatrix = glm::mat4(1.0f);
float defaultVertices[12] = {
	0.5f, 0.5f,
	-0.5f, 0.5f,
	-0.5f, -0.5f,
	0.5f, 0.5f,
	-0.5f, -0.5f,
	0.5f, -0.5f
};

GLuint LoadTexture(const char *filePath) {
	int w, h, comp;
	unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);

	if (image == NULL) {
		std::cout << "Unable to load image. Make sure the path is correct\n";
		assert(false);
	}

	GLuint retTexture;
	glGenTextures(1, &retTexture);
	glBindTexture(GL_TEXTURE_2D, retTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(image);
	return retTexture;
}

void drawText(ShaderProgram &p, GLuint &texture, char* str, int strLength, float x, float y, float height) {
	p.SetModelMatrix(modelMatrix);
	glVertexAttribPointer(p.positionAttribute, 2, GL_FLOAT, false, 0, defaultVertices);
	glEnableVertexAttribArray(p.positionAttribute);

	glm::mat4 transformMatrix;
	float u, v;
	float spriteWidth = 1.0f / (float)LETTER_COUNT_X;
	float spriteHeight = 1.0f / (float)LETTER_COUNT_Y;
	glBindTexture(GL_TEXTURE_2D, texture);
	for (int i = 0; i < strLength; i++) {
		u = (float)(str[i] % LETTER_COUNT_X) / (float)LETTER_COUNT_X;
		v = (float)(str[i] / LETTER_COUNT_X) / (float)LETTER_COUNT_Y;
		float texCoords[] = {
		u + spriteWidth, v,
		u, v,
		u, v + spriteHeight,
		u + spriteWidth, v,
		u, v + spriteHeight,
		u + spriteWidth, v + spriteHeight
		};
		glVertexAttribPointer(p.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(p.texCoordAttribute);
		transformMatrix = identityMatrix;
		transformMatrix = glm::translate(transformMatrix, glm::vec3(x + (height * i), y, 0.0f));
		transformMatrix = glm::scale(transformMatrix, glm::vec3(height, height, 0.0f));
		p.SetModelMatrix(transformMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	glDisableVertexAttribArray(p.positionAttribute);
	glDisableVertexAttribArray(p.texCoordAttribute);
}

class Entity {
public:
	float x;
	float y;
	float width;
	float height;
	int spriteIndex;
	float velocityX = 0.0f;
	float velocityY = 0.0f;
	Entity() : x(0.0f), y(0.0f), width(1.0f), height(1.0f) {}
	Entity(const float& x, const float& y, const float& width, const float& height, int spriteIndex) :
		x(x), y(y), width(width), height(height), spriteIndex(spriteIndex) {
	}

	void Draw(ShaderProgram &p, const GLuint &texture) const {
		p.SetModelMatrix(modelMatrix);
		glVertexAttribPointer(p.positionAttribute, 2, GL_FLOAT, false, 0, defaultVertices);
		glEnableVertexAttribArray(p.positionAttribute);
		glBindTexture(GL_TEXTURE_2D, texture);

		float u = (float)(((int)spriteIndex) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
		float v = (float)(((int)spriteIndex) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
		float spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
		float spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;
		float texCoords[] = {
		u + spriteWidth, v,
		u, v,
		u, v + spriteHeight,
		u + spriteWidth, v,
		u, v + spriteHeight,
		u + spriteWidth, v + spriteHeight
		};
		glVertexAttribPointer(p.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(p.texCoordAttribute);

		glm::mat4 transformMatrix = identityMatrix;
		transformMatrix = glm::translate(transformMatrix, glm::vec3(x, y, 0.0f));
		transformMatrix = glm::scale(transformMatrix, glm::vec3(width, height, 0.0f));
		p.SetModelMatrix(transformMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(p.positionAttribute);
		glDisableVertexAttribArray(p.texCoordAttribute);
	}
};


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
	ShaderProgram program;
	program.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);
	glUseProgram(program.programID);
	GLuint spriteSheet = LoadTexture(RESOURCE_FOLDER"Space Invaders Sprite Sheet.png");
	GLuint textSheet = LoadTexture(RESOURCE_FOLDER"Letters.png");
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float lastFrameTicks = 0.0f;
	float ticks, elapsed;
	float elapsedForAliens = 0.0f;
	float elapsedForBullets = 0.0f;

    SDL_Event event;
	int score = 0;
	Entity laser = Entity(0.0f, -0.9f, 0.2f, 0.2f, 8);
	Entity aliens[NUMBER_OF_ALIENS];
	float alienVelocity = 0.075f;
	float leftmostAlien = -1.6f;
	Entity bullets[MAX_BULLETS];
	int currentNumberOfBullets = 0;
	int currentNumberOfAliens = NUMBER_OF_ALIENS;
	for (int i = 0; i < NUMBER_OF_ALIENS; i++) {
		float newX = -1.5f + (0.2f * (i % 10));
		float newY = 0.8f - (0.2f * floor(i / 10.0f));
		aliens[i] = Entity(newX, newY, 0.17f, 0.17f, (int)floor(i / SPRITE_COUNT_Y));
	}

    bool done = false;

	bool leftKeyDown, rightKeyDown, spaceKeyDown;
	leftKeyDown = rightKeyDown = spaceKeyDown = false;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
			}
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
					leftKeyDown = true;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
					rightKeyDown = true;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					spaceKeyDown = true;
				}
			}
			else if (event.type == SDL_KEYUP) {
				if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
					leftKeyDown = false;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
					rightKeyDown = false;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					spaceKeyDown = false;
				}
			}
        }
        glClear(GL_COLOR_BUFFER_BIT);
		program.SetProjectionMatrix(projectionMatrix);
		program.SetViewMatrix(viewMatrix);

		//TIMING
		ticks = (float)SDL_GetTicks() / 1000.0f;
		elapsed = ticks - lastFrameTicks;
		elapsedForAliens += elapsed;
		elapsedForBullets += elapsed;
		lastFrameTicks = ticks;

		//DRAW THINGS
		laser.Draw(program, spriteSheet);
		for (int i = 0; i < currentNumberOfAliens; i++) {
			aliens[i].Draw(program, spriteSheet);
		}
		for (int i = 0; i < currentNumberOfBullets; i++) {
			bullets[i].Draw(program, spriteSheet);
		}
		char text[] = "Score 0000";
		int temporaryScore = score;
		for (int i = 0; i < 4; i++) {
			text[9-i] = '0' + (temporaryScore % 10);
			temporaryScore = floor(temporaryScore / 10);
		}
		drawText(program, textSheet, text, 10, -1.7f, 0.9f, 0.1f);
		//drawText(ShaderProgram &p, GLuint &texture, char* str, int strLength, float x, float y, float height) {

		//COLLISION AND CLEANUP CALCULATION
		for (int i = 0; i < currentNumberOfBullets; i++) {
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
		}

		//CALCULATE MOVEMENT
		if (elapsedForAliens >= 1.0f) {
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
		}
		
		if (leftKeyDown) {
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
		}
		for (int i = 0; i < currentNumberOfBullets; i++) {
			bullets[i].y += (2.5f * elapsed);
		}
		SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
