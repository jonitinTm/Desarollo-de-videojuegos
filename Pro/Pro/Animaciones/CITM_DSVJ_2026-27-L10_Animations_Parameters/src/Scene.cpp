#include "Engine.h"
#include "Input.h"
#include "Audio.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "EntityManager.h"
#include "Player.h"
#include "Entity.h"
#include "Item.h"
#include "Map.h"

Scene::Scene() : Module()
{
	name = "scene";
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	// L04: TODO 3a: Instantiate the player through the entity manager
	player = std::dynamic_pointer_cast<Player>(Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER));

	// L10: TODO 4: Hand the player its <player> config.xml section
	// ..
	player->SetParameters(Engine::GetInstance().entityManager->configParameters.child("player"));

	// L08: TODO 4: Create an item through the entity manager
	std::shared_ptr<Item> item = std::dynamic_pointer_cast<Item>(Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM));
	item->position = Vector2D(150, 200);
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/level-iv-339695.wav");

	// L06: TODO 7: Load the map through the map module
	Engine::GetInstance().map->Load("Assets/Maps/", "MapTemplate.tmx");

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	// L03: TODO 3: Make the camera movement independent of framerate.
	//   - Declare a camSpeed of 200.0f (pixels per second).
	//   - Multiply camSpeed by dt before adding/subtracting it below.
	//   - camera.x / camera.y are float (SDL_FRect), so just assign the result.
	float camSpeed = 200.0f;





	if(Engine::GetInstance().input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		Engine::GetInstance().render->camera.y -= camSpeed * dt;

	if(Engine::GetInstance().input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		Engine::GetInstance().render->camera.y += camSpeed * dt;

	if(Engine::GetInstance().input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		Engine::GetInstance().render->camera.x -= camSpeed * dt;

	if(Engine::GetInstance().input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		Engine::GetInstance().render->camera.x += camSpeed * dt;

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{

	bool ret = true;
// ---------------------------------> N Dibujamos por pantalla

	// Si estamos en la pantalla de game over

	if (player->gameOver)
	{


		//Para que nos entendamos aqui, lo que se va hacer es una especie de colaix, pegando cosas
		//desde el fondo hacia delante




		//Por asi decirlo, le assignamos a una variable el poder de poder dibujar, sobre ella

		SDL_Renderer* r = Engine::GetInstance().render->renderer;


		//Hacemos la primera capa, le assignamos una capa transparente

		SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);

		//Definimos el rectangulo, que se mostrara en la pantalla

		SDL_Rect screen = { 0, 0, 1280, 720 };

		//Ahora cogemos el rectangulo y lo dibujamos
		//180 = opacidad, true relleno, si fuera false, solo se dibujaria el borde. false, para que se quede estatico
		Engine::GetInstance().render->DrawRectangle(screen, 0, 0, 0, 180, true, false);


		//Definimos la escala de como queremos que se vean las cosas
		SDL_SetRenderScale(r, 4.0f, 4.0f);

		//Definimos El color de la letra que usaremos para dibujar
		SDL_SetRenderDrawColor(r, 255, 60, 60, 255);

		//Creamos el texto que queremos que se muestre en pantall
		//Le pasamos una posicion
		SDL_RenderDebugText(r, 124.0f, 70.0f, "GAME OVER");



		//Ahora pasamos a crear el otro texto que se dibuja abajo



		SDL_SetRenderScale(r, 2.0f, 2.0f);
		SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
		SDL_RenderDebugText(r, 240.0f, 200.0f, "Press ENTER to retry");

		//Siempre que acabemos, volver a la escala normal


		SDL_SetRenderScale(r, 1.0f, 1.0f);




	}













	//-----------









	if(Engine::GetInstance().input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
