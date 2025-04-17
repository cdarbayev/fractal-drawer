#include "fractals.h"
#include <complex>

bool diverg(std::complex<Real> &c){
    return (abs(c) >= 2);
}

int mandelbrot(Real cRe, Real cIm, unsigned int k, unsigned int e){
    std::complex<Real> z(0, 0), c(cRe, cIm);
    for(auto i = 0; i < k; i++){
        z = pow(z, e) + c;
        if(diverg(z)){
            return 1;
        }
    }
    return 0;
}


int julia(Real zRe, Real zIm, Real cRe, Real cIm, unsigned int k){
    std::complex<Real> z(zRe, zIm), c(cRe, cIm);
    for(auto i = 0; i < k; i++){
        z = pow(z, 2) + c;
        if(diverg(z)){
            return 1;
        }
    }
    return 0;
}

