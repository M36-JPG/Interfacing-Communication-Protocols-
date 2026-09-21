/*
 * BIT_MATH.h
 *
 * Created: 9/20/2026 12:27:11 PM
 *  Author: Sama.Adel
 */ 


#ifndef BIT_MATH_H_
#define BIT_MATH_H_


#define SET_BIT(VAR,BIT)      (VAR |= (1 << BIT))
#define CLR_BIT(VAR,BIT)      (VAR &= ~(1 << BIT))
#define TOGGLE_BIT(VAR,BIT)   (VAR ^= (1 << BIT))
#define GET_BIT(VAR,BIT)      ((VAR >> BIT) & 1)



#endif /* BIT_MATH_H_ */