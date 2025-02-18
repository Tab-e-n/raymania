#ifndef RM_RACECAR
#define RM_RACECAR

#include <raylib.h>
#include <raymath.h>
#include "rmlib.h"
#include "blocks.h"
#include "asset.h"

#define RACECAR_DEBUG false


typedef enum DefaultCar {CAR_ROAD = 0, CAR_DRIFT, CAR_GRIP, CAR_TERRAIN} DefaultCar;
typedef enum DefaultEnviroment {ENV_VOID, ENV_MEADOW, ENV_QUARRY, ENV_ISLAND} DefaultEnviroment;

typedef struct SurfaceStats
{
	float acceleration, // Increase to velocity when holding up.
	      decceleration, // Decrease to velocity when holding down.
	      friction, // Decrease to velocity, scales based on current speed.
	      top_speed, // Max allowed speed when accelerating (Car can go faster than speed specified).
	      min_speed, // top speed but backwards.

	      turn_speed, // Angle change when turning.
	      turn_speed_fix_threshold, // Will scale turning based on speed, making the turn radius the same until the specified speed.
	      turn_dir_gain, // When turning, turning speed will build up from zero by amount specified. A value of 1 is instant.
	      turn_dir_loss, // After turning, turning speed will lower by amount specified. A value of 1 is instant.

	      redirect_angle, // Will snap velocity's angle to car's rotation if the angle difference between them is under the value specified.
	      redirect_angle_loss;
	bool flip_turning_when_backwards, // When car is going backwards, turn speed is flipped. Feels more natural this way.
	     flip_turning_when_going_backwards, // Same as previous, but only when the player is holding down.
	     do_vel_redirection; // Activates vel redirection. see redirect_angle for explanation.
} SurfaceStats;

typedef struct CarStats
{
	SurfaceStats surface[SURFACE_AMOUNT];
	float camera_shake_threshold,
	      speed_to_shake_ratio,
	      shake_amplitude,
	      shake_frequency,
	      shake_weight;
	Vector2 size;
} CarStats;

typedef struct Racecar
{
	Wall w_front,
	     w_back,
	     w_left,
	     w_right;
	Vector2 position,
		rotation,
		velocity;
	float turn_dir;
	unsigned char palette, model;
} Racecar;

typedef struct WallCollision
{
	Wall wall;
	Vector2 point;
} WallCollision;

typedef struct MetaInfo
{
	bool checkpoint,
	     finish;
	Vector2 check_pos,
		check_rot;
} MetaInfo;


int GetModelID(unsigned char model);

CarStats DefaultStats(DefaultCar car);
Racecar InitRacecar(CarStats* stats);

void ResetRacecar(Racecar* car, Vector2 pos, Vector2 rot, Vector2 size);

void CalculateRacecarWalls(Racecar* car, Vector2 size);

void RotateRacecar(Racecar* car, float angle);
bool RacecarCanTurn(Racecar* car);
void TurnRacecar(Racecar* car, float angle, float limit, bool invert_when_reverse);

void IncreaseRacecarVelocity(Racecar* car, float amount, Vector2 direction);
void DecreaseRacecarVelocity(Racecar* car, float amount, Vector2 direction);
void AccelerateRacecar(Racecar* car, float amount);
void ApplyFrictionRacecar(Racecar* car, float amount);
void CapRacecarVelocity(Racecar* car, float max);
void CapRacecarVelocityBiDir(Racecar* car, float max, float min);
void RedirectRacecarVelocity(Vector2* velocity, Vector2 direction, float angle_change, float angle_loss);

bool RacecarMaxVelocity(Racecar* car, float max);
bool CheckRacecarWallColliding(Wall racecar_wall, BlockWallArray block_walls[MAX_LOADED_BLOCK_WALLS], WallCollision* collision);
Block CheckRacecarAreaColliding(Racecar* car, BlockWallArray block_walls[MAX_LOADED_BLOCK_WALLS], Block blocks[MAX_BLOCK_AMOUNT]);

void MoveRacecar(Racecar* car, BlockWallArray block_walls[MAX_LOADED_BLOCK_WALLS]);

Vector2int RacecarPlacement(Racecar* car);

Tri RotateTriCar(Tri tri, Vector2 rotation);
void DrawRacecar(Racecar* car, bool ghost);
void DrawRacecarWalls(Racecar* car, bool ghost);

MetaInfo ProcessRacecar(Racecar* car, CarStats* car_stats, Block blocks[MAX_BLOCK_AMOUNT], BlockWallArray block_walls[MAX_LOADED_BLOCK_WALLS], RMInput input, DefaultEnviroment env);

#endif
