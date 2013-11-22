/*
----class/struct
dot
	1ドットを表す構造体
	b1u_t b; //blue
	b1u_t g; //green
	b1u_t r; //red
	b1u_t a; //alpha

logic_error
	実行以前のエラーに起因する例外

domain_error
	実行時エラーに起因する例外

mem_error
	domain_error
	メモリアロケーションに関するエラー

BitCountError
	logic_error
	色数に関するエラー

----算術型
b4_t
b4u_t
	4バイトの型

b2_t
b2u_t
	2バイトの型

b1_t
b1u_t
	1バイトの型

----enum
ColorNum::b1
ColorNum::b4
ColorNum::b8
ColorNum::b24
ColorNum::b32
	1bit 4bit 8bit 24bit 32bitを表すenum

ColorNum::n2
ColorNum::n16
ColorNum::n256
	2色(1bit) 16色(2bit) 256色(8bit)を表すenum

----method
ctor()
	使用禁止

ctor(
	const b4_t BitCount_,
	const b4_t Width_,
	const b4_t Height_,
	const b4_t Xppm_ = 0,
	const b4_t Yppm_ = 0,
	const b4_t Compression_ = 0
)
ctor(const char *const path)
ctor(const char *const path)
ctor(std::basic_ifstream<T, U> &f)
	Xppm_及びYppm_はオプションで指定する
	Compression_が存在する意味は今の所ない
	[BitCount_]    ビット深度
	[Width_]       幅
	[Height_]      高さ
	[Xppm_]        Xのpx/meter
	[Yppm_]        Yのpx/meter
	[Compression_] 圧縮は無効
	[path]         開くファイルのパス
	[f]            開くファイルのfstream

virtual dtor()
	パレット配列と画素配列を開放する

static dot rgb(int r, int g, int b)
static dot rgb(int r, int g, int b, int a)
	色を作ってdotを返す
	引数が3つのバージョンはalpha成分が未定
	[r] 赤成分
	[g] 緑成分
	[b] 青成分
	[a] alpha成分

int width() const
int width(int v)
int width(int v, int i)
int width(int v, dot d)
int height() const
int height(int v)
int height(int v, int i)
int height(int v, dot d)
	幅/高さを得る/設定する
	設定関数は最大で設定前の画像データ + 設定後の画像データの分だけ
	ヒープ領域を消費するコストの高い関数であるが取得関数は
	メンバの値を返すだけである
	設定関数でパレットインデックスかdotを指定すると
	新たにできた空白領域がそのデータで埋められる
	指定しなかった場合は未定義となる
	空白領域が発生しない場合には意味がないものとなる
	パレットの必要な呼び出しで然るべき状態でないとw/hそれぞれ
		* 例外 width_plt_set_BitCountError (BitCountError)
		* 例外 height_plt_set_BitCountError (BitCountError)
	truecolor でないといけない呼び出しでtruecolorでないとw/hそれぞれ
		* 例外 width_trueclr_set_BitCountError (BitCountError)
		* 例外 height_trueclr_set_BitCountError (BitCountError)
	不正なパレットインデックスを渡すとw/hそれぞれ
		* 例外 width_plt_set_OOR (logic_error)
		* 例外 height_plt_set_OOR (logic_error)
	を送出する
	[v] 新しく設定する値
	[i] 空白領域を埋めるのに使うパレットインデックス
	[d] 空白領域を埋めるのに使う色情報

array_t<int, 2> resize(int w_, int h_)
array_t<int, 2> resize(int w_, int h_, int i)
array_t<int, 2> resize(int w_, int h_, dot d)
	幅/高さを設定し設定前の値を返す
	空白領域が発生する場合はなにで塗りつぶすかを指定できる
	指定しなかった場合の空白領域のデータは未定義
	パレットが必要な呼び出しで然るべき状態でないと
		* 例外 resize_plt_BitCountError (BitCountError)
	truecolor でないといけない呼び出しでtruecolorでないと
		* 例外 resize_trueclr_BitCountError (BitCountError)
	不正なパレットインデックスを渡すと
		* 例外 resize_plt_OOR (logic_error)
	を送出する
	[w_] 新しい幅
	[h_] 新しい高さ
	[i]  塗りつぶすパレットナンバー
	[d]  塗りつぶす色

int xppm()const
int xppm(int v)
int yppm()const
int yppm(int v)
	幅/高さを得る/設定する
	設定関数は設定する前の値を返す
	[v] 新しい値

int pltnum() const
	パレットの数を得る
	パレットを持っていないと例外 pltnum_BitCountError (BitCountError) を送出する

dot setplt(int i, dot d)
	パレットのi番目の色をdに設定する
	パレットを持っていないと
		* 例外 setplt_BitCountError (BitCountError)
	範囲外アクセスが起こると
		* 例外 setplt_OOR (logic_error) を送出する
	[i] 変更したい色のパレット上の位置
	[d] 色

int pltidx(int x, int y) const
int pltidx(int x, int y, int pltnum)
	指定した座標のパレットインデックスを得る/設定する
	設定用の関数では設定する前の値を返す
	パレットを持っていないと
		* 取得用の関数では例外 pltidx_get_BitCountError (BitCountError)
		* 設定用の関数では例外 pltidx_set_BitCountError (BitCountError)
	を送出する
	[x]      x座標
	[y]      y座標
	[pltnum] パレットのナンバー

dot clr(int x, int y) const
dot clr(int x, int y, dot d)
	指定座標の色を取得/設定する
	取得はパレットの所持非所持を問わず全色数で取得可能
	設定関数でパレットを所持していた場合
		* 例外 clr_set_BitCountError (BitCountError)
	を送出する

void line(int xa, int ya, int xb, int yb, dot d)
void line(int xa, int ya, int xb, int yb, int idx)
	線を引く

template<typename T, typenaem U>
	void write(basic_ofstream<T, U> &f) const
void write(const char* const path) const
void write(const char* const path) const
	ファイルに出力する
	[path] ファイルパス
	[f]    出力対象
*/

