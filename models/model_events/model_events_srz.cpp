
#include <include/symbolic_functions.h>
#include <sundials/sundials_types.h> //realtype definition
#include <cmath> 

void srz_model_events(double *srz, const int ie, const realtype t, const realtype *x, const realtype *p, const realtype *k, const realtype *h, const realtype *sx, const int ip) {
switch (ip) {
  case 0: {
    switch(ie) { 
        case 0: {
  srz[0] = sx[1]-sx[2];

        } break;

        case 1: {
  srz[1] = sx[0]-sx[2];

        } break;

        case 2: {

        } break;

        case 3: {

        } break;

    } 

  } break;

  case 1: {
    switch(ie) { 
        case 0: {
  srz[0] = sx[1]-sx[2];

        } break;

        case 1: {
  srz[1] = sx[0]-sx[2];

        } break;

        case 2: {

        } break;

        case 3: {

        } break;

    } 

  } break;

  case 2: {
    switch(ie) { 
        case 0: {
  srz[0] = sx[1]-sx[2];

        } break;

        case 1: {
  srz[1] = sx[0]-sx[2];

        } break;

        case 2: {

        } break;

        case 3: {

        } break;

    } 

  } break;

  case 3: {
    switch(ie) { 
        case 0: {
  srz[0] = sx[1]-sx[2];

        } break;

        case 1: {
  srz[1] = sx[0]-sx[2];

        } break;

        case 2: {

        } break;

        case 3: {

        } break;

    } 

  } break;

}
}

