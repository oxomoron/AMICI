
#include <include/symbolic_functions.h>
#include <include/amici.h>
#include <include/amici_model.h>
#include <string.h>
#include <include/tdata.h>
#include <include/udata.h>
#include "model_robertson_w.h"

using namespace amici;

int JvB_model_robertson(realtype t, N_Vector x, N_Vector dx, N_Vector xB, N_Vector dxB, N_Vector xBdot, N_Vector vB, N_Vector JvB, realtype cj, void *user_data, N_Vector tmpB1, N_Vector tmpB2) {
int status = 0;
TempData *tdata = (TempData*) user_data;
Model *model = (Model*) tdata->model;
UserData *udata = (UserData*) tdata->udata;
realtype *x_tmp = nullptr;
if(x)
    x_tmp = N_VGetArrayPointer(x);
realtype *dx_tmp = nullptr;
if(dx)
    dx_tmp = N_VGetArrayPointer(dx);
realtype *xB_tmp = nullptr;
if(xB)
    xB_tmp = N_VGetArrayPointer(xB);
realtype *dxB_tmp = nullptr;
if(dxB)
    dxB_tmp = N_VGetArrayPointer(dxB);
realtype *xBdot_tmp = nullptr;
if(xBdot)
    xBdot_tmp = N_VGetArrayPointer(xBdot);
realtype *vB_tmp = nullptr;
if(vB)
    vB_tmp = N_VGetArrayPointer(vB);
realtype *JvB_tmp = nullptr;
if(JvB)
    JvB_tmp = N_VGetArrayPointer(JvB);
memset(JvB_tmp,0,sizeof(realtype)*3);
status = w_model_robertson(t,x,dx,tdata);
  JvB_tmp[0] = -vB_tmp[2]+vB_tmp[0]*(cj+tdata->p[0])-tdata->p[0]*vB_tmp[1];
  JvB_tmp[1] = -vB_tmp[2]-vB_tmp[0]*tdata->dwdx[0]+vB_tmp[1]*(cj+tdata->dwdx[0]+tdata->p[2]*x_tmp[1]*2.0);
  JvB_tmp[2] = -vB_tmp[2]-vB_tmp[0]*tdata->dwdx[1]+vB_tmp[1]*tdata->dwdx[1];
return(status);

}

