#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengl.h>
#include <vector>
#include <string>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "stb_image.h"
#include <SDL_mixer.h>

GLuint LoadTexture(const char *filePath);

float lerp(float v0, float v1, float t);

void drawText(ShaderProgram &p, const GLuint &texture, char* str, float x, float y, float height);
void drawText(ShaderProgram &p, const GLuint &texture, const std::string& str, float x, float y, float height);

void imageForWholeScreen(ShaderProgram &p, GLuint &texture);

class Background {
public:
	std::vector<GLuint> backgrounds;
	float currentX = 0.0f;
	float speed = 1.25f;
	size_t currentBackground = 0;

	Background();
	Background(const GLuint& texture);
	Background(const GLuint& texture1, const GLuint& texture2);

	void addNewTexture(const GLuint& texture);
	void draw(ShaderProgram &p) const;
	void update(float timeElapsed, float timeSurvived);
};

class Button {
public:
	float xPosition, yPosition;
	float height;
	float red, green, blue;
	std::string text;

	Button();
	Button(char* newtext, float height, float x, float y, float red, float green, float blue);

	void draw(ShaderProgram& p, ShaderProgram& u, const GLuint& texture);
};