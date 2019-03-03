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

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;


int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Felicity's Homework #1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
	//glew needs to be initialized before you do viewport stuff
#endif

	glViewport(0, 0, 640, 360);
	ShaderProgram program;
	program.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");

	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);

	program.SetModelMatrix(modelMatrix);
	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);
	glUseProgram(program.programID);

	float lastFrameTicks = 0.0f;
	float animationAngle = 0.0f * (3.1415926f / 180.0f);

    SDL_Event event;
    bool done = false;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

		float vertices[] = { 0.1f, 0.6f, -0.1f, 0.2f, -0.1f, -0.1f, 0.1f, -0.1f };

		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program.positionAttribute);

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		animationAngle -= elapsed;

		float rightAngle = -90.0f * (3.1415926f / 180.0f);
		glm::mat4 identityMatrix = glm::mat4(1.0f);
		identityMatrix = glm::rotate(identityMatrix, animationAngle, glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 modelMatrix1 = identityMatrix;
		modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(-0.1f, 0.1f, 0.0f));
		modelMatrix1 = glm::rotate(modelMatrix1, (rightAngle*0), glm::vec3(0.0f, 0.0f, 1.0f));
		program.SetModelMatrix(modelMatrix1);
		glDrawArrays(GL_POLYGON, 0, 4);

		glm::mat4 modelMatrix2 = identityMatrix;
		modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(0.1f, 0.1f, 0.0f));
		modelMatrix2 = glm::rotate(modelMatrix2, (rightAngle*1), glm::vec3(0.0f, 0.0f, 1.0f));
		program.SetModelMatrix(modelMatrix2);
		glDrawArrays(GL_POLYGON, 0, 4);

		glm::mat4 modelMatrix3 = identityMatrix;
		modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(0.1f, -0.1f, 0.0f));
		modelMatrix3 = glm::rotate(modelMatrix3, (rightAngle*2), glm::vec3(0.0f, 0.0f, 1.0f));
		program.SetModelMatrix(modelMatrix3);
		glDrawArrays(GL_POLYGON, 0, 4);

		glm::mat4 modelMatrix4 = identityMatrix;
		modelMatrix4 = glm::translate(modelMatrix4, glm::vec3(-0.1f, -0.1f, 0.0f));
		modelMatrix4 = glm::rotate(modelMatrix4, (rightAngle * 3), glm::vec3(0.0f, 0.0f, 1.0f));
		program.SetModelMatrix(modelMatrix4);
		glDrawArrays(GL_POLYGON, 0, 4);

		glDisableVertexAttribArray(program.positionAttribute);
		SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
