#pragma once

struct BITMAP;
class Objeto_Fisico;

class Missis_Plow
{
public:
	Missis_Plow(void);
	~Missis_Plow(void);

	void Render(BITMAP *);
	void Update();
	
	Objeto_Fisico *DaPlough;

protected:
	int x;
	int y;

	bool Q_Last;
};
