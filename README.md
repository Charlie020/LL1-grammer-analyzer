# LL-1-Grammer-Analyzer
编译原理-LL(1)语法分析器的构造(C++实现)

## 功能
（1）主要用于识别算术文法如：5+(2*63)

（2）能够检测简单的左递归，并进行消除。

（3）能够求解FIRST集和FOLLOW集。

（4）能够构造LL(1)分析表。

（5）能够根据用户输入的句子进行分析，并打印分析过程。若句子无法由当前文法推得，将报错。

## 输入格式
（1）首先输入算数文法，每条产生式后面要换行，在最后一条产生式换行后，按CTRL+Z（显示为^Z）结束文法的输入。

（2）然后输入句子，句子中不能包含中文字符。

## 输出格式
首先，会输出文法中的非终结符和终结符。

然后，会判断是否有左递归。若有左递归则会给出消除左递归之后的文法；若没有左递归，则会直接输出一遍文法以判断是否输入正确。

随后，会输出FIRST集和FOLLOW集，并构造LL(1)分析表。

最后，会给出用户输入的句子转化后的形式，并打印利用LL(1)分析表分析句子的过程（该部分打印时用到了\t来统一格式，但当字符串较长时显示可能不太美观）。

## 示例
```
Input the grammer : 

E->E+T|T

T->T*F|F

F->(E)|i

^Z

Input the sentence : 5+(2*63)


Vn: A B E F T

Vt: ( ) * + e i

存在左递归，消除后如下：

A->+TA

A->e

B->*FB

B->e

E->TA

F->(E)

F->i

T->FB


FIRST集:

FIRST( A ): + e

FIRST( B ): * e

FIRST( E ): ( i

FIRST( F ): ( i

FIRST( T ): ( i

FIRST( ( ): (

FIRST( ) ): )

FIRST( * ): *

FIRST( + ): +

FIRST( e ): e

FIRST( i ): i


FOLLOW集:

FOLLOW( A ): # )

FOLLOW( B ): # ) +

FOLLOW( E ): # )

FOLLOW( F ): # ) * +

FOLLOW( T ): # ) +


M[A,#] = A->e

M[A,)] = A->e

M[A,+] = A->+TA

M[B,#] = B->e

M[B,)] = B->e

M[B,*] = B->*FB

M[B,+] = B->e

M[E,(] = E->TA

M[E,i] = E->TA

M[F,(] = F->(E)

M[F,i] = F->i

M[T,(] = T->FB

M[T,i] = T->FB


Format : i+(i*i)

1       #E      i+(i*i)#        E->TA

2       #AT     i+(i*i)#        T->FB

3       #ABF    i+(i*i)#        F->i

4       #ABi    i+(i*i)#

5       #AB     +(i*i)# B->e

6       #A      +(i*i)# A->+TA

7       #AT+    +(i*i)#

8       #AT     (i*i)#  T->FB

9       #ABF    (i*i)#  F->(E)

10      #AB)E(  (i*i)#

11      #AB)E   i*i)#   E->TA

12      #AB)AT  i*i)#   T->FB

13      #AB)ABF i*i)#   F->i

14      #AB)ABi i*i)#

15      #AB)AB  *i)#    B->*FB

16      #AB)ABF*        *i)#

17      #AB)ABF i)#     F->i

18      #AB)ABi i)#

19      #AB)AB  )#      B->e

20      #AB)A   )#      A->e
21      #AB)    )#

22      #AB     #       B->e

23      #A      #       A->e

24      #       #       Success
```

## 说明

由于本人对编译原理这个部分比较感兴趣，所以亲手写了该部分代码以实现一个能分析简单的算术文法的LL(1)语法分析器。

因为只花了一个下午的时间，所以代码写得还不够好，期待被完善。我尽可能的在代码里面添加了注释，希望能够起到一定作用 ^_^。