#ifndef __TTY_BMP__
#define __TTY_BMP__

#include <cmath>
#include <cstring>
#include <string>
#include <fstream>
#include <utility>
#include <exception>
#include <locale>

#include <tty/st.hpp>

namespace tty{

#ifdef _MSC_VER
#pragma warning(disable:4290)
#pragma warning(disable:4715)
#endif

#ifndef _MSC_VER
#define _DEBUG
#endif

	template<typename dammy_t>
	class xbmp{
		//--------公開型
	public:
		typedef int
			b4_t;	//4バイト
		typedef unsigned int
			b4u_t;	//4バイト unsigned

		typedef short
			b2_t;	//2バイト
		typedef unsigned short
			b2u_t;	//2バイト unsigned

		typedef char
			b1_t;	//1バイト
		typedef unsigned char
			b1u_t;	//1バイト unsigned

		//1ドットの型
		struct dot{
			b1u_t b;
			b1u_t g;
			b1u_t r;
			b1u_t a;

			bool operator ==(const dot &d){
				return (b == d.b) && (g == d.g) && (r == d.r) && (a == d.a);
			}

			bool operator !=(const dot &d){
				return (b != d.b) && (g != d.g) && (r != d.r) && (a != d.a);
			}
		};

		//色数
		struct ColorNum{
			enum{
				b1 = 1,
				b4 = 4,
				b8 = 8,
				b24 = 24,
				b32 = 32,

				n2 = 1,
				n16 = 4,
				n256 = 8
			};
		};

		//--------例外クラスなどの定義
	private:
		struct exception_{
			//例外メッセージ
			virtual const char *msg() const{ return "情報なし"; }
			//例外が出たところ
			virtual const char *prfn() const{ return "情報なし"; }
		};

	public:
		//実行より前のエラーに起因する例外
		struct logic_error : exception_{
		};

		//実行時のエラーに起因する例外
		struct domain_error : exception_{
		};

		//メモリアロケーションエラー
		struct mem_error : virtual exception_{
		};

		//色数エラー
		struct BitCountError : logic_error{
			virtual const char *msg() const{ return "ビットマップの色数エラー。"; }
		};

		//--------非公開メンバ
	private:
		//ファイルヘッダ
		char FileHeader[14];
		b4_t &FileSize_;	//ファイルサイズ byte
		b4_t &OffBits;		//ファイル先頭から画像データまでのオフセット

		//情報ヘッダ
		//非OS/2限定
		char InfomationHeader[40];
		b4_t &Width_;		//幅
		b4_t &Height_;		//高さ
		b2_t &BitCount_;	//ビットカウント
		b4_t &Compression_;	//圧縮形式
		b4_t &ImgDataSize_;	//画像データ部のサイズ
		b4_t &Xppm_;		//横方向の解像度 px/Meter
		b4_t &Yppm_;		//縦方向の解像度 px/Meter
		b4_t &ClrNum;		//パレットの使用色数
		b4_t &ImportantClr;	//重要なパレットのインデックス

		//カラーパレット
		dot *ColorPalette_;

		//画像データ
		char *ImgData_;

		//データ上の横幅
		//4バイトの倍数になってる
		b4_t width_byte;

		//pltidxの各BitCountに対応したメンバポインタを保持する
		typedef int (xbmp::*pltidx_get_t)(int, int) const;
		typedef int (xbmp::*pltidx_set_t)(int, int, int);
		pltidx_get_t pltidx_get_;
		pltidx_set_t pltidx_set_;

		//clr
		typedef dot (xbmp::*clr_get_t)(int, int) const;
		typedef dot (xbmp::*clr_set_t)(int, int, dot);
		clr_get_t clr_get_;
		clr_set_t clr_set_;

		//--------非公開メソッド
	private:
		//dotを生成する
		static dot dot_(){
			dot d;
			d.b = d.g = d.r = d.a = 0;
			return d;
		}

		static dot dot_(b1u_t r, b1u_t g, b1u_t b){
			dot d;
			d.r = r, d.g = g, d.b = b, d.a = 0;
			return d;
		}

		static dot dot_(b1u_t r, b1u_t g, b1u_t b, b1u_t a){
			dot d;
			d.r = r, d.g = g, d.b = b, d.a = a;
			return d;
		}

		static dot dot_(const dot &d_){
			dot d;
			d.r = d_.r, d.g = d_.g, d.b = d_.b, d.a = d_.a;
			return d;
		}

