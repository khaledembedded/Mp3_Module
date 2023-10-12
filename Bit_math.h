/*
 * Bit_math.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: lenovo 1
 */

#ifndef BIT_MATH_H_
#define BIT_MATH_H_

#define SET_BIT(reg,index)   (reg |=(1<<index))
#define CLR_BIT(reg,index)   (reg &=~(1<<index))
#define TOG_BIT(reg,index)   (reg^=(1<<index))
#define GET_BIT(reg,index)   ((reg>>index) & 0x01)


#define SET_HIGHER_NIBBLE     (reg) (reg=reg | 0xf0)
#define SET_LOWER_NIBBLE      (reg) (reg=reg | 0x0f)
#define CLR_LOWER_NIBBLE      (reg) (reg=reg & 0xf0)
#define CLR_HIGHER_NIBBLE     (reg) (reg=reg & 0x0f)


#define GET_HIGHER (reg)       (reg>>4)
#define GET_LOWER (reg)        (reg & 0x0f) //clear high nibble
#define PUT_VAL_HIGH (reg,val) (reg=(reg&0x0f) |(val<<4))
#define PUT_VAL_LOW (reg,val)  (reg=(reg&0xf0)|(val & 0x0f))


#define SET_REG (reg)          (reg|=0xff)
#define CLR_REG (reg)          (reg&=0x00)


#endif /* BIT_MATH_H_ */
