=begin
== vector
vectorは可変長な配列である。許容量を超えた要素の追加をした場合、自動的に拡張する。
末尾での要素の挿入・削除の計算量がO(1)であり、それ以外の位置の要素の挿入・削除の計算量はO(N)である。
インデックスによる要素のランダムアクセスが可能。
また、内部データの連続性は保証される。

vectorを使うには、以下のマクロを用いてコードを展開する必要がある。

  #include <cstl/vector.h>

  #define CSTL_VECTOR_INTERFACE(Name, Type)
  #define CSTL_VECTOR_IMPLEMENT(Name, Type)

((*CSTL_VECTOR_INTERFACE()*))は任意の名前と要素の型のvectorのインターフェイスを展開する。
((*CSTL_VECTOR_IMPLEMENT()*))はその実装を展開する。
それぞれのマクロの引数は同じものを指定すること。
また、((*CSTL_VECTOR_INTERFACE()*))を展開する前に、algorithm.hをインクルードすることにより、
((<アルゴリズム|URL:algorithm.html>))が使用可能となる。

: Name
  既存の型と重複しない任意の名前。コンテナの型名と関数のプレフィックスになる
: Type
  任意の要素の型

=== 使用例
  #include <stdio.h>
  #include <cstl/vector.h>
  
  CSTL_VECTOR_INTERFACE(IntVector, int) /* インターフェイスを展開 */
  CSTL_VECTOR_IMPLEMENT(IntVector, int) /* 実装を展開 */
  
  int main(void)
  {
      int i;
      /* intのvectorを生成 */
      IntVector *vec = IntVector_new();
  
      for (i = 0; i < 64; i++) {
          /* 末尾から要素を追加。自動的に拡張する */
          IntVector_push_back(vec, i);
      }
      /* サイズ */
      printf("size: %d\n", IntVector_size(vec));
      for (i = 0; i < 64; i++) {
          /* インデックスによる要素の読み書き */
          printf("%d,", *IntVector_at(vec, i));
          *IntVector_at(vec, i) += 1;
          printf("%d\n", *IntVector_at(vec, i));
      }
  
      /* 使い終わったら破棄 */
      IntVector_delete(vec);
      return 0;
  }

※複数のソースファイルから同じ型のコンテナを使用する場合は、
マクロ展開用のヘッダファイルとソースファイルを用意し、適宜インクルードやリンクをすればよい。

<<< hr

((*CSTL_VECTOR_INTERFACE(Name, Type)*))のNameにVector, TypeにTを指定した場合、
以下のインターフェイスを提供する。

* 型
  * ((<Vector>))
* 関数
  * 生成
    * ((<Vector_new()>))
    * ((<Vector_new_reserve()>))
  * 破棄
    * ((<Vector_delete()>))
  * サイズ
    * ((<Vector_size()>))
    * ((<Vector_empty()>))
  * 許容量
    * ((<Vector_capacity()>))
    * ((<Vector_reserve()>)) , ((<Vector_shrink()>))
  * 要素のアクセス
    * ((<Vector_at()>))
    * ((<Vector_front()>)) , ((<Vector_back()>))
  * 挿入
    * ((<Vector_insert()>)) , ((<Vector_insert_n()>)) , ((<Vector_insert_array()>)) , ((<Vector_insert_range()>))
    * ((<Vector_push_back()>))
  * 削除
    * ((<Vector_erase()>))
    * ((<Vector_pop_back()>))
    * ((<Vector_clear()>))
  * サイズの変更
    * ((<Vector_resize()>))
  * 交換
    * ((<Vector_swap()>))
<<< hr

==== Vector
  Vector
コンテナの型。抽象データ型となっており、以下の関数によってのみアクセスできる。
<<< hr

==== Vector_new()
  Vector *Vector_new(void);
* vectorを生成する。
* 生成に成功した場合、そのオブジェクトへのポインタを返す。
* メモリ不足の場合、NULLを返す。
<<< hr

==== Vector_new_reserve()
  Vector *Vector_new_reserve(size_t n);
* 許容量(内部メモリの再割り当てを行わずに格納できる要素数)がn個のvectorを生成する。
* 生成に成功した場合、そのオブジェクトへのポインタを返す。
* メモリ不足の場合、NULLを返す。
<<< hr

==== Vector_delete()
  void Vector_delete(Vector *self);
