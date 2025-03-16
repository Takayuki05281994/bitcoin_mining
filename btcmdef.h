#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <immintrin.h>


#define PARANUM /*4*/8


// 右シフト
#define ShR(x,n) (x>>n)
// 右ローテート
#define RotR(x,n) ((x>>n)|(x<<(32-n)))
// 選択関数: xのbitが1→yを選択、xのbitが0→zを選択
#define Ch(x,y,z) ((x&(y^z))^z)


// 多数決関数: x,y,zのbitのうち、2つ以上が0→0、2つ以上が1→1
#define Maj(x,y,z) ((x&(y|z))|(y&z))

#define S0(x) (RotR((x), 2)^RotR((x),13)^RotR((x),22))
#define S1(x) (RotR((x), 6)^RotR((x),11)^RotR((x),25))
#define s0(x) (RotR(x, 7)^RotR(x,18)^ ShR(x, 3))
#define s1(x) (RotR(x,17)^RotR(x,19)^ ShR(x,10))


void Show(uint32_t *M, int Len);
void dumpreg(__m128i x, char *msg);
int	CompHash(uint32_t *c1, uint32_t *c2);
int c2i(char c);
void Set(uint32_t *M, uint32_t Version, const char *PrevBlock, const char *MerkleRoot, uint32_t TimeStamp, uint32_t Difficulty, uint32_t Nonce);
void Target(uint32_t *T, uint32_t targ);
time_t Str2Time(int year, int month, int day, int hour, int min, int sec);

#if PARANUM==4
#define mmi				__m128i
#define mSet1(x)		_mm_set1_epi32((x))
#define mLoad(x)		_mm_load_si128((const __m128i *)(x))
#define mStore(x, y)	_mm_store_si128((__m128i *)(x), (y))
#define mAdd(x, y)		_mm_add_epi32((x), (y))
#define mOR(x, y)		_mm_or_si128((x), (y))
#define mXOR(x, y)		_mm_xor_si128((x), (y))
#define mAND(x, y)		_mm_and_si128((x), (y))
#define mANDNOT(x, y)	_mm_andnot_si128((x), (y))
#define mSRLI(x, y)		_mm_srli_epi32((x), (y))
#define mSLLI(x, y)		_mm_slli_epi32((x), (y))
#endif

#if PARANUM==8
#define mmi				__m256i
#define mSet1(x)		_mm256_set1_epi32((x))
#define mLoad(x)		_mm256_load_si256((const mmi *)(x))
#define mStore(x, y)	_mm256_store_si256((mmi *)(x), (y))
#define mAdd(x, y)		_mm256_add_epi32((x), (y))
#define mOR(x, y)		_mm256_or_si256((x), (y))
#define mXOR(x, y)		_mm256_xor_si256((x), (y))
#define mAND(x, y)		_mm256_and_si256((x), (y))
#define mANDNOT(x, y)	_mm256_andnot_si256((x), (y))
#define mSRLI(x, y)		_mm256_srli_epi32((x), (y))
#define mSLLI(x, y)		_mm256_slli_epi32((x), (y))
#endif

#if PARANUM==16
#define mmi				__m512i
#define mSet1(x)		_mm512_set1_epi32((x))
#define mLoad(x)		_mm512_load_si512((const mmi *)(x))
#define mStore(x, y)	_mm512_store_si512((mmi *)(x), (y))
#define mAdd(x, y)		_mm512_add_epi32((x), (y))
#define mOR(x, y)		_mm512_or_si512((x), (y))
#define mXOR(x, y)		_mm512_xor_si512((x), (y))
#define mAND(x, y)		_mm512_and_si512((x), (y))
#define mANDNOT(x, y)	_mm512_andnot_si512((x), (y))
#define mSRLI(x, y)		_mm512_srli_epi32((x), (y))
#define mSLLI(x, y)		_mm512_slli_epi32((x), (y))
#endif


#define mBIGSIGMA0(x)    (mXOR(mXOR(mROTR((x), 2),mROTR((x), 13)),mROTR((x), 22)))
#define mBIGSIGMA1(x)    (mXOR(mXOR(mROTR((x), 6),mROTR((x), 11)),mROTR((x), 25)))
#define mSIGMA0(x)       (mXOR(mXOR(mROTR((x), 7),mROTR((x), 18)),mSHR((x), 3)))
#define mSIGMA1(x)       (mXOR(mXOR(mROTR((x), 17),mROTR((x), 19)),mSHR((x), 10)))


//#define mAdd4(x0, x1, x2, x3)		mAdd(mAdd((x0), (x1)), mAdd((x2), (x3)))
#define mAdd4(x0, x1, x2, x3)		mAdd(mAdd(mAdd((x0), (x1)), (x2)), (x3))
#define mAdd5(x0, x1, x2, x3, x4)	mAdd(mAdd4((x0), (x1), (x2), (x3)), (x4))

#define mROTR(x, n)		mOR(mSRLI((x), (n)), mSLLI((x), (32 - n)))
#define mSHR(x, n)		mSRLI((x), (n))
#define mCh(b, c, d)	mXOR(mAND((b),(c)), mANDNOT((b),(d)))
#define mMaj(b, c, d)	mXOR(mXOR(mAND((b),(c)), mAND((b),(d))), mAND((c),(d)))


#define SHA256RS1(a, b, c, d, e, f, g, h, w)		\
	{mmi mKW1 = mAdd(mBIGSIGMA1(e), mCh(e, f, g)); mmi T1 = mAdd(mAdd(mKW1, mSet1(w)), h); mmi T2 = mAdd(mBIGSIGMA0(a), mMaj(a, b, c)); d = mAdd(d, T1); h = mAdd(T1, T2); }
///	{mmi mKW1 = mSet1(w); mmi T1 = mAdd(mAdd(mAdd(h, mBIGSIGMA1(e)), mCh(e, f, g)), mKW1); mmi T2 = mAdd(mBIGSIGMA0(a), mMaj(a, b, c)); d = mAdd(d, T1); h = mAdd(T1, T2); }


#define SHA256RLD(a, b, c, d, e, f, g, h, w)		\
	{mmi mKW1 = mAdd(mBIGSIGMA1(e), mCh(e, f, g)); mmi T1 = mAdd(mAdd(mKW1, mLoad(w)), h); mmi T2 = mAdd(mBIGSIGMA0(a), mMaj(a, b, c)); d = mAdd(d, T1); h = mAdd(T1, T2); }
///	{mmi mKW1 = mLoad(w); mmi T1 = mAdd(mAdd(mAdd(h, mBIGSIGMA1(e)), mCh(e, f, g)), mKW1); mmi T2 = mAdd(mBIGSIGMA0(a), mMaj(a, b, c)); d = mAdd(d, T1); h = mAdd(T1, T2); }



inline unsigned int store32(const mmi x, int i)
{
    union { unsigned int ret[/*4*/PARANUM]; mmi x; } box;
    box.x = x;
    return box.ret[i];
}
