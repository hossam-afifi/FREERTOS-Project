/*
 * key.c
 *
 *  Created on: Dec 19, 2021
 *      Author: user
 */


#include "key.h"

int keypad_rows[] = {R0, R1, R2, R3};
/* The two arrays help to loop easier in get char function */
// Array for keypad columns
int keypad_columns[] = {C0, C1, C2, C3};

// Initialization of the keypad
void keypad_init(void)
{
	// Set rows of keypad to input
	KEYPAD_PORT_DDR_R &= ~((1 << R0) | (1 << R1) | (1 << R2) | (1 << R3));

	// Set columns of keypad to output
	KEYPAD_PORT_DDR_R |= ((1 << C0) | (1 << C1) | (1 << C2) | (1 << C3));

	// Set Port Data Register to 1
	KEYPAD_PORT_DATA_R = 0xFF;
}

// keypad get char function
// loop for rows and loop for columns also
// to know the button pressed in each ( row, column )
char keypad_getchar()
{
	int i, j;
	for (i = 0; i < 4; i++)
	{
		// Set all keypad columns to 1
		KEYPAD_PORT_DATA_R |= ((1 << keypad_columns[0]) | (1 << keypad_columns[1]) | (1 << keypad_columns[2]) | (1 << keypad_columns[3]));

		// Set the current column to 0
		KEYPAD_PORT_DATA_R &= ~ (1 << keypad_columns[i]);

		// inner for loop to know which button is pressed in column
		// row position
		for (j = 0; j < 4; j++)
		{
			// Check if button is pressed in the row of column
			if (!(KEYPAD_PORT_PIN_R & (1 << keypad_rows[j])))
			{
				while(!(KEYPAD_PORT_PIN_R & (1 << keypad_rows[j])));  // Write only 1 char if pressed

				// Switch to get column number
				switch(i)
				{
					case(0):
						if (j == 0)       // Column 0 , Row 0
							return '7';
						else if (j == 1)  // Column 0 , Row 1
							return '4';
						else if (j == 2)  // Column 0 , Row 2
							return '1';
						else if (j == 3)  // Column 0 , Row 3
							return '!';
						break;
					case(1):
						if (j == 0)       // Column 1 , Row 0
							return '8';
						else if (j == 1)  // Column 1 , Row 1
							return '5';
						else if (j == 2)  // Column 1 , Row 2
							return '2';
						else if (j == 3)  // Column 1 , Row 3
							return '0';
						break;
					case(2):
						if (j == 0)       // Column 2 , Row 0
							return '9';
						else if (j == 1)  // Column 2 , Row 1
							return '6';
						else if (j == 2)  // Column 2 , Row 2
							return '3';
						else if (j == 4)  // Column 2 , Row 3
							return '=';
						break;
					case(3):
						if (j == 0)       // Column 3 , Row 0
							return '/';
						else if (j == 1)  // Column 3 , Row 1
							return '*';
						else if (j == 2)  // Column 3 , Row 2
							return '-';
						else if (j == 3)  // Column 3 , Row 3
							return '+';
						break;
				}
			}

		}
	}
	return 'N';
}
