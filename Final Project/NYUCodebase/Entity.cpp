#include "Entity.h"
#include "helper.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define SPRITE_COUNT_X 4
#define SPRITE_COUNT_Y 3
#define TILE_SIZE 0.15f

extern glm::mat4 modelMatrix;
extern glm::mat4 identityMatrix;
extern float defaultVertices[12];
float gravity = 0.098f;

Entity::Entity() : xPosition(0.0f), yPosition(0.0f), hitboxWidth(TILE_SIZE), hitboxHeight(TILE_SIZE) {}
Entity::Entity(float x, float y, int spriteIndex) :	xPosition(x), yPosition(y), spriteIndex(spriteIndex), hitboxWidth(TILE_SIZE), hitboxHeight(TILE_SIZE) {
}

void Entity::draw(ShaderProgram &p, const GLuint &texture) const {
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
	transformMatrix = glm::translate(transformMatrix, glm::vec3(xPosition, yPosition, 0.0f));
	transformMatrix = glm::scale(transformMatrix, glm::vec3(TILE_SIZE, TILE_SIZE, 0.0f));
	p.SetModelMatrix(transformMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(p.positionAttribute);
	glDisableVertexAttribArray(p.texCoordAttribute);
}

void Entity::update(float timeElapsed) {
}

InelasticBox::InelasticBox(float x, float y, float width, float height)	: Entity(x, y, 6){
	hitboxWidth = width;
	hitboxHeight = height;
}

void InelasticBox::draw(ShaderProgram &p, const GLuint &texture) const {
	p.SetColor(0.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f, 1.0f);
	p.SetModelMatrix(modelMatrix);
	glVertexAttribPointer(p.positionAttribute, 2, GL_FLOAT, false, 0, defaultVertices);
	glEnableVertexAttribArray(p.positionAttribute);

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
	transformMatrix = glm::translate(transformMatrix, glm::vec3(xPosition, yPosition, 0.0f));
	transformMatrix = glm::scale(transformMatrix, glm::vec3(hitboxWidth, hitboxHeight, 0.0f));
	p.SetModelMatrix(transformMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(p.positionAttribute);
	glDisableVertexAttribArray(p.texCoordAttribute);
}

//0 = Nothing;
//1 = Coin;
std::vector<int> nothing = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
std::vector<int> infinityCoinShape0 = { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 };
std::vector<int> infinityCoinShape1 = { 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0 };
std::vector<int> infinityCoinShape2 = { 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0 };
std::vector<int> infinityCoinShape3 = { 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0 };
std::vector<int> infinityCoinShape4 = { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 };

void insertInfinityShapeCoins(std::deque<std::vector<int>>& map) {
	map.push_back(infinityCoinShape0);
	map.push_back(infinityCoinShape1);
	map.push_back(infinityCoinShape2);
	map.push_back(infinityCoinShape2);
	map.push_back(infinityCoinShape2);
	map.push_back(infinityCoinShape1);
	map.push_back(infinityCoinShape3);
	map.push_back(infinityCoinShape4);
	map.push_back(infinityCoinShape4);
	map.push_back(infinityCoinShape3);
	map.push_back(infinityCoinShape1);
	map.push_back(infinityCoinShape2);
	map.push_back(infinityCoinShape2);
	map.push_back(infinityCoinShape2);
	map.push_back(infinityCoinShape1);
	map.push_back(infinityCoinShape0);
}

std::vector<int> topCoins = { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
void insertTopCoins(std::deque<std::vector<int>>& map) {
	for (size_t i = 0; i < rand() % 5 + 5; i++) {
		map.push_back(topCoins);
	}
}

std::vector<int> bottomCoins = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0 };
void insertBottomCoins(std::deque<std::vector<int>>& map) {
	for (size_t i = 0; i < rand() % 5 + 5; i++) {
		map.push_back(bottomCoins);
	}
}

std::vector<int> middleCoins = { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 };
void insertMiddleCoins(std::deque<std::vector<int>>& map) {
	for (size_t i = 0; i < rand() % 5 + 5; i++) {
		map.push_back(middleCoins);
	}
}

Map::Map() : xPositionOfHead(-1.777f - (TILE_SIZE / 2)), speed(1.25f) {
	insertNewPartIntoMap();
}

void Map::draw(ShaderProgram &p, const GLuint &texture) const {
	for (size_t x = 0; x < mapObjects.size(); x++) {
		for (size_t y = 0; y < mapObjects[x].size(); y++) {
			if (mapObjects[x][y] == 1) {
				int spriteIndex = 8;
				p.SetColor(255.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 1.0f);
				p.SetModelMatrix(modelMatrix);
				glVertexAttribPointer(p.positionAttribute, 2, GL_FLOAT, false, 0, defaultVertices);
				glEnableVertexAttribArray(p.positionAttribute);

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
				float yPosition = (1.0 - 0.1 - (TILE_SIZE / 2)) - (TILE_SIZE * y);
				transformMatrix = glm::translate(transformMatrix, glm::vec3(xPositionOfHead + (TILE_SIZE * x), yPosition, 0.0f));
				transformMatrix = glm::scale(transformMatrix, glm::vec3(TILE_SIZE, TILE_SIZE, 0.0f));
				p.SetModelMatrix(transformMatrix);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glDisableVertexAttribArray(p.positionAttribute);
				glDisableVertexAttribArray(p.texCoordAttribute);
			}
		}
	}
}

void Map::update(float timeElapsed) {
	xPositionOfHead -= speed * timeElapsed;
	if (xPositionOfHead < -1.777f - (TILE_SIZE / 2) && mapObjects.size() > 0) {
		mapObjects.pop_front();
		xPositionOfHead += TILE_SIZE;
	}
	if (mapObjects.size() < 30) {
		insertNewPartIntoMap();
	}
}

void Map::insertNewPartIntoMap() {
	int insertBlank = rand() % 4 + 6;
	for (size_t i = 0; i < rand() % 4 + 6; i++) {
		mapObjects.push_back(nothing);
	}
	int nextInsertShape = rand() % 4;
	switch (nextInsertShape) {
	case 0: {
		insertInfinityShapeCoins(mapObjects);
		break;
	}
	case 1: {
		insertTopCoins(mapObjects);
		break;
	}
	case 2: {
		insertMiddleCoins(mapObjects);
		break;
	}
	case 3: {
		insertBottomCoins(mapObjects);
		break;
	}
	}
}

Player::Player() : Entity(-0.7f, -0.6f, 1) {}

void Player::draw(ShaderProgram &p, const GLuint &texture) const {
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
	transformMatrix = glm::translate(transformMatrix, glm::vec3(xPosition, yPosition, 0.0f));
	transformMatrix = glm::scale(transformMatrix, glm::vec3(TILE_SIZE, TILE_SIZE, 0.0f));
	if (!gravityDown) {
		transformMatrix = glm::scale(transformMatrix, glm::vec3(1.0, -1.0f, 0.0f));
	}
	p.SetModelMatrix(transformMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(p.positionAttribute);
	glDisableVertexAttribArray(p.texCoordAttribute);
}

void Player::update(float timeElapsed) {
	if (gravityDown) {
		yVelocity -= gravity * timeElapsed;
	}
	else {
		yVelocity += gravity * timeElapsed;
	}
	yPosition += yVelocity;
	/*++spriteIndex;
	if (spriteIndex >= 4) {
		spriteIndex = 0;
	}*/
}

void Player::changeDirection() {
	gravityDown = !gravityDown;
	yVelocity = 0.0f;
}
void Player::checkInelasticCollision(const InelasticBox& box) {
	float XDistBetweenBlockAndPlayer = fabs(box.xPosition - xPosition) - ((box.hitboxWidth + hitboxWidth) / 2);
	float YDistBetweenBlockAndPlayer = fabs(box.yPosition - yPosition) - ((box.hitboxHeight + hitboxHeight) / 2);
	if (XDistBetweenBlockAndPlayer < 0.0f && YDistBetweenBlockAndPlayer < 0.0f) {
		float yPenetration = fabs(fabs(box.yPosition - yPosition) - (hitboxHeight / 2) - (box.hitboxHeight / 2));
		if (box.yPosition > yPosition) {
			yPosition -= yPenetration + 0.00001f;
		}
		else {
			yPosition += yPenetration + 0.00001f;
		}
		yVelocity = 0.0f;
	}
}

size_t Player::checkMap(Map& map) {
	size_t ret = 0;
	float almostY = (yPosition - (1.0f - 0.1f - (TILE_SIZE / 2.0f))) / (-TILE_SIZE);
	int upperY = floor(almostY);
	int lowerY = ceil(almostY);
	if (upperY == 0) {
		upperY = 1;
	}
	for (size_t x = 7; x < 10 && x < map.mapObjects.size(); x++) {
		for (size_t y = upperY - 1; y < lowerY + 2 && y < map.mapObjects[x].size(); y++) {
			//check collision against coin
			if (map.mapObjects[x][y] == 1) {
				float objectX = map.xPositionOfHead + (TILE_SIZE * x);
				float objectY = (1.0 - 0.1 - (TILE_SIZE / 2)) - (TILE_SIZE * y);
				float XDistBetweenBulletAndAlien = (float)abs(objectX - xPosition) - ((hitboxWidth + TILE_SIZE) / 2);
				float YDistBetweenBulletAndAlien = (float)abs(objectY - yPosition) - ((hitboxHeight + TILE_SIZE) / 2);
				if (XDistBetweenBulletAndAlien < 0.0f && YDistBetweenBulletAndAlien < 0.0f) {
					map.mapObjects[x][y] = 0;
					ret++;
				}
			}
		}
	}
	return ret;
}