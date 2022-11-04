#include <3ds.h>

u8 character [128][8][8] = 
	{
		{	// space 32
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{	// exclamation mark 33
			{0,0,0,1,1,0,0,0},
			{0,0,0,1,1,0,0,0},
			{0,0,0,1,1,0,0,0},
			{0,0,0,1,1,0,0,0},
			{0,0,0,1,1,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,1,1,0,0,0},
			{0,0,0,1,1,0,0,0}
		},
		{	//double quote 34
			{0,0,1,0,0,1,0,0},
			{0,0,1,0,0,1,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{	// hash 35
			{0,0,0,1,0,0,1,0},
			{0,0,0,1,0,0,1,0},
			{0,1,1,1,1,1,1,1},
			{0,0,0,0,0,1,0,0},
			{0,0,0,0,0,1,0,0},
			{1,1,1,1,1,1,1,0},
			{0,1,0,0,1,0,0,0},
			{0,1,0,0,1,0,0,0}
		},
		{	// big dot (was dollar) 36
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,1,1,0,0,0},
			{0,0,1,1,1,1,0,0},
			{0,0,1,1,1,1,0,0},
			{0,0,0,1,1,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{	// percent 37
			{0,1,1,0,0,0,1,1},
			{1,0,0,1,0,1,1,0},
			{1,0,0,1,1,1,0,0},
			{0,1,1,1,1,0,0,0},
			{0,0,0,1,1,1,1,0},
			{0,0,1,1,1,0,0,1},
			{0,1,1,0,1,0,0,1},
			{1,1,0,0,0,1,1,0}
		},
		{	// ampersand 38
			{0,0,0,0,0,0,0,0},
			{0,0,1,1,1,0,0,0},
			{0,1,0,0,1,0,0,0},
			{0,1,0,1,0,0,0,0},
			{0,0,1,0,1,1,0,0},
			{0,1,0,1,0,0,0,1},
			{0,1,1,1,1,0,0,1},
			{0,0,0,0,0,1,1,0}
		},
		{	// quote 39
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{	// brace left 40
			{0,0,0,1,1,0,0,0},
			{0,0,1,0,0,0,0,0},
			{0,1,0,0,0,0,0,0},
			{0,1,0,0,0,0,0,0},
			{0,1,0,0,0,0,0,0},
			{0,1,0,0,0,0,0,0},
			{0,0,1,0,0,0,0,0},
			{0,0,0,1,1,0,0,0}
		},
		{	// brace right 41
			{0,0,0,1,1,0,0,0},
			{0,0,0,0,0,1,0,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,1,0,0},
			{0,0,0,1,1,0,0,0}
		},
		{	// asterisk 42
			{0,0,0,0,0,0,0,0},
			{0,1,1,0,1,1,0,0},
			{0,0,1,1,1,0,0,0},
			{1,1,1,1,1,1,1,0},
			{0,0,1,1,1,0,0,0},
			{0,1,1,0,1,1,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{	// plus 43
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,1,1,1,1,1,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{	// comma 44
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,1,1,0,0,0,0,0},
			{0,1,1,0,0,0,0,0},
			{0,1,0,0,0,0,0,0}
		},
		{	// minus 45
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,1,1,1,1,1,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{	// dot 46
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,1,1,0,0,0},
			{0,0,0,1,1,0,0,0}
		},
		{	// slash 47
			{0,0,0,0,0,0,0,1},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,1,0,0},
			{0,0,0,0,1,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,1,0,0,0,0,0},
			{0,1,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0}
		},
		{	// zero 48
			{0,0,1,1,1,1,0,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,1,1,0,1,0},
			{0,1,0,1,1,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,0,1,1,1,1,0,0}
		},
		{	// one 49
			{0,0,0,1,1,0,0,0},
			{0,0,0,1,1,0,0,0},
			{0,0,0,0,1,0,0,0},
			{0,0,0,0,1,0,0,0},
			{0,0,0,0,1,0,0,0},
			{0,0,0,0,1,0,0,0},
			{0,0,0,0,1,0,0,0},
			{0,0,0,0,1,0,0,0}
		},
		{	// two 50
			{0,1,1,1,1,1,0,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,1,1,1,1,1,0,0},
			{0,1,0,0,0,0,0,0},
			{0,1,0,0,0,0,0,0},
			{0,1,1,1,1,1,1,0}
		},
		{	// three 51
			{0,0,1,1,1,1,0,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,1,1,1,0,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,1,1,1,1,1,0,0}
		},
		{	// four 52
			{0,0,1,0,0,0,1,0},
			{0,0,1,0,0,0,1,0},
			{0,0,1,0,0,0,1,0},
			{0,0,1,0,0,0,1,0},
			{0,0,0,1,1,1,1,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0}
		},
		{	// five 53
			{0,1,1,1,1,1,0,0},
			{0,1,0,0,0,0,0,0},
			{0,1,0,0,0,0,0,0},
			{0,1,1,1,1,1,1,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,1,1,1,1,1,0,0}
		},
		{	// six 54
			{0,0,1,1,1,1,0,0},
			{0,1,0,0,0,0,0,0},
			{0,1,0,0,0,0,0,0},
			{0,1,1,1,1,1,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,0,1,1,1,1,0,0}
		},
		{	// seven 55
			{0,1,1,1,1,0,0,0},
			{0,0,0,0,0,1,0,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0}
		},
		{	// eight 56
			{0,0,1,1,1,1,0,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,0,1,1,1,1,0,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,0,1,1,1,1,0,0}
		},
		{	// nine 57
			{0,0,1,1,1,1,0,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,0,1,1,1,1,1,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,0,1,1,1,1,0,0}
		},
		{	// colon 58
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,1,1,0,0,0,0,0},
			{0,1,1,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,1,1,0,0,0,0,0},
			{0,1,1,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{	// semicolon 59
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,1,1,0,0,0,0,0},
			{0,1,1,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,1,1,0,0,0,0,0},
			{0,1,1,0,0,0,0,0},
			{0,1,0,0,0,0,0,0}
		},
		{	// less 60
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,1,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,1,0,0,0,0,0},
			{0,1,0,0,0,0,0,0},
			{0,0,1,0,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,0,1,0,0,0}
		},
		{	// equal 61
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,1,1,1,1,1,1,0},
			{0,0,0,0,0,0,0,0},
			{0,1,1,1,1,1,1,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{	// greater 62
			{0,0,0,0,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,0,1,0,0,0},
			{0,0,0,0,0,1,0,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,1,0,0},
			{0,0,0,0,1,0,0,0},
			{0,0,0,1,0,0,0,0}
		},
		{	// question mark 63
			{0,1,1,1,1,1,0,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,0,1,1,1,1,0,0},
			{0,1,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,1,0,0,0,0,0,0}
		},
		{	// infinite (was AT) 64
			{0,0,0,0,0,0,0,0},
			{0,1,1,0,1,1,0,0},
			{1,0,0,1,0,0,1,0},
			{1,0,0,1,0,0,1,0},
			{1,0,0,1,0,0,1,0},
			{0,1,1,0,1,1,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{	// A 65
			{0,0,1,1,1,1,0,0},
			{0,1,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,1,1,1,1,1,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0}
		},
		{	// B 66
			{0,1,1,1,1,1,0,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,1,1,1,1,1,0,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,1,1,1,1,0,0}
		},
		{	// C 67
			{0,0,0,1,1,1,1,0},
			{0,0,1,0,0,0,0,0},
			{0,1,0,0,0,0,0,0},
			{0,1,0,0,0,0,0,0},
			{0,1,0,0,0,0,0,0},
			{0,1,0,0,0,0,0,0},
			{0,0,1,0,0,0,0,0},
			{0,0,0,1,1,1,1,0}
		},
		{	// D 68
			{0,1,1,1,1,0,0,0},
			{0,1,0,0,0,1,0,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,1,0,0},
			{0,1,0,1,1,0,0,0}
		},
		{	// E 69
			{0,0,1,1,1,1,1,0},
			{0,1,0,0,0,0,0,0},
			{0,1,0,0,0,0,0,0},
			{0,1,1,1,1,1,1,0},
			{0,1,0,0,0,0,0,0},
			{0,1,0,0,0,0,0,0},
			{0,1,0,0,0,0,0,0},
			{0,0,1,1,1,1,1,0}
		},
		{	// F 70
			{0,0,1,1,1,1,1,0},
			{0,1,0,0,0,0,0,0},
			{0,1,0,0,0,0,0,0},
			{0,1,1,1,1,1,1,0},
			{0,1,0,0,0,0,0,0},
			{0,1,0,0,0,0,0,0},
			{0,1,0,0,0,0,0,0},
			{0,1,0,0,0,0,0,0}
		},
		{	// G 71
			{0,0,1,1,1,1,1,0},
			{0,1,0,0,0,0,0,0},
			{0,1,0,0,0,0,0,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,0,1,1,1,1,0,0}
		},
		{	// H 72
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,1,1,1,1,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0}
		},
		{	// I 73
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0}
		},
		{	// J 74
			{0,0,0,0,0,1,0,0},
			{0,0,0,0,0,1,0,0},
			{0,0,0,0,0,1,0,0},
			{0,0,0,0,0,1,0,0},
			{0,0,0,0,0,1,0,0},
			{0,0,0,0,0,1,0,0},
			{0,0,0,0,0,1,0,0},
			{0,1,1,1,1,0,0,0}
		},
		{	// K 75
			{0,1,0,0,0,0,0,0},
			{0,1,0,0,0,1,1,0},
			{0,1,0,0,1,0,0,0},
			{0,1,0,0,1,0,0,0},
			{0,1,1,1,1,1,0,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0}
		},
		{	// L 76
			{0,0,1,0,0,0,0,0},
			{0,0,1,0,0,0,0,0},
			{0,0,1,0,0,0,0,0},
			{0,0,1,0,0,0,0,0},
			{0,0,1,0,0,0,0,0},
			{0,0,1,0,0,0,0,0},
			{0,0,1,0,0,0,0,0},
			{0,0,0,1,1,1,1,0}
		},
		{	// M 77
			{0,0,1,1,1,1,0,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,1,0,0,1,0},
			{0,1,0,1,0,0,1,0},
			{0,1,0,1,0,0,1,0},
			{0,1,0,1,0,0,1,0},
			{0,1,0,1,0,0,1,0},
			{0,1,0,1,0,0,1,0}
		},
		{	// N 78
			{0,0,1,1,1,0,0,0},
			{0,1,0,0,0,1,0,0},
			{0,1,0,0,0,1,0,0},
			{0,1,0,0,0,1,0,0},
			{0,1,0,0,0,1,0,0},
			{0,1,0,0,0,1,0,0},
			{0,1,0,0,0,1,0,0},
			{0,1,0,0,0,1,0,0}
		},
		{	// O 79
			{0,0,1,1,1,1,0,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,0,1,1,1,1,0,0}
		},
		{	// P 80
			{0,1,1,1,1,1,0,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,1,1,1,1,1,0,0},
			{0,1,0,0,0,0,0,0},
			{0,1,0,0,0,0,0,0},
			{0,1,0,0,0,0,0,0}
		},
		{	// Q 81
			{0,0,1,1,1,1,0,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,1,0,1,0},
			{0,1,0,0,1,0,1,0},
			{0,1,0,0,1,0,1,0},
			{0,0,1,0,0,1,0,0}
		},
		{	// R 82
			{0,1,1,1,1,1,0,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,1,1,1,1,1,0,0},
			{0,1,0,0,1,0,0,0},
			{0,1,0,0,1,0,0,0},
			{0,1,0,0,0,1,0,0}
		},
		{	// S 83
			{0,0,1,1,1,1,1,0},
			{0,1,0,0,0,0,0,0},
			{0,1,0,0,0,0,0,0},
			{0,0,1,1,1,1,0,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,1,1,1,1,1,0,0}
		},
		{	// T 84
			{0,1,1,1,1,1,1,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0}
		},
		{	// U 85
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,0,1,1,1,1,0,0}
		},
		{	// V 86
			{0,1,0,0,0,1,0,0},
			{0,1,0,0,0,1,0,0},
			{0,1,0,0,0,1,0,0},
			{0,1,0,0,0,1,0,0},
			{0,0,1,0,1,0,0,0},
			{0,0,1,0,1,0,0,0},
			{0,0,1,0,1,0,0,0},
			{0,0,0,1,0,0,0,0}
		},
		{	// W 87
			{0,1,0,1,0,0,1,0},
			{0,1,0,1,0,0,1,0},
			{0,1,0,1,0,0,1,0},
			{0,1,0,1,0,0,1,0},
			{0,1,0,1,0,0,1,0},
			{0,1,0,1,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,0,1,1,1,1,0,0}
		},
		{	// X 88
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,0,1,0,0,1,0,0},
			{0,0,0,1,1,0,0,0},
			{0,0,1,0,0,1,0,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0}
		},
		{	// Y 89
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,0,1,1,1,1,1,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,1,0},
			{0,1,0,0,0,0,1,0},
			{0,0,1,1,1,1,0,0}
		},
		{	// Z 90
			{0,1,1,1,1,1,1,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,1,0,0},
			{0,0,0,0,1,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,1,0,0,0,0,0},
			{0,1,0,0,0,0,0,0},
			{0,1,1,1,1,1,1,0}
		},
		{	// square brace left 91
			{0,0,0,1,1,1,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,1,1,0,0}
		},
		{	// back slash 92
			{1,0,0,0,0,0,0,0},
			{0,1,0,0,0,0,0,0},
			{0,0,1,0,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,0,1,0,0,0},
			{0,0,0,0,0,1,0,0},
			{0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,0,1}
		},
		{	// square brace right 93
			{0,0,0,1,1,1,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,1,1,0,0}
		},
		{	// accent 94
			{0,0,0,1,0,0,0,0},
			{0,0,1,0,1,0,0,0},
			{0,1,0,0,0,1,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{	// underscore 95
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{1,1,1,1,1,1,1,1}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{	// THETA (was a) 97
			{0,0,0,0,0,0,0,0},
			{0,0,0,1,1,0,0,0},
			{0,1,1,0,0,1,1,0},
			{1,0,0,0,0,0,0,1},
			{1,0,1,1,1,1,0,1},
			{1,0,0,0,0,0,0,1},
			{0,1,1,0,0,1,1,0},
			{0,0,0,1,1,0,0,0}
		},
		{	// PHI (was b) 98
			{0,0,1,1,1,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,1,1,1,1,1,0,0},
			{1,0,0,1,0,0,1,0},
			{1,0,0,1,0,0,1,0},
			{0,1,1,1,1,1,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,1,1,1,0,0,0}
		},
		{	// Starbase (was c) 99
			{0,0,0,0,0,0,0,0},
			{0,1,0,1,0,1,0,0},
			{0,0,1,1,1,0,0,0},
			{0,1,1,1,1,1,0,0},
			{0,0,1,1,1,0,0,0},
			{0,1,0,1,0,1,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{	// 4-zylon sector (was d) 100
			{0,0,0,0,0,0,0,0},
			{0,0,1,1,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,1,1,0,1,1,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,1,1,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{	// 3-zylon sector (was e) 101
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,1,1,1,0},
			{0,0,0,0,0,0,0,0},
			{0,1,1,1,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,1,1,1,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{	// 2-zylon sector (was f) 102
			{0,0,0,0,0,0,0,0},
			{0,1,1,0,0,0,0,0},
			{0,0,1,1,0,0,0,0},
			{0,1,1,1,1,1,0,0},
			{0,0,1,1,0,0,0,0},
			{0,1,1,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{	// infinite (was g) 101
			{0,0,0,0,0,0,0,0},
			{0,1,1,0,1,1,0,0},
			{1,0,0,1,0,0,1,0},
			{1,0,0,1,0,0,1,0},
			{1,0,0,1,0,0,1,0},
			{0,1,1,0,1,1,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{	// hyperwarp marker lu (was h) 102
			{0,0,0,0,0,0,0,1},
			{0,0,0,0,0,0,0,1},
			{0,0,0,0,0,0,0,1},
			{0,0,0,0,0,0,0,1},
			{0,0,0,0,0,0,0,1},
			{0,0,0,0,0,0,0,1},
			{0,0,0,0,0,0,0,1},
			{1,1,1,1,1,1,1,1}
		},
		{	// hyperwarp marker ru (was i) 103
			{1,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0},
			{1,1,1,1,1,1,1,1}
		},
		{	// hyperwarp marker ld (was j) 104
			{1,1,1,1,1,1,1,1},
			{0,0,0,0,0,0,0,1},
			{0,0,0,0,0,0,0,1},
			{0,0,0,0,0,0,0,1},
			{0,0,0,0,0,0,0,1},
			{0,0,0,0,0,0,0,1},
			{0,0,0,0,0,0,0,1},
			{0,0,0,0,0,0,0,1}
		},
		{	// hyperwarp marker rd (was k) 105
			{1,1,1,1,1,1,1,1},
			{1,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0}
		},
		{	// big dot (was l) 106
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,1,1,0,0,0},
			{0,0,1,1,1,1,0,0},
			{0,0,1,1,1,1,0,0},
			{0,0,0,1,1,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{	// ship (was m) 107
			{0,0,0,1,0,0,0,0},
			{0,0,0,1,0,0,0,0},
			{0,0,1,1,1,0,0,0},
			{0,0,1,1,1,0,0,0},
			{1,1,1,1,1,1,1,0},
			{1,1,1,1,1,1,1,0},
			{1,0,0,1,0,0,1,0},
			{1,0,0,1,0,0,1,0}
		},
		{	// target marker (was n) 108
			{1,1,0,0,0,0,1,1},
			{1,1,0,0,0,0,1,1},
			{1,1,0,1,1,0,1,1},
			{1,1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1,1},
			{1,1,0,1,1,0,1,1},
			{1,1,0,0,0,0,1,1},
			{1,1,0,0,0,0,1,1}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		},
	
};

u8 asteroid_meta[4] = {24, 20, 16, 6};

u8 asteroid_shape [4][24][24] =
{
		{
			{0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
			{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
			{0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
			{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
			{0,0,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
			{0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
			{0,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
			{0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,0,0},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0},
			{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
			{0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
			{0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
			{0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
			{0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
			{0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
			{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
			{0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
			{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
			{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
			{0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
			{0,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
			{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0},
			{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
			{0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
			{0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
			{0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
			{0,0,0,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
			{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
			{0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
			{0,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
			{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
			{1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
			{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
			{0,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
			{0,0,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
			{0,0,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		},
		{
			{0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		}	
};