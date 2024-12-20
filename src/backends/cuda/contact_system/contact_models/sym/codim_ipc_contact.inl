
// > Squared Version
// > D := d*d

template <typename T>
__host__ __device__ void KappaBarrier(T& R, const T& kappa, const T& D, const T& dHat, const T& xi)
{
/*****************************************************************************************************************************
Function generated by SymEigen.py 
Author: MuGdxy
GitHub: https://github.com/MuGdxy/SymEigen
E-Mail: lxy819469559@gmail.com
******************************************************************************************************************************
LaTeX expression:
//tex:$$R = - \kappa \left(D - \hat{d}^{2} - 2 \hat{d} \xi - \xi^{2}\right)^{2} \log{\left(\frac{D - \xi^{2}}{\hat{d}^{2} + 2 \hat{d} \xi} \right)}$$

Symbol Name Mapping:
kappa:
    -> {}
    -> Matrix([[kappa]])
D:
    -> {}
    -> Matrix([[D]])
dHat:
    -> {}
    -> Matrix([[dHat]])
xi:
    -> {}
    -> Matrix([[xi]])
*****************************************************************************************************************************/
/* Sub Exprs */
auto x0 = std::pow(xi, 2);
auto x1 = std::pow(dHat, 2) + 2*dHat*xi;
/* Simplified Expr */
R = -kappa*std::pow(D - x0 - x1, 2)*log((D - x0)/x1);
}
template <typename T>
__host__ __device__ void dKappaBarrierdD(T& R, const T& kappa, const T& D, const T& dHat, const T& xi)
{
/*****************************************************************************************************************************
Function generated by SymEigen.py 
Author: MuGdxy
GitHub: https://github.com/MuGdxy/SymEigen
E-Mail: lxy819469559@gmail.com
******************************************************************************************************************************
LaTeX expression:
//tex:$$R = - \kappa \left(2 D - 2 \hat{d}^{2} - 4 \hat{d} \xi - 2 \xi^{2}\right) \log{\left(\frac{D - \xi^{2}}{\hat{d}^{2} + 2 \hat{d} \xi} \right)} - \frac{\kappa \left(D - \hat{d}^{2} - 2 \hat{d} \xi - \xi^{2}\right)^{2}}{D - \xi^{2}}$$

Symbol Name Mapping:
kappa:
    -> {}
    -> Matrix([[kappa]])
D:
    -> {}
    -> Matrix([[D]])
dHat:
    -> {}
    -> Matrix([[dHat]])
xi:
    -> {}
    -> Matrix([[xi]])
*****************************************************************************************************************************/
/* Sub Exprs */
auto x0 = std::pow(xi, 2);
auto x1 = D - x0;
auto x2 = std::pow(dHat, 2);
auto x3 = dHat*xi;
auto x4 = x2 + 2*x3;
/* Simplified Expr */
R = -kappa*(2*D - 2*x0 - 2*x2 - 4*x3)*log(x1/x4) - kappa*std::pow(D - x0 - x4, 2)/x1;
}
template <typename T>
__host__ __device__ void ddKappaBarrierddD(T& R, const T& kappa, const T& D, const T& dHat, const T& xi)
{
/*****************************************************************************************************************************
Function generated by SymEigen.py 
Author: MuGdxy
GitHub: https://github.com/MuGdxy/SymEigen
E-Mail: lxy819469559@gmail.com
******************************************************************************************************************************
LaTeX expression:
//tex:$$R = - 2 \kappa \log{\left(\frac{D - \xi^{2}}{\hat{d}^{2} + 2 \hat{d} \xi} \right)} - \frac{2 \kappa \left(2 D - 2 \hat{d}^{2} - 4 \hat{d} \xi - 2 \xi^{2}\right)}{D - \xi^{2}} + \frac{\kappa \left(D - \hat{d}^{2} - 2 \hat{d} \xi - \xi^{2}\right)^{2}}{\left(D - \xi^{2}\right)^{2}}$$

Symbol Name Mapping:
kappa:
    -> {}
    -> Matrix([[kappa]])
D:
    -> {}
    -> Matrix([[D]])
dHat:
    -> {}
    -> Matrix([[dHat]])
xi:
    -> {}
    -> Matrix([[xi]])
*****************************************************************************************************************************/
/* Sub Exprs */
auto x0 = std::pow(xi, 2);
auto x1 = D - x0;
auto x2 = std::pow(dHat, 2);
auto x3 = dHat*xi;
auto x4 = x2 + 2*x3;
auto x5 = 2*kappa;
/* Simplified Expr */
R = kappa*std::pow(D - x0 - x4, 2)/std::pow(x1, 2) - x5*log(x1/x4) - x5*(2*D - 2*x0 - 2*x2 - 4*x3)/x1;
}
