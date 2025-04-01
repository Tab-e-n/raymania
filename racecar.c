#include <raylib.h>
#include <raymath.h>
#include "rmlib.h"
#include "blocks.h"
#include "asset.h"
#include "racecar.h"

int GetModelID(unsigned char model)
{
	switch(model)
	{
		default:
			return DRACECAR;
	}
	return DRACECAR;
}

// CAR DEFINITIONS
// CAR_ROAD    - Slow Accel, Base Car
// CAR_DRIFT   - Medium Accel, Drifting
// CAR_GRIP    - High Accel, High Turn Speed
// CAR_TERRAIN - Slowest Accel, More Precision (Coast)

CarStats DefaultStats(DefaultCar car)
{
	CarStats car_stats;
	SurfaceStats stats;
	switch(car)
	{
		case(CAR_ROAD):
			car_stats.size = (Vector2){14, 23};
			car_stats.camera_shake_threshold = 0.8;
			car_stats.speed_to_shake_ratio = 0.125;
			car_stats.shake_amplitude = 5.0;
			car_stats.shake_frequency = 2.0;
			car_stats.shake_weight = 5.0;

			car_stats.gears[0] = 4;
			car_stats.gears[1] = 7;
			car_stats.gears[2] = 10;
			car_stats.gears[3] = 14;
			car_stats.gears[4] = 20;
			car_stats.gear_shift_speed = FRAME * 4;

			stats.min_speed = 4;
			stats.acceleration[0] = 0.075;
			stats.acceleration[1] = 0.05;
			stats.acceleration[2] = 0.03;
			stats.acceleration[3] = 0.01;
			stats.acceleration[4] = 0.001;
			stats.decceleration = 0.1;
			stats.boost_acceleration = 0.15;
			stats.gear_shift_acceleration_punish = 0.3;
			stats.friction = 0.05;
			stats.always_apply_friction = false;

			stats.turn_speed = PI*0.0075;
			stats.turn_speed_fix_threshold = 3;
			stats.turn_dir_gain = 0.1;
			stats.turn_dir_loss = 1.0;
			stats.flip_turning_when_backwards = true;
			stats.flip_turning_when_going_backwards = false;

			stats.redirect_angle = PI*0.012;
			stats.redirect_angle_loss = 0.004;
			stats.redirect_speed_loss = 0.01;
			stats.do_vel_redirection = true;
			car_stats.surface[SURFACE_ASPHALT] = stats;

			stats.min_speed = 2;
			stats.acceleration[0] = 0.05;
			stats.acceleration[1] = 0.025;
			stats.acceleration[2] = 0.001;
			stats.acceleration[3] = 0.001;
			stats.acceleration[4] = 0.001;
			stats.friction = 0.02;
			stats.always_apply_friction = true;
			stats.redirect_angle = PI*0.007;
			stats.redirect_angle_loss = 0.014;
			stats.redirect_speed_loss = 0.05;
			car_stats.surface[SURFACE_GRASS] = stats;

			stats.min_speed = 8;
			stats.acceleration[0] = 0.075;
			stats.acceleration[1] = 0.05;
			stats.acceleration[2] = 0.03;
			stats.acceleration[3] = 0.01;
			stats.acceleration[4] = 0.001;
			stats.friction = 0.05;
			stats.always_apply_friction = false;
			stats.turn_speed = PI*0.022;
			stats.turn_speed_fix_threshold = 10;
			stats.redirect_angle = PI*0.009;
			stats.redirect_angle_loss = 0.012;
			stats.redirect_speed_loss = 0.005;
			car_stats.surface[SURFACE_DIRT] = stats;

			stats.min_speed = 8;
			stats.acceleration[0] = 0.05;
			stats.acceleration[1] = 0.04;
			stats.acceleration[2] = 0.03;
			stats.acceleration[3] = 0.02;
			stats.acceleration[4] = 0.01;
			stats.friction = 0.005;
			stats.always_apply_friction = false;
			stats.turn_speed = PI*0.0075;
			stats.turn_speed_fix_threshold = 3;
			stats.flip_turning_when_backwards = false;
			stats.flip_turning_when_going_backwards = true;
			stats.do_vel_redirection = false;
			car_stats.surface[SURFACE_ICE] = stats;

			stats.min_speed = 1;
			stats.acceleration[0] = 0.05;
			stats.acceleration[1] = 0.001;
			stats.acceleration[2] = 0.001;
			stats.acceleration[3] = 0.001;
			stats.acceleration[4] = 0.001;
			stats.friction = 0.03;
			stats.always_apply_friction = true;
			stats.flip_turning_when_backwards = false;
			stats.flip_turning_when_going_backwards = false;
			stats.do_vel_redirection = false;
			car_stats.surface[SURFACE_WATER] = stats;

			break;
		case(CAR_DRIFT):
			car_stats.size = (Vector2){14, 23};
			car_stats.camera_shake_threshold = 0.8;
			car_stats.speed_to_shake_ratio = 0.125;
			car_stats.shake_amplitude = 5.0;
			car_stats.shake_frequency = 2.0;
			car_stats.shake_weight = 5.0;

			car_stats.gears[0] = 2;
			car_stats.gears[1] = 6;
			car_stats.gears[2] = 10;
			car_stats.gears[3] = 14;
			car_stats.gears[4] = 18;
			car_stats.gear_shift_speed = FRAME * 8;

			stats.min_speed = 4;
			stats.acceleration[0] = 0.05;
			stats.acceleration[1] = 0.07;
			stats.acceleration[2] = 0.03;
			stats.acceleration[3] = 0.01;
			stats.acceleration[4] = 0.001;
			stats.decceleration = 0.05;
			stats.boost_acceleration = 0.1;
			stats.gear_shift_acceleration_punish = 0.3;
			stats.friction = 0.03;
			stats.always_apply_friction = false;

			stats.turn_speed = PI*0.0355;
			stats.turn_speed_fix_threshold = 18;
			stats.turn_dir_gain = 1.0;
			stats.turn_dir_loss = 1.0;
			stats.flip_turning_when_backwards = true;
			stats.flip_turning_when_going_backwards = false;

			stats.redirect_angle = PI*0.0045;
			stats.redirect_angle_loss = -0.005;
			stats.redirect_speed_loss = 0.01;
			stats.do_vel_redirection = true;
			car_stats.surface[SURFACE_ASPHALT] = stats;

			stats.min_speed = 4;
			stats.acceleration[0] = 0.03;
			stats.acceleration[1] = 0.02;
			stats.acceleration[2] = 0.003;
			stats.acceleration[3] = 0.001;
			stats.acceleration[4] = 0.001;
			stats.friction = 0.025;
			stats.always_apply_friction = true;
			stats.redirect_angle = PI*0.002;
			stats.redirect_angle_loss = 0.0008;
			stats.redirect_speed_loss = 0.05;
			car_stats.surface[SURFACE_GRASS] = stats;

			stats.min_speed = 8;
			stats.acceleration[0] = 0.04;
			stats.acceleration[1] = 0.075;
			stats.acceleration[2] = 0.035;
			stats.acceleration[3] = 0.01;
			stats.acceleration[4] = 0.001;
			stats.friction = 0.02;
			stats.always_apply_friction = false;
			stats.redirect_angle = PI*0.0025;
			stats.redirect_angle_loss = -0.0045;
			stats.redirect_speed_loss = 0.000;
			car_stats.surface[SURFACE_DIRT] = stats;

			stats.min_speed = 8;
			stats.acceleration[0] = 0.0245;
			stats.acceleration[1] = 0.025;
			stats.acceleration[2] = 0.022;
			stats.acceleration[3] = 0.021;
			stats.acceleration[4] = 0.0021;
			stats.friction = 0.007;
			stats.always_apply_friction = false;
			stats.redirect_angle = PI*0.0002;
			stats.redirect_angle_loss = 0.0002;
			stats.redirect_speed_loss = 0.0;
			car_stats.surface[SURFACE_ICE] = stats;

			stats.min_speed = 1;
			stats.acceleration[0] = 0.0245;
			stats.acceleration[1] = 0.025;
			stats.acceleration[2] = 0.022;
			stats.acceleration[3] = 0.021;
			stats.acceleration[4] = 0.0021;
			stats.friction = 0.02;
			stats.always_apply_friction = true;
			stats.flip_turning_when_backwards = false;
			stats.flip_turning_when_going_backwards = false;
			stats.do_vel_redirection = false;
			car_stats.surface[SURFACE_WATER] = stats;

			break;
		case(CAR_GRIP):
			car_stats.size = (Vector2){14, 23};
			car_stats.camera_shake_threshold = 0.8;
			car_stats.speed_to_shake_ratio = 0.085;
			car_stats.shake_amplitude = 5.0;
			car_stats.shake_frequency = 2.0;
			car_stats.shake_weight = 5.0;

			car_stats.gears[0] = 8;
			car_stats.gears[1] = 14;
			car_stats.gears[2] = 18;
			car_stats.gears[3] = 21;
			car_stats.gears[4] = 24;
			car_stats.gear_shift_speed = FRAME * 4;

			stats.min_speed = 5;
			stats.acceleration[0] = 0.1;
			stats.acceleration[1] = 0.05;
			stats.acceleration[2] = 0.025;
			stats.acceleration[3] = 0.005;
			stats.acceleration[4] = 0.001;
			stats.decceleration = 0.1;
			stats.boost_acceleration = 0.2;
			stats.gear_shift_acceleration_punish = 0.1;
			stats.friction = 0.01;
			stats.always_apply_friction = true;

			stats.turn_speed = PI*0.0175;
			stats.turn_speed_fix_threshold = 14;
			stats.turn_dir_gain = 0.2;
			stats.turn_dir_loss = 1.0;
			stats.flip_turning_when_backwards = true;
			stats.flip_turning_when_going_backwards = false;

			stats.redirect_angle = PI*0.022;
			stats.redirect_angle_loss = 0.012;
			stats.redirect_speed_loss = 0.05;
			stats.do_vel_redirection = true;
			car_stats.surface[SURFACE_ASPHALT] = stats;

			stats.min_speed = 5;
			stats.acceleration[0] = 0.01;
			stats.acceleration[1] = 0.005;
			stats.acceleration[2] = 0.001;
			stats.acceleration[3] = 0.001;
			stats.acceleration[4] = 0.001;
			stats.friction = 0.008;
			stats.turn_speed = PI*0.0125;
			stats.turn_speed_fix_threshold = 14;
			stats.redirect_angle = PI*0.02;
			stats.redirect_angle_loss = 0.024;
			stats.redirect_speed_loss = 0.1;
			car_stats.surface[SURFACE_GRASS] = stats;

			stats.min_speed = 6;
			stats.acceleration[0] = 0.075;
			stats.acceleration[1] = 0.0325;
			stats.acceleration[2] = 0.0035;
			stats.acceleration[3] = 0.001;
			stats.acceleration[4] = 0.001;
			stats.friction = 0.014;
			stats.turn_speed = PI*0.00875;
			stats.turn_speed_fix_threshold = 7;
			stats.redirect_angle = PI*0.0055;
			stats.redirect_angle_loss = 0.0022;
			stats.redirect_speed_loss = 0.02;
			car_stats.surface[SURFACE_DIRT] = stats;

			stats.min_speed = 8;
			stats.acceleration[0] = 0.1;
			stats.acceleration[1] = 0.075;
			stats.acceleration[2] = 0.05;
			stats.acceleration[3] = 0.025;
			stats.acceleration[4] = 0.005;
			stats.friction = 0.0004;
			stats.turn_speed = PI*0.00432;
			stats.turn_speed_fix_threshold = 3.5;
			stats.flip_turning_when_backwards = false;
			stats.flip_turning_when_going_backwards = true;
			stats.do_vel_redirection = false;
			car_stats.surface[SURFACE_ICE] = stats;

			stats.min_speed = 1;
			stats.acceleration[0] = 0.05;
			stats.acceleration[1] = 0.001;
			stats.acceleration[2] = 0.001;
			stats.acceleration[3] = 0.001;
			stats.acceleration[4] = 0.001;
			stats.friction = 0.04;
			stats.always_apply_friction = true;
			stats.flip_turning_when_backwards = false;
			stats.flip_turning_when_going_backwards = false;
			stats.do_vel_redirection = false;
			car_stats.surface[SURFACE_WATER] = stats;

			break;
		case(CAR_TERRAIN):
			car_stats.size = (Vector2){14, 23};
			car_stats.camera_shake_threshold = 0.8;
			car_stats.speed_to_shake_ratio = 0.175;
			car_stats.shake_amplitude = 5.0;
			car_stats.shake_frequency = 2.0;
			car_stats.shake_weight = 5.0;

			car_stats.gears[0] = 2;
			car_stats.gears[1] = 5;
			car_stats.gears[2] = 8;
			car_stats.gears[3] = 12;
			car_stats.gears[4] = 16;
			car_stats.gear_shift_speed = FRAME;

			stats.min_speed = 4;
			stats.acceleration[0] = 0.05;
			stats.acceleration[1] = 0.03;
			stats.acceleration[2] = 0.02;
			stats.acceleration[3] = 0.01;
			stats.acceleration[4] = 0.001;
			stats.decceleration = 0.02;
			stats.boost_acceleration = 0.1;
			stats.gear_shift_acceleration_punish = 0.3;
			stats.friction = 0.01;
			stats.always_apply_friction = false;

			stats.turn_speed = PI*0.0035;
			stats.turn_speed_fix_threshold = 1.5;
			stats.turn_dir_gain = 0.1;
			stats.turn_dir_loss = 0.1;
			stats.flip_turning_when_backwards = true;
			stats.flip_turning_when_going_backwards = false;

			stats.redirect_angle = PI*0.005;
			stats.redirect_angle_loss = 0.015;
			stats.redirect_speed_loss = 0.02;
			stats.do_vel_redirection = true;
			car_stats.surface[SURFACE_ASPHALT] = stats;

			stats.min_speed = 2;
			stats.acceleration[0] = 0.02;
			stats.acceleration[1] = 0.01;
			stats.acceleration[2] = 0.001;
			stats.acceleration[3] = 0.001;
			stats.acceleration[4] = 0.001;
			stats.friction = 0.015;
			stats.redirect_angle = PI*0.002;
			stats.redirect_angle_loss = 0.005;
			stats.redirect_speed_loss = 0.01;
			car_stats.surface[SURFACE_GRASS] = stats;

			stats.min_speed = 4;
			stats.acceleration[0] = 0.05;
			stats.acceleration[1] = 0.025;
			stats.acceleration[2] = 0.01;
			stats.acceleration[3] = 0.001;
			stats.acceleration[4] = 0.001;
			stats.friction = 0.02;
			stats.turn_speed = PI*0.0052;
			stats.turn_speed_fix_threshold = 2;
			stats.redirect_angle = PI*0.004;
			stats.redirect_angle_loss = 0.018;
			stats.redirect_speed_loss = 0.01;
			car_stats.surface[SURFACE_DIRT] = stats;

			stats.min_speed = 8;
			stats.acceleration[0] = 0.05;
			stats.acceleration[1] = 0.04;
			stats.acceleration[2] = 0.03;
			stats.acceleration[3] = 0.02;
			stats.acceleration[4] = 0.01;
			stats.friction = 0.002;
			stats.turn_speed = PI*0.0045;
			stats.turn_speed_fix_threshold = 1.5;
			stats.flip_turning_when_backwards = false;
			stats.flip_turning_when_going_backwards = true;
			stats.do_vel_redirection = false;
			car_stats.surface[SURFACE_ICE] = stats;

			stats.min_speed = 1;
			stats.acceleration[0] = 0.04;
			stats.acceleration[1] = 0.001;
			stats.acceleration[2] = 0.001;
			stats.acceleration[3] = 0.001;
			stats.acceleration[4] = 0.001;
			stats.friction = 0.02;
			stats.always_apply_friction = true;
			stats.flip_turning_when_backwards = false;
			stats.flip_turning_when_going_backwards = false;
			stats.do_vel_redirection = false;
			car_stats.surface[SURFACE_WATER] = stats;

			break;
	}

	return car_stats;
}

