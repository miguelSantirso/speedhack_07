#include "allegro.h"
#include "Missis_Plow.h"
#include "Objeto_Fisico.h"
#include "Body.h"

extern int Challenge_X;
extern int Challenge_Y;
extern void Try_Finished(int x);
extern SAMPLE * Tractor_Sound;

Missis_Plow::Missis_Plow(void) : x(Challenge_X), y(Challenge_Y-18), DaPlough(NULL), Q_Last(false)
{
	DaPlough = new Objeto_Fisico("media/snowploughs.tga", 15.0f, 135, 40);
	DaPlough->Puntero_Box->position.Set(x, y);
	play_sample(Tractor_Sound, 180, 0, 1000, 1);
}

Missis_Plow::~Missis_Plow(void)
{
	stop_sample(Tractor_Sound);
	if(DaPlough != NULL)
		delete DaPlough;
}

void Missis_Plow::Render(BITMAP * sc)
{
	if(DaPlough != NULL)
		DaPlough->Dibuja(sc);
}

void Missis_Plow::Update()
{
	
	DaPlough->Puntero_Box->AddForce(Vec2(-150,0));
	if(Q_Last)
	{
		if(key[KEY_P] && !key[KEY_L])
		{
			Q_Last = !Q_Last;
			DaPlough->Puntero_Box->AddForce(Vec2(1500,0));
		}
	}
	else
	{
		if(key[KEY_L] && !key[KEY_P])
		{
			Q_Last = !Q_Last;
			DaPlough->Puntero_Box->AddForce(Vec2(1500,0));
		}
	}
	Try_Finished(DaPlough->Puntero_Box->position.x);
}