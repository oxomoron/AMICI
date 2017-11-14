
#include <include/symbolic_functions.h>
#include <sundials/sundials_types.h> //realtype definition
#include <cmath> 

void dJrzdsigma_model_events(double *dJrzdsigma, const int iz, const realtype *p, const realtype *k, const double *rz, const double *sigmaz) {
switch(iz){
    case 0:
  dJrzdsigma[0+0*1] = (rz[0]*rz[0])*1.0/(sigmaz[0]*sigmaz[0]*sigmaz[0])*-1.0+1.0/sigmaz[0];
    break;
    case 1:
  dJrzdsigma[0+1*1] = (rz[1]*rz[1])*1.0/(sigmaz[1]*sigmaz[1]*sigmaz[1])*-1.0+1.0/sigmaz[1];
    break;
}
}

