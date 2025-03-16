#include "btcmdef.h"


const uint32_t K[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

uint32_t hash0[8];
uint32_t i1h0, i1h1, i1h2, i1h3, i1h4, i1h5, i1h6, i1h7;
uint32_t i2h0, i2h1, i2h2, i2h3, i2h4, i2h5, i2h6, i2h7;


uint32_t pnW1[64][PARANUM];
uint32_t pnPreW1[64][PARANUM];
uint32_t pnW2[64][PARANUM];
uint32_t pnPreW2[64][PARANUM];
uint32_t pnKW1[64][PARANUM];
uint32_t pnKW2[64][PARANUM];


void SHA0(uint32_t *M)
{
	uint32_t W[64];
	int m = 0;

	for (int t=0; t<16; t++, m++)
	{
		if (m < 20/*Len*/)
		{
			W[t] = M[m];
		}
		else if(m == 20/*Len*/)
		{
			W[t] = 0x80000000;
		}
		else if (t == 15)
		{
			W[t] = 20<<5; /*Len<<5;*/
		}
		else
		{
			W[t] = 0;
		}
	}
	for(int t=16; t<64; t++)
	{
		W[t] = s1(W[t-2]) + W[t-7] + s0(W[t-15]) + W[t-16];
	}

	uint32_t h0, h1, h2, h3, h4, h5, h6, h7;
	h0 = 0x6a09e667; h1 = 0xbb67ae85; h2 = 0x3c6ef372; h3 = 0xa54ff53a;
	h4 = 0x510e527f; h5 = 0x9b05688c; h6 = 0x1f83d9ab; h7 = 0x5be0cd19;

	for(int i=0; i<64; i++)
	{
		uint32_t T1 = h7 + S1(h4) + Ch(h4, h5, h6) + K[i] + W[i];
		uint32_t T2 = S0(h0) + Maj(h0, h1, h2);
		h7 = h6; h6 = h5; h5 = h4;
		h4 = h3 + T1;
		h3 = h2; h2 = h1; h1 = h0;
		h0 = T1 + T2;
	}
	hash0[0] = 0x6a09e667 + h0; hash0[1] = 0xbb67ae85 + h1; hash0[2] = 0x3c6ef372 + h2; hash0[3] = 0xa54ff53a + h3;
	hash0[4] = 0x510e527f + h4; hash0[5] = 0x9b05688c + h5; hash0[6] = 0x1f83d9ab + h6; hash0[7] = 0x5be0cd19 + h7;

	for (int i1 = 0; i1 < PARANUM; i1++)
	{
		//ワークエリア固定値準備
		pnW1[0][i1] = M[16]; pnKW1[0][i1] = pnW1[0][i1] + K[0];
		pnW1[1][i1] = M[17]; pnKW1[1][i1] = pnW1[1][i1] + K[1];
		pnW1[2][i1] = M[18]; pnKW1[2][i1] = pnW1[2][i1] + K[2]; /***W1[3] = M1[19]; nonce***/
		pnW1[4][i1] = 0x80000000; pnKW1[4][i1] = pnW1[4][i1] + K[4];
		pnW1[5][i1] = 0; pnKW1[5][i1] = K[5]; pnW1[6][i1] = 0; pnKW1[6][i1] = K[6];
		pnW1[7][i1] = 0; pnKW1[7][i1] = K[7]; pnW1[8][i1] = 0; pnKW1[8][i1] = K[8];
		pnW1[9][i1] = 0; pnKW1[9][i1] = K[9]; pnW1[10][i1] = 0; pnKW1[10][i1] = K[10];
		pnW1[11][i1] = 0; pnKW1[11][i1] = K[11]; pnW1[12][i1] = 0; pnKW1[12][i1] = K[12];
		pnW1[13][i1] = 0; pnKW1[13][i1] = K[13]; pnW1[14][i1] = 0; pnKW1[14][i1] = K[14];
		pnW1[15][i1] = 20<<5; pnKW1[15][i1] = pnW1[15][i1] + K[15];

		//先頭の2個はnonceが変化しても変わらない
		for(int t = 16; t < 18; t++)
		{
			pnW1[t][i1] = s1(pnW1[t-2][i1]) + pnW1[t-7][i1] + s0(pnW1[t-15][i1]) + pnW1[t-16][i1];
			pnKW1[t][i1] = pnW1[t][i1] + K[t];
		}

		for (int i1 = 0; i1 < PARANUM; i1++)
		{
			pnPreW1[18][i1] = s1(pnW1[16][i1]) + pnW1[11][i1] + /*nonce s0(pnW1[3][i1]) +*/ pnW1[2][i1];	//s0(W1[3])不足
			pnPreW1[19][i1] = s1(pnW1[17][i1]) + pnW1[12][i1] + s0(pnW1[4][i1]) /*+ pnW1[3][i1]*/; 			//W1[3]不足
			for (int i2 = 20; i2 < 25; i2++)
			{
				pnPreW1[i2][i1] = /*s1(pnW1[i2 - 2 18][i1]) +*/ pnW1[i2 - 7][i1] + s0(pnW1[i2 - 15][i1]) + pnW1[i2 - 16][i1];	//s1(W1[i2-2])不足
			}
			for (int i2 = 25; i2 < 33; i2++)
			{
				pnPreW1[i2][i1] = /*s1(pnW1[i2 - 2][i1]) + pnW1[i2 - 7][i1] +*/ s0(pnW1[i2 - 15][i1]) + pnW1[i2 - 16][i1];	//s1(W1[i2-2]) + W1[i2 - 7]不足
			}
		}

		pnW2[8][i1] = 0x80000000; pnKW2[8][i1] = pnW2[8][i1] + K[8];
		pnW2[9][i1] = 0; pnKW2[9][i1] = K[9]; pnW2[10][i1] = 0; pnKW2[10][i1] = K[10];
		pnW2[11][i1] = 0; pnKW2[11][i1] = K[11]; pnW2[12][i1] = 0; pnKW2[12][i1] = K[12];
		pnW2[13][i1] = 0; pnKW2[13][i1] = K[13]; pnW2[14][i1] = 0; pnKW2[14][i1] = K[14];
		pnW2[15][i1] = 8<<5; pnKW2[15][i1] = pnW2[15][i1] + K[15];

		for (int i1 = 0; i1 < PARANUM; i1++)
		{
			pnPreW2[23][i1] = s0(pnW2[23 - 15][i1]);	//S1 A1 A2不足
			for (int i2 = 24; i2 < 31; i2++)
			{
				pnPreW2[i2][i1] = /*s1(pnW2[i2 - 2][i1]) + pnW2[i2 - 7][i1] +*/ s0(pnW2[i2 - 15][i1]) + pnW2[i2 - 16][i1];	//s1(W2[i2-2]) + W2[i2 - 7]不足
			}
		}
	}

	uint32_t t1core = hash0[7] + S1(hash0[4]) + Ch(hash0[4], hash0[5], hash0[6]);
	uint32_t t2 = S0(hash0[0]) + Maj(hash0[0], hash0[1], hash0[2]);
	i1h7 = hash0[6]; i1h6 = hash0[5]; i1h5 = hash0[4];
	i1h4 = hash0[3] + t1core + pnKW1[0][0];
	i1h3 = hash0[2]; i1h2 = hash0[1]; i1h1 = hash0[0];
	i1h0 = t1core + t2 + pnKW1[0][0];

	for(int i = 1; i < 3; i++)
	{
		uint32_t T1 = i1h7 + S1(i1h4) + Ch(i1h4, i1h5, i1h6) + pnKW1[i][0];
		uint32_t T2 = S0(i1h0) + Maj(i1h0, i1h1, i1h2);
		i1h7 = i1h6; i1h6 = i1h5; i1h5 = i1h4;
		i1h4 = i1h3 + T1;
		i1h3 = i1h2; i1h2 = i1h1; i1h1 = i1h0;
		i1h0 = T1 + T2;
	}
	// nonce part
	uint32_t p3T1 = i1h7 + S1(i1h4) + Ch(i1h4, i1h5, i1h6) /*+ pnKW1[3][0]*/;
	uint32_t p3T2 = S0(i1h0) + Maj(i1h0, i1h1, i1h2);
	i1h7 = i1h6; i1h6 = i1h5; i1h5 = i1h4;
	i1h4 = i1h3 + p3T1;		//i1h4 KW1[3]不足
	i1h3 = i1h2; i1h2 = i1h1; i1h1 = i1h0;
	i1h0 = p3T1 + p3T2;		//i1h0 KW1[3]不足


	t1core = 0x5be0cd19U + S1(0x510e527f) + Ch(0x510e527f, 0x9b05688c, 0x1f83d9ab);
	t2 = S0(0x6a09e667U) + Maj(0x6a09e667U, 0xbb67ae85U, 0x3c6ef372U);
	i2h7 = 0x1f83d9ab; i2h6 = 0x9b05688c; i2h5 = 0x510e527f;
	i2h4 = 0xa54ff53a + t1core /*+ xx*/;
	i2h3 = 0x3c6ef372; i2h2 = 0xbb67ae85; i2h1 = 0x6a09e667;
	i2h0 = t1core /*+ xx*/ + t2;
}


uint32_t W1[64];
//uint32_t W2[64];
uint32_t KW1[64];


int SHA12(uint32_t *hash, uint32_t nonce /*, uint32_t hash0*/)
{
	uint32_t nonceBase = /*(nonce << 24) |*/ ((nonce & 0x0000FF00) <<  8) | ((nonce & 0x00FF0000) >> 8) | (nonce >> 24);
	mmi mH0, mH1, mH2, mH3, mH4, mH5, mH6, mH7;

	for (uint32_t nonce255 = 0x00000000U; ; nonce255 += 0x1000000U * PARANUM)
	{
		for (int i1 = 0; i1 < PARANUM; i1++)
		{
			pnW1[3][i1] = nonceBase | nonce255 + i1 * 0x1000000U;
			///pnW1[3][i1] = nonceBase | nonce255;
			pnKW1[3][i1] = pnW1[3][i1] + K[3];
		}

		mmi mWK = mAdd(mSIGMA0(mLoad(pnW1[3])), mLoad(pnPreW1[18]));
		mStore(pnW1[18], mWK);
		mStore(pnKW1[18], mAdd(mWK, mSet1(K[18])));

		mWK = mAdd(mLoad(pnW1[3]), mLoad(pnPreW1[19]));
		mStore(pnW1[19], mWK);
		mStore(pnKW1[19], mAdd(mWK, mSet1(K[19])));

		for (int i1 = 20; i1 < 25; i1++)
		{
			mWK = mAdd(mSIGMA1(mLoad(pnW1[i1 - 2])), mLoad(pnPreW1[i1]));
			mStore(pnW1[i1], mWK);
			mStore(pnKW1[i1], mAdd(mWK, mSet1(K[i1])));
		}
		for (int i1 = 25; i1 < 33; i1++)
		{
			mWK = mAdd(mAdd(mSIGMA1(mLoad(pnW1[i1 - 2])), mLoad(pnW1[i1 - 7])), mLoad(pnPreW1[i1]));
			mStore(pnW1[i1], mWK);
			mStore(pnKW1[i1], mAdd(mWK, mSet1(K[i1])));
		}

		//先頭の2個はnonceが変化しても変わらない + 差分で可能な計算は上で行う
		for(int t = 16 + 2 + 15; t < 64; t++)
		{
			//pnW1[t][i1] = s1(pnW1[t-2][i1]) + pnW1[t-7][i1] + s0(pnW1[t-15][i1]) + pnW1[t-16][i1];
			//pnKW1[t][i1] = pnW1[t][i1] + K[t];
			mWK = mAdd(mAdd(mSIGMA1(mLoad(pnW1[t-2])), mLoad(pnW1[t-7])), mAdd(mSIGMA0(mLoad(pnW1[t-15])), mLoad(pnW1[t-16])));
			mStore(pnW1[t], mWK);
			mStore(pnKW1[t], mAdd(mWK, mSet1(K[t])));
		}

		//W1[3]以外は、0-17まで同じ
		//mH0 = mSet1(i1h0 + pnKW1[0][0]);	先に加算しておく
		//mH4 = mSet1(i1h4 + pnKW1[0][0]);
		mWK = mLoad(pnKW1[3]);
		mH0 = mAdd(mSet1(i1h0), mWK);
		mH4 = mAdd(mSet1(i1h4), mWK);
		mH1 = mSet1(i1h1);
		mH2 = mSet1(i1h2);
		mH3 = mSet1(i1h3);
		mH5 = mSet1(i1h5);
		mH6 = mSet1(i1h6);
		mH7 = mSet1(i1h7);

		//10秒程度遅くなる、x64だと7秒程度遅くなる、inlineを無くすと早くなる
		SHA256RS1(mH0, mH1, mH2, mH3, mH4, mH5, mH6, mH7, pnKW1[4][0]);
		SHA256RS1(mH7, mH0, mH1, mH2, mH3, mH4, mH5, mH6, pnKW1[5][0]);
		SHA256RS1(mH6, mH7, mH0, mH1, mH2, mH3, mH4, mH5, pnKW1[6][0]);
		SHA256RS1(mH5, mH6, mH7, mH0, mH1, mH2, mH3, mH4, pnKW1[7][0]);
		SHA256RS1(mH4, mH5, mH6, mH7, mH0, mH1, mH2, mH3, pnKW1[8][0]);
		SHA256RS1(mH3, mH4, mH5, mH6, mH7, mH0, mH1, mH2, pnKW1[9][0]);
		SHA256RS1(mH2, mH3, mH4, mH5, mH6, mH7, mH0, mH1, pnKW1[10][0]);
		SHA256RS1(mH1, mH2, mH3, mH4, mH5, mH6, mH7, mH0, pnKW1[11][0]);

		SHA256RS1(mH0, mH1, mH2, mH3, mH4, mH5, mH6, mH7, pnKW1[12][0]);
		SHA256RS1(mH7, mH0, mH1, mH2, mH3, mH4, mH5, mH6, pnKW1[13][0]);
		SHA256RS1(mH6, mH7, mH0, mH1, mH2, mH3, mH4, mH5, pnKW1[14][0]);
		SHA256RS1(mH5, mH6, mH7, mH0, mH1, mH2, mH3, mH4, pnKW1[15][0]);
		SHA256RS1(mH4, mH5, mH6, mH7, mH0, mH1, mH2, mH3, pnKW1[16][0]);
		SHA256RS1(mH3, mH4, mH5, mH6, mH7, mH0, mH1, mH2, pnKW1[17][0]);
		SHA256RLD(mH2, mH3, mH4, mH5, mH6, mH7, mH0, mH1, pnKW1[18]);
		SHA256RLD(mH1, mH2, mH3, mH4, mH5, mH6, mH7, mH0, pnKW1[19]);

		SHA256RLD(mH0, mH1, mH2, mH3, mH4, mH5, mH6, mH7, pnKW1[20]);
		SHA256RLD(mH7, mH0, mH1, mH2, mH3, mH4, mH5, mH6, pnKW1[21]);
		SHA256RLD(mH6, mH7, mH0, mH1, mH2, mH3, mH4, mH5, pnKW1[22]);
		SHA256RLD(mH5, mH6, mH7, mH0, mH1, mH2, mH3, mH4, pnKW1[23]);
		SHA256RLD(mH4, mH5, mH6, mH7, mH0, mH1, mH2, mH3, pnKW1[24]);
		SHA256RLD(mH3, mH4, mH5, mH6, mH7, mH0, mH1, mH2, pnKW1[25]);
		SHA256RLD(mH2, mH3, mH4, mH5, mH6, mH7, mH0, mH1, pnKW1[26]);
		SHA256RLD(mH1, mH2, mH3, mH4, mH5, mH6, mH7, mH0, pnKW1[27]);

		SHA256RLD(mH0, mH1, mH2, mH3, mH4, mH5, mH6, mH7, pnKW1[28]);
		SHA256RLD(mH7, mH0, mH1, mH2, mH3, mH4, mH5, mH6, pnKW1[29]);
		SHA256RLD(mH6, mH7, mH0, mH1, mH2, mH3, mH4, mH5, pnKW1[30]);
		SHA256RLD(mH5, mH6, mH7, mH0, mH1, mH2, mH3, mH4, pnKW1[31]);
		SHA256RLD(mH4, mH5, mH6, mH7, mH0, mH1, mH2, mH3, pnKW1[32]);
		SHA256RLD(mH3, mH4, mH5, mH6, mH7, mH0, mH1, mH2, pnKW1[33]);
		SHA256RLD(mH2, mH3, mH4, mH5, mH6, mH7, mH0, mH1, pnKW1[34]);
		SHA256RLD(mH1, mH2, mH3, mH4, mH5, mH6, mH7, mH0, pnKW1[35]);

		SHA256RLD(mH0, mH1, mH2, mH3, mH4, mH5, mH6, mH7, pnKW1[36]);
		SHA256RLD(mH7, mH0, mH1, mH2, mH3, mH4, mH5, mH6, pnKW1[37]);
		SHA256RLD(mH6, mH7, mH0, mH1, mH2, mH3, mH4, mH5, pnKW1[38]);
		SHA256RLD(mH5, mH6, mH7, mH0, mH1, mH2, mH3, mH4, pnKW1[39]);
		SHA256RLD(mH4, mH5, mH6, mH7, mH0, mH1, mH2, mH3, pnKW1[40]);
		SHA256RLD(mH3, mH4, mH5, mH6, mH7, mH0, mH1, mH2, pnKW1[41]);
		SHA256RLD(mH2, mH3, mH4, mH5, mH6, mH7, mH0, mH1, pnKW1[42]);
		SHA256RLD(mH1, mH2, mH3, mH4, mH5, mH6, mH7, mH0, pnKW1[43]);

		SHA256RLD(mH0, mH1, mH2, mH3, mH4, mH5, mH6, mH7, pnKW1[44]);
		SHA256RLD(mH7, mH0, mH1, mH2, mH3, mH4, mH5, mH6, pnKW1[45]);
		SHA256RLD(mH6, mH7, mH0, mH1, mH2, mH3, mH4, mH5, pnKW1[46]);
		SHA256RLD(mH5, mH6, mH7, mH0, mH1, mH2, mH3, mH4, pnKW1[47]);
		SHA256RLD(mH4, mH5, mH6, mH7, mH0, mH1, mH2, mH3, pnKW1[48]);
		SHA256RLD(mH3, mH4, mH5, mH6, mH7, mH0, mH1, mH2, pnKW1[49]);
		SHA256RLD(mH2, mH3, mH4, mH5, mH6, mH7, mH0, mH1, pnKW1[50]);
		SHA256RLD(mH1, mH2, mH3, mH4, mH5, mH6, mH7, mH0, pnKW1[51]);

		SHA256RLD(mH0, mH1, mH2, mH3, mH4, mH5, mH6, mH7, pnKW1[52]);
		SHA256RLD(mH7, mH0, mH1, mH2, mH3, mH4, mH5, mH6, pnKW1[53]);
		SHA256RLD(mH6, mH7, mH0, mH1, mH2, mH3, mH4, mH5, pnKW1[54]);
		SHA256RLD(mH5, mH6, mH7, mH0, mH1, mH2, mH3, mH4, pnKW1[55]);
		SHA256RLD(mH4, mH5, mH6, mH7, mH0, mH1, mH2, mH3, pnKW1[56]);
		SHA256RLD(mH3, mH4, mH5, mH6, mH7, mH0, mH1, mH2, pnKW1[57]);
		SHA256RLD(mH2, mH3, mH4, mH5, mH6, mH7, mH0, mH1, pnKW1[58]);
		SHA256RLD(mH1, mH2, mH3, mH4, mH5, mH6, mH7, mH0, pnKW1[59]);

		SHA256RLD(mH0, mH1, mH2, mH3, mH4, mH5, mH6, mH7, pnKW1[60]);
		SHA256RLD(mH7, mH0, mH1, mH2, mH3, mH4, mH5, mH6, pnKW1[61]);
		SHA256RLD(mH6, mH7, mH0, mH1, mH2, mH3, mH4, mH5, pnKW1[62]);
		SHA256RLD(mH5, mH6, mH7, mH0, mH1, mH2, mH3, mH4, pnKW1[63]);
		// mH0 → mH4

		///////////////////////////////////////////////////////////////////////////////
		// ここからSHA256 2回目
		///////////////////////////////////////////////////////////////////////////////

		mmi mW2 = mAdd(mH4, mSet1(hash0[0]));
		mStore(pnW2[0], mW2);
		mStore(pnKW2[0], mAdd(mW2, mSet1(K[0])));

		mW2 = mAdd(mH5, mSet1(hash0[1]));
		mStore(pnW2[1], mW2);
		mStore(pnKW2[1], mAdd(mW2, mSet1(K[1])));

		mW2 = mAdd(mH6, mSet1(hash0[2]));
		mStore(pnW2[2], mW2);
		mStore(pnKW2[2], mAdd(mW2, mSet1(K[2])));

		mW2 = mAdd(mH7, mSet1(hash0[3]));
		mStore(pnW2[3], mW2);
		mStore(pnKW2[3], mAdd(mW2, mSet1(K[3])));

		mW2 = mAdd(mH0, mSet1(hash0[4]));
		mStore(pnW2[4], mW2);
		mStore(pnKW2[4], mAdd(mW2, mSet1(K[4])));

		mW2 = mAdd(mH1, mSet1(hash0[5]));
		mStore(pnW2[5], mW2);
		mStore(pnKW2[5], mAdd(mW2, mSet1(K[5])));

		mW2 = mAdd(mH2, mSet1(hash0[6]));
		mStore(pnW2[6], mW2);
		mStore(pnKW2[6], mAdd(mW2, mSet1(K[6])));

		mW2 = mAdd(mH3, mSet1(hash0[7]));
		mStore(pnW2[7], mW2);
		mStore(pnKW2[7], mAdd(mW2, mSet1(K[7])));

		mW2 = mH0;
		mH0 = mH4;
		mH4 = mW2;

		for(int t = 16; t < 23; t++)
		{
			mmi mW2 = mAdd(mAdd(mSIGMA1(mLoad(pnW2[t-2])), mLoad(pnW2[t-7])), mAdd(mSIGMA0(mLoad(pnW2[t-15])), mLoad(pnW2[t-16])));
			mStore(pnW2[t], mW2);
			mStore(pnKW2[t], mAdd(mW2, mSet1(K[t])));
		}
		{	//23のみ
			mmi mW2 = mAdd(mAdd(mSIGMA1(mLoad(pnW2[23-2])), mLoad(pnW2[23-7])), mAdd(mLoad(pnPreW2[23]), mLoad(pnW2[23-16])));	//S0のみ計算済み
			mStore(pnW2[23], mW2);
			mStore(pnKW2[23], mAdd(mW2, mSet1(K[23])));
		}

		for (int t = 24; t < 31; t++)
		{
			mmi mW2 = mAdd(mAdd(mSIGMA1(mLoad(pnW2[t-2])), mLoad(pnW2[t-7])), mLoad(pnPreW2[t]));	//S0+A2のみ計算済み
			mStore(pnW2[t], mW2);
			mStore(pnKW2[t], mAdd(mW2, mSet1(K[t])));
		}

		for(int t = 31; t < 64; t++)
		{
			mmi mW2 = mAdd(mAdd(mSIGMA1(mLoad(pnW2[t-2])), mLoad(pnW2[t-7])), mAdd(mSIGMA0(mLoad(pnW2[t-15])), mLoad(pnW2[t-16])));
			mStore(pnW2[t], mW2);
			mStore(pnKW2[t], mAdd(mW2, mSet1(K[t])));
		}


		mWK = mAdd(mH0, mSet1(hash0[0]));
		mStore(pnW2[0], mWK);
		//mH0とmH4は、pnW2[0]を加算する
		mH0 = mAdd(mWK, mSet1(/*K[0]*/0x428a2f98));
		mWK = mAdd(mH4, mSet1(hash0[4]));
		mStore(pnW2[4], mWK);

		mH4 = mAdd(mH0, mSet1(i2h4));
		mH0 = mAdd(mH0, mSet1(i2h0));

		//mH0 = _mm_set_epi32(i2h0 + pnKW2[0][3], i2h0 + pnKW2[0][2], i2h0 + pnKW2[0][1], i2h0 + pnKW2[0][0]);
		mH1 = mSet1(i2h1);
		mH2 = mSet1(i2h2);
		mH3 = mSet1(i2h3);
		//mH4 = _mm_set_epi32(i2h4 + pnKW2[0][3], i2h4 + pnKW2[0][2], i2h4 + pnKW2[0][1], i2h4 + pnKW2[0][0]);
		mH5 = mSet1(i2h5);
		mH6 = mSet1(i2h6);
		mH7 = mSet1(i2h7);

		for(int i = 1/*49*/; i < 64 - 10; i++)
		{
			mmi mKW2 = mLoad(pnKW2[i]);
			mmi mT1 = mAdd(mAdd(mAdd(mH7, mBIGSIGMA1(mH4)), mCh(mH4, mH5, mH6)), mKW2);
			mmi mT2 = mAdd(mBIGSIGMA0(mH0), mMaj(mH0, mH1, mH2));
			mH7 = mH6; mH6 = mH5; mH5 = mH4;
			mH4 = mAdd(mH3, mT1);
			mH3 = mH2; mH2 = mH1; mH1 = mH0;
			mH0 = mAdd(mT1, mT2);
		}

		mWK = mAdd(mAdd(mAdd(mH7, mBIGSIGMA1(mH4)), mCh(mH4, mH5, mH6)), mLoad(pnKW2[54]));
		mmi mAAT12 = mAdd(mAdd(mBIGSIGMA0(mH0), mMaj(mH0, mH1, mH2)), mWK);
		mmi mAAAdd = mAdd(mH3, mWK);

		mWK = mAdd(mAdd(mAdd(mH6, mBIGSIGMA1(mAAAdd)), mCh(mAAAdd, mH4, mH5)), mLoad(pnKW2[55]));
		mmi mBBT12 = mAdd(mAdd(mBIGSIGMA0(mAAT12), mMaj(mAAT12, mH0, mH1)), mWK);
		mmi mBBAdd = mAdd(mH2, mWK);

		mWK = mAdd(mAdd(mAdd(mH5, mBIGSIGMA1(mBBAdd)), mCh(mBBAdd, mAAAdd, mH4)), mLoad(pnKW2[56]));
		mmi mCCT12 = mAdd(mAdd(mBIGSIGMA0(mBBT12), mMaj(mBBT12, mAAT12, mH0)), mWK);
		mmi mCCAdd = mAdd(mH1, mWK);

		mWK = mAdd(mAdd(mAdd(mH4, mBIGSIGMA1(mCCAdd)), mCh(mCCAdd, mBBAdd, mAAAdd)), mLoad(pnKW2[57]));
		mmi mDDAdd = mAdd(mH0, mWK);

		mWK = mAdd(mAdd(mAdd(mAAAdd, mBIGSIGMA1(mDDAdd)), mCh(mDDAdd, mCCAdd, mBBAdd)), mLoad(pnKW2[58]));
		mAAAdd = mAdd(mAAT12, mWK);

		mWK = mAdd(mAdd(mAdd(mBBAdd, mBIGSIGMA1(mAAAdd)), mCh(mAAAdd, mDDAdd, mCCAdd)), mLoad(pnKW2[59]));
		mBBAdd = mAdd(mBBT12, mWK);

		mmi mResult = mAdd(mAdd(mAdd(mCCAdd, mCCT12), mBIGSIGMA1(mBBAdd)), mCh(mBBAdd, mAAAdd, mDDAdd));


		int allZero = 0;
		uint32_t result[PARANUM];
		mStore(result, mResult);

		for (int i1 = 0; i1 < PARANUM; i1++)
		{
			uint32_t zeroValue = -0x5be0cd19 - /*(pnW2[60][i1] + K[60])*/pnKW2[60][i1];
			if (result[i1] == zeroValue)
			{
				allZero = -1;
				break;
			}
		}

		if (allZero == 0)
		{
			if ((nonce255 + (PARANUM - 1) * 0x1000000U) == 0xFF000000U)
			{
				return 0;
			}
			continue;
		}

		for(int i = 64 - 10; i < 64; i++)
		{
			mmi mKW2 = mAdd(mLoad(pnW2[i]), mSet1(K[i]));
			mmi mT1 = mAdd(mAdd(mAdd(mH7, mBIGSIGMA1(mH4)), mCh(mH4, mH5, mH6)), mKW2);
			mmi mT2 = mAdd(mBIGSIGMA0(mH0), mMaj(mH0, mH1, mH2));
			mH7 = mH6; mH6 = mH5; mH5 = mH4;
			mH4 = mAdd(mH3, mT1);
			mH3 = mH2; mH2 = mH1; mH1 = mH0;
			mH0 = mAdd(mT1, mT2);
		}

		for (int i1 = 0; i1 < PARANUM; i1++)
		{
			if (store32(mH7, i1) != -0x5be0cd19 && store32(mH6, i1) != 0x1f83d9ab)
			{
				if ((nonce255 + i1 * 0x1000000U) == 0xFF000000U)
				{
					return 0;
				}
			}
			else
			{
				hash[0] = 0x6a09e667 + store32(mH0, i1); hash[1] = 0xbb67ae85 + store32(mH1, i1); hash[2] = 0x3c6ef372 + store32(mH2, i1); hash[3] = 0xa54ff53a + store32(mH3, i1);
				hash[4] = 0x510e527f + store32(mH4, i1); hash[5] = 0x9b05688c + store32(mH5, i1); hash[6] = 0x00/*0x1f83d9ab + h6*/; hash[7] = 0x00; /*0x5be0cd19 + h7*/;
				return 1;
			}
		}
	}
	return 0;
}



int main()
{
#if 0
	uint32_t Version     = 3;
	const char *PrevBlock  = "00000000000000000e8d3266c478c3102a7b54582fb90963aa0bed93747dff0d";
	const char *MerkleRoot = "36a37dc0564f1496e74342d96ba7099d987e482891d7954ebe7188f5f5074e0f";
	uint32_t TimeStamp   = 0/*1443323701*/;	/**2015/09/27 03:15:01**/
	uint32_t Difficulty  =  403867578;
	uint32_t Nonce       = 1082298256;
#endif

	uint32_t Version	 = 0x20000000;
	const char *PrevBlock  = "000000000000000000013ac78094ea7584a6d87020d33d12ad10c87752cf188d";
	const char *MerkleRoot = "93371570b3f7c9bed52a464c4af77b25d57313d91789cab4e5d36f874c3bbceb";
	uint32_t TimeStamp   = 0/*1443323701*/;	/**2015/09/27 03:15:01**/
	uint32_t Difficulty  =  387124344;
	uint32_t Nonce       = 3745729861;	//0xDF434D45

	time_t lt = Str2Time(2020, 1, 26, 6, 56, 16);
	TimeStamp = (uint32_t)lt;
	struct tm *ltm;
	char str[256];
	ltm = localtime/*gmtime*/(&lt);
	strftime(str, sizeof(str), "%Y/%m/%d %H:%M:%S", ltm);
	puts(str);

	uint32_t M[20], T[8];
	Set(M, Version, PrevBlock, MerkleRoot, TimeStamp, Difficulty, Nonce);
	Target(T, Difficulty);

	puts("-----------------------------------------------------------------------------");
	printf("Version	: "); Show(M, 1);
	printf("PrevBlock  : "); Show(M+1, 8);
	printf("MerkleRoot : "); Show(M+9, 8);
	printf("TimeStamp  : "); Show(M+17, 1);
	printf("Difficulty : "); Show(M+18, 1);
	printf("Nonce      : "); Show(M+19, 1);
	printf("Target     : "); Show(T, 8);
	puts("-----------------------------------------------------------------------------");

	uint32_t Hash[8];

	////Show(M, 20);

	SHA0(/*hash0,*/ M /*, 1*/);

	time_t start = time(NULL);
	//for (uint32_t tryNonce = 3745729861 - 0xFFFFFF; tryNonce <= 0xFFFFFFFF; tryNonce++)
	//for (uint32_t tryNonce = 3745729861 - /*0xFFFFFF*//*100*/3; tryNonce <= 0xFFFFFFFF; tryNonce++)
	//for (uint32_t tryNonce = ((3745729861 - 1000) & 0xFFFFFF00U); tryNonce <= 0xDFFFFFFFU; tryNonce += 0x100U)
	for (uint32_t tryNonce = /*0*/0xD0000000U; tryNonce <= 0xFFFFFFFFU; tryNonce += 0x100U)
	{
		if ((tryNonce & 0xFFFFFFU) == 0)
		{
			printf("Nonce:%x\n", tryNonce);
		}
		if (SHA12(Hash, tryNonce) == 0)
		{
			continue;
		}

		if (CompHash(Hash, T) < 0)
		{
			printf("BlockHash  : "); Show(Hash, 8);
			printf("Time       : %lld\n",  (time(NULL) - start));
			break;
		}
	}
	printf("Time       : %lld\n",  (time(NULL) - start));

	return 0;
}

// 256,068,033 = F434D45
// 155 -> 112 -> 62(4.13MH/s 全SSE化 - ループ使用) -> 59(4,34MH/s W1作成ループ削減) -> 58(nonceまでの計算削除)
// i3 8130U(PassMark:3677) 26(9.85MH/s - AVX2使用) -> 24(10.67MH/s)
