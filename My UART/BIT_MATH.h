/*
 * BIT_MATH.h
 *
 * Created: 9/19/2026 7:46:15 PM
 *  Author: user
 */ 


#ifndef BIT_MATH_H_
#define BIT_MATH_H_

#define SET_BIT(var,BIT) (var|=(1<<BIT))
#define CLR_BIT(var,BIT) (var&=(~(1<<BIT)))
#define TOGGLE_BIT(var,BIT) (var^=(1<<BIT))
#define  GET_BIT(VAR,BIT)  ((VAR>>BIT)&1)





#endif /* BIT_MATH_H_ */