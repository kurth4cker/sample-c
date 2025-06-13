// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 kurth4cker <kurth4cker@gmail.com>

#include <stdio.h>
#include <math.h>
#include <raylib.h>

const int GAME_SCREEN_WIDTH = 800;
const int GAME_SCREEN_HEIGHT = 450;

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
	struct {
		int width, height;
	} window;
	RenderTexture2D scene;
	float scale;

	Circle circle;
} Game;

static inline void
game_state(Game *game)
{
	game->window.width = GetScreenWidth();
	game->window.height = GetScreenHeight();
	game->scale = fminf((float)game->window.width / GAME_SCREEN_WIDTH,
	                    (float)game->window.height / GAME_SCREEN_HEIGHT);

	const int step = 1; //(game->width + game->height) / 40.0f;

	if (IsKeyDown(KEY_W) && game->circle.y > 0) {
		game->circle.y -= step;
	}
	if (IsKeyDown(KEY_S) && game->circle.y < game->scene.texture.height) {
		game->circle.y += step;
	}
	if (IsKeyDown(KEY_A) && game->circle.x > 0) {
		game->circle.x -= step;
	}
	if (IsKeyDown(KEY_D) && game->circle.x < game->scene.texture.width) {
		game->circle.x += step;
	}
}

int
main(void)
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT, "Initial window");
	// SetTargetFPS(60);

	Game game = {
		.window = {
			.width = GetScreenWidth(),
			.height = GetScreenHeight(),
		},
		.scene = LoadRenderTexture(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT),
		.scale = 1.0f,
		.circle = {
			.x = GAME_SCREEN_WIDTH / 2,
			.y = GAME_SCREEN_HEIGHT / 2,
			.radius = 10.0f,
			.color = RED,
		},
	};
	SetTextureFilter(game.scene.texture, TEXTURE_FILTER_BILINEAR);

	while (!WindowShouldClose()) {
		game_state(&game);

		BeginTextureMode(game.scene);
		{
			ClearBackground(GetColor(0x101020ff));
			circle_draw(game.circle);
			DrawFPS(10, 10);
		}
		EndTextureMode();

		BeginDrawing();
		{
			ClearBackground(BLACK);

			float offsetX = (game.window.width - (GAME_SCREEN_WIDTH * game.scale)) * 0.5f;
			float offsetY = (game.window.height - (GAME_SCREEN_HEIGHT * game.scale)) * 0.5f;

			DrawTexturePro(game.scene.texture,
			               (Rectangle){0, 0, GAME_SCREEN_WIDTH, -GAME_SCREEN_HEIGHT},
			               (Rectangle){ offsetX, offsetY, GAME_SCREEN_WIDTH * game.scale, GAME_SCREEN_HEIGHT * game.scale },
			               (Vector2){0, 0},
			               0.0f,
			               WHITE);
		}
		EndDrawing();
	}

	UnloadRenderTexture(game.scene);
	CloseWindow();
}
