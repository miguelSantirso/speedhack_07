//#define FULLSCREEN

#define TITLE_LENGTH int(2000)
#define TITLETRANSITION_LENGTH int(900)

#include <allegro.h>
#include <string>

#include "Easy_Writer.h"
#include "World.h"
#include "Body.h"
#include "Joint.h"
#include "Objeto_Fisico.h"
#include "Cat_Shooter.h"
#include "Missis_Plow.h"
#include "TheGrandmother.h"

using namespace std;

Easy_Writer * Writer;

bool Debug = false;			// Indica si se está en modo debug (se dibujan los bodies y los joints)

World world(Vec2(0.0f, 20.0f), 10);		// Inicializa el sistema de físicas

vector<Body *> bodies;	// Vector de cuerpos físicos (las cajas de colisiones)
vector<Joint *> joints;	// Vector de enlaces (enlaces entre cuerpos físicos)
vector<Objeto_Fisico *> Objetos_Fisicos;	// Almacena los punteros a todos los objetos que se rigen por las físicas.


// Global Game Variables
int Seconds_Remaining;
int Current_Challenge;
int Challenge_X;
int Challenge_Y;
int Next;
int Wait_Next;

int Target_Left;
int Target_Right;
int Target_Up;
int Target_Down;

string Challenge_Name;
string Challenge_Instructions;


// Camera Coordinates
int X_Camera;
int Y_Camera;

// Especial Objects
Cat_Shooter * Teh_Shooter = NULL;
Missis_Plow * Teh_MissisPlow = NULL;
TheGrandmother * Teh_Grandma = NULL;
vector<Objeto_Fisico *> Small_Objects;

// Sounds
SAMPLE * Success_Sound=NULL;
SAMPLE * Error_Sound=NULL;
SAMPLE * Magic_Sound=NULL;
SAMPLE * Cat_Sound=NULL;

// Resolution
int Res_Width;
int Res_Height;
float Size_Multiplier;

// timer stuff
volatile int frame_count;
volatile int fps;
volatile int logic_count;
volatile int lps;
volatile int cycle_count;
volatile int game_count;
volatile int miliseconds;

BITMAP * swap_screen;

void Reiniciar(bool First_Time=true);

// counts miliseconds from the beggining
void ms_counter(void) {
	miliseconds++;
}
END_OF_FUNCTION(ms_counter)

// keeps track of frames each second
void fps_counter(void) {
	fps = frame_count;
	frame_count = 0;
	lps = logic_count;
	logic_count = 0;
}
END_OF_FUNCTION(fps_counter)


// keeps track of internal game speed
void cycle_counter(void) {
	cycle_count++;
	game_count++;
}
END_OF_FUNCTION(game_counter)



void DrawText(BITMAP *p_bmp, int x, int y, char *string) {
    textout_ex(p_bmp, font, string, x, y, 5, -1);
}


void DrawBody(BITMAP *p_bmp, Body* body, int px, int py)
{
	Mat22 R(body->rotation);
	Vec2 x = body->position;
	Vec2 h = 0.5f * body->width;

	x.x -= X_Camera;
	x.y -= Y_Camera;

	Vec2 v1 = x + R * Vec2(-h.x, -h.y);
	Vec2 v2 = x + R * Vec2( h.x, -h.y);
	Vec2 v3 = x + R * Vec2( h.x,  h.y);
	Vec2 v4 = x + R * Vec2(-h.x,  h.y);

    int color = 3;
	if (body->mass == FLT_MAX) color = 1;

    line(p_bmp, px + (int)v1.x, py + (int)v1.y, px + (int)v2.x, py + (int)v2.y, color);
    line(p_bmp, px + (int)v2.x, py + (int)v2.y, px + (int)v3.x, py + (int)v3.y, color);
    line(p_bmp, px + (int)v3.x, py + (int)v3.y, px + (int)v4.x, py + (int)v4.y, color);
    line(p_bmp, px + (int)v4.x, py + (int)v4.y, px + (int)v1.x, py + (int)v1.y, color);

    if (body->mass != FLT_MAX && Debug) {
        textprintf_centre_ex(p_bmp, font, (int)x.x, py + (int)x.y, 5, -1, "%d", (int)body->mass);
    }
}

