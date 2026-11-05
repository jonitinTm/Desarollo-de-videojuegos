#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
#include "Log.h"
#include "Physics.h"
#include "Audio.h"
#include "Map.h"
















Player::Player() : Entity(EntityType::PLAYER)
{
	name = "Player";
}

Player::~Player()
{}



//N Inicializamos el lugar de res
bool Player::Awake()
{
	// L04: TODO 2: Initialize the player's position
	position = Vector2D(150.0f, 1625.0f);
	spawnPoint = position;

	return true;
}

bool Player::Start()
{
	// L10: TODO 5: Read texture, frame size and tuning from config.xml
	// instead of hardcoding them. GetSize() is not the fix for texW/texH --
	// the real texture (a spritesheet) would return the whole sheet's size,
	// not one frame's.
	// L04: TODO 2: Load the player texture
	texture = Engine::GetInstance().textures->Load(parameters.child("texture").attribute("path").as_string());

	texW = parameters.child("frame").attribute("width").as_int();
	texH = parameters.child("frame").attribute("height").as_int();
	jumpSpeed = parameters.child("movement").attribute("jump").as_float();

	// L10: TODO 5: Load this entity's animations from a TSX and set the current animation to "idle".
	// ...
	
	anims.LoadFromTSX(parameters.child("animations").attribute("tsx").as_string(), { {0, "idle"}, {16, "run"}, {40, "roll"}, {48, "hit"}, {56, "death"}, {20, "jump"} });
		anims.SetCurrent("idle");


	// N Añadir efectos de audio


		pickCoinFxId = Engine::GetInstance().audio->LoadFx(parameters.child("audiofx").attribute("path").as_string());
		hurtFxId = Engine::GetInstance().audio->LoadFx(parameters.child("hurtfx").attribute("path").as_string());


	//--------------->
	// L08: TODO 5: Add physics to the player - create the physics body: a CAPSULE (CreateCapsule) with fixed rotation, so the player does not roll
	pbody = Engine::GetInstance().physics->CreateCircle((int)position.getX() + texW / 2, (int)position.getY() + texH / 2, texW / 2, bodyType::DYNAMIC);

	// L08: TODO 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L08: TODO 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	return true;


}

bool Player::Update(float dt)
{
	// L08: TODO 5: Add physics to the player - updated player position using physics

	velocity = Engine::GetInstance().physics->GetLinearVelocity(pbody);
	velocity = { 0, velocity.y }; // Reset horizontal velocity by default, this way the player stops when no key is pressed


	//N Creamos variable de animacion jugador
	std::string anim = "idle";
	//N definimos en falso

	// N Reiniciar si se le da al enter
	if (gameOver && Engine::GetInstance().input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		Restart();


	//n -------------------------------<<<> mirar

	if (jumpLock > 0.0f) jumpLock -= dt;   // el bloqueo se va gastando

	// Solo "pisamos suelo" si el bloqueo ha terminado Y casi no hay velocidad vertical
	if (jumpLock <= 0.0f && velocity.y > -0.3f && velocity.y < 0.3f)
		groundTimer = groundGrace;
	else
		groundTimer -= dt;
	// L10: TODO 7: Default to the "idle" animation; a key below overrides it
	// ..

	// Move left/right

	// N mecanica de rodamiento

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		PoderGod();




	if (godMode)
	{
		Fly(anim);
	}


	else if (!gameOver&& !dying)
	{
		if (!rolling)
		{

			Move(anim);
			Jump();

		}
		Roll(dt, anim);
		if (!rolling && !IsOnGround())
			anim = "jump";
		
	}




	//  N  Mecanicas de vidas


	if (invulnerable)
	{
		invulTimer -= dt;

		if (invulTimer <= 0) invulnerable = false;

	}

	if (hitTimer > 0) hitTimer -= dt;
	if (hitTimer > 0) anim = "hit";
	//Aqui hacemos lo del parpadeo, mientras este en el estado de inmune durante los numeros pares
	if (invulnerable && (int)(invulTimer * 10) % 2 == 0)anim = "hit";









	if (dying)
	{
		anim = "death";
		velocity = { 0.0f, 0.0f };      // quieto mientras muere
		deathTimer -= dt;

		if (deathTimer <= 0.0f)
		{
			dying = false;
			if (lives <= 0) gameOver = true;   // el Game Over sale DESPUÉS de la animación
			Respawn();
		}
	}










	anims.SetCurrent(anim);
	//N actualizacion por segundo
	anims.Update(dt);

	// Apply velocity via helper
	Engine::GetInstance().physics->SetLinearVelocity(pbody, velocity);

	int x, y;
	pbody->GetPosition(x, y);
	position.setX((float)x);
	position.setY((float)y);

	// L10: TODO 8: Camera follow
	// ..

	Vector2D mapSize =	Engine::GetInstance().map->GetMapSizeInPixels();
	float camW = Engine::GetInstance().render->camera.w;
	float camH = Engine::GetInstance().render->camera.h;




	float camX = camW / 4.0f - position.getX();
	float camY = camH / 2.0f - position.getY();

	if (camX > 0) camX = 0;
	if (camX < camW - mapSize.getX()) camX = camW - mapSize.getX();
	if (camY > 0) camY = 0;
	if (camY < camH - mapSize.getY()) camY = camH - mapSize.getY();

	Engine::GetInstance().render->camera.x = camX;
	Engine::GetInstance().render->camera.y = camY;



	// L10: TODO 6: Advance the animation frame
	// ..
	anims.Update(dt);


	return true;
}




void Player::PoderGod()
{



	godMode = !godMode;
	b2Body_SetGravityScale(pbody->body, godMode ? 0.0f : 1.0f);

	if (godMode)
	{
		dying = false;
		rolling = false;
		invulnerable = false;
	}
	LOG("God mode: %s", godMode ? "ON" : "OFF");


}



void Player::Fly(std::string& anim)
{
	float flySpeed = speed * 2.0f;     // Para ir mas rapido
	velocity = { 0.0f, 0.0f };         // Para quedarte estatico en el aire

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) { velocity.x = -flySpeed; facing = -1; anim = "run"; }
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) { velocity.x = flySpeed; facing = 1; anim = "run"; }
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) { velocity.y = -flySpeed; anim = "run"; }
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) { velocity.y = flySpeed; anim = "run"; }
}




