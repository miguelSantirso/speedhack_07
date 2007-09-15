#include "allegro.h"
#include "Cat_Shooter.h"
#include "Body.h"
#include "Objeto_Fisico.h"

extern int X_Camera;
extern int Res_Width;
extern int Res_Height;
extern int Challenge_X;
extern int Challenge_Y;
extern void delete_Body(Body *);
extern void Try_Finished(int);

Cat_Shooter::Cat_Shooter(void) : Cat(NULL), Button_Pressed(false), Power(0)
{
}

Cat_Shooter::~Cat_Shooter(void)
{
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
				Cat = new Objeto_Fisico("media\\cat.pcx", 1.0, 40, 27);
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