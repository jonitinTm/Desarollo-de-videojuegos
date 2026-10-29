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
};
