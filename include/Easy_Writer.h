#pragma once
#include <string>

class Easy_Writer
{
public:
	Easy_Writer(void);
	Easy_Writer(BITMAP * Objective);
	~Easy_Writer(void);

	void Draw_7Segments_Char(char Char, int x, int y, int Size, int Thickness);
	void Write_String(std::string Cadena, int Colour, int x, int y, int Size, bool Reverse = false);
	void Write_Number(int Number, int Colour, int x, int y, int Size);

protected:
	BITMAP * Objective;

};
