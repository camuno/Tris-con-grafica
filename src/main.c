#include "raylib.h"

int board[3][3] = { 0 };

// Controllo vittoria
int controlloVittoria() {
	int cont = 0;

	// Righe
	for (int i = 0; i < 3; i++) {
		if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != 0) {
			return 1;
		}
	}

	// Colonne
	for (int i = 0; i < 3; i++) {
		if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != 0) {
			return 1;
		}
	}

	// Diagonali
	if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != 0) {
		return 1;
	}

	if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != 0) {
		return 1;
	}

	// Parità

	for (int i = 0; i < 3; i++) {
		if (board[i][0] != 0 && board[i][1] != 0 && board[i][2] != 0) {
			cont++;
		}

		if (cont == 3) {
			return 2;
		}
	}

	return 0;
}

int main(void) {
	SetTraceLogLevel(LOG_NONE);

	const int screenWidth = 1024;
	const int screenHeight = 1024;

	InitWindow(screenWidth, screenHeight, "Tris");

	InitAudioDevice();

	Sound buttonSound1 = LoadSound("src/assets/button-1.mp3");
	Sound buttonSound2 = LoadSound("src/assets/button-2.mp3");

	Music backgroundMusic = LoadMusicStream("src/assets/soundtrack/assassins-creed-soundtrack.mp3");
	backgroundMusic.looping = false;
	SetMusicVolume(backgroundMusic, 0.3f);

	Texture2D boardTexture = LoadTexture("src/assets/tris-board.png");
	Texture2D boardOTexture = LoadTexture("src/assets/tris-o.png");
	Texture2D boardXTexture = LoadTexture("src/assets/tris-x.png");

	Rectangle areaBoard[3][3] = { {
			{ 0.0f, 0.0f, boardTexture.width / 3, boardTexture.height / 3 },
			{ boardTexture.width / 3, 0.0f, boardTexture.width / 3, boardTexture.height / 3 },
			{ boardTexture.width / 3 + boardTexture.width / 3, 0.0f, boardTexture.width / 3, boardTexture.height / 3 }
		},{
			{ 0.0f, boardTexture.height / 3, boardTexture.width / 3, boardTexture.height / 3 },
			{ boardTexture.width / 3, boardTexture.height / 3, boardTexture.width / 3, boardTexture.height / 3 },
			{ boardTexture.width / 3 + boardTexture.width / 3, boardTexture.height / 3, boardTexture.width / 3, boardTexture.height / 3 }
		},{
			{ 0.0f, boardTexture.height / 3 + boardTexture.height / 3, boardTexture.width / 3, boardTexture.height / 3 },
			{ boardTexture.width / 3, boardTexture.height / 3 + boardTexture.height / 3, boardTexture.width / 3, boardTexture.height / 3 },
			{ boardTexture.width / 3 + boardTexture.width / 3, boardTexture.height / 3 + boardTexture.height / 3, boardTexture.width / 3, boardTexture.height / 3 }
		}
	};

	int turno = 0, vittoria = 0;
	Vector2 mousePoint = { 0.0f, 0.0f };
	Rectangle againButton = { 188.0f, 457.0f, 300.0f, 50.0f };
	Rectangle exitButton = { 188.0f, 517.0f, 200.0f, 50.0f };

	PlayMusicStream(backgroundMusic);

	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		mousePoint = GetMousePosition();

		if (IsMusicPlaying(backgroundMusic)) {
			UpdateMusicStream(backgroundMusic);
		}

		if (IsKeyPressed(KEY_P)) {
			if (IsMusicPlaying(backgroundMusic)) {
				PauseMusicStream(backgroundMusic);
			}
			else {
				ResumeMusicStream(backgroundMusic);
			}
		}

		BeginDrawing();

		ClearBackground(RAYWHITE);

		DrawTextureEx(boardTexture, (Vector2) { 0.0f, 0.0f }, 0.0f, 1.0f, RAYWHITE);

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (board[i][j] == 1) {
					DrawTextureEx(boardXTexture, (Vector2) { areaBoard[i][j].x + 15, areaBoard[i][j].y + 15 }, 0.0f, 1.0f, RAYWHITE);
				}
				else if (board[i][j] == 2) {
					DrawTextureEx(boardOTexture, (Vector2) { areaBoard[i][j].x + 15, areaBoard[i][j].y + 15 }, 0.0f, 1.0f, RAYWHITE);
				}
			}
		}

		vittoria = controlloVittoria();

		if (vittoria != 0) {
			if (CheckCollisionPointRec(mousePoint, exitButton)) {
				if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
					DrawRectangle(exitButton.x, exitButton.y, exitButton.width, exitButton.height, BEIGE);
				}
				else {
					DrawRectangle(exitButton.x, exitButton.y, exitButton.width, exitButton.height, BROWN);
				}

				if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
					break;
				}
			}
			else {
				DrawRectangle(exitButton.x, exitButton.y, exitButton.width, exitButton.height, MAROON);
			}
			DrawText("Exit", exitButton.x + 53, exitButton.y + 2, exitButton.height, BLACK);

			if (CheckCollisionPointRec(mousePoint, againButton)) {
				if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
					DrawRectangle(againButton.x, againButton.y, againButton.width, againButton.height, BEIGE);
				}
				else {
					DrawRectangle(againButton.x, againButton.y, againButton.width, againButton.height, BROWN);
				}

				if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
					for (int i = 0; i < 3; i++) {
						for (int j = 0; j < 3; j++) {
							board[i][j] = 0;
						}
					}
					PlaySound(buttonSound1);
				}
			}
			else {
				DrawRectangle(againButton.x, againButton.y, againButton.width, againButton.height, MAROON);
			}
			DrawText("Play Again", againButton.x + 20, againButton.y, againButton.height, BLACK);
		}
		else {
			if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						if (CheckCollisionPointRec(mousePoint, areaBoard[i][j])) {
							if (board[i][j] == 0) {
								if (turno == 0) {
									board[i][j] = 1;
									turno = 1;
								}
								else {
									board[i][j] = 2;
									turno = 0;
								}
							}
						}
					}
				}
			}
		}

		EndDrawing();
	}

	UnloadMusicStream(backgroundMusic);

	CloseAudioDevice();

	CloseWindow();

	return 0;
}