Racecar InitRacecar(CarStats* stats)
{
	Racecar car;
	//car.rotation.x = 1.0;
	//car.rotation.y = 0.0;
	car.rotation = UP_VECTOR;
	car.velocity = Vector2Zero();
	car.position = Vector2Zero();
	car.turn_dir = 0.0;
	car.gear = 0;
	car.gear_shift = 0.0;
	CalculateRacecarWalls(&car, stats->size);
	return car;
}

void ResetRacecar(Racecar* car, Vector2 pos, Vector2 dir, Vector2 size)
{
	car->rotation = dir;
	car->velocity = Vector2Zero();
	car->position = pos;
	car->turn_dir = 0.0;
	car->gear = 0;
	car->gear_shift = 0.0;
	CalculateRacecarWalls(car, size);
}

void CalculateRacecarWalls(Racecar* car, Vector2 size)
{
	const float gap = 1.0;

	Vector2 top_left     = (Vector2){-size.x + gap, -size.y},
		top_right    = (Vector2){size.x - gap, -size.y},
		bottom_left  = (Vector2){-size.x + gap, size.y},
		bottom_right = (Vector2){size.x - gap, size.y},
		left_top     = (Vector2){-size.x, -size.y + gap},
		left_bottom  = (Vector2){-size.x, size.y - gap},
		right_top    = (Vector2){size.x, -size.y + gap},
		right_bottom = (Vector2){size.x, size.y - gap};

	float angle = Vector2Angle(UP_VECTOR, car->rotation);

	car->w_front.pos_x = Vector2Rotate(top_left, angle);
	car->w_front.pos_y = Vector2Rotate(top_right, angle);

	car->w_back.pos_x = Vector2Rotate(bottom_left, angle);
	car->w_back.pos_y = Vector2Rotate(bottom_right, angle);

	car->w_left.pos_x = Vector2Rotate(left_top, angle);
	car->w_left.pos_y = Vector2Rotate(left_bottom, angle);

	car->w_right.pos_x = Vector2Rotate(right_top, angle);
	car->w_right.pos_y = Vector2Rotate(right_bottom, angle);
}

