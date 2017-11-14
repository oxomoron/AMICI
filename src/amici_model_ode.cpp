#include <include/amici_model_ode.h>

namespace amici {

    /** Jacobian of xdot with respect to states x
     * @param[in] N number of state variables
     * @param[in] t timepoint
     * @param[in] x Vector with the states
     * @param[in] xdot Vector with the right hand side
     * @param[out] J Matrix to which the Jacobian will be written
     * @param[in] user_data object with user input @type UserData
     * @param[in] tmp1 temporary storage vector
     * @param[in] tmp2 temporary storage vector
     * @param[in] tmp3 temporary storage vector
     * @return status flag indicating successful execution
     **/
    static int fJ(long int N, realtype t, N_Vector x, N_Vector xdot,
                   DlsMat J, void *user_data, N_Vector tmp1,
                   N_Vector tmp2, N_Vector tmp3) {
        UserData udata = (UserData) user_data;
        fdwdx(t,x,udata);
        memset(J->data,0.0,sizeof(realtype)*N)
        model_J(J->data,t,N_VGetArrayPointer(x),udata->p(),udata->k(),h,
                w.data(),dwdx.data());
        return checkVals(N,J->data,"Jacobian");
    }
    
    virtual void fJwrap(realtype t, realtype cj, AmiVector x, AmiVector dx,
                          AmiVector xdot, DlsMat J, const UserData *user_data){
        UserData user_data(*udata); // make a copy to remove constness
        fJ(J->N*J->M, realtype t, x.getNVector(), xdot.getNVector(),
           J, user_data, nullptr, nullptr, nullptr);
        
    }

    /** Jacobian of xBdot with respect to adjoint state xB
     * @param[in] NeqBdot number of adjoint state variables
     * @param[in] t timepoint
     * @param[in] x Vector with the states
     * @param[in] xB Vector with the adjoint states
     * @param[in] xBdot Vector with the adjoint right hand side
     * @param[out] JB Matrix to which the Jacobian will be written
     * @param[in] user_data object with user input @type UserData
     * @param[in] tmp1B temporary storage vector
     * @param[in] tmp2B temporary storage vector
     * @param[in] tmp3B temporary storage vector
     * @return status flag indicating successful execution
     **/
    static int fJB(long int NeqBdot, realtype t, N_Vector x, N_Vector xB,
                    N_Vector xBdot, DlsMat JB, void *user_data, N_Vector tmp1B,
                    N_Vector tmp2B, N_Vector tmp3B) {
        UserData udata = (UserData) user_data;
        fdwdx(t,x,udata);
        memset(JB->data,0.0,sizeof(realtype)*NeqBdot)
        if(!model_JB(JB->data,t,N_VGetArrayPointer(x),udata->p(),udata->k(),h,
                     N_VGetArrayPointer(xB),w.data(),dwdx.data()))
            return AMICI_ERROR;
        return checkVals(N,J->data,"Jacobian");
    }

    /** J in sparse form (for sparse solvers from the SuiteSparse Package)
     * @param[in] t timepoint
     * @param[in] cj scalar in Jacobian (inverse stepsize)
     * @param[in] x Vector with the states
     * @param[in] dx Vector with the derivative states
     *N_Vector
     * @param[in] xdot Vector with the right hand side
     * @param[out] J Matrix to which the Jacobian will be written
     * @param[in] user_data object with user input @type UserData
     * @param[in] tmp1 temporary storage vector
     * @param[in] tmp2 temporary storage vector
     * @param[in] tmp3 temporary storage vector
     * @return status flag indicating successful execution
     */
    static int fJSparse(realtype t, N_Vector x, N_Vector xdot, SlsMat J,
                         void *user_data, N_Vector tmp1, N_Vector tmp2,
                         N_Vector tmp3) {
        UserData udata = (UserData) user_data;
        fdwdx(t,x,udata);
        memset(J->data,0.0,sizeof(realtype)*J->nnz)
        model_JSparse(J->data,t,N_VGetArrayPointer(x),udata->p(),udata->k(),h,
                      w.data(),dwdx.data());
        return checkVals(J->nnz,J->data,"Jacobian");
    }
    
    virtual void fJSparsewrap(realtype t, realtype cj, AmiVector x, AmiVector dx,
                        AmiVector xdot, SlsMat J, const UserData *user_data){
        UserData user_data(*udata); // make a copy to remove constness
        fJSparse(t,x,xdot,J,user_data,nullptr,nullptr,nullptr);
    }

