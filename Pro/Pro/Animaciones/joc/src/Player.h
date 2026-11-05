#pragma once

#include "Entity.h"
#include <SDL3/SDL.h>
#include <box2d/box2d.h>

#include <string>
#include <set>


#include "Animation.h"
struct SDL_Texture;

class Player : public Entity
{
public:

	Player();

	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool Draw();

	bool CleanUp();



	//N Basicamente es para detectar si esta colisionando con algo el jugador
	// L08: TODO 6: Define OnCollision function for the player.
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);
	float jumpSpeed = 6.5f;
	//Spawn
	void SetSpawnPoint(Vector2D p);
public:

	// Declare player parameters
	float speed = 5.0f;
	SDL_Texture* texture;
	int texW, texH;








	//N Vidas, porque todo el mundo lo puede modificar
	int lives = 3;


	//N activacion del good mode
	bool godMode = false;
	void PoderGod();
	void Fly(std::string& anim);






	//N control de daño
	bool gameOver = false;





	// L08: TODO 5: Add physics to the player - declare a Physics body
	PhysBody* pbody = nullptr;

private:
	b2Vec2 velocity;
	// Audio fx
	int pickCoinFxId;

	// L10: TODO 5: Player's animation clips, loaded from a TSX in Start()
	// ..
	AnimationSet anims;


	// N variables para el funcionamento del roll
		// Roll
	bool rolling = false;
	float rollTimer = 0.0f;
	float rollDuration = 0.5f;   // segundos que dura el roll
	float rollSpeed = 12.0f;     // más alto = recorre más distancia
	int facing = 1;

	//N creacion de las funciones para la muerte instanteanea
	void Kill();
	bool dying = false;
	float deathTimer = 0.0f;
	void Die();







	//N variables para controlar la muerte

	bool invulnerable = false;
	float invulTimer = 0.0f;
	float invulDuration = 0.5f;
	float hitTimer = 0.0f;
	int hurtFxId;

	void TakeDamage();


	//N control de daño


	Vector2D spawnPoint;
	void Restart();
	void Respawn();



	//Declaramos los movimientos del jugador
	void Move(std::string& anim);

	void Jump();

	void Roll(float dt, std::string& anim);

	//Detección de si hay suelo

	std::set<PhysBody*>groundBodies;//creamos un set donde pondremos los valores del suelo, para saber si lo estamos pisando

	bool IsOnGround() const
	{
		return groundTimer > 0.0f;
	}
	//Variable, para no bugear salto
	float groundTimer = 0.0f;     
	float groundGrace = 0.1f;     
	float jumpLock = 0.0f;
};
