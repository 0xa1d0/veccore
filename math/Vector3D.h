#ifndef VECCORE_MATH_VECTOR3D_H
#define VECCORE_MATH_VECTOR3D_H

#include <math/base.h>

namespace VecCore {
namespace Math {

template <typename T> class Vector3D {
private:
  T fVec[3];

public:
  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  Vector3D() : fVec{T(0.0), T(0.0), T(0.0)} {}

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  Vector3D(const T a, const T b, const T c) : fVec{a, b, c} {}

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  Vector3D(const T a) : fVec{a, a, a} {}

  template <typename Type>
  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  Vector3D(Vector3D<Type> const &v) : fVec{T(v[0]), T(v[1]), T(v[2])} {}

  template <typename Type>
  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  Vector3D &operator=(Vector3D<Type> const &v) {
    fVec[0] = static_cast<T>(v[0]);
    fVec[1] = static_cast<T>(v[1]);
    fVec[2] = static_cast<T>(v[2]);
    return *this;
  }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  void Set(T const &a, T const &b, T const &c) {
    fVec[0] = a;
    fVec[1] = b;
    fVec[2] = c;
  }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  void Set(const T a) { Set(a, a, a); }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T &x() { return fVec[0]; }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T const &x() const { return fVec[0]; }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T &y() { return fVec[1]; }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T const &y() const { return fVec[1]; }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T &z() { return fVec[2]; }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T const &z() const { return fVec[2]; }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T &operator[](const int index) { return fVec[index]; }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T const &operator[](const int index) const { return fVec[index]; }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  operator const T *() const { return reinterpret_cast<const T*>(&fVec); }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T Norm() const { return sqrt(Dot(*this, *this)); }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T Norm2() const { return Dot(*this, *this); }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  void Normalize() { *this *= (1. / Norm()); }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  Vector3D Normalized() const { return Vector3D(*this) * (1. / Norm()); }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T Perp2() const { return fVec[0]*fVec[0] + fVec[1]*fVec[1]; }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T Perp() const { return Sqrt(Perp2()); }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T Phi() const { return atan2(fVec[1], fVec[0]); }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T Theta() const { return acos(fVec[2], Norm()); }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  static Vector3D FromCylindrical(T r, T phi, T z) {
    return Vector3D(r * cos(phi), r * sin(phi), z);
  }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  static Vector3D FromSpherical(const T r, const T theta, const T phi) {
    return Vector3D(r*sin(theta)*cos(phi), r*sin(theta)*sin(phi), r*cos(theta));
  }

  /// Maps each vector entry to a function that manipulates the entry type.
  /// \param f A function of type "T f(const T&)" to map over entries.
  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  void Map(T (*f)(const T &)) {
    fVec[0] = f(fVec[0]);
    fVec[1] = f(fVec[1]);
    fVec[2] = f(fVec[2]);
  }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T Min() const {
    T min = (fVec[1] < fVec[0]) ? fVec[1] : fVec[0];
    min = (fVec[2] < min) ? fVec[2] : min;
    return min;
  }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  T Max() const {
    T max = (fVec[1] > fVec[0]) ? fVec[1] : fVec[0];
    max = (fVec[2] > max) ? fVec[2] : max;
    return max;
  }

  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
  void FixZeroes() {
    for (int i = 0; i < 3; ++i) {
      if (Abs(fVec[i]) < T(1.0e-6))
	      fVec[i] = T(0.0);
    }
  }
};

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Vector3D<T>& operator+(Vector3D<T> const &v)
{
  return v;
}

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Vector3D<T> operator-(Vector3D<T> const &v)
{
  return Vector3D<T>(-v[0], -v[1], -v[2]);
}

#define VECTOR3D_SCALAR_VECTOR_OPERATOR(OP)                                    \
                                                                               \
  template <typename T>                                                        \
  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE                                \
  Vector3D<T> operator OP(const T &s, const Vector3D<T> &v)                    \
  {                                                                            \
    return Vector3D<T>(s OP v[0], s OP v[1], s OP v[2]);                       \
  }                                                                            \

#define VECTOR3D_VECTOR_SCALAR_OPERATOR(OP)                                    \
                                                                               \
  template <typename T>                                                        \
  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE                                \
  Vector3D<T> operator OP(const Vector3D<T> &v, const T &s)                    \
  {                                                                            \
    return Vector3D<T>(v[0] OP s, v[1] OP s, v[2] OP s);                       \
  }                                                                            \

#define VECTOR3D_SCALAR_COMPOUND_ASSIGNMENT_OPERATOR(OP)                       \
  template <typename T>                                                        \
  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE                                \
  Vector3D<T> &operator OP(const Vector3D<T> &v, const T &s)                   \
  {                                                                            \
    v[0] OP s; v[1] OP s; v[2] OP s;                                           \
    return v;                                                                  \
  }                                                                            \

#define VECTOR3D_VECTOR_COMPOUND_ASSIGNMENT_OPERATOR(OP)                       \
  template <typename T>                                                        \
  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE                                \
  Vector3D<T>& operator OP(const Vector3D<T> &v1, const Vector3D<T> &v2)       \
  {                                                                            \
    v1[0] OP v2[0]; v1[1] OP v2[1]; v1[2] OP v2[2];                            \
    return v1;                                                                 \
  }                                                                            \

#define VECTOR3D_BINARY_OPERATOR(OP)                                           \
                                                                               \
  template <typename T>                                                        \
  VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE                                \
  Vector3D<T> operator OP(const Vector3D<T> &v1, const Vector3D<T> &v2)        \
  {                                                                            \
    return Vector3D<T>(v1[0] OP v2[0], v1[1] OP v2[1], v1[2] OP v2[2]);        \
  }                                                                            \

VECTOR3D_SCALAR_VECTOR_OPERATOR(*)

VECTOR3D_VECTOR_SCALAR_OPERATOR(*)
VECTOR3D_VECTOR_SCALAR_OPERATOR(/)
VECTOR3D_VECTOR_SCALAR_OPERATOR(%)

VECTOR3D_SCALAR_COMPOUND_ASSIGNMENT_OPERATOR(*=)
VECTOR3D_SCALAR_COMPOUND_ASSIGNMENT_OPERATOR(/=)
VECTOR3D_SCALAR_COMPOUND_ASSIGNMENT_OPERATOR(%=)

VECTOR3D_VECTOR_COMPOUND_ASSIGNMENT_OPERATOR(+=)
VECTOR3D_VECTOR_COMPOUND_ASSIGNMENT_OPERATOR(-=)
VECTOR3D_VECTOR_COMPOUND_ASSIGNMENT_OPERATOR(%=)

VECTOR3D_BINARY_OPERATOR(+)
VECTOR3D_BINARY_OPERATOR(-)
VECTOR3D_BINARY_OPERATOR(%)

#undef VECTOR3D_SCALAR_VECTOR_OPERATOR
#undef VECTOR3D_VECTOR_SCALAR_OPERATOR
#undef VECTOR3D_BINARY_OPERATOR

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
T Dot(const Vector3D<T> &v1, const Vector3D<T> &v2)
{
    return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Vector3D<T> Cross(const Vector3D<T> &v1, const Vector3D<T> &v2)
{
    return Vector3D<T>(v1[1] * v2[2] - v1[2] * v2[1],
                       v1[2] * v2[0] - v1[0] * v2[2],
                       v1[0] * v2[1] - v1[1] * v2[0]);
}

// optional operators to make writing code simpler

#ifdef VECTOR3D_SIMPLE_SYNTAX_OPERATORS
template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Vector3D<T> operator *(const Vector3D<T> &v1, const Vector3D<T> &v2)
{
  return Dot(v1, v2);
}

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Vector3D<T> operator ^(const Vector3D<T> &v1, const Vector3D<T> &v2)
{
  return Cross(v1, v2);
}
#endif

template <typename T>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
Vector3D<T> Abs(const Vector3D<T> &v)
{
  return Vector3D<T>(abs(v[0]), abs(v[1]), abs(v[2]));
}

template <typename T, unsigned int n = 1>
VECCORE_CUDA_HOST_DEVICE VECCORE_FORCE_INLINE
T Norm(const Vector3D<T>& v)
{

  // compiler optimizes out dead code here

  switch (n) {
    case 0:
      return Abs(v).Max();
    case 1:
      return sqrt(Dot(v, v));
    case 2:
      return Dot(v, v);
    default:
      return pow(Dot(v, v), n/2.0);
  }
}

} // namespace Math
} // namespace VecCore

#endif