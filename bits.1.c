/* 
 * CS:APP Data Lab 
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#include "btest.h"
#include <limits.h>

/*
 * Instructions to Students:
 *
 * STEP 1: Fill in the following struct with your identifying info.
 */
team_struct team =
{
   /* Team name: Replace with either:
      maka1279@colorado.edu */
    "maka1279", 
   /* Student name 1: Replace with the full name of first team member */
   "Matthew Kaiser",
   /* Login ID 1: Replace with the login ID of first team member */
   "***",

   /* The following should only be changed if there are two team members */
   /* Student name 2: Full name of the second team member */
   "",
   /* Login ID 2: Login ID of the second team member */
   ""
};

#if 0
/*
 * STEP 2: Read the following instructions carefully.
 

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.
#endif

/*
 * STEP 3: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the btest test harness to check that your solutions produce 
 *      the correct answers. Watch out for corner cases around Tmin and Tmax.
 */
/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses ISO/IEC 10646 (2nd ed., published 2011-03-15) /
   Unicode 6.0.  */
/* We do not support C11 <threads.h>.  */
//
// 15 problems, 40 points
// 2 - rating 1
// 5 - rating 2
// 4 - rating 3
// 4 - rating 4
// rating 1
/* 
 * bitNor - ~(x|y) using only ~ and & 
 *   Example: bitNor(0x6, 0x5) = 0xFFFFFFF8
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitNor(int x, int y) {
  //used DeMorgan's law on ~(x|y) 
  return (~x & ~y);
}
/* 
 * TMax - return maximum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmax(void) {
  // Constant representing binary value: 1000
	int x = 0x8;

	// The compliment of the binary value 1000 0000 shifted 28 bits to the left produces the following binary value:
	// 0111 1111 1111 1111 1111 1111 1111 1111
	// This is the largest 32-bit two's compliment integer
	return(~(x << 28));
}
// rating 2
/* 
 * fitsBits - return 1 if x can be represented as an  
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *  Rating: 2
 */
int fitsBits(int x, int n) {
 int shifted = (~x)>>(n+~0);        //set x to 1 and 0
 int sign = (~x)>>31;
 return !(shifted ^ sign);

}
/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
    //Need to account for x >> n if positive and x >> n + 1 if negative
	// Subtract 1 from 2^n, accounting for the need to + 1
    
    int cover = (1 << n) + ~0;
    // Use & operator on cover and sign bit of x 
    int equalizer = (x >> 31) & cover;
    //adds 1 if x was originally negative, 0 if x was originally positive
    return (x + equalizer) >> n;

}
/* 
 * isNotEqual - return 0 if x == y, and 1 otherwise 
 *   Examples: isNotEqual(5,5) = 0, isNotEqual(4,5) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int isNotEqual(int x, int y) {
  // XOR operation (^) can be used for equality evaluation, while the double negation operation simplifies the result into a boolean value
	return(!!(x ^ y));
}
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 2
 */
int bitXor(int x, int y) {
  //NAND(negative and) Equivelent of XOR 
  return ~(~(x & ~y) & ~(y & ~x));

}
/* 
 * copyLSB - set all bits of result to least significant bit of x
 *   Example: copyLSB(5) = 0xFFFFFFFF, copyLSB(6) = 0x00000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int copyLSB(int x) {
  // x is first shifted left 31 bits to remove all but least significant bit.
	// x is then arithmetically shifted right 31 bits to copy the least significant bit to all positions.
	return ((x << 31) >> 31);
}
// rating 3
/* 
 * reverseBytes - reverse the bytes of x
 *   Example: reverseBytes(0x01020304) = 0x04030201
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 25
 *   Rating: 3
 */
