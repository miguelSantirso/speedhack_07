#include "allegro.h"
#include "TheGrandmother.h"

#include <string>
#include <vector>

#include "Body.h"
#include "World.h"
#include "Objeto_Fisico.h"

using namespace std;

extern World world;
extern vector<Body *> bodies;
extern vector<Objeto_Fisico *> Small_Objects;
extern void Try_Finished(int);
extern int X_Camera;
extern int Y_Camera;
extern int Res_Width;
extern int Res_Height;
extern float Size_Multiplier;

extern int Challenge_X;
extern int Challenge_Y;

extern void Aborta_Con_Error(std::string);

TheGrandmother::TheGrandmother(void) : t(0), Key_Pressed(false)
{
	PALETTE palette;

	// Load the sprite
	Graph = load_bitmap("media\\grandma.pcx", palette);
	if(!Graph)
		Aborta_Con_Error("ERROR:\nError de ejecucion en la funcion Inicializa() de Objeto_Fisico.cpp.\n - Can't load  media\\grandma.pcx");

	Width=Graph->w;
	Height=Graph->h;

	Puntero_Box = new Body();
	Puntero_Box->Set(Vec2(Width, Height), 100.0f); // Tamaño
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
	X_Camera = Puntero_Box->position.x-Res_Width/2;

	if(!key[KEY_RIGHT] && !key[KEY_LEFT])
	{
		Key_Pressed = false;
	}
	else
	{
		if(!Key_Pressed && Puntero_Box->velocity.x < 0.1)
		{
			if(key[KEY_RIGHT])
				Puntero_Box->force.x = 50000;
			if(key[KEY_LEFT])
				Puntero_Box->force.x = -50000;
		}
		Key_Pressed = true;
	}

	if(mouse_b)
	{
		for(int i=0; i<Small_Objects.size(); i++)
		{
			Small_Objects[i]->Puntero_Box->force.Set(0,0);
			float Inverted_Distance = 1/abs(Small_Objects[i]->Puntero_Box->position.x - Puntero_Box->position.x);
			Inverted_Distance += 0.1;
			if(Inverted_Distance > 0.111)
				Inverted_Distance = 0.111;
			Vec2 Force = Vec2(mouse_x+X_Camera, mouse_y+Y_Camera) - Small_Objects[i]->Puntero_Box->position;
			Force.x = (Force.x > 0 ? 1 : -1);
			Force.y = (Force.y > 0 ? 1 : -1);
			Force*=500000;
			Force*=Inverted_Distance*Inverted_Distance*Inverted_Distance;
			Small_Objects[i]->Puntero_Box->AddForce(Force);
		}
	}

	if(Puntero_Box->position.y > Res_Height)
		Try_Finished(0);

//	Puntero_Box->force.y = (-150/Size_Multiplier)*((-Res_Height/3 + Puntero_Box->position.y)/6);
}