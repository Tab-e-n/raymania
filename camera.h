#ifndef RM_CAMERA
#define RM_CAMERA

#include <raylib.h>
#include <raymath.h>
#include "rmlib.h"

typedef struct CameraRM
{
	Camera2D data;
	Vector2 cam_pos;
	float fade;
} CameraRM;

CameraRM InitCamera(void);

void CenterCameraOffset(CameraRM* camera, bool reset);

void MoveCameraSmooth(CameraRM* camera, Vector2 position, float speed);
void MoveCameraInstant(CameraRM* camera, Vector2 position);
void MoveCameraSmoothInvert(CameraRM* camera, Vector2 position, float speed);

void ZoomCameraSmooth(CameraRM* camera, float zoom, float speed);
void ZoomCameraInstant(CameraRM* camera, float zoom);

void ShakeCamera(CameraRM* camera, float time, float amplitude, float frequency, float weight);

bool FadeIn(CameraRM* camera, float amount);
bool FadeOut(CameraRM* camera, float amount);
void DrawFade(CameraRM* camera, Color color);

void InvertOffsetToTarget(CameraRM* camera, Vector2 position);

#endif