    /** JB in sparse form (for sparse solvers from the SuiteSparse Package)
     * @param[in] t timepoint
     * @param[in] x Vector with the states
     * @param[in] xB Vector with the adjoint states
     * @param[in] xBdot Vector with the adjoint right hand side
     * @param[out] JB Matrix to which the Jacobian will be written
     * @param[in] user_data object with user input @type UserData
     * @param[in] tmp1B temporary storage vector
     * @param[in] tmp2B temporary storage vector
     * @param[in] tmp3B temporary storage vector
     * @return status flag indicating successful execution
     */
    static int fJSparseB(realtype t, N_Vector x, N_Vector xB, N_Vector xBdot,
                          SlsMat JB, void *user_data, N_Vector tmp1B,
                          N_Vector tmp2B, N_Vector tmp3B) {
        UserData udata = (UserData) user_data;
        fdwdx(t,x,udata);
        memset(JB->data,0.0,sizeof(realtype)*JB->nnz)
        if(!model_JSparseB(JB->data,t,N_VGetArrayPointer(x),udata->p(),udata->k(),h,
                           N_VGetArrayPointer(xB),w.data(),dwdx.data()))
            return AMICI_ERROR;
        return checkVals(N,J->data,"Jacobian");
    }

    /** J in banded form (for banded solvers)
     * @param[in] N number of states
     * @param[in] mupper upper matrix bandwidth
     * @param[in] mlower lower matrix bandwidth
     * @param[in] t timepoint
     * @param[in] x Vector with the states
     * @param[in] xdot Vector with the right hand side
     * @param[out] J Matrix to which the Jacobian will be written
     * @param[in] user_data object with user input @type UserData
     * @param[in] tmp1 temporary storage vector
     * @param[in] tmp2 temporary storage vector
     * @param[in] tmp3 temporary storage vector
     * @return status flag indicating successful execution
     */
    static int fJBand(long int N, long int mupper, long int mlower, realtype t,
                       N_Vector x, N_Vector xdot, DlsMat J, void *user_data,
                       N_Vector tmp1, N_Vector tmp2, N_Vector tmp3) {
        return fJ(N,t,x,xdot,J,user_data,tmp1,tmp2,tmp3);
    }
    
    /** JB in banded form (for banded solvers)
     * @param[in] NeqBdot number of states
     * @param[in] mupper upper matrix bandwidth
     * @param[in] mlower lower matrix bandwidth
     * @param[in] t timepoint
     * @param[in] x Vector with the states
     * @param[in] xB Vector with the adjoint states
     * @param[in] xBdot Vector with the adjoint right hand side
     * @param[out] JB Matrix to which the Jacobian will be written
     * @param[in] user_data object with user input @type UserData
     * @param[in] tmp1B temporary storage vector
     * @param[in] tmp2B temporary storage vector
     * @param[in] tmp3B temporary storage vector
     * @return status flag indicating successful execution
     */
    static int fJBandB(long int NeqBdot, long int mupper, long int mlower,
                        realtype t, N_Vector x, N_Vector xB, N_Vector xBdot,
                        DlsMat JB, void *user_data, N_Vector tmp1B,
                        N_Vector tmp2B, N_Vector tmp3B) {
        return fJB(NeqBdot,t,x,xB,xBdot,JB,user_data,tmp1B,tmp2B,tmp3B);
    }

    /** diagonalized Jacobian (for preconditioning)
     * @param[in] t timepoint
     * @param[out] JDiag Vector to which the Jacobian diagonal will be written
     * @param[in] cj scaling factor, inverse of the step size
     * @param[in] x Vector with the states
     * @param[in] dx Vector with the derivative states
     * @param[in] user_data object with user input @type UserData
     **/
    static int fJDiag(realtype t, N_Vector JDiag, N_Vector x,
                       void *user_data) {
        UserData udata = (UserData) user_data;
        fdwdx(t,x,udata);
        memset(N_VGetArrayPointer(JDiag),0.0,sizeof(realtype)*nx)
        if(!model_JDiag(N_VGetArrayPointer(JDiag),t,N_VGetArrayPointer(x),udata->p(),udata->k(),h,
                        cj,N_VGetArrayPointer(dx),w.data(),dwdx.data()))
            return AMICI_ERROR;
        return checkVals(nx,N_VGetArrayPointer(JDiag),"Jacobian");
    }

