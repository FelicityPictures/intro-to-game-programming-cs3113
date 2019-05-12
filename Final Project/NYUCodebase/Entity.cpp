#include "Entity.h"
#include "helper.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define SPRITE_COUNT_X 4
#define SPRITE_COUNT_Y 4
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

InelasticBox::InelasticBox(){ }

InelasticBox::InelasticBox(float x, float y, float width, float height)	: Entity(x, y, 6){
	hitboxWidth = width;
	hitboxHeight = height;
}

void InelasticBox::draw(ShaderProgram &p, const GLuint &texture) const {
	p.SetColor(0.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f, 1.0f);
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
	transformMatrix = glm::scale(transformMatrix, glm::vec3(hitboxWidth, hitboxHeight, 0.0f));
	p.SetModelMatrix(transformMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(p.positionAttribute);
	glDisableVertexAttribArray(p.texCoordAttribute);
}

//0 = Nothing;
//1 = Coin;
std::vector<int> nothing = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void insertEmptySpace(std::deque<std::vector<int>>& map) {
	map.push_back(nothing);
}

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
	for (int i = 0; i < rand() % 5 + 5; i++) {
		map.push_back(topCoins);
	}
}

std::vector<int> bottomCoins = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0 };
void insertBottomCoins(std::deque<std::vector<int>>& map) {
	for (int i = 0; i < rand() % 5 + 5; i++) {
		map.push_back(bottomCoins);
	}
}

std::vector<int> middleCoins = { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 };
void insertMiddleCoins(std::deque<std::vector<int>>& map) {
	for (int i = 0; i < rand() % 5 + 5; i++) {
		map.push_back(middleCoins);
	}
}

Map::Map() : xPositionOfHead(-1.777f - (TILE_SIZE / 2)), speed(1.25f) {
	for (size_t i = 0; i < 30; i++) {
		insertEmptySpace(mapObjects);
	}
}

void Map::draw(ShaderProgram &p, const GLuint &texture) const {
	for (size_t x = 0; x < mapObjects.size(); x++) {
		for (size_t y = 0; y < mapObjects[x].size(); y++) {
			if (mapObjects[x][y] == 1) {
				int spriteIndex;
				float temp = fmod(animationTracker, 2.0f);
				if (temp < 0.3f || temp > 1.7) {
					spriteIndex = 8;
				}
				else if ((temp >= 0.3f && temp < 0.6f) || (temp > 1.4f && temp <= 1.7f)) {
					spriteIndex = 9;
				}
				else if ((temp >= 0.6f && temp < 0.9f) || (temp > 1.1f && temp <= 1.4f)) {
					spriteIndex = 10;
				}
				else {
					spriteIndex = 11;
				}
				p.SetColor(255.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 1.0f);
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
				float yPosition = (1.0f - 0.1f - (TILE_SIZE / 2.0f)) - (TILE_SIZE * y);
				transformMatrix = glm::translate(transformMatrix, glm::vec3(xPositionOfHead + (TILE_SIZE * x), yPosition, 0.0f));
				transformMatrix = glm::scale(transformMatrix, glm::vec3(TILE_SIZE, TILE_SIZE, 0.0f));
				p.SetModelMatrix(transformMatrix);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glDisableVertexAttribArray(p.positionAttribute);
				glDisableVertexAttribArray(p.texCoordAttribute);
			}
			else if (mapObjects[x][y] == 4) {
				int spriteIndex = 15;
				p.SetColor(255.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 1.0f);
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
				float yPosition = (1.0f - 0.1f - (TILE_SIZE / 2.0f)) - (TILE_SIZE * y);
				transformMatrix = glm::translate(transformMatrix, glm::vec3(xPositionOfHead + (TILE_SIZE * x), yPosition, 0.0f));
				transformMatrix = glm::scale(transformMatrix, glm::vec3(TILE_SIZE, TILE_SIZE, 0.0f));
				transformMatrix = glm::rotate(transformMatrix, animationTracker, glm::vec3(0.0f, 0.0f, 1.0f));
				p.SetModelMatrix(transformMatrix);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glDisableVertexAttribArray(p.positionAttribute);
				glDisableVertexAttribArray(p.texCoordAttribute);
			}
		}
	}
}

void Map::update(float timeElapsed, float timeSurvived) {
	xPositionOfHead -= (speed + (0.1 * floorf(timeSurvived / 5.0f))) * timeElapsed;
	animationTracker += timeElapsed;
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
	for (int i = 0; i < rand() % 6 + 1; i++) {
		insertEmptySpace(mapObjects);
	}
	int nextInsertShape = rand() % 7;
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
	case 4: {
		mapObjects.push_back({4,4,4,0,0,0,0,0,0,0,0,0});
		break;
	}
	case 5: {
		mapObjects.push_back({ 0,0,0,0,0,0,0,0,0,4,4,4 });
		break;
	}
	case 6: {
		mapObjects.push_back({ 0,0,0,0,4,4,4,4,0,0,0,0 });
		break;
	}
	}
}

