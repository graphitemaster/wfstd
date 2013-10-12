#ifndef WF_STDLIB_MATH_MAT4F_HDR
#define WF_STDLIB_MATH_MAT4F_HDR
#include "wfMath.h"

#include "wfVector3f.h"
#include "wfVector4f.h"

class wfMat4f {
    wfPrivate::wfMathMat4 m_value;
    wfMat4f(const wfPrivate::wfMathMat4 &mat) :
        m_value(mat)
    { };
    
public:
    wfMat4f() { }
    wfMat4f(const wfMat4f &mat) :
        m_value(mat.m_value)
    { };
    
    wfMat4f(
        const wfVector4f &mat_x,
        const wfVector4f &mat_y,
        const wfVector4f &mat_z,
        const wfVector4f &mat_w
    ) :
        m_value(
            wfPrivate::wfMathMat4Create(
                mat_x.m_value,
                mat_y.m_value,
                mat_z.m_value,
                mat_w.m_value
            )
        )
    { };

    /* Please be safe with this and ensure the data is there to do it */
    explicit wfMat4f(const float *array) {
        wfPrivate::wfMathMat4UnalignedLoad(&m_value, array);
    }

    void Load(const float *array) {
        m_value.x = wfPrivate::wfMathVec4UnalignedLoad<4>(array);
        m_value.y = wfPrivate::wfMathVec4UnalignedLoad<4>(array + 4);
        m_value.z = wfPrivate::wfMathVec4UnalignedLoad<4>(array + 8);
        m_value.w = wfPrivate::wfMathVec4UnalignedLoad<4>(array + 12);
    }

    void Store(float *array) const {
        wfPrivate::wfMathVec4UnalignedStore<4>(m_value.x, array);
        wfPrivate::wfMathVec4UnalignedStore<4>(m_value.y, array + 4);
        wfPrivate::wfMathVec4UnalignedStore<4>(m_value.z, array + 8);
        wfPrivate::wfMathVec4UnalignedStore<4>(m_value.w, array + 12);
    }

    static wfMat4f Identity() {
        wfPrivate::wfMathMat4 mat;
        wfPrivate::wfMathMat4Identity(&mat);
        return mat;
    }

    static wfMat4f Perspective(float fovy, float aspect, float znear, float zfar) {
        wfPrivate::wfMathMat4 mat;
        wfPrivate::wfMathMat4Perspective(&mat, fovy, aspect, znear, zfar);
        return mat;
    }

    static wfMat4f Orthographic(float l, float r, float b, float t, float znear, float zfar) {
        wfPrivate::wfMathMat4 mat;
        wfPrivate::wfMathMat4Orthographic(&mat, l, r, b, t, znear, zfar);
        return mat;
    }

    static wfMat4f Look(const wfVector3f &eye, const wfVector3f &center, const wfVector3f &updir) {
        wfPrivate::wfMathMat4 mat;
        wfPrivate::wfMathMat4Look(&mat, eye.m_value, center.m_value, updir.m_value);
        return mat;
    }

    static wfMat4f AxisRotation(float angle, const wfVector3f &axis) {
        wfPrivate::wfMathMat4 mat;
        wfPrivate::wfMathMat4AxisRotation(&mat, angle, axis.m_value);
        return mat;
    }

    static wfMat4f Scale(float scale) {
        return wfPrivate::wfMathMat4Create(
                   wfPrivate::wfMathVec4Create(scale, 0.0f,  0.0f,  0.0f),
                   wfPrivate::wfMathVec4Create(0.0f,  scale, 0.0f,  0.0f),
                   wfPrivate::wfMathVec4Create(0.0f,  0.0f,  scale, 0.0f),
                   wfPrivate::wfMathVec4Create(0.0f,  0.0f,  0.0f,  1.0f)
               );
    }

    static wfMat4f Scale(const wfVector3f &vec) {
        return wfPrivate::wfMathMat4Create(
                wfPrivate::wfMathVec4Create(vec.x(), 0.0f,    0.0f,    0.0f),
                wfPrivate::wfMathVec4Create(0.0f,    vec.y(), 0.0f,    0.0f),
                wfPrivate::wfMathVec4Create(0.0f,    0.0f,    vec.z(), 0.0f),
                wfPrivate::wfMathVec4Create(0.0f,    0.0f,    0.0f,    1.0f)
        );
    }

    /*
     * Operators now: these work as you expect them to.
     */
    wfMat4f operator *(const wfMat4f &mat) {
        wfPrivate::wfMathMat4 store;
        wfPrivate::wfMathMat4Mul(&m_value, &mat.m_value, &store);
        return store;
    }

    wfMat4f& operator *=(const wfMat4f &mat) {
        const wfPrivate::wfMathMat4 store = m_value;
        wfPrivate::wfMathMat4Mul(&store, &mat.m_value, &m_value);
        return *this;
    }

    wfVector4f operator *(const wfVector4f &vec) {
        wfVector4f ret;
        wfPrivate::wfMathMat4Vec4Mul(&m_value, &vec.m_value, &ret.m_value);
        return ret;
    }

    /* 
     * TODO: Transform vector for wfVector3f and wfVector4f
     * TODO: Transform point  for wfVector3f and wfVector4f
     * TODO: Orthographic transform for point and vectors (wfVector3f)
     */

    wfMat4f Transpose(const wfMat4f &mat) {
        wfPrivate::wfMathMat4 ret;
        wfPrivate::wfMathMat4Transpose(&m_value, &ret);
        return ret;
    }

    wfMat4f Inverse(const wfMat4f &mat) {
        wfPrivate::wfMathMat4 ret;
        wfPrivate::wfMathMat4Inverse(&m_value, &ret);
        return ret;
    }
};

#endif