void RotateRacecar(Racecar* car, float angle)
{
	car->rotation = Vector2Rotate(car->rotation, angle);
}

bool RacecarCanTurn(Racecar* car)
{
	return !Vector2Equals(car->velocity, Vector2Zero());
}

void TurnRacecar(Racecar* car, float angle, float limit, bool invert_when_reverse)
{
	float magnitude = Vector2Length(car->velocity);
	float car_angle = absf(Vector2Angle(car->velocity, car->rotation));
	if(car_angle > PI * 0.50 && invert_when_reverse)
	{
		angle = -angle;
	}

	if(limit == 0.0) limit = 1.0;
	float percentage = magnitude / limit;
	if(percentage > 2.0) percentage = 2.0;

	angle = angle * percentage;

	if(!invert_when_reverse || car_angle > PI * 0.5 == car_angle + angle > PI * 0.5)
	{
		RotateRacecar(car, angle);
	}
}

void IncreaseRacecarVelocity(Racecar* car, float amount, Vector2 direction)
{
	Vector2 speed_increase = (Vector2){amount, amount};
	speed_increase = Vector2Multiply(speed_increase, direction);
	car->velocity = Vector2Add(car->velocity, speed_increase);

	if(RACECAR_DEBUG) DrawUnitVector(SCREEN_CENTER, direction, ORANGE);
}

