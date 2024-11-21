#include <raylib.h>
#include <raymath.h>
#include "rmlib.h"
#include "camera.h"

CameraRM InitCamera(void)
{
	Camera2D camera;
	camera.target = Vector2Zero();
	camera.offset = SCREEN_CENTER;
	camera.rotation = 0;
	camera.zoom = 1.0;
	CameraRM rm_cam;
	rm_cam.data = camera;
	rm_cam.cam_pos = Vector2Zero();

	return rm_cam;
}

void CenterCameraOffset(CameraRM* camera, bool reset)
{
	if(reset)
	{
		camera->data.offset = SCREEN_CENTER;
	}
	else
	{
		camera->data.offset = Vector2Add(camera->data.offset, SCREEN_CENTER);
	}
}

void MoveCameraSmooth(CameraRM* camera, Vector2 position, float speed)
{
	Vector2 dif = Vector2Subtract(position, camera->data.target);
	float dist = Vector2Length(dif);
	if(dist < 1)
	{
		camera->data.target = position;
	}
	else
	{
		speed *= dist;
		if(speed < 1) speed = 1;
		Vector2 shift = Vector2Scale(Vector2Normalize(dif), speed);
		camera->data.target = Vector2Add(camera->data.target, shift);
	}
	camera->cam_pos = camera->data.target;
}

void MoveCameraInstant(CameraRM* camera, Vector2 position)
{
	camera->data.target = position;
	camera->cam_pos = camera->data.target;
}

void MoveCameraSmoothInvert(CameraRM* camera, Vector2 position, float speed)
{
	camera->data.target = camera->cam_pos;
	MoveCameraSmooth(camera, position, speed);
	camera->cam_pos = camera->data.target;
	camera->data.target = InvertAroundPoint(camera->cam_pos, position);
	
}

void ZoomCameraSmooth(CameraRM* camera, float zoom, float speed)
{
	float size_old = SCREEN_SIZE.x / camera->data.zoom;
	float size_new = SCREEN_SIZE.x / zoom;
	float dif = size_new - size_old;
	if(absf(dif) < 1)
	{
		camera->data.zoom = zoom;
	}
	else
	{
		speed *= dif;
		if(absf(speed) < 1) speed = sign(speed);
		size_old += speed;
		camera->data.zoom = SCREEN_SIZE.x / size_old;
	}
}

void ZoomCameraInstant(CameraRM* camera, float zoom)
{
	camera->data.zoom = zoom;
}

void ShakeCamera(CameraRM* camera, float time, float amplitude, float frequency, float weight)
{
	// thx BITOMIX
	float weighted_amplitude = amplitude * weight;
	float weighted_frequency = frequency * weight;

	float shake_x = (AirQuotesNoise(time * frequency, false) - 0.5) * amplitude;
	float shake_y = (AirQuotesNoise(time * frequency, true) - 0.5) * amplitude;
	shake_x += (AirQuotesNoise(time * weighted_frequency, false) - 0.5) * weighted_amplitude;
	shake_y += (AirQuotesNoise(time * weighted_frequency, true) - 0.5) * weighted_amplitude;

	camera->data.offset = (Vector2){shake_x , shake_y};
	CenterCameraOffset(camera, false);
}

bool FadeIn(CameraRM* camera, float amount)
{
	camera->fade -= amount;
	if(camera->fade < 0.0)
	{
		camera->fade = 0.0;
	}
}

bool FadeOut(CameraRM* camera, float amount)
{
	camera->fade += amount;
	if(camera->fade > 1.0)
	{
		camera->fade = 1.0;
	}
}

void DrawFade(CameraRM* camera, Color color)
{
	if(camera->fade > 0.0)
	{
		Color col = (Color){color.r, color.g, color.b, (int)(camera->fade * 255.0)};
		DrawRectangle(0, 0, SCREEN_SIZE.x, SCREEN_SIZE.y, col);
	}
}

void InvertOffsetToTarget(CameraRM* camera, Vector2 position)
{
	Vector2 dif = Vector2Subtract(position, camera->data.target);
	camera->data.offset = Vector2Scale(dif, -2);
	camera->data.offset = Vector2Add(camera->data.offset, SCREEN_CENTER);
}
