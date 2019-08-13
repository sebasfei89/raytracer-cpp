#include <MathIntrinsics.h>

#include <cassert>
#include <chrono>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <iomanip>

constexpr uint32_t TEST_REPETITION = 1000u * 1000u * 10u;

struct Vec4
{
    float x, y, z, w;

    Vec4() : Vec4(0.f, 0.f, 0.f, 0.f) {}
    Vec4(float xx, float yy, float zz, float ww) : x(xx), y(yy), z(zz), w(ww) {}
};

struct Mat44
{
    float data[4][4];

    // Identity
    Mat44() : Mat44{ 0.f, 0.f, 0.f } {}

    // Translation
    Mat44(float x, float y, float z) : data{ 1.f, 0.f, 0.f, x, 0.f, 1.f, 0.f, y, 0.f, 0.f, 1.f, z, 0.f, 0.f, 0.f, 1.f } {}

    // Scaling
    Mat44(float s) : data{ s, 0.f, 0.f, 0.f, 0.f, s, 0.f, 0.f, 0.f, 0.f, s, 0.f, 0.f, 0.f, 0.f, 1.f } {}

    Vec4 operator*(Vec4 const& v) const
    {
        auto const x = (data[0][0] * v.x) + (data[0][1] * v.y) + (data[0][2] * v.z) + (data[0][3] * v.w);
        auto const y = (data[1][0] * v.x) + (data[1][1] * v.y) + (data[1][2] * v.z) + (data[1][3] * v.w);
        auto const z = (data[2][0] * v.x) + (data[2][1] * v.y) + (data[2][2] * v.z) + (data[2][3] * v.w);
        auto const w = (data[3][0] * v.x) + (data[3][1] * v.y) + (data[3][2] * v.z) + (data[3][3] * v.w);
        return { x, y, z, w };
    }

    Mat44 operator*(Mat44 const& m) const
    {
        Mat44 r;

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                r.data[i][j] = data[i][0] * m.data[0][j]
                    + data[i][1] * m.data[1][j]
                    + data[i][2] * m.data[2][j]
                    + data[i][3] * m.data[3][j];
            }
        }

        return r;
    }
};

struct alignas(16) SSEVec4
{
    union
    {
        __m128 data;
        struct
        {
            float x, y, z, w;
        };
    };

    SSEVec4() : SSEVec4(0.f, 0.f, 0.f, 0.f) {}
    SSEVec4(__m128 d) : data{ d } {}
    SSEVec4(float x, float y, float z, float w) : data{ x, y, z, w } {}
};

struct alignas(16) SSEMat44
{
    __m128 data[4];

    // Identity
    SSEMat44() : SSEMat44{ 0.f, 0.f, 0.f } {}

    // Translation
    SSEMat44(float x, float y, float z) : data{ 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, x, y, z, 1.f } {}

    // Scaling
    SSEMat44(float s) : data{ s, 0.f, 0.f, 0.f, 0.f, s, 0.f, 0.f, 0.f, 0.f, s, 0.f, 0.f, 0.f, 0.f, 1.f } {}

    SSEVec4 operator*(SSEVec4 const& v) const
    {
        __m128 const x = _mm_set1_ps(v.x);
        __m128 const y = _mm_set1_ps(v.y);
        __m128 const z = _mm_set1_ps(v.z);
        __m128 const w = _mm_set1_ps(v.w);

        __m128 const p1 = _mm_mul_ps(x, data[0]);
        __m128 const p2 = _mm_mul_ps(y, data[1]);
        __m128 const p3 = _mm_mul_ps(z, data[2]);
        __m128 const p4 = _mm_mul_ps(w, data[3]);

        auto const r1 = _mm_add_ps(p1, p2);
        auto const r2 = _mm_add_ps(p3, p4);

        return _mm_add_ps(r1, r2);
    }

    SSEMat44 operator*(SSEMat44 const& m) const
    {
        SSEMat44 r;
        for (int i = 0; i < 4; i++)
        {
            __m128 brodX = _mm_set1_ps(m.data[i].m128_f32[0] );
            __m128 brodY = _mm_set1_ps(m.data[i].m128_f32[1] );
            __m128 brodZ = _mm_set1_ps(m.data[i].m128_f32[2] );
            __m128 brodW = _mm_set1_ps(m.data[i].m128_f32[3] );
            r.data[i] = _mm_add_ps(
                _mm_add_ps(
                    _mm_mul_ps(brodX, data[0]),
                    _mm_mul_ps(brodY, data[1])),
                _mm_add_ps(
                    _mm_mul_ps(brodZ, data[2]),
                    _mm_mul_ps(brodW, data[3])));
        }

        return r;
    }
};

