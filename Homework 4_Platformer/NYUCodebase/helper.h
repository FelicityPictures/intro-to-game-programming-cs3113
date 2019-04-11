#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengl.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "stb_image.h"

GLuint LoadTexture(const char *filePath);

class Entity {
public:
	int x;
	int y;
	int width;
	int height;
	int spriteIndex;
	float velocityX;
	float velocityY;
	bool isStatic;

	Entity();
	Entity(int x, int y, bool isStatic, int spriteIndex);

	void Draw(ShaderProgram &p, const GLuint &texture) const;
};