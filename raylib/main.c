// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 kurth4cker <kurth4cker@gmail.com>

#include <stdio.h>
#include <raylib.h>

typedef struct {
	int x, y;
	float radius;
} Circle;

typedef struct {
	int width, height;

	Circle circle;
} Game;

int
main(void)
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 600, "Initial window");
	// SetTargetFPS(60);

	Game game = {
		.width = GetScreenWidth(),
		.height = GetScreenHeight(),
		.circle = {
			.x = game.width / 2,
			.y = game.height / 2,
			.radius = game.width / 10.0f,
		},
	};

	while (!WindowShouldClose()) {
		game.width = GetScreenWidth();
		game.height = GetScreenHeight();
		game.circle.x = game.width / 2;
		game.circle.y = game.height / 2;
		game.circle.radius = (game.width + game.height) / 32.0f;

		BeginDrawing();
		{
			ClearBackground(BLACK);
			DrawFPS(10, 10);
			DrawCircle(game.circle.x, game.circle.y, game.circle.radius, RED);
		}
		EndDrawing();
	}

	CloseWindow();
}
