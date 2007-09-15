#include "allegro.h"
#include "Easy_Writer.h"

using namespace std;

Easy_Writer::Easy_Writer()
{
}

Easy_Writer::Easy_Writer(BITMAP * _Objective) : Objective(_Objective)
{
}

Easy_Writer::~Easy_Writer(void)
{
}


void Easy_Writer::Draw_7Segments_Char(char Char, int x, int y, int Size, int Thickness, int Colour) 
{
	bool Active_Segments[7];

	for(int i=0; i<7; i++)
		Active_Segments[i] = false;

   switch(Char)     
   {
        case '1': 
            Active_Segments[5] = true;
            Active_Segments[6] = true;
        break;   
        case '2': 
            Active_Segments[0] = true;
            Active_Segments[1] = true;
            Active_Segments[2] = true;
            Active_Segments[4] = true;
            Active_Segments[5] = true;
        break;
        case '3': 
            Active_Segments[0] = true;
            Active_Segments[1] = true;
            Active_Segments[2] = true;
            Active_Segments[5] = true;
            Active_Segments[6] = true;
        break;
        case '4': 
            Active_Segments[1] = true;
            Active_Segments[3] = true;
            Active_Segments[5] = true;
            Active_Segments[6] = true;
        break;
        case '5': 
            Active_Segments[0] = true;
            Active_Segments[1] = true;
            Active_Segments[2] = true;
            Active_Segments[3] = true;
            Active_Segments[6] = true;
        break;
        case '6': 
            Active_Segments[0] = true;
            Active_Segments[1] = true;
            Active_Segments[2] = true;
            Active_Segments[3] = true;
            Active_Segments[4] = true;
            Active_Segments[6] = true;
        break;
        case '7': 
            Active_Segments[0] = true;
            Active_Segments[5] = true;
            Active_Segments[6] = true;
        break;
        case '8': 
            Active_Segments[0] = true;
            Active_Segments[1] = true;
            Active_Segments[2] = true;
            Active_Segments[3] = true;
            Active_Segments[4] = true;
            Active_Segments[5] = true;
            Active_Segments[6] = true;
        break;
        case '9': 
            Active_Segments[0] = true;
            Active_Segments[1] = true;
            Active_Segments[3] = true;
            Active_Segments[5] = true;
            Active_Segments[6] = true;
        break;   
        case '0': 
            Active_Segments[0] = true;
            Active_Segments[2] = true;
            Active_Segments[3] = true;
            Active_Segments[4] = true;
            Active_Segments[5] = true;
            Active_Segments[6] = true;
        break;
        case 'A': 
            Active_Segments[0] = true;
            Active_Segments[1] = true;
            Active_Segments[3] = true;
            Active_Segments[4] = true;
            Active_Segments[5] = true;
            Active_Segments[6] = true;
        break;  
        case 'B': 
            Active_Segments[1] = true;
            Active_Segments[2] = true;
            Active_Segments[3] = true;
            Active_Segments[4] = true;
            Active_Segments[6] = true;
        break;  
        case 'C': 
            Active_Segments[0] = true;
            Active_Segments[2] = true;
            Active_Segments[3] = true;
            Active_Segments[4] = true;
        break;   
        case 'D': 
            Active_Segments[1] = true;
            Active_Segments[2] = true;
            Active_Segments[5] = true;
            Active_Segments[4] = true;
            Active_Segments[6] = true;
        break; 
        case 'E': 
            Active_Segments[0] = true;
            Active_Segments[1] = true;
            Active_Segments[2] = true;
            Active_Segments[3] = true;
            Active_Segments[4] = true;
        break; 
        case 'F': 
            Active_Segments[0] = true;
            Active_Segments[1] = true;
            Active_Segments[3] = true;
            Active_Segments[4] = true;
        break;
        case 'G': 
            Active_Segments[0] = true;
            Active_Segments[1] = true;
            Active_Segments[2] = true;
            Active_Segments[3] = true;
            Active_Segments[5] = true;
            Active_Segments[6] = true;
        break; 
        case 'H': 
            Active_Segments[1] = true;
            Active_Segments[3] = true;
            Active_Segments[4] = true;
            Active_Segments[5] = true;
            Active_Segments[6] = true;
        break;        
        case 'I': 
            Active_Segments[5] = true;
            Active_Segments[6] = true;
        break;  
        case 'J': 
            Active_Segments[2] = true;
            Active_Segments[4] = true;
            Active_Segments[5] = true;
            Active_Segments[6] = true;
        break;   
        case 'K': 
            Active_Segments[1] = true;
            Active_Segments[3] = true;
            Active_Segments[4] = true;
            Active_Segments[5] = true;
            Active_Segments[6] = true;
        break; 
        case 'L': 
            Active_Segments[2] = true;
            Active_Segments[3] = true;
            Active_Segments[4] = true;
        break;   
        case 'M': 
            Active_Segments[0] = true;
            Active_Segments[3] = true;
            Active_Segments[4] = true;
            Active_Segments[5] = true;
            Active_Segments[6] = true;
        break;
        case 'N': 
            Active_Segments[1] = true;
            Active_Segments[4] = true;
            Active_Segments[6] = true;
        break;
        case 'O': 
            Active_Segments[0] = true;
            Active_Segments[2] = true;
            Active_Segments[3] = true;
            Active_Segments[4] = true;
            Active_Segments[5] = true;
            Active_Segments[6] = true;
        break;    
        case 'P': 
            Active_Segments[0] = true;
            Active_Segments[1] = true;
            Active_Segments[3] = true;
            Active_Segments[4] = true;
            Active_Segments[5] = true;
        break;  
        case 'Q': 
            Active_Segments[0] = true;
            Active_Segments[2] = true;
            Active_Segments[3] = true;
            Active_Segments[4] = true;
            Active_Segments[5] = true;
            Active_Segments[6] = true;
        break;   
        case 'R': 
            Active_Segments[1] = true;
            Active_Segments[4] = true;
        break; 
        case 'S': 
            Active_Segments[0] = true;
            Active_Segments[1] = true;
            Active_Segments[2] = true;
            Active_Segments[3] = true;
            Active_Segments[6] = true;
        break;     
        case 'T': 
            Active_Segments[1] = true;
            Active_Segments[2] = true;
            Active_Segments[3] = true;
            Active_Segments[4] = true;
        break;
        case 'U': 
            Active_Segments[2] = true;
            Active_Segments[3] = true;
            Active_Segments[4] = true;
            Active_Segments[5] = true;
            Active_Segments[6] = true;
        break;        
        case 'V': 

            Active_Segments[2] = true;
            Active_Segments[4] = true;
            Active_Segments[6] = true;
        break; 
        case 'Y': 
            Active_Segments[1] = true;
            Active_Segments[3] = true;
            Active_Segments[5] = true;
            Active_Segments[6] = true;
        break;
		default:
			
		break;
	}

	int Y_Middle = y;
    int Y_Up = Y_Middle;
    int Y_Down = Y_Middle;
    
	int Actual_Size = Size;

    Y_Up -= Actual_Size/2;
    Y_Down += Actual_Size/2;
    
    int X_Left = x;
    int X_Right = X_Left + Actual_Size/2;

	// Segmentos horizontales  
    if(Active_Segments[0])
	{
		rectfill(Objective, X_Left, Y_Up - Thickness/2, X_Right , Y_Up + Thickness/2, Colour);
    }  
    if(Active_Segments[1])
	{
		rectfill(Objective, X_Left, Y_Middle - Thickness/2, X_Right , Y_Middle + Thickness/2, Colour);
    }                                                                                                         
    if(Active_Segments[2])
	{
		rectfill(Objective, X_Left, Y_Down - Thickness/2, X_Right , Y_Down + Thickness/2, Colour);
    }
    
    // Segmentos verticales    
    if(Active_Segments[3])
	{
		rectfill(Objective, X_Left - Thickness/2, Y_Up, X_Left + Thickness/2, Y_Middle, Colour);
    }      
    if(Active_Segments[4])
	{
		rectfill(Objective, X_Left - Thickness/2, Y_Middle, X_Left + Thickness/2, Y_Down, Colour);
    } 
    if(Active_Segments[5])
	{
		rectfill(Objective, X_Right - Thickness/2, Y_Up, X_Right + Thickness/2, Y_Middle, Colour);
    } 
    if(Active_Segments[6])
	{
		rectfill(Objective, X_Right - Thickness/2, Y_Middle, X_Right + Thickness/2, Y_Down, Colour);
    }

}

void Easy_Writer::Write_String(string Cadena, int Colour, int x, int y, int Size, bool Reverse)
{
	int X_Char;
	int j = (Reverse ? Cadena.size()-1 : 0);
	for(int i=0; i<Cadena.size(); i++)
	{
		X_Char = x+(Size+Size/2)/2*(j-Cadena.size()/2);
		Draw_7Segments_Char(Cadena[i], X_Char, y, Size, Size/20, Colour);
		j+=(Reverse ? -1 : 1);
	}
}

void Easy_Writer::Write_Number(int Number, int Colour, int x, int y, int Size)
{
	string Number_String;
	int Aux;
	do
	{
		Aux = Number%10;
		Number/=10;
		Number_String.push_back('0' + Aux);
	}while(Number != 0);
	Write_String(Number_String, Colour, x, y, Size, true);
}
