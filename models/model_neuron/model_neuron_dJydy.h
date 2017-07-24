#ifndef _am_model_neuron_dJydy_h
#define _am_model_neuron_dJydy_h

#include <sundials/sundials_types.h>
#include <sundials/sundials_nvector.h>
#include <sundials/sundials_sparse.h>
#include <sundials/sundials_direct.h>

class UserData;
class ReturnData;
class TempData;
class ExpData;

int dJydy_model_neuron(realtype t, int it, N_Vector x, void *user_data, TempData *tdata, const ExpData *edata, ReturnData *rdata);


#endif /* _am_model_neuron_dJydy_h */