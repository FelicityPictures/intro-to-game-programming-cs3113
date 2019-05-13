#include "GameState.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

GameState::GameState() {
	program.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	untexturedProgram.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	glUseProgram(program.programID);

	titleScreen = LoadTexture(RESOURCE_FOLDER"TitleScreen-01.png");
	//char text[], float height, float x, float y, float red, float green, float blue
	titleScreenButtons[0] = Button("Play", 0.25f, 0.0f, -0.25f, 255.0, 0.0f, 0.0f);
	titleScreenButtons[1] = Button("Play", 0.15f, 0.0f, 0.25f, 255.0, 0.0f, 0.0f);
	titleScreenButtons[2] = Button("Play", 0.15f, 0.0f, 0.5f, 255.0, 0.0f, 0.0f);

	spriteSheet = LoadTexture(RESOURCE_FOLDER"sprites-01.png");
	textSheet = LoadTexture(RESOURCE_FOLDER"pixel_font.png");
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLuint backgroundTexture = LoadTexture(RESOURCE_FOLDER"background2.png");
	backgrounds = Background(backgroundTexture);

	top = InelasticBox(0.0f, 1.0 - 0.05f, (640.0f / 360.0f) * 2.0f, 0.1f);
	bottom = InelasticBox(0.0f, (-1.0) + 0.05f, (640.0f / 360.0f) * 2.0f, 0.1f);
	bottom.spriteIndex = 7;

}

void GameState::RenderGame(int mode) {
	switch (mode) {
	case GameMode::STATE_MAIN_MENU:
		imageForWholeScreen(program, titleScreen);
		titleScreenButtons[0].draw(program, untexturedProgram, textSheet);
		/*for (size_t i = 0; i < 3; i++) {
			titleScreenButtons[i].draw(program, textSheet);
		}*/
		break;
	case GameMode::STATE_SINGLE_PLAYER_PLAY:
		//DRAW THINGS
		backgrounds.draw(program);
		top.draw(program, spriteSheet);
		bottom.draw(program, spriteSheet);
		map.draw(program, spriteSheet);
		player.draw(program, spriteSheet);
		for (size_t i = 0; i < enemies.size(); i++) {
			enemies[i].draw(program, spriteSheet);
		}
		int temporaryScore = score;
		char scoreText[] = "Score 0000";
		for (int i = 0; i < 4; i++) {
			scoreText[9 - i] = '0' + (temporaryScore % 10);
			temporaryScore = floor(temporaryScore / 10);
		}
		drawText(program, textSheet, scoreText, -1.7f, 0.95f, 0.05f);
		break;
	}
}

void GameState::UpdateGame(float elapsed) {
	player.update(elapsed);
	if (player.timeDead <= 0.0f) {
		backgrounds.update(elapsed, timeSurvived);
		map.update(elapsed, timeSurvived);
		player.checkInelasticCollision(top);
		player.checkInelasticCollision(bottom);
		score += player.checkMap(map);
		for (size_t i = 0; i < enemies.size(); i++) {
			if (enemies[i].update(elapsed, player.yPosition, timeSurvived)) {
				std::swap(enemies[i], enemies[enemies.size() - 1]);
				enemies.pop_back();
			}
			else {
				player.collideWithRocket(enemies[i]);
			}
		}
		int numberOfValidRockets = floor(score / 100.0);
		if (enemies.size() < numberOfValidRockets && rocketTimer > 2.0f) {
			int random = rand() % 5;
			if (random > (5 - floorf(timeSurvived / 10.0f))) {
				enemies.push_back(Enemy(player.yPosition, 0.5f, 0.5f));
				rocketTimer = 0.0f;
				if (enemies.size() >= numberOfValidRockets) {
					rocketTimer = -10.0f;
				}
			}
		}
		rocketTimer += elapsed;
		timeSurvived += elapsed;
	}
	else {
		Mix_HaltMusic();
	}
}

void GameState::ProcessInput(float elapsed) {
	return;
}
