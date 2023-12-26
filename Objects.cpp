#include "Objects.h"

Player::Player()
{
	HP = maxHP = 3;
	IsJumping = IsLeft = IsRunning = IsFalling = IsDashing = false;
	CanDash = CanGoLeft = CanGoRight = true;
	Hitbox = { 0, 0, 100, 300 };
	V = { 0, 0 };
}

Player::Player(Vector2 pos)
{
	HP = maxHP = 3;
	IsJumping = IsLeft = IsRunning = IsFalling = IsDashing = false;
	CanDash = true;
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
bool Player::getIsDashing()
{
	return IsDashing;
}
bool Player::getCanDash()
{
	return CanDash;
}
bool Player::getCanGoLeft()
{
	return CanGoLeft;
}
bool Player::getCanGoRight()
{
	return CanGoRight;
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
void Player::setCanDash(bool CanDash)
{
	this->CanDash = CanDash;
}
void Player::setCanGoLeft(bool CanGoLeft)
{
	this->CanGoLeft = CanGoLeft;
}
void Player::setCanGoRight(bool CanGoRight)
{
	this->CanGoRight = CanGoRight;
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

void Player::PushRight(float dx)
{
	Hitbox.x += dx;
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
		if (IsDashing)	dy *= DASHYFORCE;
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

void Player::Dash()
{
	IsDashing = 1;
	CanDash = 0;
	if (!IsLeft)	V.x = DASHV;
	else V.x = -DASHV;
	V.y *= DASHYFORCE;
	DashTimer = GetTime();
}

void Player::AddCanDash(bool dc)
{
	CanDash += dc;
}

void Player::UpdateHorisontalV()
{
	if (!IsDashing)
	{
		if (IsKeyDown(KEY_D) && !IsKeyDown(KEY_A) && CanGoRight)	V.x = WALKV;
		else if (IsKeyDown(KEY_A) && !IsKeyDown(KEY_D) && CanGoLeft)	V.x = -WALKV;
		else  V.x = 0;
		if (IsKeyDown(KEY_A) != IsKeyDown(KEY_D))	IsLeft = IsKeyDown(KEY_A);
		if (IsKeyDown(KEY_LEFT_CONTROL))	V.x *= RUNFORCE;
	}
	else if (IsDashing && (GetTime() - DashTimer > DASHLENGTH))
	{
		IsDashing = 0;
	}
}

void Player::UpdatePos()
{
	Hitbox.x += V.x * GetFrameTime();
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
	//cgl - can go left, cgr - can go right
	bool fall =	1, cgl = 1, cgr = 1;
	//Check rectangle collision
	for (int i = 0; i < chapter.getRecObstacles().size(); i++)
	{
		if (CheckCollisionRecs(player.getHitbox(), chapter.getRecObstacles()[i]))
		{
			Rectangle col = GetCollisionRec(player.getHitbox(), chapter.getRecObstacles()[i]);
			if (col.width > col.height)
			{
				if (abs(col.y - player.getHitbox().y) < abs(col.y - player.getHitbox().y - player.getHitbox().height))
				{
					fall = 0;
					player.EndJump();
					player.PushUp(col.height);
				}
				else
				{
					//player.EndJump();
					player.setV({ player.getV().x, -player.getV().y * (float)BUMPFORCE });
					player.PushUp(-col.height);
				}
			}
			else if (col.width < col.height)
			{
				if (abs(col.x - player.getHitbox().x) < abs(col.x - player.getHitbox().x - player.getHitbox().width))
				{
					player.setV({ 0, player.getV().y });
					player.PushRight(col.width - 1);
					cgl = false;
				}
				else
				{
					player.setV({ 0, player.getV().y });
					player.PushRight(-col.width + 1);
					cgr = false;
				}
			}
		}
	}
	player.setCanGoLeft(cgl);
	player.setCanGoRight(cgr);
	if (fall)	player.Fall();
	player.AddCanDash(!fall);
	if (IsKeyPressed(KEY_LEFT_SHIFT) && !player.getIsDashing() && player.getCanDash())	player.Dash();
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