void DecreaseRacecarVelocity(Racecar* car, float amount, Vector2 direction)
{
	Vector2 previous = car->velocity;
	IncreaseRacecarVelocity(car, -amount, direction);
	if(sign(previous.x) != sign(car->velocity.x))
	{
		car->velocity.x = 0.0;
	}
	if(sign(previous.y) != sign(car->velocity.y))
	{
		car->velocity.y = 0.0;
	}

	if(RACECAR_DEBUG) DrawUnitVector(SCREEN_CENTER, Vector2Negate(direction), GREEN);
}

void AccelerateRacecar(Racecar* car, float amount)
{
	IncreaseRacecarVelocity(car, amount, car->rotation);
}

void ApplyFrictionRacecar(Racecar* car, float amount)
{
	Vector2 unit_vector = Vector2Normalize(car->velocity);
	DecreaseRacecarVelocity(car, amount, unit_vector);
}

void CapRacecarVelocity(Racecar* car, float max)
{

	float mag = Vector2Length(car->velocity);
	if(mag <= max) return;

	Vector2 unit_vector = Vector2Normalize(car->velocity);
	Vector2 magnitude = (Vector2){max, max};
	car->velocity = Vector2Multiply(unit_vector, magnitude);
}

void CapRacecarVelocityBiDir(Racecar* car, float max, float min)
{
	float angle = absf(Vector2Angle(car->velocity, car->rotation));
	float limit = max;
	if(angle > PI * 0.5) limit = min;

	float mag = Vector2Length(car->velocity);
	if(mag <= limit) return;

	Vector2 unit_vector = Vector2Normalize(car->velocity);
	Vector2 magnitude = (Vector2){limit, limit};
	car->velocity = Vector2Multiply(unit_vector, magnitude);
}