void DrawJoint(BITMAP *p_bmp, Joint* joint, int px, int py)
{
	Body* b1 = joint->body1;
	Body* b2 = joint->body2;

	Mat22 R1(b1->rotation);
	Mat22 R2(b2->rotation);

	Vec2 x1 = b1->position;
	Vec2 p1 = x1 + R1 * joint->localAnchor1;

	Vec2 x2 = b2->position;
	Vec2 p2 = x2 + R2 * joint->localAnchor2;

	int color = 2;
	line(p_bmp, px + (int)x1.x, py + (int)x1.y, px + (int)p1.x, py + (int)p1.y, color);
	line(p_bmp, px + (int)p1.x, py + (int)p1.y, px + (int)x2.x, py + (int)x2.y, color);
	line(p_bmp, px + (int)x2.x, py + (int)x2.y, px + (int)p2.x, py + (int)p2.y, color);
	line(p_bmp, px + (int)p2.x, py + (int)p2.y, px + (int)x1.x, py + (int)x1.y, color);
}

void Aborta_Con_Error(string Error)
{
	set_gfx_mode(GFX_TEXT , 0 , 0 , 0 , 0);
	allegro_message((const char *) Error.c_str());
	exit(-1);
}


void Write_Time()
{
	int Aux = (Seconds_Remaining*1000 - miliseconds);
	int Minutes = (Aux/1000)/60;
	int Seconds = (Aux/1000)%60;
	int MiliSeconds = Aux%60;

	int Size = 15*Size_Multiplier;
	int X_Left = Res_Width-Size*6*Size_Multiplier;
	int Y_Top = (Size+Size/2)*Size_Multiplier;
/*
	//X_Left -= Res_Width/2 - (Res_Width/2)*(4000 - miliseconds>0 ? (4000 - miliseconds)/1000000 : 0);
	X_Left -= miliseconds/10;
	*/
	Writer->Write_Number(Minutes, makecol(0,0,255), X_Left, Y_Top, Size);
	Writer->Write_Number(Seconds, makecol(0,0,255), X_Left + 30*Size_Multiplier, Y_Top, Size);
	Writer->Write_Number(MiliSeconds, makecol(0,0,255), X_Left + 60*Size_Multiplier, Y_Top, Size);
}

void Write_Title()
{

	float Percent = 1;
	if(TITLE_LENGTH - miliseconds <= 0)
	{
		int Aux = TITLE_LENGTH + TITLETRANSITION_LENGTH;
		Percent = (Aux - miliseconds > 0 ? float(TITLETRANSITION_LENGTH - (miliseconds - TITLE_LENGTH))/TITLETRANSITION_LENGTH : 0);
	/*	if(Percent <= 0)
			return;*/
	}

	int Aux = (Seconds_Remaining*1000 - miliseconds);
	int Minutes = (Aux/1000)/60;
	int Seconds = (Aux/1000)%60;
	int MiliSeconds = Aux%60;

	int Size = 13*Size_Multiplier + 28*Percent*Size_Multiplier;
	int X_Left = Res_Width-Size*13 - 120*Size_Multiplier*Percent;
	int Y_Top = (Size+Size/2)+170*Size_Multiplier*Percent;

	// Write timer
	Writer->Write_Number(Minutes, makecol(0,0,255), X_Left, Y_Top, Size);
	Writer->Write_Number(Seconds, makecol(0,0,255), X_Left + (2*Size), Y_Top, Size);
	Writer->Write_Number(MiliSeconds, makecol(0,0,255), X_Left + (4*Size), Y_Top, Size);

	// Write "THE"
	Writer->Write_String("THE", makecol(0,0,255), X_Left-Size*2, Y_Top, Size);
	// Write "MINUTES"
	Writer->Write_String("MINUTES", makecol(0,0,255), X_Left+Size*8, Y_Top, Size);
	// Write "VIDEOGAME"
	Writer->Write_String("VIDEOGAME", makecol(0,0,255), X_Left+Size*5, Y_Top+1.3*Size, Size*1.3);

}