		//width_byteを決定する
		static b4_t width_byte_b1(b4_t w){
			int a;
			a = w / 8 + (w % 8 ? 1 : 0);
			a = a / 4 + (a % 4 ? 1 : 0);
			a *= 4;
			return a;
		}

		static b4_t width_byte_b4(b4_t w){
			int a;
			a = w / 2 + (w % 2 ? 1 : 0);
			a = a / 4 + (a % 4 ? 1 : 0);
			a *= 4;
			return a;
		}

		static b4_t width_byte_b8(b4_t w){
			int a;
			a = (w / 4 + (w % 4 ? 1 : 0)) * 4;
			return a;
		}

		static b4_t width_byte_b24(b4_t w){
			int a;
			a = w * 3;
			a = a / 4 + (a % 4 ? 1 : 0);
			a *= 4;
			return a;
		}

		static b4_t width_byte_b32(b4_t w){
			int a;
			a = w * 4;
			return a;
		}

		static b4_t width_byte_n(b4_t bc, b4_t w){
			switch(bc){
				case ColorNum::b1:
					return width_byte_b1(w);

				case ColorNum::b4:
					return width_byte_b4(w);

				case ColorNum::b8:
					return width_byte_b8(w);

				case ColorNum::b24:
					return width_byte_b24(w);

				case ColorNum::b32:
					return width_byte_b32(w);
			}
		}

		//--------公開メソッドと関連するクラス
	public:
		//色を作ってdotを返す
		static dot rgb(int r, int g, int b){
			return dot_(static_cast<b1u_t>(r), static_cast<b1u_t>(g), static_cast<b1u_t>(b));
		}

		static dot rgb(int r, int g, int b, int a){
			return dot_(static_cast<b1u_t>(r), static_cast<b1u_t>(g), static_cast<b1u_t>(b), static_cast<b1u_t>(a));
		}

		//ファイルに出力する
		template<typename T, typename U>
		void write(std::basic_ofstream<T, U> &f) const{
				//各ヘッダの値を決定する（遅延）
				int pltsize; //パレットサイズ
				switch(BitCount_){
					case ColorNum::n2:
					case ColorNum::n16:
					case ColorNum::n256:
						pltsize = (1 << BitCount_) * 4;
						break;

					default:
						pltsize = 0;
				}

				//ファイルサイズ
				FileSize_
					= /* ファイルヘッダ */	14
					+ /* 情報ヘッダ */		40
					+ /* パレット */		pltsize
					+ /* 画像データ */		width_byte * Height_;

				//画像データ部までのオフセット
				OffBits = FileSize_ - width_byte * Height_;

				//画像データ部のサイズ
				ImgDataSize_ = width_byte * Height_;

				//パレットの使用色数
				if(BitCount_ <= ColorNum::n256){
					ClrNum = 1 << BitCount_;
				}else{ ClrNum = 0; }

				//ファイルヘッダ及び情報ヘッダ
				f.write(reinterpret_cast<const T*>(FileHeader), 14);
				f.write(reinterpret_cast<const T*>(InfomationHeader), 40);

				//カラーパレット
				switch(BitCount_){
					{
					case ColorNum::n2:
					case ColorNum::n16:
					case ColorNum::n256:
						f.write(reinterpret_cast<const T*>(ColorPalette_), sizeof(dot) * (1 << BitCount_));
						break;
					}

					case ColorNum::b24:
						break;

					case ColorNum::b32:
						break;
				}

				//画像データ部
				f.write(reinterpret_cast<const T*>(ImgData_), width_byte * Height_);
		}

		void write(const char* const path) const{
			std::ofstream f(path, std::ios::binary);
			write(f);
		}

		//幅/高さを得る/設定する
		struct width_plt_set_BitCountError : BitCountError{
			virtual const char *prfn() const{ return "width (plt/set)"; }
		};

		struct width_plt_set_OOR : logic_error{
			virtual const char *msg() const{ return "パレットのインデックスが範囲を超えました。"; }
			virtual const char *prfn() const{ return "width (plt/set)"; }
		};

		struct width_trueclr_set_BitCountError : BitCountError{
			virtual const char *prfn() const{ return "width (trueclr/set)"; }
		};

		struct height_plt_set_BitCountError : BitCountError{
			virtual const char *prfn() const{ return "height (plt/set)"; }
		};

		struct height_plt_set_OOR : logic_error{
			virtual const char *msg() const{ return "パレットのインデックスが範囲を超えました。"; }
			virtual const char *prfn() const{ return "height (plt/set)"; }
		};

		struct height_trueclr_set_BitCountError : BitCountError{
			virtual const char *prfn() const{ return "height (trueclr/set)"; }
		};

		int width() const{
			return Width_;
		}

		int width(int v){
			if(v == Width_){ return Width_; }

			int Width_old = Width_;
			Width_ = v;

			int width_byte_old = width_byte;
			width_byte = width_byte_n(BitCount_, Width_);

			char *ImgData_old = ImgData_;
			ImgData_ = new char[width_byte * Height_];

			int width_regline = Width_ > Width_old ? width_byte_old : width_byte;

			for(int y = 0; y < Height_; y++)
				std::memcpy(
					(ImgData_ + (y * width_byte)),
					(ImgData_old + (y * width_byte_old)),
					width_regline
				);

			delete[] ImgData_old;

			return Width_old;
		}