void RedirectRacecarVelocity(Vector2* velocity, Vector2 direction, float angle_change, float angle_loss, float vel_percent)
{
	float angle = Vector2Angle(*velocity, direction);
	float abs_angle = absf(angle);
	if(abs_angle > PI)
	{
		abs_angle -= PI;
	}
	if(abs_angle > PI * 0.5)
	{
		angle = -sign(angle) * PI + angle;
		abs_angle = PI - abs_angle;
		direction = (Vector2){-direction.x, -direction.y};
	}
	float magnitude = Vector2Length(*velocity);
	float turn_angle = max(angle_change - abs_angle * INV_PI * angle_loss, 0.0) * sign(angle);
	float diff = abs_angle - absf(turn_angle);
	if(diff > 0)
	{
		direction = Vector2Normalize(*velocity);
		if(turn_angle != 0.0)
		{
			direction = Vector2Rotate(direction, turn_angle);
		}
		magnitude *= max(1.0 - vel_percent * diff, 0);
	}

	*velocity = Vector2Scale(direction, magnitude);

	if(RACECAR_DEBUG) DrawUnitVector(SCREEN_CENTER, direction, PURPLE);
}

bool RacecarMaxVelocity(Racecar* car, float max)
{
	float mag = Vector2Length(car->velocity);
	return mag >= max;
}

bool CheckRacecarWallColliding(Wall racecar_wall, BlockWallArray block_walls[MAX_LOADED_BLOCK_WALLS], WallCollision* collision)
{
	bool collided = false;

	for(int i = 0; i < MAX_LOADED_BLOCK_WALLS; i++)
	{
		if(block_walls[i].wall_amount == 0)
		{
			continue;
		}
		for(int j = 0; j < block_walls[i].wall_amount; j++)
		{
			collision->wall = block_walls[i].walls[j];
			if(CheckWallCollision(racecar_wall, collision->wall, &collision->point))
			{
				collided = true;
				break;
			}
		}
		if(collided)
		{
			break;
		}
	}
	return collided;
}

Block CheckRacecarAreaColliding(Racecar* car, BlockWallArray block_walls[MAX_LOADED_BLOCK_WALLS], Block blocks[MAX_BLOCK_AMOUNT])
{
	Block block;
	{
		Vector2int pos = (Vector2int){0, 0};
		block = MakeBlock(0, pos, ROT_NORTH);
	}
	bool found = false;
	for(int i = 0; i < MAX_LOADED_BLOCK_WALLS; i++)
	{
		Block new_block = blocks[block_walls[i].block_id];
		if(found && new_block.z < block.z)
		{
			continue;
		}
		if(CheckCollisionPointRec(car->position, new_block.area.rect))
		{
			block = new_block;
			found = true;
		}
	}
	return block;
}

void RacecarGearUp(Racecar* car, float gears[GEAR_AMOUNT])
{
	if(car->gear < GEAR_AMOUNT - 1)
	{
		if(Vector2Length(car->velocity) >= gears[car->gear] - 0.1)
		{
			car->gear++;
			car->gear_shift = 1.0;
			if(RACECAR_DEBUG) TraceLog(LOG_INFO, "Gear Up: %i (%f)", car->gear, Vector2Length(car->velocity));
		}
	}
}

