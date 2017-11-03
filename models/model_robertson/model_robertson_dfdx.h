#ifndef _am_model_robertson_dfdx_h
#define _am_model_robertson_dfdx_h

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

int dfdx_model_robertson(realtype t, N_Vector x, N_Vector dx, void *user_data);


#endif /* _am_model_robertson_dfdx_h */