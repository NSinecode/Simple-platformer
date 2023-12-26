#include "Objects.h"

Player::Player()
{
	HP = maxHP = 3;
	IsJumping = IsLeft = IsRunning = IsFalling = 0;
	Hitbox = { 0, 0, 100, 300 };
	V = { 0, 0 };
}

Player::Player(Vector2 pos)
{
	HP = maxHP = 3;
	IsJumping = IsLeft = IsRunning = IsFalling = 0;
	Hitbox = { pos.x, pos.y, 100, 300 };
	V = { 0, 0 };
}

int Player::getHP()
{
	return HP;
}
bool Player::getIsJumping()
{
	return IsJumping;
}
bool Player::getIsLeft()
{
	return IsLeft;
}
bool Player::getIsRunning()
{
	return IsRunning;
}
bool Player::getIsFalling()
{
	return IsFalling;
}
bool Player::getAlive()
{
	return Alive;
}
Rectangle Player::getHitbox()
{
	return Hitbox;
}
Vector2 Player::getV()
{
	return V;
}

void Player::setIsJumping(bool IsJumping)
{
	this->IsJumping = IsJumping;
}
void Player::setIsLeft(bool IsLeft)
{
	this->IsLeft = IsLeft;
}
void Player::setIsRunning(bool IsRunning)
{
	this->IsRunning = IsRunning;
}
void Player::setIsFalling(bool IsFalling)
{
	this->IsFalling = IsFalling;
}
void Player::setHitbox(Rectangle Hitbox)
{
	this->Hitbox = Hitbox;
}
void Player::setV(Vector2 V)
{
	this->V = V;
}

void Player::setPos(Vector2 pos)
{
	Hitbox.x = pos.x;
	Hitbox.y = pos.y;
}

void Player::Heal(int dHP)
{
	HP += dHP;
	if (HP > maxHP)	HP = maxHP;
}

void Player::Damage(int dHP)
{
	HP -= dHP;
	if (HP <= 0) Alive = false;
}

void Player::PushUp(float dy)
{
	Hitbox.y += dy;
}

void Player::Jump()
{
	IsJumping = 1;
	V.y = JUMPV;
	GravityForce = GRAVITYFORCE;
	Hitbox.y++;
}

void Player::EndJump()
{
	V.y = 0;
	IsJumping = 0;
}

void Player::UpdateJumpV()
{
	if (V.y > MINABSV)
	{
		float dy = GravityForce;
		if (IsKeyDown(KEY_SPACE))	dy *= HOLDFORCE;
		if (abs(V.y) < 50)	dy *= 0.2;
		else if (V.y < 0) dy *= 1.2;
		V.y -= dy;
	}
	else V.y = MINABSV;
}

void Player::Fall()
{
	IsFalling = 1;
}

void Player::UpdateHorisontalV()
{
	if (IsKeyDown(KEY_D) && !IsKeyDown(KEY_A))	V.x = WALKV;
	else if (IsKeyDown(KEY_A) && !IsKeyDown(KEY_D))	V.x = -WALKV;
	else  V.x = 0;
	IsLeft = IsKeyDown(KEY_A) && IsKeyDown(KEY_A) == !IsKeyDown(KEY_D);
	if (IsKeyDown(KEY_LEFT_CONTROL))	V.x *= RUNFORCE;
}

void Player::UpdatePos()
{
	Hitbox.x += V.x;
	Hitbox.y += V.y * GetFrameTime();
}

void DrawRectangleRecOrdinary(Rectangle rec, Color color)
{
	DrawRectangle(rec.x, -rec.y - rec.height, rec.width, rec.height, color);
}

void UpdatePlayerMovement(Player& player, Chapter chapter)
{
	if ((IsKeyPressed(KEY_SPACE) or IsKeyDown(KEY_SPACE)) && !player.getIsJumping())	player.Jump();
	else if (player.getIsJumping() or player.getIsFalling()) player.UpdateJumpV();
	bool fall =	1;
	for (int i = 0; i < chapter.getRecObstacles().size(); i++)
	{
		if (CheckCollisionRecs(player.getHitbox(), chapter.getRecObstacles()[i]))
		{
			fall = 0;
			player.EndJump();
			player.PushUp(GetCollisionRec(player.getHitbox(), chapter.getRecObstacles()[i]).height - 1);
		}
	}
	if (fall)	player.Fall();
	player.UpdateHorisontalV();
	player.UpdatePos();
}
std::vector<Rectangle> Chapter::getRecObstacles()
{
	return RecObstacles;
}
std::vector<RightTriangle> Chapter::getTriObstacles()
{
	return TriObstacles;
}
std::vector<Danger> Chapter::getDanger()
{
	return Dangers;
}
std::vector<Enemy> Chapter::getEnemies()
{
	return Enemies;
}
std::vector<Texture2D> Chapter::getBackgrounds()
{
	return Backgrounds;
}
void Chapter::setRecObstacles(std::vector<Rectangle> RecObstacles)
{
	this->RecObstacles = RecObstacles;
}
void Chapter::setTriObstacles(std::vector<RightTriangle> TriObstacles)
{
	this->TriObstacles = TriObstacles;
}
void Chapter::setDanger(std::vector<Danger> Dangers)
{
	this->Dangers = Dangers;
}
void Chapter::setEnemies(std::vector<Enemy> Enemies)
{
	this->Enemies = Enemies;
}
void Chapter::setBackgrounds(std::vector<Texture2D> Backgrounds)
{
	this->Backgrounds = Backgrounds;
}

bool CheckCollisionRTriRec(RightTriangle tri, Rectangle rec)
{
	if (tri.x == 0 or tri.y == 0)
	{

	}
	else if (tri.x < 0 && tri.y < 0)
	{

	}
	else if (tri.x < 0 && tri.y > 0)
	{

	}
	else if (tri.x > 0 && tri.y < 0)
	{

	}
	else 
	{
		if (CheckCollisionRecs(rec, { tri.O.x, tri.O.y, tri.x, tri.y }))
		{

		}
	}
	
	if (CheckCollisionRecs(rec, { tri.O.x +tri.x, tri.O.y + tri.y, tri.x, tri.y }))
	
	
	return false;
}
