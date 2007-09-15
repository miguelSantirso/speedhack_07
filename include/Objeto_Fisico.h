#pragma once

// Declaraciones anticipadas
struct Body;
struct BITMAP;

#include <string>

// Clase Objeto_Fisico. Esta clase es muy importante ya que ser� la clase de la que podr�n heredar todos los objetos
// que est�n regidos por las normas de la f�sica del juego. Dota de comportamiento f�sico a cualquier objeto que 
// herede de esta clase.
// Proporciona adem�s funciones para la carga/descarga del gr�fico del objeto y para su dibujado en pantalla
class Objeto_Fisico
{
public:
	// Constructores
	Objeto_Fisico(void);
	Objeto_Fisico(std::string Ruta_Fichero, float Peso=10.0f, int Ancho=0, int Alto=0);
	Objeto_Fisico(float Peso, int Ancho, int Alto);

	// Destructor
	~Objeto_Fisico(void);

	// Este m�todo permite que las clases hijas se inicialicen
	void Inicializa(std::string Ruta_Fichero, float Peso=10.0f, int Ancho=0, int Alto=0);

	void Dibuja(BITMAP * Pantalla); // Dibuja el objeto en pantalla

	Body * Puntero_Box; // Puntero a la caja de colisiones asociada.
protected:
	BITMAP * Grafico_Objeto; // Puntero al sprite que representa al objeto en pantalla
	
	int Width;
	int Height;
};