void Start_Challenge(int ID_Challenge)
{
	// INICIAR RETOS
	Current_Challenge = ID_Challenge;
	switch(ID_Challenge)
	{
		case 1:
		{
			X_Camera = Y_Camera = 0;
			Challenge_X = Res_Width/10;
			Challenge_Y = Res_Height - Res_Height/3;

			Objeto_Fisico *Floor;
		    

			// Load and create the floor
			Floor = new Objeto_Fisico("media/snowplatform.pcx",FLT_MAX, 500, 53);
			Floor->Puntero_Box->friction=1.0f;
			Floor->Puntero_Box->position.Set(Challenge_X+100, Challenge_Y+30);
			Objetos_Fisicos.push_back(Floor);
		   // Load and create the floor
			Floor = new Objeto_Fisico("media/snowplatform.pcx", FLT_MAX, 500, 50);
			Floor->Puntero_Box->friction=0.5f;
			Floor->Puntero_Box->position.Set(Challenge_X+250, Challenge_Y);
			Floor->Puntero_Box->rotation=-0.2;
			Objetos_Fisicos.push_back(Floor);

			// Load and create the platforms
			Floor = new Objeto_Fisico("media/grassplatform.pcx",FLT_MAX, 100, 150);
			Floor->Puntero_Box->friction=0.3f;
			Floor->Puntero_Box->position.Set(Challenge_X+760, Challenge_Y+90);
			Objetos_Fisicos.push_back(Floor);
			Floor = new Objeto_Fisico("media/iceplatform.pcx",FLT_MAX, 180, 270);
			Floor->Puntero_Box->friction=0.05f;
			Floor->Puntero_Box->position.Set(Challenge_X+900, Challenge_Y+150);
			Objetos_Fisicos.push_back(Floor);

			Target_Left = Challenge_X+900-90;
			Target_Right = Challenge_X+900+90;
			Target_Up = Challenge_Y+90-175;
			Target_Down = Challenge_Y+90-75;

			Challenge_Name = "FREE2E DA CAT2";
			Challenge_Instructions = "PRESS SPACE TO THROUU THE CATS TO THE ICE PLATFORM";

			// Create the cat shooter
			Teh_Shooter = new Cat_Shooter();
			break;
		}
		case 2:
		{
			X_Camera = Y_Camera = 0;
			Challenge_X = Res_Width/2;
			Challenge_Y = Res_Height - Res_Height/3;

			Objeto_Fisico *Floor;
		   
			// Load and create the floor
			Floor = new Objeto_Fisico("media/snowplatform.pcx", FLT_MAX, 400, 53);
			Floor->Puntero_Box->friction=1.0f;
			Floor->Puntero_Box->position.Set(Challenge_X, Challenge_Y+26);
			Objetos_Fisicos.push_back(Floor);
			// Load and create the platforms
			Floor = new Objeto_Fisico("media/grassplatform.pcx",FLT_MAX, 100, 150);
			Floor->Puntero_Box->friction=3.0f;
			Floor->Puntero_Box->position.Set(Challenge_X+250, Challenge_Y+75);
			Objetos_Fisicos.push_back(Floor);
			// Load and create the platforms
			Floor = new Objeto_Fisico("media/grassplatform.pcx",FLT_MAX, 100, 150);
			Floor->Puntero_Box->friction=3.0f;
			Floor->Puntero_Box->position.Set(Challenge_X-250, Challenge_Y+75);
			Objetos_Fisicos.push_back(Floor);

			Teh_MissisPlow = new Missis_Plow();

			Target_Left = Challenge_X+250 - 50;
			Target_Right = Challenge_X+350 - 50;
			Target_Up = Challenge_Y+90-175;
			Target_Down = Challenge_Y+90-75;

			Challenge_Name = "MISSIS PLOU";
			Challenge_Instructions = "PRESS L AND P QUICKLY TO PUSH THE OTHER SNOU PLOUGH";
			break;
		}
		case 3:
		{
			X_Camera = Y_Camera = 0;
			Challenge_X = Res_Width/10;
			Challenge_Y = Res_Height - Res_Height/3;

			Objeto_Fisico *Floor;
			Objeto_Fisico *Object;
		   
			// Load and create the floor
			Floor = new Objeto_Fisico("media/snowplatform.pcx", FLT_MAX, 500, 53);
			Floor->Puntero_Box->friction=1.0f;
			Floor->Puntero_Box->position.Set(Challenge_X, Challenge_Y+26);
			Objetos_Fisicos.push_back(Floor);
			Floor = new Objeto_Fisico("media/snowplatform.pcx", FLT_MAX, 500, 53);
			Floor->Puntero_Box->friction=1.0f;
			Floor->Puntero_Box->position.Set(Challenge_X+800, Challenge_Y+30);
			Objetos_Fisicos.push_back(Floor);

			Floor = new Objeto_Fisico("media/rockplatform.pcx", 145.0f, 275, 20);
			Floor->Puntero_Box->friction=1.0f;
			Floor->Puntero_Box->position.Set(Challenge_X+400, Challenge_Y+10);
			Objetos_Fisicos.push_back(Floor);
			Floor = new Objeto_Fisico("media/rockcolumn.pcx",FLT_MAX, 40, 200);
			Floor->Puntero_Box->friction=5.0f;
			Floor->Puntero_Box->position.Set(Challenge_X+400, Challenge_Y+120);
			Objetos_Fisicos.push_back(Floor);

			Object = new Objeto_Fisico("media/rockbig.pcx",32.0f, 27, 27);
			Object->Puntero_Box->friction=1.0f;
			Object->Puntero_Box->position.Set(Challenge_X+200, Challenge_Y+12);
			Small_Objects.push_back(Object);
			Object = new Objeto_Fisico("media/rocksmall.pcx",22.0f, 20, 20);
			Object->Puntero_Box->friction=1.0f;
			Object->Puntero_Box->position.Set(Challenge_X+160, Challenge_Y+12);
			Small_Objects.push_back(Object);

			Teh_Grandma = new TheGrandmother();

			Target_Left = Challenge_X+800 - 75;
			Target_Right = Challenge_X+800 + 75;
			Target_Up = Challenge_Y+90-175;
			Target_Down = Challenge_Y+90-75;

			Challenge_Name = "FIND DA FORCE";
			Challenge_Instructions = "USE ARROWS TO MOVE AND MOUSE TO PICK OBJECTS WITH DA FORCE";
			break;
		}
	}
}
void delete_Body(Body* b)
{
	world.delete_Body(b);
	
	vector<Body *>::iterator i;
	for(i=bodies.begin(); i!=bodies.end(); i++)
	{
		if(*i == b)
		{
			bodies.erase(i);
			break;
		}
	}
}

