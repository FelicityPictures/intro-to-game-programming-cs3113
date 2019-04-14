#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "helper.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define LEVEL_HEIGHT 6
#define LEVEL_WIDTH 18
#define TILE_SIZE 0.333f
#define FIXED_TIMESTEP 0.0166666f

SDL_Window* displayWindow;
glm::mat4 projectionMatrix = glm::mat4(1.0f);
glm::mat4 viewMatrix = glm::mat4(1.0f);
int mapWidth, mapHeight;
unsigned char** levelData;
vector<Entity> immovableBlocks;
Entity player;

void placeEntity(string type, float placeX, float placeY) {
	if (type == "blocks") {
		Entity newBlock = Entity(placeX, placeY, true, 6);
		immovableBlocks.push_back(newBlock);
	}
	else if (type == "player") {
		player = Entity(placeX, placeY + 0.5f, false, 80);
	}
	return;
}


bool readHeader(ifstream &stream) {
	string line;
	mapWidth = -1;
	mapHeight = -1;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "width") {
			mapWidth = atoi(value.c_str());
		}
		else if (key == "height") {
			mapHeight = atoi(value.c_str());
		}
	}
	if (mapWidth == -1 || mapHeight == -1) {
		return false;
	}
	else { // allocate our map data
		levelData = new unsigned char*[mapHeight];
		for(int i =	0; i < mapHeight; ++i) {
			levelData[i] = new unsigned char[mapWidth];
		}
		return true;
	}
}

bool readLayerData(ifstream &stream) {
	string line;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "data") {
			for (int y = 0; y < mapHeight; y++) {
				getline(stream, line);
				istringstream lineStream(line);
				string tile;
				for (int x = 0; x < mapWidth; x++) {
					getline(lineStream, tile, ',');
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						// be careful, the tiles in this format are indexed from 1 not 0
						levelData[y][x] = val - 1;
					}
					else {
						levelData[y][x] = 0;
					}
				}
			}
		}
	}
	return true;
}
bool readEntityData(ifstream &stream) {
	string line;
	string type;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "type") {
			type = value;
		}
		else if (key == "location") {
			istringstream lineStream(value);
			string xPosition, yPosition;
			getline(lineStream, xPosition, ',');
			getline(lineStream, yPosition, ',');
			float placeX = atoi(xPosition.c_str())*TILE_SIZE;
			float placeY = (atoi(yPosition.c_str())-1)*TILE_SIZE;
			placeX = -1.777f + (TILE_SIZE / 2) + placeX;
			placeY = 1.0f - (TILE_SIZE / 2) - placeY;
			placeEntity(type, placeX, placeY);
		}
	}
	return true;
}

int main(int argc, char *argv[]){
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Felicity's Homework #4", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

	glViewport(0, 0, 640, 360);
	//No Textures
	ShaderProgram program;
	program.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);

	glUseProgram(program.programID);
	float elapsed = 0.0f;
	float accumulator = 0.0f;
	float velocity_x = 0.0f;
	float velocity_y = 0.0f;
	float acceleration_x = 0.01f;
	float gravity = 0.098f;
	//float gravity = 0.0f;
	float friction_x = 0.1f;
	float friction_y = 0.0f;

    SDL_Event event;
    bool done = false;

	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);

	//Read from input file
	//==================================================================================
	ifstream infile("map.txt");
	string line;

	while (getline(infile, line)) {
		if (line == "[header]") {
			if (!readHeader(infile)) {
				return 0;
			}
		}
		else if (line == "[layer]") {
			readLayerData(infile);
		}
		else if (line == "[ObjectsLayer]") {
			readEntityData(infile);
		}
	}
	//==================================================================================
	GLuint spriteSheet = LoadTexture(RESOURCE_FOLDER"sprites.png");
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	while (!done) {
        while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					velocity_y = 0.5f;
				}
			}
        }
		glClear(GL_COLOR_BUFFER_BIT);

		for (int y = 0; y < mapHeight; y++) {
			for (int x = 0; x < mapWidth; x++) {
				if (levelData[y][x] == (unsigned char)atoi("1")) {
					drawStatic(program, spriteSheet, x, y, 1);
				}
			}
		}
		//DRAW WITH GAME STATE
		for (size_t i = 0; i < immovableBlocks.size(); i++) {
			immovableBlocks[i].Draw(program, spriteSheet);
		}
		player.Draw(program, spriteSheet);

		//TIMING
		elapsed += accumulator;
		if (elapsed < FIXED_TIMESTEP) {
			accumulator = elapsed;
			//continue;
		}
		while (elapsed >= FIXED_TIMESTEP) {
			//Update(FIXED_TIMESTEP);
			elapsed -= FIXED_TIMESTEP;
		}
		accumulator = elapsed;


		//UPDATE GAME
		if (keys[SDL_SCANCODE_LEFT]) {
			acceleration_x = 0.05f;
			velocity_x -= acceleration_x * FIXED_TIMESTEP;
		}
		else if (keys[SDL_SCANCODE_RIGHT]) {
			// go right!
			acceleration_x = 0.05f;
			velocity_x += acceleration_x * FIXED_TIMESTEP;
		}
		player.x += velocity_x * FIXED_TIMESTEP;
		for (const Entity& block : immovableBlocks) {
			float XDistBetweenBlockAndPlayer = fabs(block.x - player.x) - ((block.width + player.width) / 2);
			float YDistBetweenBlockAndPlayer = fabs(block.y - player.y) - ((block.height + player.height) / 2);
			if (XDistBetweenBlockAndPlayer < 0.0f && YDistBetweenBlockAndPlayer < 0.0f) {
				float penetrationX = fabs(fabs(block.x - player.x) - (player.width / 2) - (block.width / 2));
				if (block.x > player.x) {
					player.x -= penetrationX + 0.00001f;
				}
				else {
					player.x += penetrationX + 0.00001f;
				}
				velocity_x = 0.0f;
				acceleration_x = 0.0f;
			}
		}
		velocity_x = lerp(velocity_x, 0.0f, FIXED_TIMESTEP * friction_x);

		velocity_y -= gravity * FIXED_TIMESTEP;
		player.y += velocity_y * FIXED_TIMESTEP;
		for (const Entity& block : immovableBlocks) {
			float XDistBetweenBlockAndPlayer = fabs(block.x - player.x) - ((block.width + player.width) / 2);
			float YDistBetweenBlockAndPlayer = fabs(block.y - player.y) - ((block.height + player.height) / 2);
			if (XDistBetweenBlockAndPlayer < 0.0f && YDistBetweenBlockAndPlayer < 0.0f) {
				float penetrationY = fabs(fabs(block.y - player.y) - (player.height / 2) - (block.height / 2));
				if (block.y > player.y) {
					player.y -= penetrationY + 0.0000001f;
				}
				else {
					player.y += penetrationY + 0.0000001f;
				}
				velocity_y = 0.0f;
				//gravity = 0.0f;
			}
		}

		//movement and collision detection
		//between blocks and player
		// TODO: check X velocity
		// - apply x_velocity
		// - do the collision check
		// TODO: then apply friction (if still in motion)
		// TODO: then apply acceleration (prob same step as friction because they're both vectors)
		// TODO: first apply Y velocity
		// -apply y_velocity
		// ```player.y += velocity_y * FIXED_TIMESTEP;```
		// TODO: then check FULL box collision on all entities

		glClearColor(10.0f / 255.0f, 152.0f / 255.0f, 172.0f / 255.0f, 1.0f);
		SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
