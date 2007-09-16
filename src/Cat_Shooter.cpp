#include "allegro.h"
#include <string>
#include "Cat_Shooter.h"
#include "Body.h"
#include "Objeto_Fisico.h"

extern int X_Camera;
extern int Y_Camera;
extern int Res_Width;
extern int Res_Height;
extern int Challenge_X;
extern int Challenge_Y;
extern void delete_Body(Body *);
extern void Try_Finished(int);
extern void Aborta_Con_Error(std::string);
extern SAMPLE * Cat_Sound;

Cat_Shooter::Cat_Shooter(void) : Cat(NULL), Button_Pressed(false), Power(0), Grandma_Sprite(NULL), Arm_Sprite(NULL)
{
	PALETTE palette;

	// Load the sprite
	Grandma_Sprite = load_bitmap("media/grandma.pcx", palette);
	if(!Grandma_Sprite)
		Aborta_Con_Error("ERROR:\nError de ejecucion en la funcion Inicializa() de Objeto_Fisico.cpp.\n - Can't load  media/grandma.pcx");

	Arm_Sprite = load_bitmap("media/grandmaarm.pcx", palette);
	if(!Arm_Sprite)
		Aborta_Con_Error("ERROR:\nError de ejecucion en la funcion Inicializa() de Objeto_Fisico.cpp.\n - Can't load  media/grandmaarm.pcx");

}

Cat_Shooter::~Cat_Shooter(void)
{
	if(Grandma_Sprite != NULL)
		destroy_bitmap(Grandma_Sprite);
	if(Arm_Sprite != NULL)
		destroy_bitmap(Arm_Sprite);

	if(Cat != NULL)
		delete Cat;
}

void Cat_Shooter::Update()
{
	if(key[KEY_SPACE])
	{
		if(Cat == NULL)
			Power++;
		Button_Pressed = true;
	}
	else
	{
		if(Button_Pressed)
		{
			if(Cat == NULL)
			{
				play_sample(Cat_Sound, 200, 0, 1000, 0);
				Cat = new Objeto_Fisico("media/cat.pcx", 1.0, 40, 27);
				Cat->Puntero_Box->friction = 1;
				Cat->Puntero_Box->position.Set(Challenge_X+50, Challenge_Y);
				Cat->Puntero_Box->AddForce(Vec2(Power*100, 500));
			}
		}
		Power = 0;
		Button_Pressed = false;
	}
	
	if(Cat!=NULL)
	{
		if(Cat->Puntero_Box->position.y > Res_Height)
			Try_Finished(0);
	
		if(Cat->Puntero_Box->velocity.x > 0.1)
		{
			if(Cat->Puntero_Box->position.x + 150 >= Res_Width)
				X_Camera = Cat->Puntero_Box->position.x + 150 - Res_Width;
		}
		else if(Cat->Puntero_Box->force.x < 1)
		{
			Try_Finished(Cat->Puntero_Box->position.x);
		}
	}
}

void Cat_Shooter::Render(BITMAP * sc)
{
	triangle(sc, Challenge_X+50, Challenge_Y, Challenge_X+50+Power, Challenge_Y-Power/5, Challenge_X+50+Power, Challenge_Y, makecol(50, 50, 255));

	// Dibujamos en la pantalla el objeto.
	draw_sprite(sc,												// Se dibujará en el bitmap indicado
				Grandma_Sprite,									// bitmap a dibujar
				Challenge_X-22-X_Camera,		// Posición
				Challenge_Y-90-Y_Camera);
	draw_sprite(sc,												// Se dibujará en el bitmap indicado
				Arm_Sprite,											// bitmap a dibujar
				Challenge_X-22-X_Camera - 27,	// Posición
				Challenge_Y-90-Y_Camera + 23);
				//itofix((128*Power)/(2*10000)));	// Rotación

	if(Cat != NULL)
	{
		Cat->Dibuja(sc);
	}
}

void Cat_Shooter::delete_Cat()
{
	delete_Body(Cat->Puntero_Box);
	delete Cat;
	Cat = NULL;
}