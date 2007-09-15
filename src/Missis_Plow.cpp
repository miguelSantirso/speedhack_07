#include "allegro.h"
#include "Missis_Plow.h"
#include "Objeto_Fisico.h"
#include "Body.h"

extern int Challenge_X;
extern int Challenge_Y;

Missis_Plow::Missis_Plow(void) : x(Challenge_X-50), y(Challenge_Y-18), DaPlough(NULL), Q_Last(false)
{
	DaPlough = new Objeto_Fisico("media\\snowploughs.pcx", 15.0f, 135, 40);
	DaPlough->Puntero_Box->position.Set(x, y);
}

Missis_Plow::~Missis_Plow(void)
{
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
		if(key[KEY_P])
		{
			Q_Last = !Q_Last;
			DaPlough->Puntero_Box->AddForce(Vec2(600,0));
		}
	}
	else
	{
		if(key[KEY_O])
		{
			Q_Last = !Q_Last;
			DaPlough->Puntero_Box->AddForce(Vec2(600,0));
		}
	}
}