		int width(int v, int i){
#ifdef _DEBUG
			if(BitCount_ > ColorNum::b8){
				throw(width_plt_set_BitCountError());
			}

			if(i >= (1 << BitCount_)){
				throw(width_plt_set_OOR());
			}
#endif
			int Width_old = Width_;
			int a = width(v);

			//塗りつぶし
			if(v > Width_old){
				switch(BitCount_){
					case ColorNum::b1:
						for(int y = 0; y < Height_; y++)
							for(int x = Width_old; x < Width_; x++)
								pltidx_2(x, y, i);
						break;

					case ColorNum::b4:
						for(int y = 0; y < Height_; y++)
							for(int x = Width_old; x < Width_; x++)
								pltidx_16(x, y, i);
						break;

					case ColorNum::b8:
						for(int y = 0; y < Height_; y++)
							for(int x = Width_old; x < Width_; x++)
								pltidx_256(x, y, i);
						break;
				}
			}

			return a;
		}

		int width(int v, dot d){
#ifdef _DEBUG
			if(BitCount_ < ColorNum::b24){
				throw(width_trueclr_set_BitCountError());
			}
#endif
			int Width_old = Width_;
			int a = width(v);

			//塗りつぶし
			if(v > Width_old){
				switch(BitCount_){
					case ColorNum::b24:
						for(int y = 0; y < Height_; y++)
							for(int x = Width_old; x < Width_; x++)
								clr_24(x, y, d);
						break;

					case ColorNum::b32:
						for(int y = 0; y < Height_; y++)
							for(int x = Width_old; x < Width_; x++)
								clr_32(x, y, d);
						break;
				}
			}

			return a;
		}

		int height() const{
			return Height_;
		}

		int height(int v){
			if(v == Height_){ return Height_; }

			int Height_old = Height_;
			Height_ = v;

			char *ImgData_old = ImgData_;
			ImgData_ = new char[width_byte * Height_];

			if(Height_ > Height_old){
				std::memcpy(ImgData_ + (Height_ - Height_old) * width_byte, ImgData_old, width_byte * Height_old);
			}else{
				std::memcpy(ImgData_, ImgData_old + (Height_old - Height_) * width_byte, width_byte * Height_);
			}

			delete[] ImgData_old;

			return Height_old;
		}

		int height(int v, int i){
#ifdef _DEBUG
			if(BitCount_ > ColorNum::b8){
				throw(height_plt_set_BitCountError());
			}

			if(i >= (1 << BitCount_)){
				throw(height_plt_set_OOR());
			}
#endif
			int Height_old = Height_;
			int a = height(v);

			//塗りつぶし
			if(v > Height_old){
				switch(BitCount_){
					case ColorNum::b1:
						for(int y = Height_old; y < Height_; y++)
							for(int x = 0; x < Width_; x++)
								pltidx_2(x, y, i);
						break;

					case ColorNum::b4:
						for(int y = Height_old; y < Height_; y++)
							for(int x = 0; x < Width_; x++)
								pltidx_16(x, y, i);
						break;

					case ColorNum::b8:
						for(int y = Height_old; y < Height_; y++)
							for(int x = 0; x < Width_; x++)
								pltidx_256(x, y, i);
						break;
				}
			}

			return a;
		}

		int height(int v, dot d){
#ifdef _DEBUG
			if(BitCount_ < ColorNum::b24){
				throw(width_trueclr_set_BitCountError());
			}
#endif
			int Height_old = Height_;
			int a = height(v);

			//塗りつぶし
			if(v > Height_old){
				switch(BitCount_){
					case ColorNum::b24:
						for(int y = Height_old; y < Height_; y++)
							for(int x = 0; x < Width_; x++)
								clr_24(x, y, d);
						break;

					case ColorNum::b32:
						for(int y = Height_old; y < Height_; y++)
							for(int x = 0; x < Width_; x++)
								clr_32(x, y, d);
						break;
				}
			}

			return a;
		}

		//伸縮する
		struct resize_plt_BitCountError : BitCountError{
			virtual const char *prfn() const{ return "resize (plt)"; }
		};

		struct resize_trueclr_BitCountError : BitCountError{
			virtual const char *prfn() const{ return "resize (trueclr)"; }
		};

		struct resize_plt_OOR : logic_error{
			virtual const char *msg() const{ return "パレットのインデックスが範囲を超えました。"; }
			virtual const char *prfn() const{ return "resize (plt)"; }
		};