* selfのすべての要素を削除し、selfを破棄する。
* selfがNULLの場合、何もしない。
<<< hr

==== Vector_size()
  size_t Vector_size(Vector *self);
* selfの現在の要素数を返す。
<<< hr

==== Vector_empty()
  int Vector_empty(Vector *self);
* selfが空の場合、0以外の値を返す。
* selfが空でない場合、0を返す。
<<< hr

==== Vector_capacity()
  size_t Vector_capacity(Vector *self);
* selfの許容量を返す。
<<< hr

==== Vector_reserve()
  int Vector_reserve(Vector *self, size_t n);
* selfの許容量を要素n個の領域に拡張する。
* selfが持つ要素は維持され、拡張した領域の初期化はしない。
* 拡張に成功した場合、0以外の値を返す。
* nがselfの現在の許容量以下の場合、selfの変更を行わず0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< hr

==== Vector_shrink()
  void Vector_shrink(Vector *self, size_t n);
* selfの許容量を要素n個の領域に縮小する。
* nがselfの現在の要素数以下の場合、selfの許容量を要素数と同じにする。
* nがselfの現在の許容量以上の場合、何もしない。
<<< hr

==== Vector_at()
  T *Vector_at(Vector *self, size_t idx);
* selfのidx番目の要素へのポインタを返す。
* 戻り値はselfの変更により無効となる。
* 事前条件
  * idxがselfの現在の要素数より小さい値であること。
<<< hr

==== Vector_front()
  T *Vector_front(Vector *self);
* selfの最初の要素へのポインタを返す。
* 事前条件
  * selfが空でないこと。
<<< hr

==== Vector_back()
  T *Vector_back(Vector *self);
* selfの最後の要素へのポインタを返す。
* 事前条件
  * selfが空でないこと。
<<< hr

==== Vector_insert()
  int Vector_insert(Vector *self, size_t idx, T elem);
* selfのidx番目の位置にelemのコピーを挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * idxがselfの現在の要素数以下の値であること。
<<< hr

==== Vector_insert_n()
  int Vector_insert_n(Vector *self, size_t idx, size_t n, T elem);
* selfのidx番目の位置にelemのコピーをn個挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * idxがselfの現在の要素数以下の値であること。
<<< hr

==== Vector_insert_array()
  int Vector_insert_array(Vector *self, size_t idx, T const *elems, size_t n);
* selfのidx番目の位置にelemsという配列からn個の要素のコピーを挿入する。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * elemsがNULLでないこと。
  * idxがselfの現在の要素数以下の値であること。
<<< hr

==== Vector_insert_range()
  int Vector_insert_range(Vector *self, size_t idx, Vector *x, size_t xidx, size_t n);
* selfのidx番目の位置にxのxidx番目からn個の要素のコピーを挿入する。
* selfとxは同じオブジェクトでもよい。
* 挿入に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
* 事前条件
  * xidx + nがxの現在の要素数以下の値であること。
  * idxがselfの現在の要素数以下の値であること。
<<< hr

==== Vector_push_back()
  int Vector_push_back(Vector *self, T elem);
* elemのコピーをselfの最後の要素として追加する。
* 追加に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< hr

==== Vector_erase()
  void Vector_erase(Vector *self, size_t idx, size_t n);
* selfのidx番目からn個の要素を削除する。
* 事前条件
  * idx + nがselfの現在の要素数以下の値であること。
<<< hr

==== Vector_pop_back()
  void Vector_pop_back(Vector *self);
* selfの最後の要素を削除する。
* 事前条件
  * selfが空でないこと。
<<< hr

==== Vector_clear()
  void Vector_clear(Vector *self);
* selfのすべての要素を削除する。
<<< hr

==== Vector_resize()
  int Vector_resize(Vector *self, size_t n, T elem);
* selfの要素数をn個に変更する。
* nがselfの現在の要素数以下の場合、要素数がn個になるまで末尾から要素が削除される。
* nがselfの現在の要素数より大きい場合、要素数がn個になるまでelemのコピーが末尾から追加される。
* 要素数の変更に成功した場合、0以外の値を返す。
* メモリ不足の場合、selfの変更を行わず0を返す。
<<< hr

==== Vector_swap()
  void Vector_swap(Vector *self, Vector *x);
* selfとxの内容を交換する。
<<< hr

=end