Enemy::Enemy(float y, float w, float h) : Entity(1.6f, y, 12), width(w), height(h){
	hitboxWidth = width * 0.95f;
	hitboxHeight = height * 0.4f;
}

// returns true is it's past the screen
bool Enemy::update(float timeElapsed, float targetY, float timeSurvived) {
	timeAlive += timeElapsed;
	if (timeAlive <= 2.0f) {
		if (targetY > yPosition) {
			yPosition += 1.2f * timeElapsed;
		}
		else {
			yPosition -= 1.2f * timeElapsed;
		}
	}
	else{
		if (!launched) {
			launched = true;
			Mix_Chunk *rocketSound;
			rocketSound = Mix_LoadWAV("Missle_Launch.wav");
			Mix_VolumeChunk(rocketSound, 90);
			Mix_PlayChannel(-1, rocketSound, 0);
		}
		xPosition -= (3.0f + (0.1 * floorf(timeSurvived / 5.0f))) * timeElapsed;
		animationTracker += timeElapsed;
		if (fmod(animationTracker, 0.2) < 0.1) {
			spriteIndex = 13;
		}
		else {
			spriteIndex = 14;
		}
		if (xPosition < -2.0f) {
			return true;
		}
	}
	return false;
}

void Enemy::draw(ShaderProgram &p, const GLuint &texture) const {
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
	transformMatrix = glm::scale(transformMatrix, glm::vec3(width, height, 0.0f));
	p.SetModelMatrix(transformMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(p.positionAttribute);
	glDisableVertexAttribArray(p.texCoordAttribute);
}

Player::Player() : Entity(-0.7f, -0.6f, 4) {
	hitboxWidth = TILE_SIZE * 0.75f;
	hitboxHeight = TILE_SIZE * 0.75f;
}

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
	if (timeDead > 0.0) {
		timeDead += timeElapsed;
		xPosition -= 0.5f * timeElapsed;
		yPosition -= 0.5f * timeElapsed;
		return;
	}
	if (gravityDown) {
		yVelocity -= gravity * timeElapsed;
	}
	else {
		yVelocity += gravity * timeElapsed;
	}
	yPosition += yVelocity;
	if (animationTracker > 0.0f) {
		animationTracker += timeElapsed;
		float runSpeed = 0.7f;
		float temp = fmod(animationTracker, runSpeed);
		if (temp < runSpeed * 0.25f) {
			spriteIndex = 0;
		}
		else if (temp >= runSpeed * 0.25f && temp < runSpeed * 0.5f) {
			spriteIndex = 1;
		}
		else if (temp >= runSpeed * 0.5f && temp < runSpeed * 0.75f) {
			spriteIndex = 2;
		}
		else {
			spriteIndex = 3;
		}
	}
}

void Player::changeDirection() {
	gravityDown = !gravityDown;
	yVelocity = 0.0f;
	spriteIndex = 4;
	animationTracker = 0.0f;
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
		animationTracker += 0.00001f;
	}
}

bool Player::collideWithRocket(const Enemy& enemy) {
	float XDistBetweenBlockAndPlayer = fabs(enemy.xPosition - xPosition) - ((enemy.hitboxWidth + hitboxWidth) / 2);
	float YDistBetweenBlockAndPlayer = fabs(enemy.yPosition - yPosition) - ((enemy.hitboxHeight + hitboxHeight) / 2);
	if (XDistBetweenBlockAndPlayer < 0.0f && YDistBetweenBlockAndPlayer < 0.0f) {
		yVelocity = 0.0f;
		spriteIndex = 5;
		timeDead += 0.0001f;
		return true;
	}
	return false;
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
			float objectX = map.xPositionOfHead + (TILE_SIZE * x);
			float objectY = (1.0 - 0.1 - (TILE_SIZE / 2)) - (TILE_SIZE * y);
			if (map.mapObjects[x][y] == 1) {
				//circle circle collision
				float aSquared = pow(objectX - xPosition, 2.0f);
				float bSquared = pow(objectY - yPosition, 2.0f);
				float c = sqrt(aSquared + bSquared);
				if (c < hitboxHeight + (TILE_SIZE / 2.0f)) {
					map.mapObjects[x][y] = 0;
					ret++;
				}
			}
			else if (map.mapObjects[x][y] == 4) {
				float XDistBetweenObstacleAndPlayer = (float)abs(objectX - xPosition) - ((TILE_SIZE + hitboxWidth) / 2);
				float YDistBetweenObstacleAndPlayer = (float)abs(objectY - yPosition) - ((TILE_SIZE + hitboxHeight) / 2);
				if (XDistBetweenObstacleAndPlayer < 0.0f && YDistBetweenObstacleAndPlayer < 0.0f) {
					yVelocity = 0.0f;
					spriteIndex = 5;
					timeDead += 0.0001f;
				}
			}
		}
	}
	return ret;
}