struct alignas(16) AVXMat44
{
    __m128 data[4];

    // Identity
    AVXMat44() : AVXMat44{ 0.f, 0.f, 0.f } {}

    // Translation
    AVXMat44(float x, float y, float z) : data{ 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, x, y, z, 1.f } {}

    // Scaling
    AVXMat44(float s) : data{ s, 0.f, 0.f, 0.f, 0.f, s, 0.f, 0.f, 0.f, 0.f, s, 0.f, 0.f, 0.f, 0.f, 1.f } {}

    SSEVec4 operator*(SSEVec4 const& v) const
    {

        __m128 x = _mm_set1_ps(v.x);
        __m128 y = _mm_set1_ps(v.y);
        __m128 z = _mm_set1_ps(v.z);
        __m128 w = _mm_set1_ps(v.w);

        __m128 p1 = _mm_mul_ps(x, data[0]);
        __m128 p2 = _mm_fmadd_ps(y, data[1], p1);
        __m128 p3 = _mm_mul_ps(z, data[2]);
        __m128 p4 = _mm_fmadd_ps(w, data[3], p3);

        return _mm_add_ps(p2, p4);
    }

    AVXMat44 operator*(AVXMat44 const& m) const
    {
        AVXMat44 r;
        return r;
    }
};

template<typename OP1, typename OP2>
OP2 RunTest(char const* msg, OP1 const& op1, OP2 const& op2)
{
    std::cout << "||---------------------||------------||" << std::endl;
    std::cout << "||" << std::left << std::setw(21) << std::setfill(' ') << msg;
    std::cout << "|| ";
    OP2 res;
    using hrc = std::chrono::high_resolution_clock;
    auto t1 = hrc::now();
    for (uint32_t i = 0u; i < TEST_REPETITION; i++)
    {
        res = op1 * op2;
    }

    auto t2 = hrc::now();
    auto time_span = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
    std::cout << std::right << std::setw(7) << std::setfill(' ') << time_span.count() << " us ||" << std::endl;  

    return res;
}

int RunTests(float x, float y, float z, float mx, float my, float mz)
{

    Mat44 const mat(mx, my, mz);
    Mat44 const mat2(2.f);
    Vec4 const vec(x, y, z, 1.f);

    SSEMat44 const sseMat(mx, my, mz);
    SSEMat44 const sseMat2(2.f);
    SSEVec4 const sseVec(x, y, z, 1.f);

    AVXMat44 const avxMat(mx, my, mz);
    AVXMat44 const avxMat2(2.f);
    SSEVec4 const avxVec(x, y, z, 1.f);

    std::cout << "=======================================" << std::endl;
    std::cout << "||       Testing Mat44 * Vec4        ||" << std::endl;
    std::cout << "=======================================" << std::endl;
    std::cout << "||      Operation      ||    Time    ||" << std::endl;
    RunTest<Mat44, Vec4>("    Mat44 * Vec4     ", mat, vec);
    RunTest<SSEMat44, SSEVec4>(" SSEMat44 * SSEVec4  ", sseMat, sseVec);
    RunTest<AVXMat44, SSEVec4>(" AVXMat44 * SSEVec4  ", avxMat, avxVec);
    std::cout << "=======================================" << std::endl << std::endl;

    std::cout << "=======================================" << std::endl;
    std::cout << "||      Testing Mat44 * Mat44        ||" << std::endl;
    std::cout << "=======================================" << std::endl;
    std::cout << "||      Operation      ||    Time    ||" << std::endl;
    RunTest<Mat44, Mat44>("    Mat44 * Mat44    ", mat, mat2);
    RunTest<SSEMat44, SSEMat44>(" SSEMat44 * SSEMat44 ", sseMat, sseMat2);
    RunTest<AVXMat44, AVXMat44>(" AVXMat44 * AVXMat44 ", avxMat, avxMat2);
    std::cout << "=======================================" << std::endl << std::endl;

    return 0;
}

int main()
{
    float const x = 1.f;
    float const y = 2.f;
    float const z = 3.f;
    return RunTests(x, y, z, z, y, x);
}
