#ifndef WF_STDLIB_MATH_VECTOR4F_HDR
#define WF_STDLIB_MATH_VECTOR4F_HDR

#include "wfMath.h"

class wfVector4f {
public:
    wfVector4f() { /* EMPTY */ }

    wfVector4f(const wfVector4f& value) :
        m_value(value.m_value)
    {}

    wfVector4f(float x, float y, float z, float w) :
        m_value(wfPrivate::wfMathVec4Create(x, y, z, w))
    {}

    float x() const { return wfPrivate::wfMathVec4GetX(m_value); }
    float y() const { return wfPrivate::wfMathVec4GetY(m_value); }
    float z() const { return wfPrivate::wfMathVec4GetZ(m_value); }
    float w() const { return wfPrivate::wfMathVec4GetW(m_value); }

    /* Operators: */
    wfVector4f operator -(void) {
        return wfVector4f (
            wfPrivate::wfMathVec4Sub(
                wfPrivate::wfMathVec4Zero(),
                m_value
            )
        );
    }

    wfVector4f operator -(const wfVector4f& next) {
        return wfVector4f (
            wfPrivate::wfMathVec4Sub(
                m_value,
                next.m_value
            )
        );
    }

    wfVector4f operator +(const wfVector4f& next) {
        return wfVector4f (
            wfPrivate::wfMathVec4Add(
                m_value,
                next.m_value
            )
        );
    }

    wfVector4f operator *(const wfVector4f& next) {
        return wfVector4f (
            wfPrivate::wfMathVec4Mul(
                m_value,
                next.m_value
            )
        );
    }

    wfVector4f operator /(const wfVector4f& next) {
        return wfVector4f (
            wfPrivate::wfMathVec4Div(
                m_value,
                next.m_value
            )
        );
    }

    /* Modifying Operators */
    wfVector4f& operator +=(const wfVector4f& next) {
        m_value = wfPrivate::wfMathVec4Add(m_value, next.m_value);
        return *this;
    }

    wfVector4f& operator -=(const wfVector4f& next) {
        m_value = wfPrivate::wfMathVec4Sub(m_value, next.m_value);
        return *this;
    }

    wfVector4f& operator *=(const wfVector4f& next) {
        m_value = wfPrivate::wfMathVec4Mul(m_value, next.m_value);
        return *this;
    }

    wfVector4f& operator /=(const wfVector4f& next) {
        m_value = wfPrivate::wfMathVec4Div(m_value, next.m_value);
        return *this;
    }

    /* Float varations */
    wfVector4f operator +(float next) {
        return wfVector4f (
            wfPrivate::wfMathVec4Add(
                m_value,
                wfPrivate::wfMathVec4Single(next)
            )
        );
    }

    wfVector4f operator -(float next) {
        return wfVector4f (
            wfPrivate::wfMathVec4Sub(
                m_value,
                wfPrivate::wfMathVec4Single(next)
            )
        );
    }

    wfVector4f operator *(float next) {
        return wfVector4f (
            wfPrivate::wfMathVec4Mul(
                m_value,
                wfPrivate::wfMathVec4Single(next)
            )
        );
    }

    wfVector4f operator /(float next) {
        return wfVector4f (
            wfPrivate::wfMathVec4Div(
                m_value,
                wfPrivate::wfMathVec4Single(next)
            )
        );
    }

    /* Modifying Operators */
    wfVector4f& operator +=(float next) {
        m_value = wfPrivate::wfMathVec4Add(m_value, wfPrivate::wfMathVec4Single(next));
        return *this;
    }

    wfVector4f& operator -=(float next) {
        m_value = wfPrivate::wfMathVec4Sub(m_value, wfPrivate::wfMathVec4Single(next));
        return *this;
    }

    wfVector4f& operator *=(float next) {
        m_value = wfPrivate::wfMathVec4Mul(m_value, wfPrivate::wfMathVec4Single(next));
        return *this;
    }

    wfVector4f& operator /=(float next) {
        m_value = wfPrivate::wfMathVec4Div(m_value, wfPrivate::wfMathVec4Single(next));
        return *this;
    }

    /* Dot/Length/Normalize/LengthSquared */
    float Dot(const wfVector4f &next) const {
        return wfPrivate::wfMathVec4GetX(wfPrivate::wfMathVec4Dot<4>(m_value, next.m_value));
    }

    float Length() const {
        return wfPrivate::wfMathVec4GetX(wfPrivate::wfMathVec4Length<4>(m_value));
    }

    float LengthSquared() const {
        return wfPrivate::wfMathVec4GetX(wfPrivate::wfMathVec4LengthSquared<4>(m_value));
    }

    wfVector4f Normalize() {
        return wfVector4f(wfPrivate::wfMathVec4Normalize<4>(m_value));
    }


protected:
    friend class wfMat4f;

    wfVector4f(const wfPrivate::wfMathVec4& data) :
        m_value(data)
    { }

    wfPrivate::wfMathVec4 m_value;
};

#endif /*! WF_STDLIB_MATH_VECTOR4F_HDR */
