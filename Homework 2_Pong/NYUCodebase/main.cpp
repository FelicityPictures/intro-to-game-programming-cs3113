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

SDL_Window* displayWindow;

glm::mat4 projectionMatrix = glm::mat4(1.0f);
glm::mat4 modelMatrix = glm::mat4(1.0f);
glm::mat4 viewMatrix = glm::mat4(1.0f);
glm::mat4 identityMatrix = glm::mat4(1.0f);

class Entity {
public:
	float x;
	float y;
	//float rotation;
	//GLuint textureID;

	float width;
	float height;
	float vertices[12];
	float velocityX = 0.0f;
	float velocityY = 0.0f;
	float displaceX = 0.0f;
	float displaceY = 0.0f;
	bool winner = false;
	//float direction_x;
	//float direction_y;
	Entity(const float& x, const float& y, const float& width, const float& height) :
		x(x), y(y), width(width), height(height) {
		float halfWidth = width / 2;
		float halfHeight = height / 2;
		vertices[0] = x + halfWidth;
		vertices[1] = y + halfHeight;
		vertices[2] = x - halfWidth;
		vertices[3] = y + halfHeight;
		vertices[4] = x - halfWidth;
		vertices[5] = y - halfHeight;

		vertices[6] = x + halfWidth;
		vertices[7] = y + halfHeight;
		vertices[8] = x - halfWidth;
		vertices[9] = y - halfHeight;
		vertices[10] = x + halfWidth;
		vertices[11] = y - halfHeight;
	}

	void Draw(ShaderProgram &p) const {
		p.SetModelMatrix(modelMatrix);
		p.SetProjectionMatrix(projectionMatrix);
		p.SetViewMatrix(viewMatrix);
		glVertexAttribPointer(p.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(p.positionAttribute);
		glm::mat4 transformMatrix = identityMatrix;
		transformMatrix = glm::translate(transformMatrix, glm::vec3(displaceX, displaceY, 0.0f));
		p.SetModelMatrix(transformMatrix);
		if (winner) {
			p.SetColor(0.0f / 255.0f, 153.0f / 255.0f, 0.0f / 255.0f, 1.0f);
		}
		else {
			p.SetColor(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f);
		}
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(p.positionAttribute);
		glDisableVertexAttribArray(p.texCoordAttribute);
	}
	void update(float elapsed) {
		displaceX += velocityX*elapsed;
		displaceY += velocityY*elapsed;
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
	//No Textures
	ShaderProgram program;
	program.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);

	glUseProgram(program.programID);

	float lastFrameTicks = 0.0f;

    SDL_Event event;
	float leftDisplacement, rightDisplacement, pongX, pongY, pongVelocityX, pongVelocityY;
	leftDisplacement = rightDisplacement = 0.0f;
	pongX = pongY = 0.0f;
	pongVelocityX = pongVelocityY = 0.75f;
	bool rightUp, rightDown, leftUp, leftDown;
	rightUp = rightDown = leftUp = leftDown = false;
	Entity pong = Entity(0.0f, 0.0f, 0.1f, 0.1f);
	pong.velocityX = 0.75f;
	pong.velocityY = 0.75f;
	Entity leftPaddle = Entity(-1.6f, 0.0f, 0.02f, 0.2f);
	Entity rightPaddle = Entity(1.6f, 0.0f, 0.02f, 0.2f);

    bool done = false;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
			}
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
					rightUp = true;
				}
				if(event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
					rightDown = true;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_W) {
					leftUp = true;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_S) {
					leftDown = true;
				}
			}
			else if (event.type == SDL_KEYUP) {
				if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
					rightUp = false;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
					rightDown = false;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_W) {
					leftUp = false;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_S) {
					leftDown = false;
				}
			}
        }
        glClear(GL_COLOR_BUFFER_BIT);

		////PONG
		pong.Draw(program);

		//Paddles
		leftPaddle.Draw(program);
		rightPaddle.Draw(program);

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		if (rightUp == true) {
			rightPaddle.displaceY += elapsed;
		}
		if (rightDown == true) {
			rightPaddle.displaceY -= elapsed;
		}
		if (leftUp == true) {
			leftPaddle.displaceY += elapsed;
		}
		if (leftDown == true) {
			leftPaddle.displaceY -= elapsed;
		}

		//BOUNCE
		if (pong.displaceY > 1.0f || pong.displaceY < -1.0f) {
			pong.velocityY *= -1;
		}

		float XDistBetweenRightAndPong = (float)abs((rightPaddle.x + rightPaddle.displaceX) - pong.displaceX) - ((rightPaddle.width + pong.width) / 2);
		float YDistBetweenRightAndPong = (float)abs((rightPaddle.y + rightPaddle.displaceY) - pong.displaceY) - ((rightPaddle.height + pong.height) / 2);

		float XDistBetweenLeftAndPong = (float)abs((leftPaddle.x + leftPaddle.displaceX) - pong.displaceX) - ((leftPaddle.width + pong.width) / 2);
		float YDistBetweenLeftAndPong = (float)abs((leftPaddle.y + leftPaddle.displaceY) - pong.displaceY) - ((leftPaddle.height + pong.height) / 2);
		//Check right paddle
		if (XDistBetweenRightAndPong < 0.0f && YDistBetweenRightAndPong < 0.0f) {
			pong.velocityX *= -1;
		}
		//Check left paddle
		else if (XDistBetweenLeftAndPong < 0.0f && YDistBetweenLeftAndPong < 0) {
			pong.velocityX *= -1;
		}

		//Left Side Wins
		if (pong.displaceX >= 1.777f) {
			pong.velocityX = 0.0f;
			pong.velocityY = 0.0f;
			leftPaddle.winner = true;
			//glClearColor(86.0f / 255.0f, 0.0f / 255.0f, 39.0f / 255.0f, 1.0f);
		}
		//Right Side Wins
		else if (pong.displaceX <= -1.777f) {
			pong.velocityX = 0.0f;
			pong.velocityY = 0.0f;
			rightPaddle.winner = true;
			//glClearColor(0.0f / 255.0f, 54.0f / 255.0f, 58.0f / 255.0f, 1.0f);
		}
		pong.update(elapsed);
		SDL_GL_SwapWindow(displayWindow);

    }
    
    SDL_Quit();
    return 0;
}
