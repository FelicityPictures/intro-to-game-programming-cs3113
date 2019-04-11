#include "helper.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define LETTER_COUNT_X 16
#define LETTER_COUNT_Y 16
#define SPRITE_COUNT_X 16
#define SPRITE_COUNT_Y 8

glm::mat4 modelMatrix = glm::mat4(1.0f);
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

Entity::Entity() : x(0.0f), y(0.0f), width(1.0f), height(1.0f) {}
Entity::Entity(int x, int y, bool isStatic, int spriteIndex) :
		x(x), y(y), isStatic(isStatic), spriteIndex(spriteIndex) {
	}

void Entity::Draw(ShaderProgram &p, const GLuint &texture) const {
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
	transformMatrix = glm::translate(transformMatrix, glm::vec3(-1.777+(0.333/2)+(0.333*x), 1.0-(0.333/2)-(0.333*y), 0.0f));
	transformMatrix = glm::scale(transformMatrix, glm::vec3(0.333, 0.333, 0.0f));
	p.SetModelMatrix(transformMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(p.positionAttribute);
	glDisableVertexAttribArray(p.texCoordAttribute);
}