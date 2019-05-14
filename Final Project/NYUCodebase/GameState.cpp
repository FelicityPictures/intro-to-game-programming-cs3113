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

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	bgm = Mix_LoadMUS("bgm.mp3");
	Mix_PlayMusic(bgm, -1);

	titleScreen = LoadTexture(RESOURCE_FOLDER"TitleScreen-01.png");
	//char text[], float height, float x, float y, float red, float green, float blue
	titleScreenButtons[0] = Button("Play!", 0.25f, 0.0f, -0.35f, 101.0f, 115.0f, 137.0f);
	titleScreenButtons[1] = Button("2 Players", 0.12f, 0.0f, -0.65f, 255.0f, 0.0f, 0.0f);
	titleScreenButtons[2] = Button("Quit", 0.1f, 0.0f, -0.9f, 255.0f, 0.0f, 0.0f);

	gameOverScreen = LoadTexture(RESOURCE_FOLDER"GameOverScreen-02.png");
	gameOverButtons[0] = Button("Play Again", 0.25f, 0.0f, -0.35f, 101.0f, 115.0f, 137.0f);
	gameOverButtons[1] = Button("Main Menu", 0.12f, -0.3f, -0.75f, 255.0f, 0.0f, 0.0f);
	gameOverButtons[2] = Button("Quit", 0.12f, 0.4f, -0.75f, 255.0f, 0.0f, 0.0f);

	spriteSheet = LoadTexture(RESOURCE_FOLDER"sprites-01.png");
	textSheet = LoadTexture(RESOURCE_FOLDER"pixel_font.png");
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLuint backgroundTexture = LoadTexture(RESOURCE_FOLDER"background2.png");
	backgrounds = Background(backgroundTexture);

	player[0] = Player();
	player[1] = Player(true);
	top = InelasticBox(0.0f, 1.0 - 0.05f, (640.0f / 360.0f) * 2.0f, 0.1f);
	bottom = InelasticBox(0.0f, (-1.0) + 0.05f, (640.0f / 360.0f) * 2.0f, 0.1f);
	bottom.spriteIndex = 7;
}

void GameState::RenderGame(int mode) {
	int temporaryScore;
	char P1scoreText[] = "Score 0000";
	temporaryScore = P1score;
	for (int i = 0; i < 4; i++) {
		P1scoreText[9 - i] = '0' + (temporaryScore % 10);
		temporaryScore = floor(temporaryScore / 10);
	}
	if (mode == GameMode::STATE_MAIN_MENU) {
		imageForWholeScreen(program, titleScreen);
		for (size_t i = 0; i < 3; i++) {
			titleScreenButtons[i].draw(program, untexturedProgram, textSheet);
		}
	}
	else if (mode == GameMode::STATE_SINGLE_PLAYER_PLAY || mode == GameMode::STATE_TWO_PLAYER_PLAY) {
		backgrounds.draw(program);
		top.draw(program, spriteSheet);
		bottom.draw(program, spriteSheet);
		map.draw(program, spriteSheet, mode);
		for (size_t i = 0; i < enemies.size(); i++) {
			enemies[i].draw(program, spriteSheet);
		}
		player[0].draw(program, spriteSheet);
		drawText(program, textSheet, P1scoreText, -1.7f, 0.95f, 0.05f);
		if (mode == GameMode::STATE_TWO_PLAYER_PLAY) {
			player[1].draw(program, spriteSheet);
			char P2scoreText[] = "Score 0000";
			temporaryScore = P2score;
			for (int i = 0; i < 4; i++) {
				P2scoreText[9 - i] = '0' + (temporaryScore % 10);
				temporaryScore = floor(temporaryScore / 10);
			}
			drawText(program, textSheet, P2scoreText, -1.7f, -0.95f, 0.05f);
		}
	}
	else if (mode == GameMode::STATE_SINGLE_PLAYER_GAME_OVER) {
		imageForWholeScreen(program, gameOverScreen);
		for (size_t i = 0; i < 3; i++) {
			gameOverButtons[i].draw(program, untexturedProgram, textSheet);
		}
		float height = 0.1f;
		float xPositionOfText = 0.0f - (float(strlen(P1scoreText) / 2.0f) * height) + (height / 2);
		float yPositionOfText = 0.1f - (height / 2);
		drawText(program, textSheet, P1scoreText, xPositionOfText, yPositionOfText, height);
	}
	else if (mode == GameMode::STATE_TWO_PLAYER_GAME_OVER) {
		imageForWholeScreen(program, gameOverScreen);
		for (size_t i = 0; i < 3; i++) {
			gameOverButtons[i].draw(program, untexturedProgram, textSheet);
		}
		float height = 0.1f;
		float xPositionOfText = 0.9f - (float(strlen(P1scoreText) / 2.0f) * height) + (height / 2);
		float yPositionOfText = 0.1f - (height / 2);
		P1scoreText[0] = 'B';
		P1scoreText[1] = 'L';
		P1scoreText[2] = 'U';
		P1scoreText[3] = 'E';
		P1scoreText[4] = ':';
		drawText(program, textSheet, P1scoreText, xPositionOfText, yPositionOfText, height);

		char P2scoreText[] = "RED: 0000";
		temporaryScore = P2score;
		for (int i = 0; i < 4; i++) {
			P2scoreText[8 - i] = '0' + (temporaryScore % 10);
			temporaryScore = floor(temporaryScore / 10);
		}
		xPositionOfText = -0.9f - (float(strlen(P2scoreText) / 2.0f) * height) + (height / 2);
		yPositionOfText = 0.1f - (height / 2);
		drawText(program, textSheet, P2scoreText, xPositionOfText, yPositionOfText, height);
	}
	else if (mode == GameMode::STATE_SINGLE_PLAYER_PAUSED || mode == GameMode::STATE_TWO_PLAYER_PAUSED) {
		drawText(program, textSheet, "PAUSED", 0.0f - (3.0f * 0.15f), 0.0f, 0.15f);
	}
}