// Cargador de la partida
void Iniciar_Partida(int i)
{
	Next = -1;
	// INICIALIZAR TODO
	Writer = new Easy_Writer(swap_screen);
	Success_Sound = load_wav("media/success.wav");
	Error_Sound = load_wav("media/wrong.wav");
	Magic_Sound = load_wav("media/magic.wav");
	Cat_Sound = load_wav("media/cat.wav");

	Seconds_Remaining = 120;

	Start_Challenge(i);

}

void Reiniciar(bool First_Time)
{
	// ELIMINAR TODO

    for(int i = 0; i < (int)joints.size(); i ++) {
        delete joints[i];
    }
    joints.clear();

    for(int i = 0; i < (int)bodies.size(); i ++) {
        delete bodies[i];
    }
    bodies.clear();

    for(int i = 0; i < (int)Objetos_Fisicos.size(); i ++) {
        delete Objetos_Fisicos[i];
    }
	Objetos_Fisicos.clear();

    for(int i = 0; i < (int)Small_Objects.size(); i ++) {
        delete Small_Objects[i];
    }
	Small_Objects.clear();

	delete Writer;
	Writer = NULL;

	world.Clear();
	
	if(Teh_Shooter != NULL)
	{
		delete Teh_Shooter;
		Teh_Shooter = NULL;
	}
	if(Teh_MissisPlow != NULL)
	{
		delete Teh_MissisPlow;
		Teh_MissisPlow = NULL;
	}
	if(Teh_Grandma != NULL)
	{
		delete Teh_Grandma;
		Teh_Grandma = NULL;
	}

	destroy_sample(Success_Sound);
	Success_Sound = NULL;
	destroy_sample(Error_Sound);
	Error_Sound = NULL;
	destroy_sample(Magic_Sound);
	Magic_Sound = NULL;
	destroy_sample(Cat_Sound);
	Cat_Sound = NULL;

	// Reiniciamos la partida
	if(First_Time)
		Iniciar_Partida(1);
}