int reverseBytes(int x) {
  int newbyte0 = (x >> 24) & 0xff;
        int newbyte1 = (x >> 8) & 0xff00;
        int newbyte2 = (x << 8) & 0xff0000;
        int newbyte3 = x << 24;

        return newbyte0 | newbyte1 | newbyte2 | newbyte3;
        
}
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 1 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
  // (x >> n) computes the arithmetic right shift of x by n.
	// ((1 << ((~n + 1) + 32)) + ~0) computes a dynamic mask that allows for arithmetically shifted bits to be converted to logical when necessary.
	return ((x >> n) & ((1 << ((~n + 1) + 32)) + ~0));

}
/* 
 * isGreater - if x > y  then return 1, else return 0 
 *   Example: isGreater(4,5) = 0, isGreater(5,4) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isGreater(int x, int y) {
  // Boolean value indicating sign of x and y
  // 1 = Negative
  // 0 = Non-Negative                 >>31 b/c we are working in 32 bit
  int sign_x = x >> 31;
  int sign_y = y >> 31;
  //if the signs are equal, sign bit of (~y +x) is 0 if x is larger, 1 if y is larger
  int equal = !(sign_x ^ sign_y) & ((~y + x) >> 31);
  
	// if signs are not equal, the principles are reversed.
	int notEqual = sign_x & !sign_y; 
	//this or(|) returns 0 when x is greater, so it must be negated 
	return !( equal | notEqual);
}
/* 
 * bitMask - Generate a mask consisting of all 1's 
 *   lowbit and highbit
 *   Examples: bitMask(5,3) = 0x38
 *   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
 *   If lowbit > highbit, then mask should be all 0's
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int bitMask(int highbit, int lowbit) {
  //mask1 masks the bits above highbit.
    // mask2 masks the bits below lowbit.
     //mask3 masks all but highest bit.
     //combine masks to get 1's above and below. Not to get 1's in final mask
  

  int mask1 = ~0 << highbit;
  int mask2 = ~(~0 << lowbit);
  int mask3 = ~(1 << highbit);
  
  mask1 = mask1 & mask3;

  return ~(mask1 | mask2);
}
// rating 4
/* 
 * abs - absolute value of x (except returns TMin for TMin)
 *   Example: abs(-1) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int abs(int x) {
     // Boolean value indicating sign of x
        // 1 = Negative
        // 0 = Non-Negative
        int sign_x = x >> 31;

	// XOR of value x and the sign bit of value x plus 1 plus the compliment of the sign bit of x returns the absolute value of x.
	return((x ^ (sign_x)) + (1 + ( ~(sign_x))));
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
  // Mask 1 encompasses the 2 least significant bytes
        int mask1 = 0x11 | (0x11 << 8);

        // Mask 2 encompasses the final bytes
        int mask2 = mask1 | (mask1 << 16);

        // Sum will hold the number of 1 bits in the bit string
        // Computes the number of 1 bits within the first four bits
        int sum = x & mask2;
        sum = sum + ((x >> 1) & mask2);
        sum = sum + ((x >> 2) & mask2);
        sum = sum + ((x >> 3) & mask2);

        // At this point, sum represents the number of 1 bits within the first 4 bits.
        // in addition to extraneous bits beyond the first four bits.
        // As the binary position of these values do not represent their appropriate value in relation to the sum, they must be stripped.

        // Adjusts for overestimated sum value due to addition of 1 bits beyond first four bits.
        sum = sum + (sum >> 16);

        // Used to preserve current sum, and continue to mask 1 bits in the next byte.
        mask1 = 0xF | (0xF << 8);

        // Alternates the preserved bits of sum and adds alternating 4 bits together.
        sum = (sum & mask1) + ((sum >> 4) & mask1);

        // Shift sum value 1 byte and implement mask to limit resulting sum to 6 bits
        // Maximum representation of 6 bits, or a decimal value of 32, the word size for this problem set.
        return((sum + (sum >> 8)) & 0x3F);





}
/* 
 * isNonZero - Check whether x is nonzero using
 *              the legal operators except !
 *   Examples: isNonZero(3) = 1, isNonZero(0) = 0
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4 
 */
int isNonZero(int x) {
   int n = ~x +1;         //takes two's compliment
   return ((x>>31) | (n >>31))&1; //shifting over and checking if its a 1
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
     
//use or to accumlutate any 1's in the binary number
  x = ( x >> 16 ) | x;
  x = ( x >> 8 ) | x;
  x = ( x >> 4 ) | x;
  x = ( x >> 2 ) | x;
  x = ( x >> 1) | x;
  
  return ~x & 1;
}
