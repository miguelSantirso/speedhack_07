#pragma once

// Declaraciones anticipadas
struct Body;
struct BITMAP;

class TheGrandmother
{
public:
	TheGrandmother();
	~TheGrandmother(void);

	void Render(BITMAP * s);
	void Update();

	Body * Puntero_Box; // Puntero a la caja de colisiones asociada.
protected:
	BITMAP * Graph;

	long t;
	
	int Width;
	int Height;
};
