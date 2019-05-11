#include "helper.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define LETTER_COUNT_X 16
#define LETTER_COUNT_Y 16

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

float lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t * v1;
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