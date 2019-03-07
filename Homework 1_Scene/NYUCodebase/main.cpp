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
	program.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	ShaderProgram untexturedProgram;
	untexturedProgram.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");

	GLuint centerTexture = LoadTexture(RESOURCE_FOLDER"flower.png");
	GLuint wingsTexture = LoadTexture(RESOURCE_FOLDER"dots.jpg");
	GLuint stemTexture = LoadTexture(RESOURCE_FOLDER"stem.jpg");
	GLuint grassTexture = LoadTexture(RESOURCE_FOLDER"grass.jpg");
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(208.0/255.0f, 232.0/255.0f, 250.0/255.0f, 1.0f);

	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);

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

		//grass
		program.SetModelMatrix(modelMatrix);
		glBindTexture(GL_TEXTURE_2D, grassTexture);

		float grass[] = { 1.777f, -0.75f, -1.777f, -0.75f, -1.777f, -1.777f, 1.777f, -0.75f, -1.777f, -1.777f, 1.777f, -1.777f };

		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, grass);
		glEnableVertexAttribArray(program.positionAttribute);

		float grassTex[] = { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0 };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, grassTex);
		glEnableVertexAttribArray(program.texCoordAttribute);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		//STEM
		glUseProgram(untexturedProgram.programID);
		untexturedProgram.SetModelMatrix(modelMatrix);
		untexturedProgram.SetProjectionMatrix(projectionMatrix);
		untexturedProgram.SetViewMatrix(viewMatrix);

		float stem[] = { 0.01f, 0.0f, -0.01f, 0.0f, -0.01f, -1.8f, 0.01f, 0.0f, -0.01f, -1.8f, 0.01f, -1.8f };

		glVertexAttribPointer(untexturedProgram.positionAttribute, 2, GL_FLOAT, false, 0, stem);
		glEnableVertexAttribArray(untexturedProgram.positionAttribute);
		untexturedProgram.SetColor(0.0/255.0f, 153.0/255.0f, 45.0/255.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(untexturedProgram.positionAttribute);
		glDisableVertexAttribArray(untexturedProgram.texCoordAttribute);
		glUseProgram(program.programID);

		//WINGS
		program.SetModelMatrix(modelMatrix);
		program.SetProjectionMatrix(projectionMatrix);
		program.SetViewMatrix(viewMatrix);

		glBindTexture(GL_TEXTURE_2D, wingsTexture);
		float vertices[] = { 0.1f, 0.1f, 0.1f, 0.6f, -0.1f, 0.2f, -0.1f, -0.1f, 0.1f, -0.1f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program.positionAttribute);
		float wingTexCoords[] = { 1.0, 0.7, 1.0, 0.0, 0.0, 0.57, 0.0, 1.0, 1.0, 1.0 };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, wingTexCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);

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
		glDrawArrays(GL_TRIANGLE_FAN, 0, 5);

		glm::mat4 modelMatrix2 = identityMatrix;
		modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(0.1f, 0.1f, 0.0f));
		modelMatrix2 = glm::rotate(modelMatrix2, (rightAngle*1), glm::vec3(0.0f, 0.0f, 1.0f));
		program.SetModelMatrix(modelMatrix2);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 5);

		glm::mat4 modelMatrix3 = identityMatrix;
		modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(0.1f, -0.1f, 0.0f));
		modelMatrix3 = glm::rotate(modelMatrix3, (rightAngle*2), glm::vec3(0.0f, 0.0f, 1.0f));
		program.SetModelMatrix(modelMatrix3);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 5);

		glm::mat4 modelMatrix4 = identityMatrix;
		modelMatrix4 = glm::translate(modelMatrix4, glm::vec3(-0.1f, -0.1f, 0.0f));
		modelMatrix4 = glm::rotate(modelMatrix4, (rightAngle * 3), glm::vec3(0.0f, 0.0f, 1.0f));
		program.SetModelMatrix(modelMatrix4);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 5);

		glDisableVertexAttribArray(program.positionAttribute);

		//CENTER
		program.SetModelMatrix(modelMatrix);
		program.SetProjectionMatrix(projectionMatrix);
		program.SetViewMatrix(viewMatrix);
		glBindTexture(GL_TEXTURE_2D, centerTexture);

		float center[] = { 0.1f, 0.1f, -0.1f, 0.1f, -0.1f, -0.1f, 0.1f, 0.1f, -0.1f, -0.1f, 0.1f, -0.1f };

		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, center);
		glEnableVertexAttribArray(program.positionAttribute);

		float texCoords[] = { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0 };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);
		SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
