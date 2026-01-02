#ifndef RM_RACECAR
#define RM_RACECAR

#include <raylib.h>
#include <raymath.h>
#include "rmlib.h"
#include "blocks.h"
#include "asset.h"

#define RACECAR_DEBUG false
#define GEAR_AMOUNT 5
#define SKID_LINE_COUNT 60


typedef enum DefaultCar {CAR_ROAD = 0, CAR_DRIFT, CAR_GRIP, CAR_TERRAIN} DefaultCar;
typedef enum DefaultEnviroment {ENV_VOID, ENV_MEADOW, ENV_QUARRY, ENV_ISLAND} DefaultEnviroment;

typedef struct SurfaceStats
{
	float acceleration[GEAR_AMOUNT], // Increase to velocity when holding up.
		decceleration, // Decrease to velocity when holding down.
		boost_acceleration, // Increase to velocity when on a booster
		gear_shift_acceleration_punish, // When shifting gears, multiplies accel by this number.
		friction, // Decrease to velocity, scales based on current speed.
		//top_speed, // Max allowed speed when accelerating (Car can go faster than speed specified).
		min_speed, // top speed but backwards.

		turn_speed, // Angle change when turning.
		turn_speed_fix_threshold, // Will scale turning based on speed, making the turn radius the same until the specified speed.
		turn_dir_gain, // When turning, turning speed will build up from zero by amount specified. A value of 1 is instant.
		turn_dir_loss, // After turning, turning speed will lower by amount specified. A value of 1 is instant.

		redirect_angle, // Will snap velocity's angle to car's rotation if the angle difference between them is under the value specified.
		redirect_angle_loss,
		redirect_speed_loss; // Percentage of speed that remains after redirect.
	bool flip_turning_when_backwards, // When car is going backwards, turn speed is flipped. Feels more natural this way.
		flip_turning_when_going_backwards, // Same as previous, but only when the player is holding down.
		do_vel_redirection, // Activates vel redirection. see redirect_angle for explanation.
		redirect_towards_held_dir, // Instead of redirecting towards the closer dir, redirect towards the dir the player is holding.
		always_apply_friction; // Friction is applied even when holding a direction.
} SurfaceStats;

typedef struct CarStats
{
	SurfaceStats surface[SURFACE_AMOUNT];
	float gears[GEAR_AMOUNT],
		gear_shift_speed,
		gear_shift_turn_penalty,
		camera_shake_threshold,
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
	float turn_dir, gear_shift, boost;
	unsigned char palette, model, gear;
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

typedef struct SkidLinePoint
{
	bool draw;
	Vector2 position,
		rotation;
} SkidLinePoint;


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

void RacecarRedirect(Vector2* velocity, Vector2 direction, float abs_angle, float angle_change, float vel_percent);
void RacecarRedirectCollision(Vector2* velocity, Vector2 direction, float angle_change);
float RacecarAngleChange(float base, float vel_angle, float abs_angle, float loss);
void RacecarRedirectClosest(Racecar* car, float angle_change, float angle_loss, float vel_percent);
void RacecarRedirectHeld(Racecar* car, bool flip, float angle_change, float angle_loss, float vel_percent);

bool RacecarMaxVelocity(Racecar* car, float max);
bool CheckRacecarWallColliding(Wall racecar_wall, BlockWallArray block_walls[MAX_LOADED_BLOCK_WALLS], WallCollision* collision);
Block CheckRacecarAreaColliding(Racecar* car, BlockWallArray block_walls[MAX_LOADED_BLOCK_WALLS], Block blocks[MAX_BLOCK_AMOUNT]);

void RacecarGearUp(Racecar* car, float gears[GEAR_AMOUNT]);
void RacecarGearDown(Racecar* car, float gears[GEAR_AMOUNT]);

void MoveRacecar(Racecar* car, BlockWallArray block_walls[MAX_LOADED_BLOCK_WALLS]);

Vector2int RacecarPlacement(Racecar* car);

Asset* AllocRacecarAsset(unsigned char model);
Tri RotateTriCar(Tri tri, Vector2 rotation);
void DrawRacecar(Racecar* car, bool ghost);
void DrawRacecarWalls(Racecar* car, bool ghost);

MetaInfo ProcessRacecar(Racecar* car, CarStats* car_stats, Block blocks[MAX_BLOCK_AMOUNT], BlockWallArray block_walls[MAX_LOADED_BLOCK_WALLS], RMInput input, DefaultEnviroment env);

void ClearSkidLine(SkidLinePoint points[SKID_LINE_COUNT]);
void AddSkidLinePoint(Racecar* car, SkidLinePoint points[SKID_LINE_COUNT]);
void DrawSkidLine(SkidLinePoint points[SKID_LINE_COUNT], unsigned char model, Color color);

#endif