void Scroll_Controls()
{
	if(key[KEY_A])
		X_Camera -=5;
	if(key[KEY_D])
		X_Camera +=5;
	if(key[KEY_S])
		Y_Camera +=5;
	if(key[KEY_W])
		Y_Camera -=5;
}

void Try_Finished(int x)
{
	if(Next != -1)
		return;
	if(x < 0)
	{
		Wait_Next = miliseconds;
		Next = Current_Challenge;
		play_sample(Error_Sound, 255, 126, 1000, 0);
	}

	switch (Current_Challenge)
	{
	case 1:
		if(x<Target_Left || x>Target_Right)
		{
			Wait_Next = miliseconds;
			Next = Current_Challenge;
			play_sample(Error_Sound, 255, 126, 1000, 0);
		}
		else
		{
			Wait_Next = miliseconds;
			Next = Current_Challenge+1;
			play_sample(Success_Sound, 255, 126, 1000, 0);
		}
		break;
	case 2:
		if(x<Challenge_X-250+50)
		{
			Wait_Next = miliseconds;
			Next = Current_Challenge;
			play_sample(Error_Sound, 255, 126, 1000, 0);
		}
		if(x>Target_Left && x<Target_Right)
		{
			Wait_Next = miliseconds;
			Next = Current_Challenge+1;
			play_sample(Success_Sound, 255, 126, 1000, 0);
		}
		break;
	case 3:
		if(x>Target_Left && x<Target_Right)
		{
			Wait_Next = miliseconds;
			Next = Current_Challenge+1;
			play_sample(Success_Sound, 255, 126, 1000, 0);
		}
		break;
	}
}

void Update_Challenge()
{
	if(Current_Challenge == 1)
	{
		if(Teh_Shooter != NULL)
			Teh_Shooter->Update();
	}
	else if(Current_Challenge == 2)
	{
		if(Teh_MissisPlow != NULL)
			Teh_MissisPlow->Update();
	}
	else if(Current_Challenge == 3)
	{
		if(Teh_Grandma != NULL)
			Teh_Grandma->Update();
	}
}

// Función que se ocupa de actualizar la lógica y la física del juego
void Update()
{
	// ACTUALIZAR TODA LA LÓGICA
	world.Step(0.05);	// Actualizar las físicas

	Update_Challenge();

	Scroll_Controls();

	if(Next != -1)
	{
		if(miliseconds - Wait_Next > 500)
		{
			Reiniciar(false);
			Iniciar_Partida(Next);
		}
	}
}

// draw a rectangle
// since we use it three times in the editor, it would be nice to have
// a function for it.
// Esta función la encontré por internet; aunque está muy bien hecha, tiene un funcionamiento bastante sencillo
void drawRectangle(BITMAP *bmp,int x, int y, int w,int h,int color, int style, int bgcolor)
{
	w--;
	h--;
	if (style==0)
	{
		vline(bmp,x,y,y+h,color);
		vline(bmp,x+w,y,y+h,color);
		hline(bmp,x,y,x+w,color);
		hline(bmp,x,y+h,x+w,color);
	}
	if (style==1)
	{
		rectfill(bmp,x,y,x+w,y+h,bgcolor);
		vline(bmp,x,y,y+h,color);
		vline(bmp,x+w,y,y+h,color);
		hline(bmp,x,y,x+w,color);
		hline(bmp,x,y+h,x+w,color);
	}


	// this is for our dialog. This draws a box with rounded corners.
	if (style==2)
	{									
		arc(bmp,x+10,y+10,itofix(64),itofix(128),10,255);
		arc(bmp,x+10,y+h-10,itofix(128),itofix(192),10,255);
		arc(bmp,x+w-10,y+10,itofix(0),itofix(64),10,255);
		arc(bmp,x+w-10,y+h-10,itofix(192),itofix(256),10,255);


		hline(bmp,x+10,y,x+w-10,255);
		hline(bmp,x+10,y+h,x+w-10,255);
		vline(bmp,x,y+10,y+h-10,255);
		vline(bmp,x+w,y+10,y+h-10,255);
	
		floodfill(bmp,x+w/2,y+h/2,255);

		arc(bmp,x+10,y+10,itofix(64),itofix(128),8,bgcolor);
		arc(bmp,x+10,y+h-10,itofix(128),itofix(192),8,bgcolor);
		arc(bmp,x+w-10,y+10,itofix(0),itofix(64),8,bgcolor);
		arc(bmp,x+w-10,y+h-10,itofix(192),itofix(256),8,bgcolor);


		hline(bmp,x+10,y+2,x+w-10,bgcolor);
		hline(bmp,x+10,y+h-2,x+w-10,bgcolor);
		vline(bmp,x+2,y+10,y+h-10,bgcolor);
		vline(bmp,x+w-2,y+10,y+h-10,bgcolor);

		floodfill(bmp,x+w/2,y+h/2,bgcolor);
	}
}

