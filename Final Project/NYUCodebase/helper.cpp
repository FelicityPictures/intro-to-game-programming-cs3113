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

void drawText(ShaderProgram &p, const GLuint &texture, char* str, float x, float y, float height) {
	p.SetModelMatrix(modelMatrix);
	glVertexAttribPointer(p.positionAttribute, 2, GL_FLOAT, false, 0, defaultVertices);
	glEnableVertexAttribArray(p.positionAttribute);

	float u, v;
	float spriteWidth = 1.0f / (float)LETTER_COUNT_X;
	float spriteHeight = 1.0f / (float)LETTER_COUNT_Y;
	glBindTexture(GL_TEXTURE_2D, texture);
	for (int i = 0; i < strlen(str); i++) {
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
		glm::mat4 transformMatrix;
		transformMatrix = identityMatrix;
		transformMatrix = glm::translate(transformMatrix, glm::vec3(x + (height * i), y, 0.0f));
		transformMatrix = glm::scale(transformMatrix, glm::vec3(height, height, 0.0f));
		p.SetModelMatrix(transformMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	glDisableVertexAttribArray(p.positionAttribute);
	glDisableVertexAttribArray(p.texCoordAttribute);
}

void drawText(ShaderProgram &p, const GLuint &texture, const std::string& str, float x, float y, float height) {
	p.SetModelMatrix(modelMatrix);
	glVertexAttribPointer(p.positionAttribute, 2, GL_FLOAT, false, 0, defaultVertices);
	glEnableVertexAttribArray(p.positionAttribute);

	float u, v;
	float spriteWidth = 1.0f / (float)LETTER_COUNT_X;
	float spriteHeight = 1.0f / (float)LETTER_COUNT_Y;
	glBindTexture(GL_TEXTURE_2D, texture);
	for (int i = 0; i < str.size(); i++) {
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
		glm::mat4 transformMatrix;
		transformMatrix = identityMatrix;
		transformMatrix = glm::translate(transformMatrix, glm::vec3(x + (height * i), y, 0.0f));
		transformMatrix = glm::scale(transformMatrix, glm::vec3(height, height, 0.0f));
		p.SetModelMatrix(transformMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	glDisableVertexAttribArray(p.positionAttribute);
	glDisableVertexAttribArray(p.texCoordAttribute);
}


void imageForWholeScreen(ShaderProgram &p, GLuint &texture) {
	p.SetModelMatrix(modelMatrix);
	glVertexAttribPointer(p.positionAttribute, 2, GL_FLOAT, false, 0, defaultVertices);
	glEnableVertexAttribArray(p.positionAttribute);
	glBindTexture(GL_TEXTURE_2D, texture);
	float texCoords[] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
	glVertexAttribPointer(p.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(p.texCoordAttribute);

	glm::mat4 transformMatrix = identityMatrix;
	transformMatrix = glm::scale(transformMatrix, glm::vec3(3.554f, 2.0f, 0.0f));
	p.SetModelMatrix(transformMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(p.positionAttribute);
	glDisableVertexAttribArray(p.texCoordAttribute);
}

Background::Background() { }

Background::Background(const GLuint& texture) {
	backgrounds.push_back(texture);
	backgrounds.push_back(texture);
}

Background::Background(const GLuint& texture1, const GLuint& texture2) {
	backgrounds.push_back(texture1);
	backgrounds.push_back(texture2);
}

void Background::addNewTexture(const GLuint& texture) {
	backgrounds.push_back(texture);
}

void Background::draw(ShaderProgram &p) const {
	p.SetModelMatrix(modelMatrix);
	glVertexAttribPointer(p.positionAttribute, 2, GL_FLOAT, false, 0, defaultVertices);
	glEnableVertexAttribArray(p.positionAttribute);
	glBindTexture(GL_TEXTURE_2D, backgrounds[0]);
	float texCoords[] = {
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f
	};
	glVertexAttribPointer(p.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(p.texCoordAttribute);

	glm::mat4 transformMatrix = identityMatrix;
	transformMatrix = glm::translate(transformMatrix, glm::vec3(currentX, 0.0f, 0.0f));
	transformMatrix = glm::scale(transformMatrix, glm::vec3(4.0f, 2.0f, 0.0f));
	p.SetModelMatrix(transformMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindTexture(GL_TEXTURE_2D, backgrounds[1]);
	transformMatrix = identityMatrix;
	transformMatrix = glm::translate(transformMatrix, glm::vec3(currentX + 4.0f, 0.0f, 0.0f));
	transformMatrix = glm::scale(transformMatrix, glm::vec3(4.0f, 2.0f, 0.0f));
	p.SetModelMatrix(transformMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(p.positionAttribute);
	glDisableVertexAttribArray(p.texCoordAttribute);
}

void Background::update(float timeElapsed, float timeSurvived) {
	currentX -= (speed + (0.1f * floorf(timeSurvived / 5.0f))) * timeElapsed;
	if (currentX < -3.777f) {
		currentX = currentX + 4.0f;
		backgrounds.push_back(backgrounds[0]);
		backgrounds.erase(backgrounds.begin());
	}
}

Button::Button(){ }

Button::Button(char* newtext, float height, float x, float y, float red, float green, float blue) :
	height(height), xPosition(x), yPosition(y), red(red), green(green), blue(blue){
	text = newtext;
}

bool Button::clicked(float clickX, float clickY) {
	float sizeOfLetters = height * 0.5f;
	float halfWidth = (text.size() + 1) * sizeOfLetters;
	float halfHeight = height / 2;
	if ((clickX > xPosition - halfWidth) && (clickX < xPosition + halfWidth) &&
		(clickY > yPosition - halfHeight) && (clickY < yPosition + halfHeight) ) {
		return true;
	}
	return false;
}

void Button::draw(ShaderProgram& p, ShaderProgram& u, const GLuint& texture) {
	glUseProgram(u.programID);
	u.SetModelMatrix(modelMatrix);

	glVertexAttribPointer(u.positionAttribute, 2, GL_FLOAT, false, 0, defaultVertices);
	glEnableVertexAttribArray(u.positionAttribute);
	u.SetColor(red / 255.0f, blue / 255.0f, green / 255.0f, 1.0f);
	glm::mat4 transformMatrix = identityMatrix;
	transformMatrix = glm::translate(transformMatrix, glm::vec3(xPosition, yPosition, 0.0f));

	float sizeOfLetters = height * 0.5f;
	transformMatrix = glm::scale(transformMatrix, glm::vec3((text.size() + 1)*sizeOfLetters, height, 0.0f));
	u.SetModelMatrix(transformMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(u.positionAttribute);

	glUseProgram(p.programID);
	float leftSideTextPlacement = xPosition - (float(text.size() / 2.0f) * sizeOfLetters) + (sizeOfLetters / 2);
	drawText(p, texture, text, leftSideTextPlacement, yPosition, sizeOfLetters);
	//drawText(p, texture, text, xPosition, yPosition, height*0.75f);
}