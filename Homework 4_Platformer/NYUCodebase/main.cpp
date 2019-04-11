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
#define TILE_SIZE 16

//background color: 10, 152, 172

SDL_Window* displayWindow;
glm::mat4 projectionMatrix = glm::mat4(1.0f);
glm::mat4 viewMatrix = glm::mat4(1.0f);
int mapWidth, mapHeight;
unsigned char** levelData;
//unsigned int levelData[LEVEL_HEIGHT][LEVEL_WIDTH];
vector<Entity> immovableBlocks;

void placeEntity(string type, float placeX, float placeY) {
	if (type == "blocks") {
		//Entity(const float& x, const float& y, bool isStatic, int spriteIndex);
		//Entity newBlock = Entity(placeX, placeY,);
		//immovableBlocks.push_back(newBlock);
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
			float placeY = atoi(yPosition.c_str())*-TILE_SIZE;
			//placeEntity(type, placeX, placeY);
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
	float lastFrameTicks = 0.0f;
	float ticks, elapsed;
	vector<Entity> test;

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
	//for (int i = 0; i < mapHeight; i++) {
	//	for (int m = 0; m < mapWidth; m++) {
	//		if (levelData[i][m] != 0) {
	//			//test.push_back(Entity(0,0, 0.1f, 0.1f, 1));
	//			test.push_back(Entity(-1.777 + (0.1*m), 1.0 - (0.1f*i), 0.1f, 0.1f, 0));
	//		}
	//	}
	//}
	GLuint spriteSheet = LoadTexture(RESOURCE_FOLDER"sprites.png");
	Entity testingSprite = Entity(0, 0, true, 1);
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
			}
			if (event.type == SDL_KEYDOWN) {
			}
        }
		glClear(GL_COLOR_BUFFER_BIT);


		//DRAW WITH GAME STATE
		for (int i = 0; i < test.size(); i++) {
			test[i].Draw(program, spriteSheet);
		}

		//TIMING
		ticks = (float)SDL_GetTicks() / 1000.0f;
		elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		//UPDATE GAME

		SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