bool GameState::UpdateGame(float elapsed, int mode) {
	player[0].update(elapsed);
	if (mode == GameMode::STATE_SINGLE_PLAYER_PLAY) {
		if (player[0].timeDead <= 0.0f) {
			backgrounds.update(elapsed, timeSurvived);
			map.update(elapsed, timeSurvived);
			player[0].checkInelasticCollision(top);
			player[0].checkInelasticCollision(bottom);
			P1score += player[0].checkMap(map, mode);
			for (size_t i = 0; i < enemies.size(); i++) {
				if (enemies[i].update(elapsed, player[0].yPosition, timeSurvived)) {
					std::swap(enemies[i], enemies[enemies.size() - 1]);
					enemies.pop_back();
				}
				else {
					player[0].collideWithRocket(enemies[i]);
				}
			}
			int numberOfValidRockets = floor(P1score / 100.0);
			if (enemies.size() < numberOfValidRockets && rocketTimer > 2.0f - (0.1f * (numberOfValidRockets - 1))) {
				int random = rand() % 5;
				if (random > (5 - floorf(timeSurvived / 10.0f))) {
					enemies.push_back(Enemy(player[0].yPosition, 0.5f, 0.5f));
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
			if (player[0].xPosition < -1.777f || player[0].yPosition < -1.0f) {
				return true;
			}
		}
	}
	else if (mode == GameMode::STATE_TWO_PLAYER_PLAY) {
		player[1].update(elapsed);
		if (player[0].timeDead <= 0.0f || player[1].timeDead <= 0.0f) {
			backgrounds.update(elapsed, timeSurvived);
			map.update(elapsed, timeSurvived);
			player[0].checkInelasticCollision(top);
			player[0].checkInelasticCollision(bottom);
			player[1].checkInelasticCollision(top);
			player[1].checkInelasticCollision(bottom);
			P1score += player[0].checkMap(map, mode);
			P2score += player[1].checkMap(map, mode);
			// rockets follor player 1
			for (size_t i = 0; i < enemies.size(); i++) {
				if (enemies[i].update(elapsed, player[0].yPosition, timeSurvived)) {
					std::swap(enemies[i], enemies[enemies.size() - 1]);
					enemies.pop_back();
				}
				else {
					player[0].collideWithRocket(enemies[i]);
					player[1].collideWithRocket(enemies[i]);
				}
			}
			int numberOfValidRockets = floor((P1score + P2score) / 100.0);
			if (enemies.size() < numberOfValidRockets && rocketTimer > 2.0f - (0.1f * (numberOfValidRockets - 1))) {
				int random = rand() % 5;
				if (random > (5 - floorf(timeSurvived / 10.0f))) {
					enemies.push_back(Enemy(player[0].yPosition, 0.5f, 0.5f));
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
			if ((player[0].xPosition < -1.777f || player[0].yPosition < -1.0f) && (player[1].xPosition < -1.777f || player[1].yPosition < -1.0f)) {
				return true;
			}
		}
	}
	return false;
}

void GameState::ProcessInput(float elapsed) {
	return;
}

void GameState::reset() {
	Background backgrounds;

	player[0] = Player();
	player[1] = Player(true);
	map = Map();
	enemies.clear();
	P1score = 0;
	P2score = 0;
	timeSurvived = 0.0f;
	rocketTimer = 0.0f;

	Mix_PlayMusic(bgm, -1);
}