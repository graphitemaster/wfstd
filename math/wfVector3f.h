#ifndef WF_STDLIB_MATH_VECTOR3F_HDR
#define WF_STDLIB_MATH_VECTOR3F_HDR

#include "wfMath.h"

class wfVector3f {
public:
	wfVector3f() { /* EMPTY */ }
	
    wfVector3f(const wfVector3f& value) :
        m_value(value.m_value)
    { }

	wfVector3f(float x, float y, float z) :
		m_value(wfPrivate::wfMathVec4Create(x, y, z, 0.0f))
	{ }
	
    explicit wfVector3f(float xyz) :
        m_value(wfPrivate::wfMathVec4Single(xyz))
    { }

    float x() const { return wfPrivate::wfMathVec4GetX(m_value); }
    float y() const { return wfPrivate::wfMathVec4GetY(m_value); }
    float z() const { return wfPrivate::wfMathVec4GetZ(m_value); }

    wfVector3f operator -(void) {
        return wfVector3f (
            wfPrivate::wfMathVec4Sub(
                wfPrivate::wfMathVec4Zero(),
                m_value
            )
        );
    }

    wfVector3f operator -(const wfVector3f& next) {
        return wfVector3f (
            wfPrivate::wfMathVec4Sub(
                m_value,
                next.m_value
            )
        );
    }

    wfVector3f operator +(const wfVector3f& next) {
        return wfVector3f (
            wfPrivate::wfMathVec4Add(
                m_value,
                next.m_value
            )
        );
    }

    wfVector3f operator *(const wfVector3f& next) {
        return wfVector3f (
            wfPrivate::wfMathVec4Mul(
                m_value,
                next.m_value
            )
        );
    }

    wfVector3f operator /(const wfVector3f& next) {
        return wfVector3f (
            wfPrivate::wfMathVec4Div(
                m_value,
                next.m_value
            )
        );
    }

    /* Modifying Operators */
    wfVector3f& operator +=(const wfVector3f& next) {
        m_value = wfPrivate::wfMathVec4Add(m_value, next.m_value);
        return *this;
    }

    wfVector3f& operator -=(const wfVector3f& next) {
        m_value = wfPrivate::wfMathVec4Sub(m_value, next.m_value);
        return *this;
    }

    wfVector3f& operator *=(const wfVector3f& next) {
        m_value = wfPrivate::wfMathVec4Mul(m_value, next.m_value);
        return *this;
    }

    wfVector3f& operator /=(const wfVector3f& next) {
        m_value = wfPrivate::wfMathVec4Div(m_value, next.m_value);
        return *this;
    }

    /* Float varations */
    wfVector3f operator +(float next) {
        return wfVector3f (
            wfPrivate::wfMathVec4Add(
                m_value,
                wfPrivate::wfMathVec4Single(next)
            )
        );
    }

    wfVector3f operator -(float next) {
        return wfVector3f (
            wfPrivate::wfMathVec4Sub(
                m_value,
                wfPrivate::wfMathVec4Single(next)
            )
        );
    }

    wfVector3f operator *(float next) {
        return wfVector3f (
            wfPrivate::wfMathVec4Mul(
                m_value,
                wfPrivate::wfMathVec4Single(next)
            )
        );
    }

    wfVector3f operator /(float next) {
        return wfVector3f (
            wfPrivate::wfMathVec4Div(
                m_value,
                wfPrivate::wfMathVec4Single(next)
            )
        );
    }

    /* Modifying Operators */
    wfVector3f& operator +=(float next) {
        m_value = wfPrivate::wfMathVec4Add(m_value, wfPrivate::wfMathVec4Single(next));
        return *this;
    }

    wfVector3f& operator -=(float next) {
        m_value = wfPrivate::wfMathVec4Sub(m_value, wfPrivate::wfMathVec4Single(next));
        return *this;
    }

    wfVector3f& operator *=(float next) {
        m_value = wfPrivate::wfMathVec4Mul(m_value, wfPrivate::wfMathVec4Single(next));
        return *this;
    }

    wfVector3f& operator /=(float next) {
        m_value = wfPrivate::wfMathVec4Div(m_value, wfPrivate::wfMathVec4Single(next));
        return *this;
    }

    /* Dot/Length/Normalize/LengthSquared */
    float Dot(const wfVector3f &next) const {
        return wfPrivate::wfMathVec4GetX(wfPrivate::wfMathVec4Dot<4>(m_value, next.m_value));
    }

    float Length() const {
        return wfPrivate::wfMathVec4GetX(wfPrivate::wfMathVec4Length<3>(m_value));
    }

    float LengthSquared() const {
        return wfPrivate::wfMathVec4GetX(wfPrivate::wfMathVec4LengthSquared<3>(m_value));
    }

    wfVector3f Normalize() {
        return wfVector3f(wfPrivate::wfMathVec4Normalize<3>(m_value));
    }

protected:
    friend class wfMat4f;

    wfPrivate::wfMathVec4 m_value;

    wfVector3f(const wfPrivate::wfMathVec4& value) :
        m_value(value)
    {}
};

#endif /*! WF_STDLIB_MATH_VECTOR3F_HDR */
