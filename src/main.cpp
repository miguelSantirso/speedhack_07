//#define FULLSCREEN

#define TITLE_LENGTH int(5000)
#define TITLETRANSITION_LENGTH int(900)

#include <allegro.h>
#include <string>

#include "Easy_Writer.h"
#include "World.h"
#include "Body.h"
#include "Joint.h"
#include "Objeto_Fisico.h"

using namespace std;

Easy_Writer * Writer;

bool Debug = true;			// Indica si se está en modo debug (se dibujan los bodies y los joints)

World world(Vec2(0.0f, 20.0f), 10);		// Inicializa el sistema de físicas

vector<Body *> bodies;	// Vector de cuerpos físicos (las cajas de colisiones)
vector<Joint *> joints;	// Vector de enlaces (enlaces entre cuerpos físicos)
vector<Objeto_Fisico *> Objetos_Fisicos;	// Almacena los punteros a todos los objetos que se rigen por las físicas.


// Global Game Variables
int Seconds_Remaining;

// Camera Coordinates
int X_Camera;
int Y_Camera;

// Especial Objects


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

// Cargador de la partida
void Iniciar_Partida()
{
	// INICIALIZAR TODO
	Writer = new Easy_Writer(swap_screen);

	Seconds_Remaining = 120;
	X_Camera = Y_Camera = 0;

   // Carga y creación del sprite de prueba
	Objeto_Fisico *Techo = new Objeto_Fisico("media\\nave.pcx", FLT_MAX);
	Techo->Puntero_Box->Set(Vec2(35, 25), FLT_MAX);
	Techo->Puntero_Box->friction=500.0f;
	Techo->Puntero_Box->position.Set(Res_Width/2, Res_Height/2);
	Objetos_Fisicos.push_back(Techo);
}

void Reiniciar()
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

	delete Writer;
	Writer = NULL;

	world.Clear();
	
	// Reiniciamos la partida
	Iniciar_Partida();
}

void Scroll_Controls()
{
/*	X_Camera = Cosa_Movil->Puntero_Box->position.x - Res_Width/2;
	Y_Camera = Cosa_Movil->Puntero_Box->position.y - Res_Height/2;*/
	if(key[KEY_LEFT])
		X_Camera -=5;
	if(key[KEY_RIGHT])
		X_Camera +=5;
	if(key[KEY_DOWN])
		Y_Camera +=5;
	if(key[KEY_UP])
		Y_Camera -=5;
}

// Función que se ocupa de actualizar la lógica y la física del juego
void Update()
{
	// ACTUALIZAR TODA LA LÓGICA
	world.Step(0.05);	// Actualizar las físicas

	Scroll_Controls();
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

// Realiza todas las tareas de dibujo.
void Render()
{
	clear_to_color(swap_screen, makecol(245,245,255));

	// DIBUJAR TODO
	//Write_Time();
	Write_Title();

	// Dibujar objetos físicos
    for(int i = 0; i < (int)Objetos_Fisicos.size(); i ++) {
		Objetos_Fisicos[i]->Dibuja(swap_screen);
    }

    if (Debug)
	{
		// Dibujar cajas de colisiones
		for(int i = 0; i < (int)bodies.size(); i ++) {
			DrawBody(swap_screen, bodies[i], 0, 0);
		}
		// Dibujar uniones
        for(int i = 0; i < (int)joints.size(); i ++) {
            DrawJoint(swap_screen, joints[i], 0, 0);
        }
	}

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
/*
	if (kp == KEY_ESC && !Bienvenida) {Bienvenida = true; Muerto = false; Pausa = false; Inicializar(9); }
	if (kp == KEY_TAB) Debug = !Debug;
	if (kp == KEY_P) Pausa = !Pausa;
	if (kp == KEY_R) { Inicializar(9); Muerto = false; Pausa = false; Bienvenida = false; }
	if (kp == KEY_SPACE) Puntero_Agua->Modifica_Amplitud(50);*/
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
