#include "btcmdef.h"


void Show(uint32_t *M, int Len)
{
    for(int i=Len-1; i>=0; i--)
	{
        for(int j=0; j<4; j++)
		{
			printf("%02x", ((M[i]>>(8*j))&0xff));
            //cout << setw(2) << setfill('0') << hex << ((M[i]>>(8*j))&0xff);
        }
    }
    //cout << endl;
	printf("\n");
}


int	CompHash(uint32_t *c1, uint32_t *c2)
{
	for (int i1 = 7; i1 >= 0; i1--)
	{
		if (c1[i1] == c2[i1])
		{
			continue;
		}
		if (c1[i1] > c2[i1])
		{
			return 1;
		}
		if (c1[i1] < c2[i1])
		{
			return -1;
		}
	}
	return 0;
}


// 0-9,a-zの文字を数値に変換する関数
int c2i(char c)
{
    if('0'<=c && c<='9'){
        return c-'0';
    } else if('a'<=c && c<='f'){
        return c-'a'+10;
    } else if('A'<=c && c<='F'){
        return c-'A'+10;
    }
    return 0;
}

// ブロックの各フィールドにデータを格納
void Set(uint32_t *M, uint32_t Version, const char *PrevBlock, const char *MerkleRoot, uint32_t TimeStamp, uint32_t Difficulty, uint32_t Nonce)
{
	memset(M, 0x00, sizeof(M));
    const char *Ptr[2] = {PrevBlock, MerkleRoot};
    for(int t=0; t<2; t++)
	{
        int k = 64;
        for(int j=t*8+1; j<=t*8+8; j++)
		{
            for(int i=0; i<4; i++)
			{
                char c0 = c2i(Ptr[t][--k]);
                char c1 = c2i(Ptr[t][--k]);
                M[j] = (M[j]<<8) + (c1<<4) + c0;
            }
        }
    }

    uint32_t Value[4] = {Version, TimeStamp, Difficulty, Nonce};
    int Pos[4] = {0, 17, 18, 19};

    for(int t=0; t<4; t++)
	{
        int ps = Pos[t];
        for(int i=0; i<32; i+=8)
		{
            M[ps] = (M[ps]<<8) + (0xff & (Value[t]>>i));
        }
    }
}


void Target(uint32_t *T, uint32_t targ)
{
    for(int i=0; i<8; i++){
        T[i] = 0;
    }
    int s = (targ>>24)-3;
    if(s>=0 && s<30){
        for(int i=0; i<3; i++, s++){
            T[s>>2] += ((targ>>(i*8))&0xff)<<(((~s)&3)*8);
        }
    }
}


time_t Str2Time(int year, int month, int day, int hour, int min, int sec)
{
		time_t mk;
		struct tm tm_mk;

		memset(&tm_mk, 0x00, sizeof(tm_mk));
		tm_mk.tm_year = year - 1900;
		tm_mk.tm_mon = month - 1;
		tm_mk.tm_mday = day;
		tm_mk.tm_hour = hour;
		tm_mk.tm_min = min;
		tm_mk.tm_sec = sec;
		tm_mk.tm_isdst = -1;
		mk = mktime(&tm_mk);
		return mk - _timezone;
}


void dumpreg(__m128i x, char *msg)
{
	union { uint32_t ret[4]; __m128i x; } box;
	box.x = x ;
	printf("%s %08x %08x %08x %08x\n", msg, box.ret[0], box.ret[1], box.ret[2], box.ret[3]);
}