    /** Matrix vector product of J with a vector v (for iterative solvers)
     * @param[in] t timepoint @type realtype
     * @param[in] x Vector with the states
     * @param[in] xdot Vector with the right hand side
     * @param[in] v Vector with which the Jacobian is multiplied
     * @param[out] Jv Vector to which the Jacobian vector product will be
     *written
     * @param[in] user_data object with user input @type UserData
     * @param[in] tmp temporary storage vector
     * @return status flag indicating successful execution
     **/
    static int fJv(N_Vector v, N_Vector Jv, realtype t, N_Vector x, N_Vector xdot,
                    void *user_data, N_Vector tmp) {
        UserData udata = (UserData) user_data;
        fdwdx(t,x,udata);
        memset(N_VGetArrayPointer(Jv),0.0,sizeof(realtype)*nx)
        if(!model_Jv(N_VGetArrayPointer(Jv),t,N_VGetArrayPointer(x),udata->p(),udata->k(),h,
                     N_VGetArrayPointer(v),w.data(),dwdx.data()))
            return AMICI_ERROR;
        return checkVals(nx,N_VGetArrayPointer(Jv),"Jacobian");
    }

    /** Matrix vector product of JB with a vector v (for iterative solvers)
     * @param[in] t timepoint
     * @param[in] x Vector with the states
     * @param[in] xB Vector with the adjoint states
     * @param[in] xBdot Vector with the adjoint right hand side
     * @param[in] vB Vector with which the Jacobian is multiplied
     * @param[out] JvB Vector to which the Jacobian vector product will be
     *written
     * @param[in] user_data object with user input @type UserData
     * @param[in] tmpB temporary storage vector
     * @return status flag indicating successful execution
     **/
    static int fJvB(N_Vector vB, N_Vector JvB, realtype t, N_Vector x, N_Vector xB, N_Vector xBdot,
                     void *user_data, N_Vector tmpB) {
        UserData udata = (UserData) user_data;
        fdwdx(t,x,udata);
        memset(N_VGetArrayPointer(JvB),0.0,sizeof(realtype)*nx)
        if(!model_JvB(N_VGetArrayPointer(JvB),t,N_VGetArrayPointer(x),udata->p(),udata->k(),h,
                      N_VGetArrayPointer(xB),N_VGetArrayPointer(vB),w.data(),dwdx.data()))
            return AMICI_ERROR;
        return checkVals(nx,N_VGetArrayPointer(JvB),"Jacobian");
    }

    /** Event trigger function for events
     * @param[in] t timepoint
     * @param[in] x Vector with the states
     * @param[out] root array with root function values
     * @param[in] user_data object with user input @type UserData
     * @return status flag indicating successful execution
     */
    static int froot(realtype t, N_Vector x, realtype *root,
                      void *user_data) {
        UserData udata = (UserData) user_data;
        memset(root,0.0,sizeof(realtype)*ne)
        if(!model_root(root,t,N_VGetArrayPointer(x),udata->p(),udata->k(),h)) {
            return AMICI_ERROR;
        }
        return checkVals(ne,root,"root function");
    }
    
    virtual void frootwrap(realtype t, AmiVector x, AmiVector dx, realtype *root,
                           const UserData *udata){
        UserData user_data(*udata); // make a copy to remove constness
        froot(t,x.getNVector(),root,user_data);
    }

    /** residual function of the DAE
     * @param[in] t timepoint
     * @param[in] x Vector with the states
     * @param[out] xdot Vector with the right hand side
     * @param[in] user_data object with user input @type UserData
     * @return status flag indicating successful execution
     */
    static int fxdot(realtype t, N_Vector x, N_Vector xdot, void *user_data) {
        UserData udata = (UserData) user_data;
        fw(t,x,udata);
        memset(N_VGetArrayPointer(xdot),0.0,sizeof(realtype)*nx)
        model_xdot(N_VGetArrayPointer(xdot),t,N_VGetArrayPointer(x),udata->p(),udata->k(),h,
                   w.data());
        return checkVals(nx,xdot,"residual function");
    }
    
    virtual void fxdotwrap(realtype t, AmiVector x, AmiVector dx, AmiVector xdot,
                           const UserData *udata){
        UserData user_data(*udata); // make a copy to remove constness
        fxdot(t,x.getNVector(),xdot.getNVector(),user_data);
    }

    /** Right hand side of differential equation for adjoint state xB
     * @param[in] t timepoint
     * @param[in] x Vector with the states
     * @param[in] xB Vector with the adjoint states
     * @param[out] xBdot Vector with the adjoint right hand side
     * @param[in] user_data object with user input @type UserData
     * @return status flag indicating successful execution
     */
    static int fxBdot(realtype t, N_Vector x, N_Vector xB,
                       N_Vector xBdot, void *user_data) {
        UserData udata = (UserData) user_data;
        fdwdx(t,x,udata);
        memset(N_VGetArrayPointer(xBdot),0.0,sizeof(realtype)*nx)
        model_xBdot(N_VGetArrayPointer(xBdot),t,N_VGetArrayPointer(x),udata->p(),udata->k(),h,
                    N_VGetArrayPointer(xB),w.data(),dwdx.data());
        return checkVals(nx,N_VGetArrayPointer(xBdot),"adjoint residual function");
    }

