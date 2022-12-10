#include "scaleMouse.h"

Vector2 scaleMouse = Vector2{ 1.0f, 1.0f };

Vector2 GetScaledMousePos()
{
	Vector2 pos = GetMousePosition();
#if defined (PLATFORM_WEB)
	pos.x *= scaleMouse.x;
	pos.y *= scaleMouse.y;
#endif
	return pos;
}

void MouseScaleSet(Vector2 target)
{
	scaleMouse = target;
}