		array_t<int, 2> resize(int w_, int h_){
			array_t<int, 2> a;
			a.data[0] = w_;
			a.data[1] = h_;

			bool wb = w_ == Width_, hb = h_ == Height_;

			if(wb && hb){
				return a;
			}else{
				int Width_old = Width_, Height_old = Height_;
				Width_ = w_;
				Height_ = h_;

				int width_byte_old = width_byte;
				width_byte = width_byte_n(BitCount_, Width_);

				char *ImgData_old = ImgData_;
				ImgData_ = new char[width_byte * Height_];

				int
					width_regline = Width_ > Width_old ? width_byte_old : width_byte,
					Height_regline = Height_ > Height_old ? Height_old : Height_;

				if(Height_ > Height_old){
					char *dst = ImgData_ + (Height_ - Height_old) * width_byte;
					for(int y = 0; y < Height_regline; y++)
						std::memcpy(
							(dst + (y * width_byte)),
							(ImgData_old + (y * width_byte_old)),
							width_regline
						);
				}else{
					char *src = ImgData_old + (Height_old - Height_) * width_byte_old;
					for(int y = 0; y < Height_regline; y++)
						std::memcpy(
							(ImgData_ + (y * width_byte)),
							(src + (y * width_byte_old)),
							width_regline
						);
				}
			}

			return a;
		}

		array_t<int, 2> resize(int w_, int h_, int i){
#ifdef _DEBUG
			if(BitCount_ > ColorNum::b8)
				throw(resize_plt_BitCountError());
			if(i >= pltnum())
				throw(resize_plt_OOR());
#endif
			bool wb = w_ == Width_, hb = h_ == Height_;

			if(wb && hb){
				array_t<int, 2> a;
				a.data[0] = w_;
				a.data[1] = h_;

				return a;
			}else{
				int Width_old = Width_, Height_old = Height_;
				int width_byte_old = width_byte;
				int
					width_regline = Width_ > Width_old ? width_byte_old : width_byte,
					Height_regline = Height_ > Height_old ? Height_old : Height_;

				array_t<int, 2> a = resize(w_, h_);

				if(w_ > Width_old){
					switch(BitCount_){
						case ColorNum::b1:
							for(int y = 0; y < Height_; y++)
								for(int x = Width_old; x < Width_; x++)
									pltidx_2(x, y, i);
							break;

						case ColorNum::b4:
							for(int y = 0; y < Height_; y++)
								for(int x = Width_old; x < Width_; x++)
									pltidx_16(x, y, i);
							break;

						case ColorNum::b8:
							for(int y = 0; y < Height_; y++)
								for(int x = Width_old; x < Width_; x++)
									pltidx_256(x, y, i);
							break;
					}
				}

				if(h_ > Height_old){
					switch(BitCount_){
						case ColorNum::b1:
							for(int y = Height_old; y < Height_; y++)
								for(int x = 0; x < Width_; x++)
									pltidx_2(x, y, i);
							break;

						case ColorNum::b4:
							for(int y = Height_old; y < Height_; y++)
								for(int x = 0; x < Width_; x++)
									pltidx_16(x, y, i);
							break;

						case ColorNum::b8:
							for(int y = Height_old; y < Height_; y++)
								for(int x = 0; x < Width_; x++)
									pltidx_256(x, y, i);
							break;
					}
				}

				return a;
			}
		}

		array_t<int, 2> resize(int w_, int h_, dot d){
#ifdef _DEBUG
			if(BitCount_ < ColorNum::b24)
				throw(resize_trueclr_BitCountError());
#endif
			bool wb = w_ == Width_, hb = h_ == Height_;

			if(wb && hb){
				array_t<int, 2> a;
				a.data[0] = w_;
				a.data[1] = h_;

				return a;
			}else if(wb){
				array_t<int, 2> a;
				a.data[0] = w_;
				a.data[1] = height(h_, d);

				return a;
			}else if(hb){
				array_t<int, 2> a;
				a.data[0] = width(w_, d);
				a.data[1] = h_;

				return a;
			}else{
				int Width_old = Width_, Height_old = Height_;
				int widht_byte_old = width_byte;
				int
					widht_regline = Width_ > Width_old ? widht_byte_old : width_byte,
					Height_regline = Height_ > Height_old ? Height_old : Height_;

				array_t<int, 2> a = resize(w_, h_);

				if(w_ > Width_old){
					switch(BitCount_){
						case ColorNum::b24:
							for(int y = 0; y < Height_; y++)
								for(int x = Width_old; x < Width_; x++)
									clr_24(x, y, d);
							break;

						case ColorNum::b32:
							for(int y = 0; y < Height_; y++)
								for(int x = Width_old; x < Width_; x++)
									clr_32(x, y, d);
							break;
					}
				}

				if(h_ > Height_old){
					switch(BitCount_){
						case ColorNum::b24:
							for(int y = Height_old; y < Height_; y++)
								for(int x = 0; x < Width_; x++)
									clr_24(x, y, d);
							break;

						case ColorNum::b32:
							for(int y = Height_old; y < Height_; y++)
								for(int x = 0; x < Width_; x++)
									clr_32(x, y, d);
							break;
					}
				}

				return a;
			}
		}

		//パレットの数を得る
		struct pltnum_BitCountError : BitCountError{
			virtual const char *prfn() const{ return "pltnum"; }
		};

		int pltnum() const{
#ifdef _DEBUG
			if(BitCount_ > ColorNum::n256) throw(pltnum_BitCountError());
#endif //_DEBUG
			return 1 << BitCount_;
		}

		//パレットを設定する
		struct setplt_BitCountError : BitCountError{//色数例外
			virtual const char *prfn() const{ return "setplt"; }
		};

		struct setplt_OOR : logic_error{//範囲外アクセス
			virtual const char *msg() const{ return "パレットのインデックスが範囲を超えました。"; }
			virtual const char *prfn() const{ return "setplt"; }
		};