void Render_Mouse()
{
	circle(swap_screen, mouse_x, mouse_y, 5*Size_Multiplier, makecol(80, 80, 80));
}

void Render_Target(BITMAP * sc)
{
	line(sc, Target_Left-X_Camera, 0, Target_Left-X_Camera, Res_Height, makecol(200, 20, 50));
	line(sc, Target_Right-X_Camera, 0, Target_Right-X_Camera, Res_Height, makecol(200, 20, 50));
}

void Render_Challenge()
{
	Render_Target(swap_screen);
	if(Current_Challenge == 1)
	{
		if(Teh_Shooter != NULL)
			Teh_Shooter->Render(swap_screen);
	}
	else if(Current_Challenge == 2)
	{
		if(Teh_MissisPlow != NULL)
			Teh_MissisPlow->Render(swap_screen);
	}
	else if(Current_Challenge == 3)
	{
		Render_Mouse();
		if(Teh_Grandma != NULL)
			Teh_Grandma->Render(swap_screen);

		for(int i=0; i<(int)Small_Objects.size(); i++)
			Small_Objects[i]->Dibuja(swap_screen);
	}
}

// Realiza todas las tareas de dibujo.
void Render()
{
	clear_to_color(swap_screen, makecol(245,245,255));

	// DIBUJAR TODO
	// Dibujar objetos físicos
    for(int i = 0; i < (int)Objetos_Fisicos.size(); i ++) {
		Objetos_Fisicos[i]->Dibuja(swap_screen);
    }

    if (Debug)
	{
		line(swap_screen, Challenge_X - X_Camera, 0, Challenge_X - X_Camera, Res_Height, makecol(255, 0, 0));
		line(swap_screen, 0, Challenge_Y-Y_Camera, Res_Width, Challenge_Y-Y_Camera, makecol(255, 0, 0));

		// Dibujar cajas de colisiones
		for(int i = 0; i < (int)bodies.size(); i ++) {
			DrawBody(swap_screen, bodies[i], 0, 0);
		}
		// Dibujar uniones
        for(int i = 0; i < (int)joints.size(); i ++) {
            DrawJoint(swap_screen, joints[i], 0, 0);
        }
	}

	Render_Challenge();

	if(Next == Current_Challenge)
	{
		int Cross_x = Res_Width/2;
		int Cross_y = Res_Height/2;
		int Cross_Size = 400*Size_Multiplier;
		int Cross_Thickness = 50;
		int points[8] = {
			Cross_x - Cross_Size/2 + Cross_Thickness,
			Cross_y + Cross_Size/2,
			Cross_x + Cross_Size/2,
			Cross_y - Cross_Size/2 + Cross_Thickness,
			Cross_x + Cross_Size/2 - Cross_Thickness,
			Cross_y - Cross_Size/2,
			Cross_x - Cross_Size/2,
			Cross_y + Cross_Size/2 - Cross_Thickness};

		polygon(swap_screen, 4, points, makecol(255, 0, 0));

		int points2[8] = {
			Cross_x + Cross_Size/2 - Cross_Thickness,
			Cross_y + Cross_Size/2,
			Cross_x + Cross_Size/2,
			Cross_y + Cross_Size/2 - Cross_Thickness,
			Cross_x - Cross_Size/2 + Cross_Thickness,
			Cross_y - Cross_Size/2,
			Cross_x - Cross_Size/2,
			Cross_y - Cross_Size/2 + Cross_Thickness};

		polygon(swap_screen, 4, points2, makecol(255, 0, 0));
	}
	if(Next > Current_Challenge)
	{
		int Circle_x = Res_Width/2;
		int Circle_y = Res_Height/2;
		int Circle_Size = 300;

		circlefill(swap_screen, Circle_x, Circle_y, Circle_Size, makecol(0, 255, 0));
		circlefill(swap_screen, Circle_x, Circle_y, Circle_Size*0.8, makecol(255, 255, 255));
	}

	Write_Title();
	Writer->Write_String("CHALLENGE ", makecol(0, 0, 255), (5*25)*Size_Multiplier, 25*Size_Multiplier, 25*Size_Multiplier);
	Writer->Write_Number(Current_Challenge, makecol(0, 0, 255), (9*25)*Size_Multiplier, 25*Size_Multiplier, 25*Size_Multiplier);
	Writer->Write_String("OF 3", makecol(0, 0, 255), (12*25)*Size_Multiplier, 25*Size_Multiplier, 25*Size_Multiplier);
	Writer->Write_String(Challenge_Name, makecol(0, 0, 255), (7*25)*Size_Multiplier, 50*Size_Multiplier, 20*Size_Multiplier);
	Writer->Write_String(Challenge_Instructions, makecol(0, 0, 255), Res_Width/2, Res_Height - 25*Size_Multiplier, 15*Size_Multiplier);

    acquire_screen();	// Bloquear la pantalla antes de dibujar
    blit(swap_screen, screen, 0, 0, 0, 0, Res_Width, Res_Height); // Copiar todo desde swap_screen hasta la pantalla (screen)
    release_screen();	// La dejamos libre de nuevo
}

