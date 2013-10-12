#ifndef WF_STDLIB_MATH_SCALAR_HDR
#define WF_STDLIB_MATH_SCALAR_HDR

namespace wfPrivate {
    struct wfMathVec4 {
        float x;
        float y;
        float z;
        float w;
    };

    struct wfMathMat4 {
        wfMathVec4 x;
        wfMathVec4 y;
        wfMathVec4 z;
        wfMathVec4 w;
    };

    enum wfMathVec4Splat {
        VEC_SPLAT_X,
        VEC_SPLAT_Y,
        VEC_SPLAT_Z,
        VEC_SPLAT_W
    };


    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Create(float x, float y, float z, float w) {
        wfMathVec4 data = {
            x, y, z, w
        };
        return data;
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Zero() {
        wfMathVec4 data = {
            0.0f, 0.0f, 0.0f, 0.0f
        };
        return data;
    }


    /* Unaligned load: (partial template specializations) */
    template <size_t E>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4UnalignedLoad(const float *array);

    template<>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4UnalignedLoad<4>(const float *array) {
        wfMathVec4 data = {
            array[0], array[1], array[2], array[3]
        };
        return data;
    }

    template<>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4UnalignedLoad<3>(const float *array) {
        wfMathVec4 data = {
            array[0], array[1], array[2], 0.0f
        };
        return data;
    }

    template<>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4UnalignedLoad<2>(const float *array) {
        wfMathVec4 data = {
            array[0], array[1], 0.0f, 0.0f
        };
        return data;
    }

    /* Unaligned stores: (partial template specializations) */
    template <size_t E>
    WF_MATH_VEC_INLINE
    void wfMathVec4UnalignedStore(const wfMathVec4& value, float *array);

    template <>
    WF_MATH_VEC_INLINE
    void wfMathVec4UnalignedStore<4>(const wfMathVec4& value, float *array) {
        memcpy(array, &value, sizeof(float) * 4);
    }

    template <>
    WF_MATH_VEC_INLINE
    void wfMathVec4UnalignedStore<3>(const wfMathVec4& value, float *array) {
        memcpy(array, &value, sizeof(float) * 3);
    }

    template <>
    WF_MATH_VEC_INLINE
    void wfMathVec4UnalignedStore<2>(const wfMathVec4& value, float *array) {
        memcpy(array, &value, sizeof(float) * 2);
    }

    /* Utilites */
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Single(float splat) {
        wfMathVec4 data = {
            splat, splat, splat, splat
        };
        return data;
    }

    /*
     * Overload for wfMathVec4 used for X/Y/Z/W splatter (with partial
     * template specializations as well.
     */  
    template <wfMathVec4Splat E>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Splat(const wfMathVec4&);

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Splat<VEC_SPLAT_X>(const wfMathVec4& splat) {
        wfMathVec4 data = {
            splat.x, splat.x, splat.x, splat.x
        };
        return data;
    }

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Splat<VEC_SPLAT_Y>(const wfMathVec4& splat) {
        wfMathVec4 data = {
            splat.y, splat.y, splat.y, splat.y
        };
        return data;
    }

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Splat<VEC_SPLAT_Z>(const wfMathVec4& splat) {
        wfMathVec4 data = {
            splat.z, splat.z, splat.z, splat.z
        };
        return data;
    }

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Splat<VEC_SPLAT_W>(const wfMathVec4& splat) {
        wfMathVec4 data = {
            splat.w, splat.w, splat.w, splat.w
        };
        return data;
    }

    /* RCP/SQRT/RSQRT*/
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Reciprocal(const wfMathVec4& vec) {
        wfMathVec4 data = {
            1.0f / vec.x,
            1.0f / vec.y,
            1.0f / vec.z,
            1.0f / vec.w
        };

        return data;
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Sqrt(const wfMathVec4& vec) {
        wfMathVec4 data = {
            sqrtf(vec.x),
            sqrtf(vec.y),
            sqrtf(vec.z),
            sqrtf(vec.w)
        };

        return data;
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4RSqrt(const wfMathVec4& vec) {
        wfMathVec4 data = {
            1.0f / sqrtf(vec.x),
            1.0f / sqrtf(vec.y),
            1.0f / sqrtf(vec.z),
            1.0f / sqrtf(vec.w)
        };

        return data;
    }

    /* Arithmetic and stuff that actually matters */
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Add(const wfMathVec4& lhs, const wfMathVec4& rhs) {
        wfMathVec4 data = {
            lhs.x + rhs.x,
            lhs.y + rhs.y,
            lhs.z + rhs.z,
            lhs.w + rhs.w
        };

        return data;
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Sub(const wfMathVec4& lhs, const wfMathVec4& rhs) {
        wfMathVec4 data = {
            lhs.x - rhs.x,
            lhs.y - rhs.y,
            lhs.z - rhs.z,
            lhs.w - rhs.w
        };

        return data;
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Mul(const wfMathVec4& lhs, const wfMathVec4& rhs) {
        wfMathVec4 data = {
            lhs.x * rhs.x,
            lhs.y * rhs.y,
            lhs.z * rhs.z,
            lhs.w * rhs.w
        };

        return data;
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Div(const wfMathVec4& lhs, const wfMathVec4& rhs) {
        wfMathVec4 data = {
            lhs.x / rhs.x,
            lhs.y / rhs.y,
            lhs.z / rhs.z,
            lhs.w / rhs.w
        };

        return data;
    }

    /* The rest of the misc functionality that is common in such types */
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4MulAdd(const wfMathVec4& a, const wfMathVec4& b, const wfMathVec4& c) {
        return wfMathVec4Add(wfMathVec4Mul(a, b), c);
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Cross3(const wfMathVec4& lhs, const wfMathVec4& rhs) {
        return wfMathVec4Create (
            lhs.y * rhs.z - lhs.z * rhs.y,
            lhs.z * rhs.x - lhs.x * rhs.z,
            lhs.x * rhs.y - lhs.y * rhs.x,
            0.0f
        );
    }

    /* Getters */
    WF_MATH_VEC_INLINE float wfMathVec4GetX(const wfMathVec4 &vec) { return vec.x; }
    WF_MATH_VEC_INLINE float wfMathVec4GetY(const wfMathVec4 &vec) { return vec.y; }
    WF_MATH_VEC_INLINE float wfMathVec4GetZ(const wfMathVec4 &vec) { return vec.z; }
    WF_MATH_VEC_INLINE float wfMathVec4GetW(const wfMathVec4 &vec) { return vec.w; }

    /* Shuffling */
    enum wfMathVec4Shuffle {
        VEC_SHUFFLE_WXYZ,
        VEC_SHUFFLE_ZWXY,
        VEC_SHUFFLE_YZWX
    };

    template <wfMathVec4Shuffle E>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Shuffle(const wfMathVec4& vec);

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Shuffle<VEC_SHUFFLE_WXYZ>(const wfMathVec4& vec) {
        return wfMathVec4Create(vec.w, vec.x, vec.y, vec.z);
    }

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Shuffle<VEC_SHUFFLE_ZWXY>(const wfMathVec4& vec) {
        return wfMathVec4Create(vec.z, vec.w, vec.x, vec.y);
    }

    template <>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Shuffle<VEC_SHUFFLE_YZWX>(const wfMathVec4& vec) {
        return wfMathVec4Create(vec.y, vec.z, vec.w, vec.x);
    }

    enum wfMathFlipSign {
        VEC_SIGN_0101,
        VEC_SIGN_1010
    };

    template<wfMathFlipSign E>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4FlipSign(const wfMathVec4& vec);

    template<>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4FlipSign<VEC_SIGN_0101>(const wfMathVec4& mat) {
       return wfMathVec4Create(mat.x, -mat.y, mat.z, -mat.w);
    }

    template<>
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4FlipSign<VEC_SIGN_1010>(const wfMathVec4& mat) {
        return wfMathVec4Create(-mat.x, mat.y, -mat.z, mat.w);
    }


    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Merge(const wfMathVec4 &lhs, const wfMathVec4 &rhs) {
        return wfMathVec4Create(lhs.z, lhs.w, rhs.z, rhs.w);
    }

    /* Minimum and Maximum */
    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Min(const wfMathVec4 &a, const wfMathVec4 &b) {
        return wfMathVec4Create(
            a.x < b.x ? a.x : b.x,
            a.y < b.y ? a.y : b.y,
            a.z < b.z ? a.z : b.z,
            a.w < b.w ? a.w : b.w
        );
    }

    WF_MATH_VEC_INLINE
    wfMathVec4 wfMathVec4Max(const wfMathVec4 &a, const wfMathVec4 &b) {
        return wfMathVec4Create(
            a.x > b.x ? a.x : b.x,
            a.y > b.y ? a.y : b.y,
            a.z > b.z ? a.z : b.z,
            a.w > b.w ? a.w : b.w
        );
    }

    /*
     * 4x4 matrix tranposition inplace.  We store the dereference of
     * matrix for less dereferences.
     */   
    WF_MATH_VEC_INLINE
    void wfMathMat4TransposeInplace(wfMathMat4 *matrix) {
        wfMathMat4 mat = *matrix;

        matrix->x.x = mat.x.x; /* compiled out, only here for reference */
        {
            matrix->x.y = mat.y.x;
            matrix->x.z = mat.z.x;
            matrix->x.w = mat.w.x;
            matrix->y.x = mat.x.y;
            matrix->y.y = mat.y.y;
            matrix->y.z = mat.z.y;
            matrix->y.w = mat.w.y;
            matrix->z.x = mat.x.z;
            matrix->z.y = mat.y.z;
            matrix->z.z = mat.z.z;
            matrix->z.w = mat.w.z;
            matrix->w.x = mat.x.w;
            matrix->w.y = mat.y.w;
            matrix->w.z = mat.z.w;
        }
        matrix->w.w = mat.w.w; /* compiled out, only here for reference */
    }

    WF_MATH_VEC_INLINE
    void wfMathMat4Transpose(const wfMathMat4 *mat, wfMathMat4 *out) {
        *out = *mat;
        wfMathMat4TransposeInplace(out);
    }
}

#endif /*! WF_STDLIB_MATH_SCALAR_HDR */
