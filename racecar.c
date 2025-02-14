#include <raylib.h>
#include <raymath.h>
#include "rmlib.h"
#include "blocks.h"
#include "asset.h"
#include "racecar.h"

// CAR_ROAD    - Slow Accel, Base Car
// CAR_DRIFT   - High Accel, Drifting
// CAR_GRIP    - Medium Accel, High Turn Speed
// CAR_TERRAIN - Slowest Accel, More Precision Based

CarStats DefaultStats(DefaultCar car)
{
	CarStats car_stats;
	switch(car)
	{
		case(CAR_ROAD):
			car_stats.size = (Vector2){14, 23};
			car_stats.camera_shake_threshold = 0.8;
			car_stats.speed_to_shake_ratio = 0.125;
			car_stats.shake_amplitude = 5.0;
			car_stats.shake_frequency = 2.0;
			car_stats.shake_weight = 5.0;

			SurfaceStats stats;
			stats.top_speed = 10;
			stats.min_speed = 4;
			stats.acceleration = 0.1;
			stats.decceleration = 0.1;
			stats.friction = 0.05;

			stats.turn_speed = PI*0.0075;
			stats.turn_speed_fix_threshold = 3;
			stats.turn_dir_gain = 0.1;
			stats.turn_dir_loss = 1.0;
			stats.flip_turning_when_backwards = true;
			stats.flip_turning_when_going_backwards = false;
			
			stats.redirect_angle = PI*0.012;
			stats.do_vel_redirection = true;
			car_stats.surface[SURFACE_ASPHALT] = stats;
			
			stats.top_speed = 5;
			stats.min_speed = 2;
			car_stats.surface[SURFACE_GRASS] = stats;
			
			stats.top_speed = 10;
			stats.min_speed = 8;
			stats.turn_speed = PI*0.0225;
			stats.turn_speed_fix_threshold = 10;
			stats.redirect_angle = PI*0.032;
			car_stats.surface[SURFACE_DIRT] = stats;
			
			stats.top_speed = 20;
			stats.min_speed = 8;
			stats.turn_speed = PI*0.0075;
			stats.turn_speed_fix_threshold = 3;
			stats.redirect_angle = PI*0.012;
			stats.flip_turning_when_backwards = false;
			stats.flip_turning_when_going_backwards = true;
			stats.do_vel_redirection = false;
			car_stats.surface[SURFACE_ICE] = stats;

			stats.top_speed = 1;
			stats.min_speed = 1;
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
	CalculateRacecarWalls(&car, stats->size);
	return car;
}

void ResetRacecar(Racecar* car, Vector2 pos, Vector2 dir, Vector2 size)
{
	car->rotation = dir;
	car->velocity = Vector2Zero();
	car->position = pos;
	car->turn_dir = 0.0;
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

void RedirectRacecarVelocity(Vector2* velocity, Vector2 direction, float max_angle)
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
	if(abs_angle > max_angle)
	{
		float turn_angle = max_angle * sign(angle);
		direction = Vector2Normalize(*velocity);
		direction = Vector2Rotate(direction, turn_angle);
	} 

	float mag = Vector2Length(*velocity);
	Vector2 magnitude = (Vector2){mag, mag};
	*velocity = Vector2Multiply(direction, magnitude);

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

void MoveRacecar(Racecar* car, BlockWallArray block_walls[MAX_LOADED_BLOCK_WALLS])
{
	Vector2 quarter_step = Vector2Scale(car->velocity, 0.25);
	bool collided = false;
	for(int i = 0; i < 4; i++)
	{
		Vector2 prev_pos = car->position;
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
					RedirectRacecarVelocity(&quarter_step, Vector2Normalize(wall_rot), PI * 0.05);

					//if(RACECAR_DEBUG) DrawUnitVector(SCREEN_CENTER, wall_rot, ORANGE);
				}
			}
			car->velocity = Vector2Scale(car->velocity, fraction);
			car->position = prev_pos;
			for(int i = 0; i < 4; i++)
			{
				if(hit[i])
				{
					car->position.x += (car->position.x - col[i].point.x) * 0.01;
					car->position.y += (car->position.y - col[i].point.y) * 0.01;
				}
			}
		}
	}
	if(collided)
	{
		RedirectRacecarVelocity(&car->velocity, Vector2Normalize(quarter_step), PI);
	}

	if(RACECAR_DEBUG) DrawUnitVector(SCREEN_CENTER, car->velocity, BLUE);
}

Vector2int RacecarPlacement(Racecar* car)
{
	return PositionToPlacement(car->position);
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
	Asset* asset = AllocAsset(DRACECAR, ROT_NORTH, 0.0);
	for(int i = 0; i < asset->tri_amount; i++)
	{
		asset->tris[i] = RotateTriCar(asset->tris[i], car->rotation);
		if(ghost)
		{
			asset->tris[i].color = 33; 
		}
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
			case(ENV_STADIUM):
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

	float angle = absf(Vector2Angle(car->velocity, car->rotation));
	bool facing_foward = angle < PI * 0.5;

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

	if(InputHeld(input, INPUT_UP))
	{
		if(!RacecarMaxVelocity(car, stats->top_speed) || !facing_foward)
		{
			AccelerateRacecar(car, stats->acceleration);
			if(facing_foward)
			{
				CapRacecarVelocity(car, stats->top_speed);
			}
		}
	}
	else if(InputHeld(input, INPUT_DOWN))
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
	else
	{
		ApplyFrictionRacecar(car, stats->friction);
	}

	if(stats->do_vel_redirection)
	{
		RedirectRacecarVelocity(&car->velocity, car->rotation, stats->redirect_angle);
	}

	MoveRacecar(car, block_walls);

	return meta;
}

