#include "Entity.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define SPRITE_COUNT_X 16
#define SPRITE_COUNT_Y 8
#define TILE_SIZE 0.2f

glm::mat4 modelMatrix = glm::mat4(1.0f);
glm::mat4 identityMatrix = glm::mat4(1.0f);
float gravity = 0.098f;
float defaultVertices[12] = {
	0.5f, 0.5f,
	-0.5f, 0.5f,
	-0.5f, -0.5f,
	0.5f, 0.5f,
	-0.5f, -0.5f,
	0.5f, -0.5f
};

Entity::Entity() : xPosition(0.0f), yPosition(0.0f), hitboxWidth(TILE_SIZE), hitboxHeight(TILE_SIZE) {}
Entity::Entity(float x, float y, int spriteIndex) :	xPosition(x), yPosition(y), spriteIndex(spriteIndex), hitboxWidth(TILE_SIZE), hitboxHeight(TILE_SIZE) {
}
void Entity::draw(ShaderProgram &p) const {
	p.SetColor(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f);
	p.SetModelMatrix(modelMatrix);
	glVertexAttribPointer(p.positionAttribute, 2, GL_FLOAT, false, 0, defaultVertices);
	glEnableVertexAttribArray(p.positionAttribute);

	glm::mat4 transformMatrix = identityMatrix;
	transformMatrix = glm::translate(transformMatrix, glm::vec3(xPosition, yPosition, 0.0f));
	transformMatrix = glm::scale(transformMatrix, glm::vec3(TILE_SIZE, TILE_SIZE, 0.0f));
	p.SetModelMatrix(transformMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(p.positionAttribute);
	glDisableVertexAttribArray(p.texCoordAttribute);
}

void Entity::draw(ShaderProgram &p, const GLuint &texture) const {
	p.SetModelMatrix(modelMatrix);
	glVertexAttribPointer(p.positionAttribute, 2, GL_FLOAT, false, 0, defaultVertices);
	glEnableVertexAttribArray(p.positionAttribute);
	glBindTexture(GL_TEXTURE_2D, texture);

	/*float u = (float)(((int)spriteIndex) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
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
	glEnableVertexAttribArray(p.texCoordAttribute);*/

	glm::mat4 transformMatrix = identityMatrix;
	transformMatrix = glm::translate(transformMatrix, glm::vec3(xPosition, yPosition, 0.0f));
	transformMatrix = glm::scale(transformMatrix, glm::vec3(hitboxWidth, hitboxHeight, 0.0f));
	p.SetModelMatrix(transformMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(p.positionAttribute);
	glDisableVertexAttribArray(p.texCoordAttribute);
}

void Entity::update(float timeElapsed) {
}

InelasticBox::InelasticBox(float x, float y, float width, float height)	: Entity(x, y, 0){
	hitboxWidth = width;
	hitboxHeight = height;
}
void InelasticBox::draw(ShaderProgram &p) const {
	p.SetColor(0.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f, 1.0f);
	p.SetModelMatrix(modelMatrix);
	glVertexAttribPointer(p.positionAttribute, 2, GL_FLOAT, false, 0, defaultVertices);
	glEnableVertexAttribArray(p.positionAttribute);

	glm::mat4 transformMatrix = identityMatrix;
	transformMatrix = glm::translate(transformMatrix, glm::vec3(xPosition, yPosition, 0.0f));
	transformMatrix = glm::scale(transformMatrix, glm::vec3(hitboxWidth, hitboxHeight, 0.0f));
	p.SetModelMatrix(transformMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(p.positionAttribute);
	glDisableVertexAttribArray(p.texCoordAttribute);
}

Player::Player() : Entity(0.0f, 1.0f, 0) {}

void Player::update(float timeElapsed) {
	if (gravityDown) {
		yVelocity -= gravity * timeElapsed;
	}
	else {
		yVelocity += gravity * timeElapsed;
	}
	yPosition += yVelocity;
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