		dot setplt(int i, dot d){
#ifdef _DEBUG
			if(BitCount_ > ColorNum::n256) throw(setplt_BitCountError());
			if(i > ((1 << BitCount_) - 1)) throw(setplt_OOR());
#endif //_DEBUG
			dot a = dot_(ColorPalette_[i]);
			ColorPalette_[i] = d;
			return a;
		}

		//指定座標のパレットを得る
		struct pltidx_get_BitCountError : BitCountError{
			virtual const char *prfn() const{ return "pltidx (get)"; }
		};

		struct pltidx_set_BitCountError : BitCountError{
			virtual const char *prfn() const{ return "pltidx (set)"; }
		};

	private:
		int pltidx_2(int w_, int h_) const{
			return ((ImgData_[(Height_ - h_ - 1) * width_byte + w_ / 8] >> (7 - (w_ % 8))) & 1);
		}

		int pltidx_16(int w_, int h_) const{
			return (ImgData_[(Height_ - h_ - 1) * width_byte + w_ / 2] >> (w_ % 2 ? 4 : 0)) & 15;
		}

		int pltidx_256(int w_, int h_) const{
			return ImgData_[(Height_ - h_ - 1) * width_byte + w_];
		}

		int pltidx_truecolor(int, int) const{
#ifdef _DEBUG
			throw(pltidx_get_BitCountError());
#endif //_DEBUG
			return 0;
		}

		int pltidx_2(int w_, int h_, int pltnum){
			b1u_t &data(reinterpret_cast<b1u_t*>(ImgData_)[(Height_ - h_ - 1) * width_byte + w_ / 8]);
			int shiftnum = 7 - (w_ % 8);
			int buff = (data >> shiftnum) & 1;
			data &= ~(1 << shiftnum);
			data |= pltnum << shiftnum;
			return buff;
		}

		int pltidx_16(int w_, int h_, int pltnum){
			b1u_t &data(reinterpret_cast<b1u_t*>(ImgData_)[(Height_ - h_ - 1) * width_byte + w_ / 2]);
			int shiftnum = (w_ % 2 ? 4 : 0);
			int buff = (data >> shiftnum) & 1;
			data &= ~(15 << shiftnum);
			data |= pltnum << shiftnum;
			return buff;
		}

		int pltidx_256(int w_, int h_, int pltnum){
			b1u_t &data(reinterpret_cast<b1u_t*>(ImgData_)[(Height_ - h_ - 1) * width_byte + w_]);
			int buff = data;
			data = static_cast<b1u_t>(pltnum);
			return buff;
		}

		int pltidx_truecolor(int, int, int){
#ifdef _DEBUG
			throw(pltidx_set_BitCountError());
#endif //_DEBUG
			return 0;
		}

	public:
		int pltidx(int x, int y) const{
			return (this->*pltidx_get_)(x, y);
		}

		int pltidx(int x, int y, int pltnum){
			int a = pltidx(x, y);
			(this->*pltidx_set_)(x, y, pltnum);
			return a;
		}

		//指定座標の色を得る/設定する
		struct clr_set_BitCountError : BitCountError{
			virtual const char *prfn() const{ return "clr (set)"; }
		};

	private:
		dot clr_1(int w, int h) const{
			return ColorPalette_[pltidx_2(w, h)];
		}

		dot clr_4(int w, int h) const{
			return ColorPalette_[pltidx_16(w, h)];
		}

		dot clr_8(int w, int h) const{
			return ColorPalette_[pltidx_256(w, h)];
		}

		dot clr_24(int w, int h) const{
			return *reinterpret_cast<dot*>(&ImgData_[(Height_ - h - 1) * width_byte + w * 3]);
		}

		dot clr_32(int w, int h) const{
			return *reinterpret_cast<dot*>(&ImgData_[(Height_ - h - 1) * width_byte + w * 4]);
		}

		dot clr_pltclr(int, int, dot){
			throw(clr_set_BitCountError());
			return dot_();
		}

		dot clr_24(int w, int h, dot d){
			dot &d_(*reinterpret_cast<dot*>(&ImgData_[(Height_ - h - 1) * width_byte + w * 3])), prv;
			prv = d_;
			d_.r = d.r, d_.g = d.g, d_.b = d.b;
			return prv;
		}

		dot clr_32(int w, int h, dot d){
			dot &d_(*reinterpret_cast<dot*>(&ImgData_[(Height_ - h - 1) * width_byte + w * 4])), prv;
			prv = d_;
			d_ = d;
			return prv;
		}

	public:
		dot clr(int x, int y) const{
			return (this->*clr_get_)(x, y);
		}

		dot clr(int x, int y, dot d){
			return (this->*clr_set_)(x, y, d);
		}

		//ppmを取得/設定する
		int xppm()const{
			return Xppm_;
		}

		int xppm(int v){
			int a = Xppm_;
			Xppm_ = v;
			return a;
		}

		int yppm()const{
			return Yppm_;
		}

		int yppm(int v){
			int a = Yppm_;
			Yppm_ = v;
			return a;
		}

