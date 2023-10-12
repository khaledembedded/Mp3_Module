/*
 * main.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: lenovo 1
 */

/*
 * main.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: lenovo 1
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#undef F_CPU
#define F_CPU 1000000
#include <util/delay.h>
#include "Std_types.h"
#include "Bit_math.h"
#include "Mp3.h"

int main(void)
{


Mp3_Init();
Mp3_SetVolume(30);
Mp3_PlayTrack(2);
_delay_ms(5000);
Mp3_PlayTrack(1);



while(1)
{

}

return 0;
}