// Controla el teclado
void Keyboard()
{
	// Lectura de teclas sin bloqueo
	//int Aux = Config_Teclas[0].Id_Tecla >> 8;

	// Lecturas de teclas con bloqueo
    int kp = readkey() >> 8;

	if (kp == KEY_TAB) Debug = !Debug;
	if (kp == KEY_C) miliseconds = 0;
	if (kp == KEY_R) Next = Current_Challenge;
	
	if (kp == KEY_1) Next = 1;
	if (kp == KEY_2) Next = 2;
	if (kp == KEY_3) Next = 3;
}

// Función main
void main(int argc, char** argv)
{
    allegro_init();
	set_color_depth(16);
#ifdef FULLSCREEN
	get_desktop_resolution(& Res_Width, & Res_Height);
	set_gfx_mode(GFX_AUTODETECT, Res_Width, Res_Height, 0, 0);
#else
	Res_Width = 800;
	Res_Height = 600;
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, Res_Width, Res_Height, 0, 0);
#endif
	Size_Multiplier = Res_Width/640 ;
	set_window_title("Speedhack 07");

	srand ( time(NULL) );

	PALETTE palette;
/*
	COLOR_MAP global_trans_table;
    create_trans_table(&global_trans_table, palette, 128, 128, 128, NULL);
*/
	install_keyboard();
	install_mouse();
	char * p=NULL;
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, p);

	// install timers
	install_timer();
	LOCK_VARIABLE(cycle_count);
	LOCK_VARIABLE(logic_count);
	LOCK_VARIABLE(lps);
	LOCK_VARIABLE(fps);
	LOCK_VARIABLE(miliseconds);
	LOCK_VARIABLE(frame_count);
	srand(time(NULL));

	LOCK_FUNCTION(ms_counter);
	install_int(ms_counter, 1);
	LOCK_FUNCTION(fps_counter);
	install_int(fps_counter, 1000);
	fps = 0;
	frame_count = 0;
	cycle_count = 0;

	LOCK_FUNCTION(cycle_counter);
	install_int(cycle_counter, 10);
	game_count = 0;

	swap_screen = create_bitmap(Res_Width, Res_Height);
	if (!swap_screen) {
	    Aborta_Con_Error("failed to create swap screen");
	}

	Reiniciar();

    clear_keybuf();
    int needToDraw = 1;
    cycle_count = 0;

	// BUCLE PRINCIPAL
	while(!key[KEY_ESC]) {
		while(cycle_count > 0) {
			logic_count ++;

            // input
            if (keypressed()) Keyboard();

            // do logic
            Update();

            // yield
            rest(0);

			needToDraw = 1;
			cycle_count --;
		}
		// draw
		if (needToDraw) {
			frame_count ++;

			Render();

			needToDraw = 0;
		}
	}

	allegro_exit();
}
END_OF_MAIN()