void RacecarGearDown(Racecar* car, float gears[GEAR_AMOUNT])
{
	float vel = Vector2Length(car->velocity);
	while(car->gear > 0 && vel < gears[car->gear - 1] - 0.1)
	{
		car->gear--;
		car->gear_shift = 1.0;
		if(RACECAR_DEBUG) TraceLog(LOG_INFO, "Gear Down: %i (%f)", car->gear, vel);
	}
}

void MoveRacecar(Racecar* car, BlockWallArray block_walls[MAX_LOADED_BLOCK_WALLS])
{
	Vector2 quarter_step = Vector2Scale(car->velocity, 0.25);
	bool collided = false;
	for(int i = 0; i < 4; i++)
	{
		car->position = Vector2Add(car->position, quarter_step);

		Wall wall[4];
	       	wall[0] = ShiftWall(car->w_front, car->position);
		wall[1] = ShiftWall(car->w_back, car->position);
		wall[2] = ShiftWall(car->w_left, car->position);
		wall[3] = ShiftWall(car->w_right, car->position);

		WallCollision col[4];
		bool hit[4];
		for(int i = 0; i < 4; i++)
		{
			hit[i] = CheckRacecarWallColliding(wall[i], block_walls, &col[i]);
		}

		if(hit[0] || hit[1] || hit[2] || hit[3])
		{
			collided = true;
			float fraction = 1.0;
			for(int i = 0; i < 4; i++)
			{
				if(hit[i])
				{
					Vector2 wall_rot = Vector2Subtract(col[i].wall.pos_x, col[i].wall.pos_y);
					float angle = absf(Vector2Angle(quarter_step, wall_rot));
					if(angle > PI)
					{
						angle -= PI;
					}
					if(angle > PI * 0.5)
					{
						angle = PI - angle;
					}
					fraction *= 1.0 - angle * INV_PI;
					RedirectRacecarVelocity(&quarter_step, Vector2Normalize(wall_rot), PI * 0.05, 0.0, 0.0);

					//if(RACECAR_DEBUG) DrawUnitVector(SCREEN_CENTER, wall_rot, ORANGE);
				}
			}
			car->velocity = Vector2Scale(car->velocity, fraction);
			for(int i = 0; i < 4; i++)
			{
				if(hit[i])
				{
					Vector2 pos_close = ClosestPoint(col[i].wall.pos_x, col[i].wall.pos_y, car->position);
					Vector2 away_direction = Vector2Normalize(Vector2Subtract(car->position, pos_close));

					pos_close = ClosestPoint(col[i].wall.pos_x, col[i].wall.pos_y, wall[i].pos_x);
					Vector2 diff = Vector2Subtract(pos_close, wall[i].pos_x);
					if(Vector2Equals(Vector2Normalize(diff), away_direction))
					{
						car->position = Vector2Add(car->position, diff);
					}

					pos_close = ClosestPoint(col[i].wall.pos_x, col[i].wall.pos_y, wall[i].pos_y);
					diff = Vector2Subtract(pos_close, wall[i].pos_y);
					if(Vector2Equals(Vector2Normalize(diff), away_direction))
					{
						car->position = Vector2Add(car->position, diff);
					}

					wall[0] = ShiftWall(car->w_front, car->position);
					wall[1] = ShiftWall(car->w_back, car->position);
					wall[2] = ShiftWall(car->w_left, car->position);
					wall[3] = ShiftWall(car->w_right, car->position);
				}
			}
		}
	}
	if(collided)
	{
		RedirectRacecarVelocity(&car->velocity, Vector2Normalize(quarter_step), PI, 0.0, 0.0);
	}

	if(RACECAR_DEBUG) DrawUnitVector(SCREEN_CENTER, car->velocity, BLUE);
}

Vector2int RacecarPlacement(Racecar* car)
{
	return PositionToPlacement(car->position);
}

Asset* AllocRacecarAsset(unsigned char model)
{
	return AllocAsset(GetModelID(model), ROT_NORTH, 0.0);
}

Tri RotateTriCar(Tri tri, Vector2 rotation)
{
	Tri result = (Tri){0};
	result.color = tri.color;
	result.a = (Vector2){tri.a.x * -rotation.y - tri.a.y * rotation.x, tri.a.x * rotation.x - tri.a.y * rotation.y};
	result.b = (Vector2){tri.b.x * -rotation.y - tri.b.y * rotation.x, tri.b.x * rotation.x - tri.b.y * rotation.y};
	result.c = (Vector2){tri.c.x * -rotation.y - tri.c.y * rotation.x, tri.c.x * rotation.x - tri.c.y * rotation.y};
	return result;
}

void DrawRacecar(Racecar* car, bool ghost)
{
	//TraceLog(LOG_INFO, "Rotation: %f, %f", car->rotation.x, car->rotation.y);
	Asset* asset = AllocRacecarAsset(car->model);
	for(int i = 0; i < asset->tri_amount; i++)
	{
		asset->tris[i] = RotateTriCar(asset->tris[i], car->rotation);
		if(ghost)
		{
			asset->tris[i].color = 33;
		}
	}
	if(!ghost)
	{
		PaintAsset(asset, GetPalette(car->palette));
	}
	DrawAsset(asset, 0.5, car->position);
	FreeAsset(asset);
}

