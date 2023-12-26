#pragma once
#include <vector>
#include "raylib.h"
#include "Enemies.h"

#define JUMPV			2000
#define MINABSV			-5000
#define GRAVITYFORCE	80
#define HOLDFORCE		0.5
#define BUMPFORCE		0.5
#define WALKV			1000
#define RUNFORCE		1.5
#define DASHV			7000
#define DASHYFORCE		0
#define DASHLENGTH		0.08		//in seconds

void DrawRectangleRecOrdinary(Rectangle rec, Color color);

class Chapter;

class Player
{
public:
	Player();
	Player(Vector2 pos);
	
	//getters
	int getHP();
	bool getIsJumping();
	bool getIsLeft();
	bool getIsRunning();
	bool getIsFalling();
	bool getIsDashing();
	bool getCanDash();
	bool getCanGoLeft();
	bool getCanGoRight();
	bool getAlive();
	Rectangle getHitbox();
	Vector2 getV();

	//setters
	void setIsJumping(bool IsJumping);
	void setIsLeft(bool IsLeft);
	void setIsRunning(bool IsRunning);
	void setIsFalling(bool IsFalling);
	void setCanDash(bool CanDash);
	void setCanGoLeft(bool CanGoLeft);
	void setCanGoRight(bool CanGoRight);
	void setHitbox(Rectangle Hitbox);
	void setV(Vector2 V);
	void setPos(Vector2 pos);

	//Updating player's stats
	void Heal(int dHP);
	void Damage(int dHP);

	//Updating player's movement
	void PushUp(float dy);
	void PushRight(float dx);
	void Jump();
	void EndJump();
	void UpdateJumpV();
	void Fall();
	void Dash();
	void AddCanDash(bool dc);
	void UpdateHorisontalV();
	void UpdatePos();

private:
	float GravityForce;

	double DashTimer;

	int HP, maxHP;

	bool IsJumping, IsLeft, IsRunning, IsFalling, Alive, IsDashing, CanDash, CanGoLeft, CanGoRight;
	Rectangle Hitbox;
	std::vector<Texture2D> animations;
	Vector2 V;
};
void UpdatePlayerMovement(Player &player, Chapter chapter);

struct Danger
{
	Rectangle rec;
	int damage;
};

//This triangle just like a half of Rec
struct RightTriangle
{
	//Center point(near right angle)
	Vector2 O;
	float x;
	float y;
};

class Chapter
{
public:

	//getters
	std::vector<Rectangle> getRecObstacles();
	std::vector<RightTriangle> getTriObstacles();
	std::vector<Danger> getDanger();
	std::vector<Enemy> getEnemies();
	std::vector<Texture2D> getBackgrounds();

	//setters
	void setRecObstacles(std::vector<Rectangle> RecObstacles);
	void setTriObstacles(std::vector<RightTriangle> TriObstacles);
	void setDanger(std::vector<Danger> Dangers);
	void setEnemies(std::vector<Enemy> Enemies);
	void setBackgrounds(std::vector<Texture2D> Backgrounds);

private:
	std::vector<Rectangle> RecObstacles;
	std::vector<RightTriangle> TriObstacles;
	std::vector<Danger> Dangers;
	std::vector<Enemy> Enemies;
	std::vector<Texture2D> Backgrounds;
};

bool CheckCollisionRTriRec(RightTriangle tri, Rectangle rec);

RightTriangle GetCollisionRTriRec(RightTriangle tri, Rectangle rec);

