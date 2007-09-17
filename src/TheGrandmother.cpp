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

extern SAMPLE * Magic_Sound;

extern int Challenge_X;
extern int Challenge_Y;

extern void Aborta_Con_Error(std::string);

TheGrandmother::TheGrandmother(void) : t(0), Key_Pressed(false), Graph(NULL), Arm_Sprite(NULL), Sound_Active(false)
{
	PALETTE palette;

	// Load the sprite
	Graph = load_bitmap("media/grandma.tga", palette);
	if(!Graph)
		Aborta_Con_Error("ERROR:\nError de ejecucion en la funcion Inicializa() de Objeto_Fisico.cpp.\n - Can't load  media/grandma.tga");

	Arm_Sprite = load_bitmap("media/grandmaarm.tga", palette);
	if(!Arm_Sprite)
		Aborta_Con_Error("ERROR:\nError de ejecucion en la funcion Inicializa() de Objeto_Fisico.cpp.\n - Can't load  media/grandmaarm.tga");

	Width=Graph->w;
	Height=Graph->h;

	Puntero_Box = new Body();
	Puntero_Box->Set(Vec2(Width, Height), 98.0f); // Tamaño
	Puntero_Box->position.Set(Challenge_X, Challenge_Y-Height/2); // Posición
    Puntero_Box->friction = 0.01; // Fricción del objeto
	world.Add(Puntero_Box); // La añadimos al "mundo"
	bodies.push_back(Puntero_Box); // Y la añadimos al vector que almacena todos los cuerpos
}

TheGrandmother::~TheGrandmother(void)
{
	if(Graph != NULL)
		destroy_bitmap(Graph);
	if(Arm_Sprite != NULL)
		destroy_bitmap(Arm_Sprite);
}


void TheGrandmother::Render(BITMAP * sc)
{
	Vec2 VectorToMouse = Vec2(mouse_x + X_Camera, mouse_y + Y_Camera) - Puntero_Box->position;
	//float Aux_Rotation = fixatan(ftofix(VectorToMouse.y/VectorToMouse.x));
	float Aux_Rotation = VectorToMouse.y/VectorToMouse.x;

	if(VectorToMouse.x > 0)
	{
		// Dibujamos en la pantalla el objeto.
		rotate_sprite(sc,												// Se dibujará en el bitmap indicado
					Graph,												// bitmap a dibujar
					Puntero_Box->position.x-Graph->w/2-X_Camera,		// Posición
					Puntero_Box->position.y-Graph->h/2-Y_Camera,
					itofix((256*Puntero_Box->rotation)/(2*3.141516)));	// Rotación
		rotate_sprite(sc,												// Se dibujará en el bitmap indicado
					Arm_Sprite,											// bitmap a dibujar
					Puntero_Box->position.x-Graph->w/2-X_Camera - 27,	// Posición
					Puntero_Box->position.y-Graph->h/2-Y_Camera + 23,
					itofix((128*Aux_Rotation)/(2*3.141516)));	// Rotación
	}
	else
	{
		// Dibujamos en la pantalla el objeto.
		rotate_sprite_v_flip(sc,												// Se dibujará en el bitmap indicado
					Graph,												// bitmap a dibujar
					Puntero_Box->position.x-Graph->w/2-X_Camera,		// Posición
					Puntero_Box->position.y-Graph->h/2-Y_Camera,
					itofix(128) + itofix((125*Puntero_Box->rotation)/(2*3.141516)));	// Rotación
		rotate_sprite_v_flip(sc,												// Se dibujará en el bitmap indicado
					Arm_Sprite,											// bitmap a dibujar
					Puntero_Box->position.x-Graph->w/2-X_Camera ,	// Posición
					Puntero_Box->position.y-Graph->h/2-Y_Camera + 23,
					itofix(128) + itofix((125*Aux_Rotation)/(2*3.141516)));	// Rotación
	}
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
		if(!Key_Pressed && Puntero_Box->velocity.x < 0.5 && !mouse_b)
		{
			if(key[KEY_RIGHT])
				Puntero_Box->force.x = 30000;
			if(key[KEY_LEFT])
				Puntero_Box->force.x = -30000;
		}
		Key_Pressed = true;
	}

	if(mouse_b)
	{
		if(!Sound_Active)
		{
			Sound_Active = true;
			play_sample(Magic_Sound, 0, 0, 1000, 1);
			t = 0;
		}
		else
		{
			t++;
			if(t>2550)
				t=2550;
			adjust_sample(Magic_Sound, t/10, 0, 1000, 1);
		}
		for(int i=0; i<Small_Objects.size(); i++)
		{
			Small_Objects[i]->Puntero_Box->force.Set(0,0);
			float Inverted_Distance = 1/fabs(double(Small_Objects[i]->Puntero_Box->position.x - Puntero_Box->position.x));
			Inverted_Distance += 0.1;
			if(Inverted_Distance > 0.111)
				Inverted_Distance = 0.111;
			Vec2 Force = Vec2(mouse_x+X_Camera, mouse_y+Y_Camera) - Small_Objects[i]->Puntero_Box->position;
			Force.x = (Force.x > 0 ? 1 : -1);
			Force.y = (Force.y > 0 ? 1 : -1);
			Force*=520000;
			Force*=Inverted_Distance*Inverted_Distance*Inverted_Distance;
			Small_Objects[i]->Puntero_Box->AddForce(Force);
		}
	}
	else if(Sound_Active)
	{
		t-=10;
		if(t>0)
			adjust_sample(Magic_Sound, t/10, 0, 1000, 1);
		else
		{
			Sound_Active = false;
			stop_sample(Magic_Sound);
		}
	}

	int Aux = Puntero_Box->position.x - Width/2;
	if(Puntero_Box->velocity.x > 0.1)
		Aux = 0;
	if(Puntero_Box->position.y > Res_Height)
		Aux = -1;

	Try_Finished(Aux);

//	Puntero_Box->force.y = (-150/Size_Multiplier)*((-Res_Height/3 + Puntero_Box->position.y)/6);
}