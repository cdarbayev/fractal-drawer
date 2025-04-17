#ifndef FRACTALS_H
#define FRACTALS_H

#include "datatype.h"


int mandelbrot(Real cRe, double cIm,
               unsigned int k, unsigned int e);

int julia(Real zRe, Real zIm, Real cRe, Real cIm, unsigned int k);

//https://pastebin.com/15n5MLg9

#endif // FRACTALS_H