		//線を引く
		void line(int x0, int y0, int x1, int y1, dot d){
			int dx, dy, sx, sy, a, i;
			if(x1 > x0){
				dx = x1 - x0;
				sx = 1;
			}else{
				dx = x0 - x1;
				sx = -1;
			}
			if(y1 > y0){
				dy = y1 - y0;
				sy = 1;
			}else{
				dy = y0 - y1;
				sy = -1;
			}
			if(dx > dy){
				a = -dx;
				for(i = 0; i <= dx; ++i){
					clr(x0, y0, d);
					x0 += sx;
					a += 2 * dy;
					if(a >= 0){
						y0 += sy;
						a -= 2 * dx;
					}
				}
			}else{
				a = -dy;
				for(i = 0; i <= dy; ++i){
					clr(x0, y0, d);
					y0 += sy;
					a += 2 * dx;
					if(a >= 0){
						x0 += sx;
						a -= 2 * dy;
					}
				}
			}
		}

		void line(int x0, int y0, int x1, int y1, int idx){
			int dx, dy, sx, sy, a, i;
			if(x1 > x0){
				dx = x1 - x0;
				sx = 1;
			}else{
				dx = x0 - x1;
				sx = -1;
			}
			if(y1 > y0){
				dy = y1 - y0;
				sy = 1;
			}else{
				dy = y0 - y1;
				sy = -1;
			}
			if(dx > dy){
				a = -dx;
				for(i = 0; i <= dx; ++i){
					pltidx(x0, y0, idx);
					x0 += sx;
					a += 2 * dy;
					if(a >= 0){
						y0 += sy;
						a -= 2 * dx;
					}
				}
			}else{
				a = -dy;
				for(i = 0; i <= dy; ++i){
					pltidx(x0, y0, idx);
					y0 += sy;
					a += 2 * dx;
					if(a >= 0){
						x0 += sx;
						a -= 2 * dy;
					}
				}
			}
		}

		//--------初期化処理
	private:
		//共通初期化処理
		void CommonInit(){
			//ファイルタイプ
			FileHeader[0] = 'B';
			FileHeader[1] = 'M';

			//予約領域
			FileHeader[6] = FileHeader[7] = FileHeader[8] = FileHeader[9] = 0;

			//情報ヘッダのサイズ
			b4_t &InfoHead_Size(*reinterpret_cast<b4_t*>(&InfomationHeader[0]));
			InfoHead_Size = 40;

			//プレーン数
			b2_t &Planes(*reinterpret_cast<b2_t*>(&InfomationHeader[12]));
			Planes = 1;

			//px per meter
			Xppm_ = 0;
			Yppm_ = 0;

			//ImportantColorなんだけど今一意味が分からない
			ImportantClr = 0;
		}

		//pltidxメンバポインタを初期化
		void Init_pltidx(){
			switch(BitCount_){
				case ColorNum::n2:
					pltidx_get_ = &xbmp::pltidx_2;
					pltidx_set_ = &xbmp::pltidx_2;
					break;

				case ColorNum::n16:
					pltidx_get_ = &xbmp::pltidx_16;
					pltidx_set_ = &xbmp::pltidx_16;
					break;

				case ColorNum::n256:
					pltidx_get_ = &xbmp::pltidx_256;
					pltidx_set_ = &xbmp::pltidx_256;
					break;

				case ColorNum::b24:
				case ColorNum::b32:
					pltidx_get_ = &xbmp::pltidx_truecolor;
					pltidx_set_ = &xbmp::pltidx_truecolor;
					break;
			}
		}

		//clrメンバポインタを初期化
		void Init_clr(){
			switch(BitCount_){
				case ColorNum::n2:
					clr_get_ = &xbmp::clr_1;
					clr_set_ = &xbmp::clr_pltclr;
					break;

				case ColorNum::n16:
					clr_get_ = &xbmp::clr_4;
					clr_set_ = &xbmp::clr_pltclr;
					break;

				case ColorNum::n256:
					clr_get_ = &xbmp::clr_8;
					clr_set_ = &xbmp::clr_pltclr;
					break;

				case ColorNum::b24:
					clr_get_ = &xbmp::clr_24;
					clr_set_ = &xbmp::clr_24;
					break;

				case ColorNum::b32:
					clr_get_ = &xbmp::clr_32;
					clr_set_ = &xbmp::clr_32;
					break;
			}
		}

	//共通メンバ初期化子
#define CommonMemberInit \
		FileSize_(*reinterpret_cast<b4_t*>(&FileHeader[2])), \
		OffBits(*reinterpret_cast<b4_t*>(&FileHeader[10])), \
		Width_(*reinterpret_cast<b4_t*>(&InfomationHeader[4])), \
		Height_(*reinterpret_cast<b4_t*>(&InfomationHeader[8])), \
		BitCount_(*reinterpret_cast<b2_t*>(&InfomationHeader[14])), \
		Compression_(*reinterpret_cast<b4_t*>(&InfomationHeader[16])), \
		ImgDataSize_(*reinterpret_cast<b4_t*>(&InfomationHeader[20])), \
		Xppm_(*reinterpret_cast<b4_t*>(&InfomationHeader[24])), \
		Yppm_(*reinterpret_cast<b4_t*>(&InfomationHeader[28])), \
		ClrNum(*reinterpret_cast<b4_t*>(&InfomationHeader[32])), \
		ImportantClr(*reinterpret_cast<b4_t*>(&InfomationHeader[36])), \
		ColorPalette_(NULL), \
		ImgData_(NULL)

