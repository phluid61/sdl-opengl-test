#ifndef __QOL_H__
#define __QOL_H__

/* Quality of Life */

#define __EVAL__(...) __VA_ARGS__
#define __CALL__(F,...) F(__VA_ARGS__)

#define __VA_COUNT__(...) __CALL__(__VA_COUNTz_,__VA_ARGS__,9,8,7,6,5,4,3,2,1)
#define __VA_COUNTz_(_9,_8,_7,_6,_5,_4,_3,_2,_1,N_,...) N_

/*
#define __VA_FIRST__(...) __CALL__(__VA_FIRSTz_,__VA_ARGS__)
#define __VA_FIRSTz_(X,...) X

#define __VA_TAIL__(...) __CALL__(__VA_TAILz_,__VA_ARGS__)
#define __VA_TAILz_(_X,...) (__VA_ARGS__)
*/

#define __GLUE__(A,B) __GLUEz_(A,B)
#define __GLUEz_(A,B) A##B

#define __VA_CAT1(X) X
#define __VA_CAT2(X,...) __GLUE__(X,__VA_CAT1(__VA_ARGS__))
#define __VA_CAT3(X,...) __GLUE__(X,__VA_CAT2(__VA_ARGS__))
#define __VA_CAT4(X,...) __GLUE__(X,__VA_CAT3(__VA_ARGS__))
#define __VA_CAT5(X,...) __GLUE__(X,__VA_CAT4(__VA_ARGS__))
#define __VA_CAT6(X,...) __GLUE__(X,__VA_CAT5(__VA_ARGS__))
#define __VA_CAT7(X,...) __GLUE__(X,__VA_CAT6(__VA_ARGS__))
#define __VA_CAT8(X,...) __GLUE__(X,__VA_CAT7(__VA_ARGS__))
#define __VA_CAT9(X,...) __GLUE__(X,__VA_CAT8(__VA_ARGS__))

#define __VA_CAT__(...) __CALL__(__GLUE__(__VA_CAT, __VA_COUNT__(__VA_ARGS__)),__VA_ARGS__)

#endif

/* vim: ts=4 sts=4 sw=4
 */