void Player::Move(std::string& anim)
{
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		velocity.x = -speed;
		anim = "run";
		facing = -1;
	}
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		velocity.x = speed;
		anim = "run";
		facing = 1;
	}
}


void Player::Jump()
{
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && IsOnGround())
	{
		velocity.y = -jumpSpeed;
		groundTimer = 0.0f;
		jumpLock = (jumpSpeed / -GRAVITY_Y) * 1.3f;
	}


}


void Player::Roll(float dt, std::string& anim)
{
	// El roll pasa a LSHIFT, porque SPACE ya es el salto
	if (!rolling && Engine::GetInstance().input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN)
	{
		rolling = true;
		rollTimer = rollDuration;
	}

	if (rolling)
	{
		velocity.x = rollSpeed * facing;
		anim = "roll";
		rollTimer -= dt;
		if (rollTimer <= 0) rolling = false;
	}
}













bool Player::Draw()
{
	// L08: TODO 5: Add physics to the player - updated player position using physics
	// L04: TODO 4: Render the player texture at its current position

	// L10: TODO 6: Draw the current animation frame instead of the whole spritesheet
	// ..
	SDL_Rect currentFrame = anims.GetCurrentFrame();
	//Para que haga un parpadeo en los jnumeros pares
	if (invulnerable && ((int)(invulTimer * 10) % 2 == 0)) return true;



	Engine::GetInstance().render->DrawTexture(texture, (int)position.getX() - texW / 2, (int)position.getY() - texH / 2, &currentFrame,facing);

	return true;
}

bool Player::CleanUp()
{
	LOG("Cleanup player");
	// Release the physics body first, then the texture
	if (pbody != nullptr)
	{
		pbody->listener = nullptr;
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
	}

	if (texture != nullptr)
	{
		Engine::GetInstance().textures->UnLoad(texture);
		texture = nullptr;
	}
	return true;
}



// N Crear la funcion de la muerte------->


void Player:: TakeDamage()
{

	if (godMode||invulnerable || gameOver||dying) return;// si esta invulnerable mo pasa nada

	lives--; //Quitamos una vida
	invulnerable = true; // tiempo que es invulnerable

	invulTimer = invulDuration;

	hitTimer = 0.4f; //Animacion de hit


	Engine::GetInstance().audio->PlayFx(hurtFxId);
	LOG("Jugador herido, tiene: %d", lives);



	if (lives <= 0)
	{

		lives = 0;
		Die();
	}


}


//----> definimos la muerte del player



void Player::Kill()
{
	if (godMode||gameOver || dying) return;


	lives--;
	Engine::GetInstance().audio->PlayFx(hurtFxId);
	LOG("Player killed by pit. Lives left: %d", lives);


	if (lives <= 0)
	{
		lives = 0;
	}

	Die();
}


void Player::Die()
{
	if (dying)return;

	dying = true;
	deathTimer = 1.4f;
	rolling = false;
	hitTimer = 0.0f;
}






//------------------------> N vamos a definir las funciones de Respawn y Restart



void Player::Respawn()
{



	
	//seteamos al jugador en la mitad de la Tile
	pbody->SetPosition((int)spawnPoint.getX() + texW / 2, (int)spawnPoint.getY() + texH / 2);

	//Le quitamos cualquier tipo de velocidad que pueda tener, poniendola a 0
	Engine::GetInstance().physics->SetLinearVelocity(pbody, 0.0f, 0.0f);

	//Quitamos el rolling en caso de que muera con el

	rolling = false;

}




void Player::Restart()
{
	lives = 3;
	gameOver = false;
	invulnerable = false;
	Respawn();

}






// L08: TODO 6: Define OnCollision function for the player.
void Player::OnCollision(PhysBody* physA, PhysBody* physB)
{
	// Handle collision with other physics bodies
	// For example, if the player collides with an item, you can implement logic here
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
	{
		//N Hacemos que detecte lo que este debajo del jugador
		int px, py, bx, by;

		physA->GetPosition(px, py);
		physB->GetPosition(bx, by);

		if (by - py > texW / 2 + 8)
			groundBodies.insert(physB);

		LOG("Collision PLATFORM");
		break;
	}
	case ColliderType::ITEM:
		
		//Play the coin pickup sound effect 
		Engine::GetInstance().audio->PlayFx(pickCoinFxId);

		// Destroy the item. 
		physB->listener->Destroy(); 

		LOG("Collision ITEM");

		break;


		//N La etiqueta llama, y todo el mundo obedece
		case ColliderType:: DEATH:

			TakeDamage();

		break;

		//N La etiqueta llama, y todo el mundo obedece
		case ColliderType::PIT:

			Kill();

			break;


	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}

}


//borramos los elementos de la lista
void Player::OnCollisionEnd(PhysBody* physA, PhysBody* physB)

{
	groundBodies.erase(physB);


}
