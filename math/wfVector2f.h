#ifndef WF_STDLIB_MATH_VECTOR2F_HDR
#define WF_STDLIB_MATH_VECTOR2F_HDR

#include "wfMath.h"

class wfVector2f {
public:
	wfVector2f() { /* EMPTY */ }
	
	wfVector2f(const wfVector2f& value) :
		m_value(value.m_value)
	{ }
	
	wfVector2f(float x, float y) :
		m_value(wfPrivate::wfMathVec4Create(x, y, 0.0f, 0.0f))
	{ }
	
	explicit wfVector2f(float xy) :
		m_value(wfPrivate::wfMathVec4Single(xy))
	{ }
	
	float x() const { return wfPrivate::wfMathVec4GetX(m_value); }
	float y() const { return wfPrivate::wfMathVec4GetY(m_value); }
	
	wfVector2f operator -(void) {
        return wfVector2f (
            wfPrivate::wfMathVec4Sub(
                wfPrivate::wfMathVec4Zero(),
                m_value
            )
        );
    }

    wfVector2f operator -(const wfVector2f& next) {
        return wfVector2f (
            wfPrivate::wfMathVec4Sub(
                m_value,
                next.m_value
            )
        );
    }

    wfVector2f operator +(const wfVector2f& next) {
        return wfVector2f (
            wfPrivate::wfMathVec4Add(
                m_value,
                next.m_value
            )
        );
    }

    wfVector2f operator *(const wfVector2f& next) {
        return wfVector2f (
            wfPrivate::wfMathVec4Mul(
                m_value,
                next.m_value
            )
        );
    }

    wfVector2f operator /(const wfVector2f& next) {
        return wfVector2f (
            wfPrivate::wfMathVec4Div(
                m_value,
                next.m_value
            )
        );
    }

    /* Modifying Operators */
    wfVector2f& operator +=(const wfVector2f& next) {
        m_value = wfPrivate::wfMathVec4Add(m_value, next.m_value);
        return *this;
    }

    wfVector2f& operator -=(const wfVector2f& next) {
        m_value = wfPrivate::wfMathVec4Sub(m_value, next.m_value);
        return *this;
    }

    wfVector2f& operator *=(const wfVector2f& next) {
        m_value = wfPrivate::wfMathVec4Mul(m_value, next.m_value);
        return *this;
    }

    wfVector2f& operator /=(const wfVector2f& next) {
        m_value = wfPrivate::wfMathVec4Div(m_value, next.m_value);
        return *this;
    }

    /* Float varations */
    wfVector2f operator +(float next) {
        return wfVector2f (
            wfPrivate::wfMathVec4Add(
                m_value,
                wfPrivate::wfMathVec4Single(next)
            )
        );
    }

    wfVector2f operator -(float next) {
        return wfVector2f (
            wfPrivate::wfMathVec4Sub(
                m_value,
                wfPrivate::wfMathVec4Single(next)
            )
        );
    }

    wfVector2f operator *(float next) {
        return wfVector2f (
            wfPrivate::wfMathVec4Mul(
                m_value,
                wfPrivate::wfMathVec4Single(next)
            )
        );
    }

    wfVector2f operator /(float next) {
        return wfVector2f (
            wfPrivate::wfMathVec4Div(
                m_value,
                wfPrivate::wfMathVec4Single(next)
            )
        );
    }

    /* Modifying Operators */
    wfVector2f& operator +=(float next) {
        m_value = wfPrivate::wfMathVec4Add(m_value, wfPrivate::wfMathVec4Single(next));
        return *this;
    }

    wfVector2f& operator -=(float next) {
        m_value = wfPrivate::wfMathVec4Sub(m_value, wfPrivate::wfMathVec4Single(next));
        return *this;
    }

    wfVector2f& operator *=(float next) {
        m_value = wfPrivate::wfMathVec4Mul(m_value, wfPrivate::wfMathVec4Single(next));
        return *this;
    }

    wfVector2f& operator /=(float next) {
        m_value = wfPrivate::wfMathVec4Div(m_value, wfPrivate::wfMathVec4Single(next));
        return *this;
    }

    /* Dot/Length/Normalize/LengthSquared */
    float Dot(const wfVector2f &next) const {
        return wfPrivate::wfMathVec4GetX(wfPrivate::wfMathVec4Dot<2>(m_value, next.m_value));
    }

    float Length() const {
        return wfPrivate::wfMathVec4GetX(wfPrivate::wfMathVec4Length<2>(m_value));
    }

    float LengthSquared() const {
        return wfPrivate::wfMathVec4GetX(wfPrivate::wfMathVec4LengthSquared<2>(m_value));
    }

    wfVector2f Normalize() {
        return wfVector2f(wfPrivate::wfMathVec4Normalize<2>(m_value));
    }
	
private:
	wfPrivate::wfMathVec4 m_value;
		
	wfVector2f(const wfPrivate::wfMathVec4& value) :
		m_value(value)
	{}
};

#endif /*! WF_STDLIB_MATH_VECTOR2F_HDR */
