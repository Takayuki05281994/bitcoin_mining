# bitcoin_mining
ビットコインマイニングの高速化にチャレンジしました。

## 概要
ビットコイン、ブロックチェーン、マイニングといったキーワードを見る機会が多くなってきましたが、実際のところ何を意味しているか良く知りませんでした。
これらの概要を調べおおよその概要がわかったので、マイニングの高速かにチャレンジしてみました。
## 構築環境
VisualStudio 2022で最適化オプションは速度優先に設定しました。
## 実行環境
CPUはAMD Ryzen 7 3700X 8-Core Processorを使用しました。

## 実行結果・考察など

https://kobayashi.hub.hit-u.ac.jp/topics/hash.html  
このページのプログラムが、わかりやすかったので使用させていただきました。  

Step.1  
	uint32_t Version     = 3;  
	const char *PrevBlock  = "00000000000000000e8d3266c478c3102a7b54582fb90963aa0bed93747dff0d";  
	const char *MerkleRoot = "36a37dc0564f1496e74342d96ba7099d987e482891d7954ebe7188f5f5074e0f";  
	uint32_t TimeStamp   = 1443323701;  
	uint32_t Difficulty  =  403867578;  
	uint32_t Nonce       = 1082298256;  

参考ページのプログラムは上記の履歴データで検証しています。  
まずは、参考にしたプログラムが他の履歴データでも正しく動作するか試してみました。  

	uint32_t Version     = 0x20000000;    
	const char *PrevBlock  = "000000000000000000013ac78094ea7584a6d87020d33d12ad10c87752cf188d";   
	const char *MerkleRoot = "93371570b3f7c9bed52a464c4af77b25d57313d91789cab4e5d36f874c3bbceb";  
	uint32_t TimeStamp   = 1443323701;  
	uint32_t Difficulty  =  387124344;  
	uint32_t Nonce       = 3745729861;  

問題なく動作しました。  

Step.2  
参考にしたプログラムは、正解となったNounce値からハッシュ値を求めているので、ハッシュレートは求められません。  
Nonceを0からループで回して正解がヒットするように改造しました。


	time_t start = time(NULL);  
	for (uint32_t tryNonce = 0xD0000000U; tryNonce <= 0xFFFFFFFF; tryNonce++)  
	{  
		if ((tryNonce & 0xFFFFFFU) == 0)  
		{  
			printf("Nonce:%x\n", tryNonce);  
		}  
		Set(M, Version, PrevBlock, MerkleRoot, TimeStamp, Difficulty, tryNonce);  
		SHA(Hash, M, 1);  
		SHA(Hash, Hash, 2);  

		if (CompHash(Hash, T) < 0)  
		{  
			cout << "BlockHash  : "; Show(Hash, 8);  
			printf("Time       : %lld\n",  (time(NULL) - start));  
			break;  
		}  
	}  


for文の「tryNonce = 0xD0000000U」は「tryNonce = 0U」としていたのですが、余りに時間がかかってしまので0xD0000000としました。  

Step.3  
アルゴリズムの見直しを行いました。  
(1)毎回最初に実行される同じコードがあるので、それを外出しにしました。  
(2)最終的なハッシュ時の上位ビットは、0がセットさているので最後の何回かのループを削減可能でした。  


Step.4  
高速化の技法を調べ以下のような改造を加えてみました。  
(1)ループを展開  
(2)ストリーミングSIMD拡張命令の利用  
AVX2というストリーミングSIMD拡張命令(AVX-512は何故か動作しませんでした)を使用しました。  


検証結果  
・オリジナル  
ハッシュレート1.16MH/s  
・最終版  
ハッシュレート10.07MH/s  

マルチコアCPUなので、コア数分だけ同時に実行しても性能は劣化しない。  
最近のCPUは10コア程度はあるので、100MH/sが実現できると考えられる。  

10倍のハッシュレートを実現できたが、プログラムの可読性は最悪となった。  
しばらくして見直すと、自分でも訳がわからくなる程の醜いプログラムとなった。  
NVIDIA GPUのCudaを使用して、並列処理を増やすことができるらしいが、グラボを持っていないので試せませんでした。  

### オリジナルモデル
![Image](https://github.com/user-attachments/assets/0759eab5-7b9c-44ed-bcf8-0d9da6af700e)

### 高速化モデル
![Image](https://github.com/user-attachments/assets/a3448395-b201-4e4c-b3c0-1daafaa40188)
