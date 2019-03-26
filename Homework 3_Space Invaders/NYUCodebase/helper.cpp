#include "helper.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define NUMBER_OF_ALIENS 30
#define MAX_BULLETS 10
#define LETTER_COUNT_X 16
#define LETTER_COUNT_Y 16
#define SPRITE_COUNT_X 2
#define SPRITE_COUNT_Y 5

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

Entity::Entity() : x(0.0f), y(0.0f), width(1.0f), height(1.0f) {}
Entity::Entity(const float& x, const float& y, const float& width, const float& height, int spriteIndex) :
		x(x), y(y), width(width), height(height), spriteIndex(spriteIndex) {
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
	transformMatrix = glm::translate(transformMatrix, glm::vec3(x, y, 0.0f));
	transformMatrix = glm::scale(transformMatrix, glm::vec3(width, height, 0.0f));
	p.SetModelMatrix(transformMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(p.positionAttribute);
	glDisableVertexAttribArray(p.texCoordAttribute);
}

GameState::GameState() {
	program.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	glUseProgram(program.programID);
	spriteSheet = LoadTexture(RESOURCE_FOLDER"Space Invaders Sprite Sheet.png");
	textSheet = LoadTexture(RESOURCE_FOLDER"Letters.png");
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	player.velocityX = 1.0f;
	std::fill_n(keyDowns, 3, false);
	for (int i = 0; i < NUMBER_OF_ALIENS; i++) {
		float newX = -1.5f + (0.2f * (i % 10));
		float newY = 0.8f - (0.2f * floor(i / 10.0f));
		enemies[i] = Entity(newX, newY, 0.17f, 0.17f, (int)floor(i / SPRITE_COUNT_Y));
		enemies[i].velocityX = 0.075f;
		enemies[i].velocityY = -0.2f;
	}
}

void GameState::RenderGame() {
	// render all the entities in the game
	// render score and other UI elements
	//DRAW THINGS
	player.Draw(program, spriteSheet);
	for (int i = 0; i < currentNumberOfAliens; i++) {
		enemies[i].Draw(program, spriteSheet);
	}
	for (int i = 0; i < currentNumberOfBullets; i++) {
		bullets[i].Draw(program, spriteSheet);
	}
	char text[] = "Score 0000";
	int temporaryScore = score;
	for (int i = 0; i < 4; i++) {
		text[9 - i] = '0' + (temporaryScore % 10);
		temporaryScore = floor(temporaryScore / 10);
	}
	drawText(program, textSheet, text, 10, -1.7f, 0.9f, 0.1f);
}
void GameState::UpdateGame(float elapsed) {
	//move aliens
	elapsedForAliens += elapsed;
	if (elapsedForAliens >= 1.0f) {
		if (leftmostAlien > -0.2f || leftmostAlien < -1.6f) {
			for (int i = 0; i < currentNumberOfAliens; i++) {
				//enemies[i].y += -0.2f;
				enemies[i].y += enemies[i].velocityY;
				enemies[i].velocityX *= -1;
			}
		}
		for (int i = 0; i < currentNumberOfAliens; i++) {
			enemies[i].x += (enemies[i].velocityX * elapsedForAliens);
			if (enemies[i].spriteIndex % 2 == 0) {
				enemies[i].spriteIndex++;
			}
			else {
				enemies[i].spriteIndex--;
			}
		}
		leftmostAlien += (enemies[0].velocityX * elapsedForAliens);
		elapsedForAliens = 0.0f;
	}

	//movement and collision detection
	//between bullets and aliens
	for (int i = 0; i < currentNumberOfBullets; i++) {
		bullets[i].y += (bullets[i].velocityY * elapsed);
		if (bullets[i].y > 1.0f + (bullets[i].height / 2)) {
			bullets[i] = bullets[currentNumberOfBullets - 1];
			currentNumberOfBullets--;
		}
		else {
			for (int m = 0; m < currentNumberOfAliens; m++) {
				float XDistBetweenBulletAndAlien = (float)abs(bullets[i].x - enemies[m].x) - ((bullets[i].width + enemies[m].width) / 2);
				float YDistBetweenBulletAndAlien = (float)abs(bullets[i].y - enemies[m].y) - ((bullets[i].height + enemies[m].height) / 2);
				if (XDistBetweenBulletAndAlien < 0.0f && YDistBetweenBulletAndAlien < 0.0f) {
					bullets[i] = bullets[currentNumberOfBullets - 1];
					currentNumberOfBullets--;
					enemies[m] = enemies[currentNumberOfAliens - 1];
					currentNumberOfAliens--;
					score += 5;
					break;
				}
			}
		}
	}
	elapsedForBullets += elapsed;
	ProcessInput(elapsed);
}
void GameState::ProcessInput(float elapsed) {
	if (keyDowns[0]) {
		player.x -= (player.velocityX * elapsed);
	}
	if (keyDowns[1]) {
		player.x += (player.velocityX * elapsed);
	}
	if (keyDowns[2] && elapsedForBullets >= 0.5f) {
		//create bullet
		if (currentNumberOfBullets < MAX_BULLETS) {
			bullets[currentNumberOfBullets] = Entity(player.x, player.y, 0.02f, 0.1f, 9);
			bullets[currentNumberOfBullets].velocityY = 2.5f;
			currentNumberOfBullets++;
		}
		elapsedForBullets = 0.0f;
	}
}