	private:
		xbmp() : CommonMemberInit{}

		template<typename T, typename U>
		static void ctor_read(xbmp &obj, std::basic_ifstream<T, U> &f){
			//情報ヘッダとファイルヘッダを読む
			f.read(reinterpret_cast<T*>(obj.FileHeader), 14);
			f.read(reinterpret_cast<T*>(obj.InfomationHeader), 40);

			//負数には対応していない
			if(obj.Width_ <= 0 || obj.Height_ <= 0) throw(bmp_SizeIsNegative());

			//他のメンバを決定する
			obj.width_byte = width_byte_n(obj.BitCount_, obj.Width_);
			obj.ImgData_ = new char[obj.width_byte * obj.Height_];
			if(obj.BitCount_ <= ColorNum::b8) obj.ColorPalette_ = new dot[1 << obj.BitCount_]();
			obj.Init_pltidx();
			obj.Init_clr();

			//カラーパレット
			switch(obj.BitCount_){
				{
				case ColorNum::n2:
				case ColorNum::n16:
				case ColorNum::n256:
					f.read(reinterpret_cast<T*>(obj.ColorPalette_), sizeof(dot) * (1 << obj.BitCount_));
					break;
				}

				case ColorNum::b24:
					break;

				case ColorNum::b32:
					break;

				default:
					throw(bmp_BitCountIsIllegal());
			}

			//画像データ部
			f.read(reinterpret_cast<T*>(obj.ImgData_), obj.width_byte * obj.Height_);
		}

	public:
		struct bmp_BitCountError : logic_error{
			virtual const char *msg() const{ return "指定されたビット数が不正です"; }
			virtual const char *prfn() const{ return "ctor"; }
		};

		struct bmp_SizeError : logic_error{
			virtual const char *msg() const{ return "指定された幅または高さが不正です"; }
			virtual const char *prfn() const{ return "ctor"; }
		};

		struct bmp_ReadError : domain_error{
			virtual const char *msg() const{ return "読み込み失敗"; }
			virtual const char *prfn() const{ return "ctor"; }
		};

		struct bmp_SizeIsNegative : bmp_ReadError{
			virtual const char *msg() const{ return "幅/高さが負数です"; }
		};

		struct bmp_BitCountIsIllegal : bmp_ReadError{
			virtual const char *msg() const{ return "ビット数が不正です"; }
		};

		xbmp(
			const b4_t BitCount_,
			const b4_t Width_,
			const b4_t Height_,
			const b4_t Xppm_ = 0,
			const b4_t Yppm_ = 0,
			const b4_t Compression_ = 0
		) : CommonMemberInit
		{
			CommonInit();

#ifdef _DEBUG
			if(Width_ <= 0 || Height_ <= 0) throw(bmp_SizeError());
#endif //_DEBUG

			this->BitCount_ = BitCount_;
			this->Width_ = Width_;
			this->Height_ = Height_;
			this->Compression_ = 0 /*Compression_ 現在圧縮は無効*/;
			this->Xppm_ = Xppm_;
			this->Yppm_ = Yppm_;

			Init_pltidx();
			Init_clr();

			//カラーパレットと画像を生成する
			switch(BitCount_){
			case ColorNum::b1:
					ColorPalette_ = new dot[1 << BitCount_]();
					width_byte = width_byte_b1(Width_);
					ImgData_ = new char[width_byte * Height_];
					break;

				case ColorNum::b4:
					ColorPalette_ = new dot[1 << BitCount_]();
					width_byte = width_byte_b4(Width_);
					ImgData_ = new char[width_byte * Height_];
					break;

				case ColorNum::b8:
					ColorPalette_ = new dot[1 << BitCount_]();
					width_byte = width_byte_b8(Width_);
					ImgData_ = new char[width_byte * Height_];
					break;

				case ColorNum::b24:
					width_byte = width_byte_b24(Width_);
					ImgData_ = new char[width_byte * Height_];
					break;

				case ColorNum::b32:
					width_byte = width_byte_b32(Width_);
					ImgData_ = new char[width_byte * Height_];
					break;

#ifdef _DEBUG
				default:
					throw(bmp_BitCountError());
#endif //_DEBUG
			}

			memset(static_cast<void*>(ImgData_), 0, width_byte * Height_);
		}

		xbmp(const char *const path) : CommonMemberInit{
			std::ifstream f(path, std::ios::binary);
			ctor_read(*this, f);
		}

		template<typename T, typename U>
		xbmp(std::basic_ifstream<T, U> &f) : CommonMemberInit{
			ctor_read(*this, f);
		}

		virtual ~xbmp(){
			delete[] ColorPalette_; ColorPalette_ = NULL;
			delete[] ImgData_; ImgData_ = NULL;
		}

#undef CommonMemberInit
	};

#ifdef _MSC_VER
#pragma warning(default:4290)
#pragma warning(default:4715)
#endif

#ifndef _MSC_VER
#undef _DEBUG
#endif

	typedef xbmp<void> bmp;

} //namespace tty
#endif // __TTY_BMP__
