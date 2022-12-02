#include "normal_snake.h"

Vector2 up = Vector2{ 0.0f, 1.0f };
Vector2 down = Vector2{ 0.0f, -1.0f };
Vector2 left = Vector2{ -1.0f, 0.0f };
Vector2 right = Vector2{ 1.0f, 0.0f };

NormalSnake::NormalSnake(Vector2 startPos, Vector2 dir)
{
	controls[up] = KEY_UP;
	controls[down] = KEY_DOWN;
	controls[left] = KEY_LEFT;
	controls[right] = KEY_RIGHT;


}