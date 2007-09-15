#pragma once

struct BITMAP;
class Objeto_Fisico;

class Cat_Shooter
{
public:
	Cat_Shooter(void);
	~Cat_Shooter(void);

	void Update();
	void Render(BITMAP * s);

protected:
	Objeto_Fisico * Cat;
	int Power;
	bool Button_Pressed;

	void delete_Cat();
};
