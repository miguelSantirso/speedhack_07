#include "allegro.h"
#include "TheGrandmother.h"
#include <string>
#include <vector>

#include "Body.h"
#include "World.h"

using namespace std;

extern World world;
extern vector<Body *> bodies;
extern int X_Camera;
extern int Y_Camera;
extern int Res_Width;
extern int Res_Height;
extern float Size_Multiplier;

extern int Challenge_X;
extern int Challenge_Y;

extern void Aborta_Con_Error(std::string);

TheGrandmother::TheGrandmother(void) : t(0)
{
	PALETTE palette;

	// Load the sprite
	Graph = load_bitmap("media\\grandma.pcx", palette);
	if(!Graph)
		Aborta_Con_Error("ERROR:\nError de ejecucion en la funcion Inicializa() de Objeto_Fisico.cpp.\n - Can't load  media\\grandma.pcx");

	Width=Graph->w;
	Height=Graph->h;

	Puntero_Box = new Body();
	Puntero_Box->Set(Vec2(Width, Height), 200.0f); // Tamaño
	Puntero_Box->position.Set(Challenge_X, Challenge_Y-Height/2); // Posición
    Puntero_Box->friction = 0.5; // Fricción del objeto
	world.Add(Puntero_Box); // La añadimos al "mundo"
	bodies.push_back(Puntero_Box); // Y la añadimos al vector que almacena todos los cuerpos
}

TheGrandmother::~TheGrandmother(void)
{
	if(Graph != NULL)
		destroy_bitmap(Graph);
}


void TheGrandmother::Render(BITMAP * sc)
{
	Puntero_Box->rotation = 0;
	// Dibujamos en la pantalla el objeto.
	rotate_sprite(sc,												// Se dibujará en el bitmap indicado
				Graph,												// bitmap a dibujar
				Puntero_Box->position.x-Graph->w/2-X_Camera,		// Posición
				Puntero_Box->position.y-Graph->h/2-Y_Camera,
				itofix((256*Puntero_Box->rotation)/(2*3.141516)));	// Rotación
}

void TheGrandmother::Update()
{
/*	t++;

	if(key[KEY_RIGHT])
		Puntero_Box->force.x = 2000;
	if(key[KEY_LEFT])
		Puntero_Box->force.x = -2000;
	if(key[KEY_UP])
		Puntero_Box->force.y = -2000;
	if(key[KEY_DOWN])
		Puntero_Box->force.y = 2000;

	if(mouse_b)
	{
		for(int i=0; i<world.bodies.size(); i++)
		{
			Vec2 Force = Vec2(mouse_x+X_Camera, mouse_y+Y_Camera) - world.bodies[i]->position;
			world.bodies[i]->AddForce(Force);
		}
	}

	Puntero_Box->force.y = (-150/Size_Multiplier)*((-Res_Height/3 + Puntero_Box->position.y)/6);*/
}