void DrawRacecarWalls(Racecar* car, bool ghost)
{
	Wall front = car->w_front,
		    back = car->w_back,
		    left = car->w_left,
		    right = car->w_right;

	front.pos_x = Vector2Add(front.pos_x, car->position);
	front.pos_y = Vector2Add(front.pos_y, car->position);
	back.pos_x = Vector2Add(back.pos_x, car->position);
	back.pos_y = Vector2Add(back.pos_y, car->position);
	left.pos_x = Vector2Add(left.pos_x, car->position);
	left.pos_y = Vector2Add(left.pos_y, car->position);
	right.pos_x = Vector2Add(right.pos_x, car->position);
	right.pos_y = Vector2Add(right.pos_y, car->position);

	if(ghost)
	{
		DrawWallDebug(front, BLACK);
		DrawWallDebug(back, BLACK);
		DrawWallDebug(left, BLACK);
		DrawWallDebug(right, BLACK);
	}
	else
	{
		DrawWallDebug(front, BLUE);
		DrawWallDebug(back, GREEN);
		DrawWallDebug(left, RED);
		DrawWallDebug(right, YELLOW);
	}
}

MetaInfo ProcessRacecar(Racecar* car, CarStats* car_stats, Block blocks[MAX_BLOCK_AMOUNT], BlockWallArray block_walls[MAX_LOADED_BLOCK_WALLS], RMInput input, DefaultEnviroment env)
{
	MetaInfo meta = (MetaInfo){false, false};
	Block current_block = CheckRacecarAreaColliding(car, block_walls, blocks);
	int type = current_block.area.type;
	if(type == TYPE_VOID)
	{
		switch(env)
		{
			case(ENV_VOID):
				type = TYPE_ICE;
				break;
			case(ENV_MEADOW):
				type = TYPE_GRASS;
				break;
			case(ENV_QUARRY):
				type = TYPE_DIRT;
				break;
			case(ENV_ISLAND):
				type = TYPE_WATER;
				break;
		}
	}
	SurfaceStats* stats;

	switch(type)
	{
		case(TYPE_GRASS):
			stats = &car_stats->surface[SURFACE_GRASS];
			break;
		case(TYPE_DIRT):
			stats = &car_stats->surface[SURFACE_DIRT];
			break;
		case(TYPE_ICE):
			stats = &car_stats->surface[SURFACE_ICE];
			break;
		case(TYPE_WATER):
			stats = &car_stats->surface[SURFACE_WATER];
			break;
		case(TYPE_CHECKPOINT):
			stats = &car_stats->surface[SURFACE_ASPHALT];
			meta.checkpoint = true;
			meta.check_pos = CheckPosition(current_block.pos);
			meta.check_rot = CheckRotation(current_block.rot);
			break;
		case(TYPE_FINISH):
			stats = &car_stats->surface[SURFACE_ASPHALT];
			meta.finish = true;
			break;
		case(TYPE_START):
		case(TYPE_ASPHALT):
		case(TYPE_BOOSTER):
		default:
			stats = &car_stats->surface[SURFACE_ASPHALT];
			break;
	}

	bool lower_turn_dir = true;
	if(InputHeld(input, INPUT_LEFT))
	{
		if(car->turn_dir > -1.0)
		{
			car->turn_dir -= stats->turn_dir_gain;
			if(car->turn_dir < -1.0)
			{
				car->turn_dir = -1.0;
			}
		}
		lower_turn_dir = car->turn_dir > 0;
	}

	if(InputHeld(input, INPUT_RIGHT))
	{
		if(car->turn_dir < 1.0)
		{
			car->turn_dir += stats->turn_dir_gain;
			if(car->turn_dir > 1.0)
			{
				car->turn_dir = 1.0;
			}
		}
		lower_turn_dir = car->turn_dir < 0;
	}

	if(car->turn_dir != 0.0 && lower_turn_dir)
	{
		int s = sign(car->turn_dir);
		car->turn_dir -= s * stats->turn_dir_loss;
		if(s != sign(car->turn_dir))
		{
			car->turn_dir = 0.0;
		}
	}

	bool flip_turning = false;

	if(stats->flip_turning_when_backwards)
	{
		flip_turning = true;
	}
	else if(stats->flip_turning_when_going_backwards)
	{
		flip_turning = InputHeld(input, INPUT_DOWN);
	}

	if(car->turn_dir != 0.0 && RacecarCanTurn(car))
	{
		TurnRacecar(car, stats->turn_speed * car->turn_dir, stats->turn_speed_fix_threshold, flip_turning);
		CalculateRacecarWalls(car, car_stats->size);
	}

	bool facing_foward = true;
	if(!zero(car->velocity.x) || !zero(car->velocity.y))
	{
		float angle = absf(Vector2Angle(car->velocity, car->rotation));
		facing_foward = angle < PI * 0.5;
	}

	//TraceLog(LOG_INFO, "prev vel %.3f %.3f", car->velocity.x, car->velocity.y);
	if(car->gear_shift > 0.0)
	{
		car->gear_shift -= car_stats->gear_shift_speed;
	}
	else
	{
		RacecarGearDown(car, car_stats->gears);
	}
	if(InputHeld(input, INPUT_UP))
	{
		float acceleration = stats->acceleration[car->gear];
		if(car->gear_shift <= 0.0)
		{
			RacecarGearUp(car, car_stats->gears);
		}
		else
		{
			acceleration *= stats->gear_shift_acceleration_punish;
		}
		if(!RacecarMaxVelocity(car, car_stats->gears[car->gear]) || !facing_foward)
		{
			AccelerateRacecar(car, acceleration);
			if(facing_foward)
			{
				CapRacecarVelocity(car, car_stats->gears[car->gear]);
			}
		}
	}
	if(type == TYPE_BOOSTER)
	{
		float acceleration = stats->boost_acceleration;
		AccelerateRacecar(car, acceleration);
	}
	if(InputHeld(input, INPUT_DOWN))
	{
		if(!RacecarMaxVelocity(car, stats->min_speed) || facing_foward)
		{
			AccelerateRacecar(car, -stats->decceleration);
			if(!facing_foward)
			{
				CapRacecarVelocity(car, stats->min_speed);
			}
		}
	}
	//TraceLog(LOG_INFO, "acc vel %.3f %.3f", car->velocity.x, car->velocity.y);
	if(!zero(car->velocity.x) || !zero(car->velocity.y))
	{
		if(absf(car->velocity.x) < stats->friction || absf(car->velocity.y) < stats->friction)
		{
			float angle = absf(Vector2Angle(car->velocity, car->rotation));
			facing_foward = angle < PI * 0.5;
		}
		if(stats->always_apply_friction && car->gear_shift <= 0.0)
		{
			ApplyFrictionRacecar(car, stats->friction);
		}
		else if((!InputHeld(input, INPUT_UP) && facing_foward) || (!InputHeld(input, INPUT_DOWN) && !facing_foward))
		{
			ApplyFrictionRacecar(car, stats->friction);
		}
	}
	//TraceLog(LOG_INFO, "frict vel %.3f %.3f", car->velocity.x, car->velocity.y);

	if(stats->do_vel_redirection)
	{
		RedirectRacecarVelocity(&car->velocity, car->rotation, stats->redirect_angle, stats->redirect_angle_loss, stats->redirect_speed_loss);
	}
	//TraceLog(LOG_INFO, "redir vel %.3f %.3f", car->velocity.x, car->velocity.y);

	MoveRacecar(car, block_walls);

	if(type == TYPE_BOOSTER)
	{
		car->boost = 1.0;
	}
	else if(car->boost > 0)
	{
		car->boost -= FRAME;
		if(car->boost < 0.0)
		{
			car->boost = 0.0;
		}
	}

	return meta;
}

