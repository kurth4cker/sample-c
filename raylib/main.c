// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 kurth4cker <kurth4cker@gmail.com>

#include <stdio.h>
#include <raylib.h>

typedef struct {
	int x, y;
	float radius;
	Color color;
} Circle;

static inline void
circle_draw(Circle circle)
{
	DrawCircle(circle.x, circle.y, circle.radius, circle.color);
}

typedef struct {
	int width, height;

	Circle circle;
} Game;

static inline void
game_state(Game *game)
{
	game->width = GetScreenWidth();
	game->height = GetScreenHeight();
	// game->circle.x = game->width / 2;
	// game->circle.y = game->height / 2;
	game->circle.radius = (game->width + game->height) / 32.0f;

	const int step = 1; //(game->width + game->height) / 40.0f;

	if (IsKeyDown(KEY_W) && game->circle.y > 0) {
		game->circle.y -= step;
	}
	if (IsKeyDown(KEY_S) && game->circle.y < game->height) {
		game->circle.y += step;
	}
	if (IsKeyDown(KEY_A) && game->circle.x > 0) {
		game->circle.x -= step;
	}
	if (IsKeyDown(KEY_D) && game->circle.x < game->width) {
		game->circle.x += step;
	}
}

int
main(void)
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1920, 1080, "Initial window");
	// SetTargetFPS(60);

	Game game = {
		.width = GetScreenWidth(),
		.height = GetScreenHeight(),
		.circle = {
			.x = game.width / 2,
			.y = game.height / 2,
			.radius = game.width / 10.0f,
			.color = RED,
		},
	};

	while (!WindowShouldClose()) {
		game_state(&game);

		BeginDrawing();
		{
			ClearBackground(BLACK);
			circle_draw(game.circle);
			DrawFPS(10, 10);
		}
		EndDrawing();
	}

	CloseWindow();
}
