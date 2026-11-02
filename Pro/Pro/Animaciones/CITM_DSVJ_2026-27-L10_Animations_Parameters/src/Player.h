#pragma once

#include "Entity.h"
#include <SDL3/SDL.h>
#include <box2d/box2d.h>
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

	// L08: TODO 6: Define OnCollision function for the player.
	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	// Declare player parameters
	float speed = 5.0f;
	SDL_Texture* texture;
	int texW, texH;



	//N Vidas, porque todo el mundo lo puede modificar
	int lives = 3;





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


	//N variables para controlar la muerte

	bool invulnerable = false;
	float invulTimer = 0.0f;
	float invulDuration = 1.5f;
	float hitTimer = 0.0f;
	int hurtFxId;

	void TakeDamage();


	//N control de daño
	bool gameOver = false;

	Vector2D spawnPoint;
	void Restart();
	void Respawn();

};
