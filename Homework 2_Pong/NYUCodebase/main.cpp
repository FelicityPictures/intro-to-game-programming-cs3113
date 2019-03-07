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

//float* hexToNumbers(char x[]) {
//	if (x.length != 6) {
//		return NULL;
//	}
//	float ret[3];
//	ret[0] = (((int)x[0] * 16) + (int)x[1]) / 255.0f;
//	ret[1] = (((int)x[2] * 16) + (int)x[3]) / 255.0f;
//	ret[2] = (((int)x[4] * 16) + (int)x[5]) / 255.0f;
//	return ret;
//}

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
	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	glm::mat4 identityMatrix = glm::mat4(1.0f);
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

		//PONG
		program.SetModelMatrix(modelMatrix);
		program.SetProjectionMatrix(projectionMatrix);
		program.SetViewMatrix(viewMatrix);
		float pong[] = { 0.05f, 0.05f, -0.05f, 0.05f, -0.05f, -0.05f, 0.05f, 0.05f, -0.05f, -0.05f, 0.05f, -0.05f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, pong);
		glEnableVertexAttribArray(program.positionAttribute);
		glm::mat4 pongTransformMatrix = identityMatrix;
		pongTransformMatrix = glm::translate(pongTransformMatrix, glm::vec3(pongX, pongY, 0.0f));
		program.SetModelMatrix(pongTransformMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		//Left Paddle
		program.SetModelMatrix(modelMatrix);
		program.SetProjectionMatrix(projectionMatrix);
		program.SetViewMatrix(viewMatrix);
		float left[] = { 0.01f, 0.1f, -0.01f, 0.1f, -0.01f, -0.1f, 0.01f, 0.1f, -0.01f, -0.1f, 0.01f, -0.1f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, left);
		glEnableVertexAttribArray(program.positionAttribute);

		glm::mat4 leftTransformMatrix = identityMatrix;
		leftTransformMatrix = glm::translate(leftTransformMatrix, glm::vec3(-1.6f, leftDisplacement, 0.0f));
		program.SetModelMatrix(leftTransformMatrix);
		program.SetColor(248.0 / 255.0f, 187.0 / 255.0f, 208.0 / 255.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glm::mat4 rightTransformMatrix = identityMatrix;
		rightTransformMatrix = glm::translate(rightTransformMatrix, glm::vec3(1.6f, rightDisplacement, 0.0f));
		program.SetModelMatrix(rightTransformMatrix);
		program.SetColor(178.0 / 255.0f, 235.0 / 255.0f, 2042.0 / 255.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		pongX += (elapsed*pongVelocityX);
		pongY += (elapsed*pongVelocityY);

		
		if (rightUp == true) {
			rightDisplacement += elapsed;
		}
		if (rightDown == true) {
			rightDisplacement -= elapsed;
		}
		if (leftUp == true) {
			leftDisplacement += elapsed;
		}
		if (leftDown == true) {
			leftDisplacement -= elapsed;
		}

		if (pongY > 1.0f || pongY < -1.0f) {
			pongVelocityY *= -1;
		}
		if (pongX > 1.777f) {
			pongVelocityX = 0.0f;
			pongVelocityY = 0.0f;
			glClearColor(86.0 / 255.0f, 0.0 / 255.0f, 39.0 / 255.0f, 1.0f);
		}
		else if (pongX < -1.777f) {
			pongVelocityX = 0.0f;
			pongVelocityY = 0.0f;
			glClearColor(0.0 / 255.0f, 54.0 / 255.0f, 58.0 / 255.0f, 1.0f);
		}

		SDL_GL_SwapWindow(displayWindow);

    }
    
    SDL_Quit();
    return 0;
}