    /** Right hand side of integral equation for quadrature states qB
     * @param[in] t timepoint @type realtype
     * @param[in] x Vector with the states @type N_Vector
     * @param[in] dx Vector with the derivative states (only DAE) @type
     *N_Vector
     * @param[in] xB Vector with the adjoint states @type N_Vector
     * @param[in] dxB Vector with the adjoint derivative states (only DAE)
     * @type N_Vector
     * @param[out] qBdot Vector with the adjoint quadrature right hand side
     * @type N_Vector
     * @param[in] user_data pointer to temp data object @type TempDat
     * @return status flag indicating successful execution @type int
     */
    static int fqBdot(realtype t, N_Vector x, N_Vector xB, N_Vector qBdot,
                       void *user_data) {
        UserData udata = (UserData) user_data;
        fdwdp(t,x,udata);
        memset(N_VGetArrayPointer(qBdot),0.0,sizeof(realtype)*nJ*udata->nplist)
        realtype *qBdot_tmp = N_VGetArrayPointer(qBdot);
        for(int ip = 1; ip < nplist; ip++)
            model_qBdot(&qBdot[ip*nJ],udata->plist[ip],t,N_VGetArrayPointer(x),udata->p(),udata->k(),h,
                        N_VGetArrayPointer(xB),w.data(),dwdp.data());
        return checkVals(nx,N_VGetArrayPointer(qBdot),"adjoint quadrature function");
    }
    
    /** Sensitivity of dx/dt wrt model parameters p
     * @param[in] t timepoint @type realtype
     * @param[in] x Vector with the states @type N_Vector
     * @param[in] dx Vector with the derivative states
     * @param[in] user_data pointer to temp data object
     */
    virtual void fdxdotdp(const realtype t, const N_Vector x, const UserData *udata) {
        UserData udata = (UserData) user_data;
        std::fill(dxdotdp.begin(),dxdotdp.end(),0.0);
        fdwdp(t,x,udata);
        for(int ip = 1; ip < nplist; ip++)
            model_dxdotdp(dxdotdp.data(),t,N_VGetArrayPointer(x),udata->p(),udata->k(),h,
                          udata->plist[ip],w.data(),dwdP.data());
    }
    
    /** Right hand side of differential equation for state sensitivities sx
     * @param[in] Ns number of parameters
     * @param[in] t timepoint
     * @param[in] x Vector with the states
     * @param[in] dx Vector with the derivative states
     * @param[in] xdot Vector with the right hand side
     * @param[in] ip parameter index
     * @param[in] sx Vector with the state sensitivities
     * @param[in] sdx Vector with the derivative state sensitivities
     * @param[out] sxdot Vector with the sensitivity right hand side
     * @param[in] user_data object with user input @type UserData
     * @param[in] tmp1 temporary storage vector
     * @param[in] tmp2 temporary storage vector
     * @param[in] tmp3 temporary storage vector
     * @return status flag indicating successful execution
     */
    static int fsxdot(int Ns, realtype t, N_Vector x, N_Vector xdot, int ip,
                       N_Vector sx, N_Vector sxdot, void *user_data,
                       N_Vector tmp1, N_Vector tmp2) {
        UserData udata = (UserData) user_data;
        if(ip == 0) { // we only need to call this for the first parameter index will be the same for all remaining
            if(!fdxdotdp(t,x,dx,udata))
                return AMICI_ERROR;
            if(!fJSparse(t,x,J,udata,tmp1,tmp2,nullptr))// also calls dwdx & dx
                return AMICI_ERROR;
        }
        memset(N_VGetArrayPointer(sxdot),0.0,sizeof(realtype)*nx)
        if(!model_sxdot(N_VGetArrayPointer(sxdot),t,N_VGetArrayPointer(x),udata->p(),udata->k(),h,
                        udata->plist[ip],N_VGetArrayPointer(sx),
                        w.data(),dwdx.data(),J.data(),dxdotdp.data())) {
            return AMICI_ERROR;
        }
        return checkVals(nx,N_VGetArrayPointer(sxdot),"sensitivity rhs");
    }
    
}
