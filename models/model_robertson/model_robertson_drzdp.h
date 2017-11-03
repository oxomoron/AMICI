#ifndef _am_model_robertson_drzdp_h
#define _am_model_robertson_drzdp_h

#include <sundials/sundials_types.h>
#include <sundials/sundials_nvector.h>
#include <sundials/sundials_sparse.h>
#include <sundials/sundials_direct.h>

using namespace amici;

namespace amici {
class UserData;
class ReturnData;
class TempData;
class ExpData;
}

int drzdp_model_robertson(realtype t, int ie, N_Vector x, amici::TempData *tdata);


#endif /* _am_model_robertson_drzdp_h */