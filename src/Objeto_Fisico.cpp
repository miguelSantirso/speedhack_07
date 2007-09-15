#include "Objeto_Fisico.h"

#include <allegro.h>
#include <string>

#include "Body.h"
#include "World.h"

using namespace std;

// Obtener variables globales
extern World world;
extern vector<Body *> bodies;
extern void Aborta_Con_Error(string msg);
extern int X_Camera;
extern int Y_Camera;

// Constructor por defecto (no se usa)
Objeto_Fisico::Objeto_Fisico(void): Puntero_Box(NULL), Grafico_Objeto(NULL)
{
}

// Constructor "�til" de la clase. Recibe:
// Ruta_Fichero	Ruta del gr�fico que se utilizar� para el objeto
// Peso			Peso que se le dar�. Tiene 10.0 como valor por defecto
// Ancho, Alto	Tama�o del gr�fico. Si no se indica nada, se le dar� el ancho y alto del gr�fico.		
Objeto_Fisico::Objeto_Fisico(string Ruta_Fichero, float Peso, int Ancho, int Alto)
{
	Inicializa(Ruta_Fichero, Peso, Ancho, Alto);	// Inicializamos seg�n los par�metros indicados
	Puntero_Box->mass=Peso;					// Le damos el peso indicado
}

Objeto_Fisico::Objeto_Fisico(float Peso, int Ancho, int Alto) : Grafico_Objeto(NULL)
{
	Width = Ancho;
	Height = Alto;
	// Creamos la caja de colisiones asociada
	Puntero_Box = new Body();
	Puntero_Box->Set(Vec2(Ancho, Alto), Peso); // Tama�o
	world.Add(Puntero_Box); // La a�adimos al "mundo"
	bodies.push_back(Puntero_Box); // Y la a�adimos al vector que almacena todos los cuerpos

	Puntero_Box->mass=Peso;					// Le damos el peso indicado
}

// Destructor
Objeto_Fisico::~Objeto_Fisico(void)
{
	// Liberamos la memoria ocupada por el gr�fico cargado.
	if(Grafico_Objeto != NULL)
		destroy_bitmap(Grafico_Objeto);
}

// Carga los recursos adecuados e inicializa las variables del objeto
void Objeto_Fisico::Inicializa(string Ruta_Fichero, float Peso, int Ancho, int Alto)
{
	PALETTE palette;	// Variable auxiliar
	
	// Cargamos en memoria el gr�fico indicado
	Grafico_Objeto=load_bitmap((char *)Ruta_Fichero.c_str(), palette);

	// <revisar> Controlar mejor error en la carga del fichero </revisar>

	if(!Grafico_Objeto)
		Aborta_Con_Error("ERROR:\nError de ejecucion en la funcion Inicializa() de Objeto_Fisico.cpp.\n - Error al cargar el grafico " + Ruta_Fichero);

	// Si no se ha indicado nada, la caja de colisiones tomar� el mismo tama�o que el gr�fico
	if(Ancho==0)
		Ancho=Grafico_Objeto->w;
	if(Alto==0)
		Alto=Grafico_Objeto->h;

	// Creamos la caja de colisiones asociada
	Puntero_Box = new Body();
	Puntero_Box->Set(Vec2(Ancho, Alto), Peso); // Tama�o
	world.Add(Puntero_Box); // La a�adimos al "mundo"
	bodies.push_back(Puntero_Box); // Y la a�adimos al vector que almacena todos los cuerpos
}

// Dibuja el objeto en pantalla
void Objeto_Fisico::Dibuja(BITMAP * Pantalla)
{
	if(Grafico_Objeto != NULL)
	{
		// Dibujamos en la pantalla el objeto.
		rotate_sprite(Pantalla,												// Se dibujar� en el bitmap indicado
					Grafico_Objeto,											// bitmap a dibujar
					Puntero_Box->position.x-Grafico_Objeto->w/2-X_Camera,	// Posici�n
					Puntero_Box->position.y-Grafico_Objeto->h/2-Y_Camera,
					itofix((256*Puntero_Box->rotation)/(2*3.141516)));		// Rotaci�n
	}
	else
	{
		rectfill(Pantalla, 
				Puntero_Box->position.x-Width/2-X_Camera, 
				Puntero_Box->position.y-Height/2-Y_Camera,
				Puntero_Box->position.x+Width/2-X_Camera, 
				Puntero_Box->position.y+Height/2-Y_Camera,
				makecol(50, 50 ,50));
	}
}

