#pragma once
#include <Eigen/Core>
#include <string_view>
namespace uipc
{
using Float  = double;
using IndexT = int;
using SizeT  = std::size_t;

using Eigen::Matrix;
using Eigen::Vector;

using Vector2 = Vector<Float, 2>;
using Vector3 = Vector<Float, 3>;
using Vector4 = Vector<Float, 4>;

using Vector2i = Vector<IndexT, 2>;
using Vector3i = Vector<IndexT, 3>;
using Vector4i = Vector<IndexT, 4>;

using Vector6  = Vector<Float, 6>;
using Vector9  = Vector<Float, 9>;
using Vector12 = Vector<Float, 12>;

using Matrix2x2   = Matrix<Float, 2, 2>;
using Matrix3x3   = Matrix<Float, 3, 3>;
using Matrix4x4   = Matrix<Float, 4, 4>;
using Matrix6x6   = Matrix<Float, 6, 6>;
using Matrix9x9   = Matrix<Float, 9, 9>;
using Matrix12x12 = Matrix<Float, 12, 12>;

using StringView      = std::string_view;
}  // namespace uipc