void ClearSkidLine(SkidLinePoint points[SKID_LINE_COUNT])
{
	for(int i = 0; i < SKID_LINE_COUNT; i++)
	{
		points[i] = (SkidLinePoint){0};
	}
}

void AddSkidLinePoint(Racecar* car, SkidLinePoint points[SKID_LINE_COUNT])
{
	for(int i = 0; i < SKID_LINE_COUNT - 1; i++)
	{
		points[i] = points[i + 1];
	}
	bool draw = absf(Vector2Angle(car->velocity, car->rotation)) > PI * 0.02;
	points[SKID_LINE_COUNT - 1] = (SkidLinePoint){draw, car->position, car->rotation};
}

void DrawSkidLine(SkidLinePoint points[SKID_LINE_COUNT], unsigned char model, Color color)
{
	Vector2 offsets[6] = {0};
	Vector2 prev_off[6] = {0};
	int off_count = 0;
	switch(model)
	{
		case 0:
			offsets[0] = (Vector2){14, 23};
			offsets[1] = (Vector2){-14, 23};
			offsets[2] = (Vector2){-14, -23};
			offsets[3] = (Vector2){14, -23};
			off_count = 4;
			break;
	}
	for(int i = 0; i < off_count; i++)
	{
		prev_off[i] = (Vector2)
		{
			offsets[i].x * -points[0].rotation.y - offsets[i].y * points[0].rotation.x,
			offsets[i].x * points[0].rotation.x - offsets[i].y * points[0].rotation.y
		};
	}
	for(int i = 0; i < SKID_LINE_COUNT - 1; i++)
	{
		if(points[i].draw && points[i + 1].draw)
		{
			for(int j = 0; j < off_count; j++)
			{
				Vector2 offset = (Vector2)
				{
					offsets[j].x * -points[i + 1].rotation.y - offsets[j].y * points[i + 1].rotation.x,
					offsets[j].x * points[i + 1].rotation.x - offsets[j].y * points[i + 1].rotation.y
				};
				DrawLineV(Vector2Add(points[i].position, prev_off[j]), Vector2Add(points[i + 1].position, offset), color);
				prev_off[j] = offset;
			}
		}
	}
}

