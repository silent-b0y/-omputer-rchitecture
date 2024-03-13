#include <iostream>
#include <xmmintrin.h>
#include <mmintrin.h>
#include <immintrin.h>
#include <dvec.h>
#include <intrin.h>

using std::cin;
using std::cout;
using std::endl;
using std::cerr;
using std::string;

__m64 _mm_compare(__m64 m1, __m64 m2)
{
  __m64 res;
  __asm
  {
    movq mm0, m1
    movq mm1, m2
    pcmpeqb mm0, mm1
    movq res, mm0
  }
  return res;
}

__m128 _mm_addsub(__m128 m1, __m128 m2)
{
  __m128 res;
  __asm
  {
    movups xmm0, m1
    movups xmm1, m2
    addsubps xmm0, xmm1
    movups res, xmm0
  }
  return res;
}

__m256 _mm_min(__m256 m1, __m256 m2)
{
  __m256 res;
  __asm
  {
    vmovupd ymm0, m1
    vmovupd ymm1, m2
    vminpd  ymm2, ymm1, ymm0
    vmovupd res, ymm2
  }
  return res;
}

int main(void) {
  setlocale(LC_ALL, "rus");
  int compare0[2] = { 15, 1024 };
  int compare1[2] = { 225, 1024 };

  _asm { //mmx
    movq mm0, compare0
    movq mm1, compare1
    pcmpeqd mm0, mm1
    movq compare0, mm0
  }
  cout << "Сравнение элементов векторов compare0 и compare1: ";
  for (int i = 0; i < 2; i++)
  {
    cout << compare0[i] << " ";
  }

  int ctrl = 0;
  __asm
  {
    finit
    fstcw ctrl
    and ctrl, 0xFFFE
    fldcw ctrl
  }
  cout << endl;

  float reverse[4] = { 10.0, -5.0, 0.5, -0.1 };

  _asm { //sse
    movups xmm1, reverse
    rcpps xmm0, xmm1
    movups reverse, xmm0
  }
  cout << endl << "Обратные числа для вектора reverse: ";
  for (int i = 0; i < 4; i++)
  {
    cout << reverse[i] << " ";
  }
  cout << endl;

  double src[2] = { 100.0, 25.0 };
  double dest[2] = { 10.0, 200.0 };

  _asm { //sse2
    movups xmm0, src
    movups xmm1, dest
    subpd xmm1, xmm0
    movups src, xmm1
  }
  cout << endl << "Вычитание элементов векторов dest и src: " << src[0] << " " << src[1] << endl;
  cout << endl;

  float avx1[8] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0 };
  float avx2[8] = { -1.0, 5, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0 };
  float result[8] = {};

  _asm {//avx
    vmovups ymm0, avx1
    vmovups ymm1, avx2
    vmaxps ymm2, ymm1, ymm0
    vmovups result, ymm2
    vzeroall
  }

  cout << "Максимальные значения массивов avx1 и avx2 : ";
  for (int i = 0; i < 8; ++i)
  {
    cout << result[i] << ' ';
  }
  cout << endl;

  cout << endl;

  //dvec.h
  F64vec2 v1(99.0, 250.0);
  F64vec2 v2(100.0, 50.0);
  v1 = v2 - v1;
  cout << "Разность векторов: ";
  for (int i = 1; i >= 0; i--)
  {
    cout << v1[i] << " ";
  }
  cout << endl;
  cout << endl;

  //_mm_64
  char arr_mmx_1[8] = { 0b1, 0b11, 0b111, 0b11, 0b101, 0b11, 0b100, 0b11 };
  char arr_mmx_2[8] = { 0b11, 0b11, 0b11, 0b11, 0b11, 0b11, 0b11, 0b11 };
  __m64 m64_1, m64_2;

  __asm {
    movq mm0, arr_mmx_1
    movq m64_1, mm0
    movq mm0, arr_mmx_2
    movq m64_2, mm0
  }
  m64_1 = _mm_compare(m64_1, m64_2);
  __asm {
    movq mm0, m64_1
    movq arr_mmx_1, mm0
  }
  cout << "Сравнение массивов байтов arr_mmx_1 и arr_mmx_2 : ";
  for (int i = 0; i < 8; ++i)
  {
    cout << arr_mmx_1[i] + 0 << ' ';
  }
  cout << endl;
  cout << endl;

  //_mm_128
  float arr_xmm_1[4] = { 1, 2.0, 7, -4.0 };
  float arr_xmm_2[4] = { 10, 5.0, 3, -6.0 };
  __m128 m128_1, m128_2;

  __asm {
    emms
    movups xmm0, arr_xmm_1
    movups m128_1, xmm0
    movups xmm0, arr_xmm_2
    movups m128_2, xmm0
  }
  m128_1 = _mm_addsub(m128_1, m128_2);
  __asm {
    movups xmm0, m128_1
    movups arr_xmm_1, xmm0
  }
  cout << "Cложение 2-х и 4-х float и вычитание 1-х и 3-х arr_xmm_1 и arr_xmm_2: ";
  for (int i = 0; i < 4; ++i)
  {
    cout << arr_xmm_1[i] << ' ';
  }
  cout << endl;
  cout << endl;

  //_mm_256
  double arr_avx_1[4] = { 1, 8, 2, 9 };
  double arr_avx_2[4] = { 5, 4, 20, 3 };
  __m256 m256_1, m256_2;

  __asm {
    vmovupd ymm0, arr_avx_1
    vmovupd m256_1, ymm0
    vmovupd ymm0, arr_avx_2
    vmovupd m256_2, ymm0
  }
  m256_1 = _mm_min(m256_1, m256_2);
  __asm {
    vmovupd ymm0, m256_1
    vmovupd arr_avx_1, ymm0
  }
  cout << "Минимальные значения массивов arr_avx_1 и arr_avx_2 : ";
  for (int i = 0; i < 4; ++i)
  {
    cout << arr_avx_1[i] << ' ';
  }
  cout << endl;
  cout << endl;


  //intrin.h

  double im256_1[12] = { 1,2,3,4,5,6,7,8,9,10,11,12 };
  double im256_2[12] = { 1,1,1,2,2,2,3,3,3,4,4,4 };
  double im256_3[12];
  for (int i = 0; i < 12; i += 4) {

    __m256d x = _mm256_loadu_pd(&im256_1[i]);
    __m256d y = _mm256_loadu_pd(&im256_2[i]);
    __m256d z;
    _mm256_sub_pd(x, y);
    __asm vmovupd z, ymm0;
    _mm256_store_pd(&im256_3[i], z);
  }
  cout << "Разность массивов im256_1 и im256_2: ";
  for (int i = 0; i < 12; i++)
  {
    cout << im256_3[i] << " ";
  }
  __asm vzeroall;

  cout << endl;

  return 0;
}