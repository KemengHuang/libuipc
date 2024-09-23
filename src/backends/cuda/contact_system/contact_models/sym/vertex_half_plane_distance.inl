
// > Squared Version
// > D := d*d 
// > DHat := dHat*dHat

template <typename T>
__host__ __device__ void HalfPlaneD(T& R, const Eigen::Vector<T,3>& x, const Eigen::Vector<T,3>& P, const Eigen::Vector<T,3>& N)
{
/*****************************************************************************************************************************
Function generated by SymEigen.py 
Author: MuGdxy
GitHub: https://github.com/MuGdxy/SymEigen
E-Mail: lxy819469559@gmail.com
******************************************************************************************************************************
LaTeX expression:
//tex:$$R = \left(N(0) \left(- P(0) + x(0)\right) + N(1) \left(- P(1) + x(1)\right) + N(2) \left(- P(2) + x(2)\right)\right)^{2}$$

Symbol Name Mapping:
x:
    -> {}
    -> Matrix([[x(0)], [x(1)], [x(2)]])
P:
    -> {}
    -> Matrix([[P(0)], [P(1)], [P(2)]])
N:
    -> {}
    -> Matrix([[N(0)], [N(1)], [N(2)]])
*****************************************************************************************************************************/
/* Sub Exprs */
/* Simplified Expr */
R = std::pow(N(0)*(-P(0) + x(0)) + N(1)*(-P(1) + x(1)) + N(2)*(-P(2) + x(2)), 2);
}
template <typename T>
__host__ __device__ void dHalfPlaneDdx(Eigen::Vector<T,3>& R, const Eigen::Vector<T,3>& x, const Eigen::Vector<T,3>& P, const Eigen::Vector<T,3>& N)
{
/*****************************************************************************************************************************
Function generated by SymEigen.py 
Author: MuGdxy
GitHub: https://github.com/MuGdxy/SymEigen
E-Mail: lxy819469559@gmail.com
******************************************************************************************************************************
LaTeX expression:
//tex:$$R = \left[\begin{matrix}2 N(0) \left(N(0) \left(- P(0) + x(0)\right) + N(1) \left(- P(1) + x(1)\right) + N(2) \left(- P(2) + x(2)\right)\right)\\2 N(1) \left(N(0) \left(- P(0) + x(0)\right) + N(1) \left(- P(1) + x(1)\right) + N(2) \left(- P(2) + x(2)\right)\right)\\2 N(2) \left(N(0) \left(- P(0) + x(0)\right) + N(1) \left(- P(1) + x(1)\right) + N(2) \left(- P(2) + x(2)\right)\right)\end{matrix}\right]$$

Symbol Name Mapping:
x:
    -> {}
    -> Matrix([[x(0)], [x(1)], [x(2)]])
P:
    -> {}
    -> Matrix([[P(0)], [P(1)], [P(2)]])
N:
    -> {}
    -> Matrix([[N(0)], [N(1)], [N(2)]])
*****************************************************************************************************************************/
/* Sub Exprs */
auto x0 = 2*N(0)*(-P(0) + x(0)) + 2*N(1)*(-P(1) + x(1)) + 2*N(2)*(-P(2) + x(2));
/* Simplified Expr */
R(0) = N(0)*x0;
R(1) = N(1)*x0;
R(2) = N(2)*x0;
}
template <typename T>
__host__ __device__ void ddHalfPlaneDddx(Eigen::Matrix<T,3,3>& R, const Eigen::Vector<T,3>& x, const Eigen::Vector<T,3>& P, const Eigen::Vector<T,3>& N)
{
/*****************************************************************************************************************************
Function generated by SymEigen.py 
Author: MuGdxy
GitHub: https://github.com/MuGdxy/SymEigen
E-Mail: lxy819469559@gmail.com
******************************************************************************************************************************
LaTeX expression:
//tex:$$R = \left[\begin{matrix}2 N(0)^{2} & 2 N(0) N(1) & 2 N(0) N(2)\\2 N(0) N(1) & 2 N(1)^{2} & 2 N(1) N(2)\\2 N(0) N(2) & 2 N(1) N(2) & 2 N(2)^{2}\end{matrix}\right]$$

Symbol Name Mapping:
x:
    -> {}
    -> Matrix([[x(0)], [x(1)], [x(2)]])
P:
    -> {}
    -> Matrix([[P(0)], [P(1)], [P(2)]])
N:
    -> {}
    -> Matrix([[N(0)], [N(1)], [N(2)]])
*****************************************************************************************************************************/
/* Sub Exprs */
auto x0 = 2*N(0);
auto x1 = N(1)*x0;
auto x2 = N(2)*x0;
auto x3 = 2*N(1)*N(2);
/* Simplified Expr */
R(0,0) = 2*std::pow(N(0), 2);
R(0,1) = x1;
R(0,2) = x2;
R(1,0) = x1;
R(1,1) = 2*std::pow(N(1), 2);
R(1,2) = x3;
R(2,0) = x2;
R(2,1) = x3;
R(2,2) = 2*std::pow(N(2), 2);
}
template <typename T>
__host__ __device__ void TR(Eigen::Vector<T,2>& R, const Eigen::Vector<T,3>& x, const Eigen::Vector<T,3>& x_prev, const Eigen::Vector<T,3>& e1, const Eigen::Vector<T,3>& e2)
{
/*****************************************************************************************************************************
Function generated by SymEigen.py 
Author: MuGdxy
GitHub: https://github.com/MuGdxy/SymEigen
E-Mail: lxy819469559@gmail.com
******************************************************************************************************************************
LaTeX expression:
//tex:$$R = \left[\begin{matrix}e1(0) \left(x(0) - x_{prev(0)}\right) + e1(1) \left(x(1) - x_{prev(1)}\right) + e1(2) \left(x(2) - x_{prev(2)}\right)\\e2(0) \left(x(0) - x_{prev(0)}\right) + e2(1) \left(x(1) - x_{prev(1)}\right) + e2(2) \left(x(2) - x_{prev(2)}\right)\end{matrix}\right]$$

Symbol Name Mapping:
x:
    -> {}
    -> Matrix([[x(0)], [x(1)], [x(2)]])
x_prev:
    -> {}
    -> Matrix([[x_prev(0)], [x_prev(1)], [x_prev(2)]])
e1:
    -> {}
    -> Matrix([[e1(0)], [e1(1)], [e1(2)]])
e2:
    -> {}
    -> Matrix([[e2(0)], [e2(1)], [e2(2)]])
*****************************************************************************************************************************/
/* Sub Exprs */
auto x0 = x(0) - x_prev(0);
auto x1 = x(1) - x_prev(1);
auto x2 = x(2) - x_prev(2);
/* Simplified Expr */
R(0) = e1(0)*x0 + e1(1)*x1 + e1(2)*x2;
R(1) = e2(0)*x0 + e2(1)*x1 + e2(2)*x2;
}
template <typename T>
__host__ __device__ void dTRdx(Eigen::Matrix<T,2,3>& R, const Eigen::Vector<T,3>& x, const Eigen::Vector<T,3>& x_prev, const Eigen::Vector<T,3>& e1, const Eigen::Vector<T,3>& e2)
{
/*****************************************************************************************************************************
Function generated by SymEigen.py 
Author: MuGdxy
GitHub: https://github.com/MuGdxy/SymEigen
E-Mail: lxy819469559@gmail.com
******************************************************************************************************************************
LaTeX expression:
//tex:$$R = \left[\begin{matrix}e1(0) & e1(1) & e1(2)\\e2(0) & e2(1) & e2(2)\end{matrix}\right]$$

Symbol Name Mapping:
x:
    -> {}
    -> Matrix([[x(0)], [x(1)], [x(2)]])
x_prev:
    -> {}
    -> Matrix([[x_prev(0)], [x_prev(1)], [x_prev(2)]])
e1:
    -> {}
    -> Matrix([[e1(0)], [e1(1)], [e1(2)]])
e2:
    -> {}
    -> Matrix([[e2(0)], [e2(1)], [e2(2)]])
*****************************************************************************************************************************/
/* Sub Exprs */
/* Simplified Expr */
R(0,0) = e1(0);
R(0,1) = e1(1);
R(0,2) = e1(2);
R(1,0) = e2(0);
R(1,1) = e2(1);
R(1,2) = e2(2);
}