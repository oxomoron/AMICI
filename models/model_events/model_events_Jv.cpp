
#include "amici/symbolic_functions.h"
#include "amici/defines.h" //realtype definition
typedef amici::realtype realtype;
#include <cmath> 

using namespace amici;

void Jv_model_events(realtype *Jv, const realtype t, const realtype *x, const realtype *p, const realtype *k, const realtype *h, const realtype *v, const realtype *w, const realtype *dwdx) {
  Jv[0] = p[0]*v[0]*(h[3]-1.0);
  Jv[1] = -p[2]*v[1]+p[1]*v[0]*exp(t*(-1.0/1.0E1));
  Jv[2] = -v[2];
}

