/*
 * BIT_MATH.h
 *
 * Created: 9/30/2026 7:54:57 PM
 *  Author: S
 */ 


#ifndef BIT_MATH_H_
#define BIT_MATH_H_



#define SET_BIT(REG,BIT)    ((REG) |=  (1U << (BIT)))
#define CLR_BIT(REG,BIT)    ((REG) &= ~(1U << (BIT)))
#define TOG_BIT(REG,BIT)    ((REG) ^=  (1U << (BIT)))
#define GET_BIT(REG,BIT)    (((REG) >> (BIT)) & 1U)





#endif /* BIT_MATH_H_ */