/* 
 * Copyright (c) 2007, KATO Noriaki
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*! 
 * \file string.h
 * \brief 文字列コンテナ
 * \author KATO Noriaki <katono@users.sourceforge.jp>
 * \date 2007-01-09
 */
#ifndef CSTL_STRING_H_INCLUDED
#define CSTL_STRING_H_INCLUDED

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include "vector.h"

#ifdef __cplusplus
#define STRING_BEGIN_EXTERN_C()	extern "C" {
#define STRING_END_EXTERN_C()	}
#else
#define STRING_BEGIN_EXTERN_C()
#define STRING_END_EXTERN_C()
#endif

#ifndef NDEBUG
#define STRING_MAGIC(x) x
#else
#define STRING_MAGIC(x)
#endif

/*! 
 * \brief 検索失敗用インデックス
 */
#define NPOS	((size_t)-1)

/*! 
 * \brief インターフェイスマクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define STRING_INTERFACE(Name, Type)	\
typedef struct Name##_t Name;\
\
STRING_BEGIN_EXTERN_C()\
Name *Name##_new(size_t n);\
void Name##_delete(Name *self);\
void Name##_clear(Name *self);\
size_t Name##_size(Name *self);\
size_t Name##_length(Name *self);\
size_t Name##_capacity(Name *self);\
int Name##_reserve(Name *self, size_t n);\
void Name##_shrink(Name *self, size_t n);\
int Name##_empty(Name *self);\
int Name##_compare(Name *x, Name *y);\
Type *Name##_at(Name *self, size_t idx);\
Type *Name##_c_str(Name *self);\
void Name##_erase(Name *self, size_t idx, size_t len);\
int Name##_resize(Name *self, size_t n, Type c);\
void Name##_swap(Name *x, Name *y);\
int Name##_assign(Name *self, Type *cstr, size_t cstr_len);\
int Name##_assign_c(Name *self, size_t n, Type c);\
int Name##_append(Name *self, Type *cstr, size_t cstr_len);\
int Name##_append_c(Name *self, size_t n, Type c);\
int Name##_push_back(Name *self, Type c);\
int Name##_insert(Name *self, size_t idx, Type *cstr, size_t cstr_len);\
int Name##_insert_c(Name *self, size_t idx, size_t n, Type c);\
int Name##_replace(Name *self, size_t idx, size_t len, Type *cstr, size_t cstr_len);\
int Name##_replace_c(Name *self, size_t idx, size_t len, size_t n, Type c);\
size_t Name##_find(Type *x, Type *cstr, size_t idx, size_t cstr_len);\
size_t Name##_find_c(Type *x, Type c, size_t idx);\
size_t Name##_rfind(Type *x, Type *cstr, size_t idx, size_t cstr_len);\
size_t Name##_rfind_c(Type *x, Type c, size_t idx);\
size_t Name##_find_first_of(Type *x, Type *cstr, size_t idx, size_t cstr_len);\
size_t Name##_find_first_of_c(Type *x, Type c, size_t idx);\
size_t Name##_find_last_of(Type *x, Type *cstr, size_t idx, size_t cstr_len);\
size_t Name##_find_last_of_c(Type *x, Type c, size_t idx);\
size_t Name##_find_first_not_of(Type *x, Type *cstr, size_t idx, size_t cstr_len);\
size_t Name##_find_first_not_of_c(Type *x, Type c, size_t idx);\
size_t Name##_find_last_not_of(Type *x, Type *cstr, size_t idx, size_t cstr_len);\
size_t Name##_find_last_not_of_c(Type *x, Type c, size_t idx);\
STRING_END_EXTERN_C()\


/*! 
 * \brief 実装マクロ
 * 
 * \param Name コンテナ名
 * \param Type 要素の型
 */
#define STRING_IMPLEMENT(Name, Type)	\
\
static int Name##my_memcmp(const Type *x, const Type *y, size_t size)\
{\
	if (size) {\
		do {\
			if (*x != *y) {\
				return *x - *y;\
			}\
			x++;\
			y++;\
			size--;\
		} while (size);\
	}\
	return 0;\
}\
\
static size_t Name##my_strlen(const Type *cstr)\
{\
	size_t i = 0;\
	while (*cstr != '\0') {\
		cstr++;\
		i++;\
	}\
	return i;\
}\
\
VECTOR_INTERFACE(Name##CharVector, Type)\
VECTOR_IMPLEMENT(Name##CharVector, Type)\
\
/*! 
 * \brief string構造体
 */\
struct Name##_t {\
	Name##CharVector *data;\
	STRING_MAGIC(void *magic;)\
};\
\
Name *Name##_new(size_t n)\
{\
	Name *self;\
	self = (Name *) malloc(sizeof(Name));\
	if (!self) return 0;\
	self->data = Name##CharVector_new(n+1);\
	if (!self->data) {\
		free(self);\
		return 0;\
	}\
	Name##CharVector_push_back(self->data, '\0');\
	STRING_MAGIC(self->magic = self);\
	return self;\
}\
\
void Name##_delete(Name *self)\
{\
	assert(self && "String_delete");\
	assert(self->magic == self && "String_delete");\
	STRING_MAGIC(self->magic = 0);\
	Name##CharVector_delete(self->data);\
	free(self);\
}\
\
void Name##_clear(Name *self)\
{\
	assert(self && "String_clear");\
	assert(self->magic == self && "String_clear");\
	Name##CharVector_clear(self->data);\
	Name##CharVector_push_back(self->data, '\0');\
}\
\
size_t Name##_size(Name *self)\
{\
	assert(self && "String_size");\
	assert(self->magic == self && "String_size");\
	return Name##CharVector_size(self->data) -1;\
}\
\
size_t Name##_length(Name *self)\
{\
	assert(self && "String_length");\
	assert(self->magic == self && "String_length");\
	return Name##CharVector_size(self->data) -1;\
}\
\
size_t Name##_capacity(Name *self)\
{\
	assert(self && "String_capacity");\
	assert(self->magic == self && "String_capacity");\
	return Name##CharVector_capacity(self->data) -1;\
}\
\
int Name##_reserve(Name *self, size_t n)\
{\
	assert(self && "String_reserve");\
	assert(self->magic == self && "String_reserve");\
	return Name##CharVector_reserve(self->data, n +1);\
}\
\
void Name##_shrink(Name *self, size_t n)\
{\
	assert(self && "String_shrink");\
	assert(self->magic == self && "String_shrink");\
	Name##CharVector_shrink(self->data, n +1);\
}\
\
int Name##_empty(Name *self)\
{\
	assert(self && "String_empty");\
	assert(self->magic == self && "String_empty");\
	return Name##CharVector_size(self->data) == 1;\
}\
\
int Name##_compare(Name *x, Name *y)\
{\
	size_t xlen, ylen;\
	int ret;\
	assert(x && "String_compare");\
	assert(y && "String_compare");\
	assert(x->magic == x && "String_compare");\
	assert(y->magic == y && "String_compare");\
	xlen = Name##_size(x);\
	ylen = Name##_size(y);\
	ret = Name##my_memcmp(Name##_c_str(x), Name##_c_str(y), (xlen > ylen) ? ylen : xlen);\
	return ret ? ret : xlen - ylen;\
}\
\
Type *Name##_at(Name *self, size_t idx)\
{\
	assert(self && "String_at");\
	assert(self->magic == self && "String_at");\
	assert(Name##_size(self) > idx && "String_at");\
	return Name##CharVector_at(self->data, idx);\
}\
\
Type *Name##_c_str(Name *self)\
{\
	assert(self && "String_c_str");\
	assert(self->magic == self && "String_c_str");\
	return Name##CharVector_at(self->data, 0);\
}\
\
void Name##_erase(Name *self, size_t idx, size_t len)\
{\
	size_t size;\
	assert(self && "String_erase");\
	assert(self->magic == self && "String_erase");\
	size = Name##_size(self);\
	assert(size >= idx && "String_erase");\
	if (len > size - idx) {\
		len = size - idx;\
	}\
	Name##CharVector_erase(self->data, idx, len);\
}\
\
int Name##_resize(Name *self, size_t n, Type c)\
{\
	size_t num;\
	assert(self && "String_resize");\
	assert(self->magic == self && "String_resize");\
	num = Name##CharVector_size(self->data) -1;\
	if (!Name##CharVector_resize(self->data, n +1, c)) {\
		return 0;\
	}\
	if (num < Name##CharVector_size(self->data)) {\
		/* '\0'を上書き */\
		*Name##CharVector_at(self->data, num) = c;\
	}\
	*Name##CharVector_at(self->data, Name##CharVector_size(self->data) -1) = '\0';\
	return 1;\
}\
\
void Name##_swap(Name *x, Name *y)\
{\
	Name##CharVector *tmp_data;\
	assert(x && "String_swap");\
	assert(y && "String_swap");\
	assert(x->magic == x && "String_swap");\
	assert(y->magic == y && "String_swap");\
	tmp_data = x->data;\
	x->data = y->data;\
	y->data = tmp_data;\
}\
\
int Name##_assign(Name *self, Type *cstr, size_t cstr_len)\
{\
	assert(self && "String_assign");\
	assert(self->magic == self && "String_assign");\
	assert(cstr && "String_assign");\
	if (cstr_len == NPOS) {\
		cstr_len = Name##my_strlen(cstr);\
	}\
	if (!Name##CharVector_reserve(self->data, cstr_len +1)) {\
		return 0;\
	}\
	Name##CharVector_assign_array(self->data, cstr, cstr_len);\
	Name##CharVector_push_back(self->data, '\0');\
	return 1;\
}\
\
int Name##_assign_c(Name *self, size_t n, Type c)\
{\
	size_t i;\
	assert(self && "String_assign_c");\
	assert(self->magic == self && "String_assign_c");\
	if (!Name##_reserve(self, n)) {\
		return 0;\
	}\
	Name##_clear(self);\
	for (i = 0; i < n; i++) {\
		Name##_push_back(self, c);\
	}\
	return 1;\
}\
\
int Name##_append(Name *self, Type *cstr, size_t cstr_len)\
{\
	assert(self && "String_append");\
	assert(self->magic == self && "String_append");\
	assert(cstr && "String_append");\
	if (cstr_len == NPOS) {\
		cstr_len = Name##my_strlen(cstr);\
	}\
	return Name##CharVector_insert_n(self->data, Name##CharVector_size(self->data)-1, cstr, cstr_len);\
}\
\
int Name##_append_c(Name *self, size_t n, Type c)\
{\
	size_t i;\
	assert(self && "String_append_c");\
	assert(self->magic == self && "String_append_c");\
	if (!Name##_reserve(self, Name##_size(self) + n)) {\
		return 0;\
	}\
	for (i = 0; i < n; i++) {\
		Name##_push_back(self, c);\
	}\
	return 1;\
}\
\
int Name##_push_back(Name *self, Type c)\
{\
	assert(self && "String_push_back");\
	assert(self->magic == self && "String_push_back");\
	return Name##CharVector_insert(self->data, Name##CharVector_size(self->data)-1, c);\
}\
\
int Name##_insert(Name *self, size_t idx, Type *cstr, size_t cstr_len)\
{\
	assert(self && "String_insert");\
	assert(self->magic == self && "String_insert");\
	assert(Name##_size(self) >= idx && "String_insert");\
	assert(cstr && "String_insert");\
	if (cstr_len == NPOS) {\
		cstr_len = Name##my_strlen(cstr);\
	}\
	return Name##CharVector_insert_n(self->data, idx, cstr, cstr_len);\
}\
\
int Name##_insert_c(Name *self, size_t idx, size_t n, Type c)\
{\
	size_t i;\
	assert(self && "String_insert_c");\
	assert(self->magic == self && "String_insert_c");\
	assert(Name##_size(self) >= idx && "String_insert_c");\
	if (!Name##_reserve(self, Name##_size(self) + n)) {\
		return 0;\
	}\
	for (i = 0; i < n; i++) {\
		Name##CharVector_insert(self->data, idx, c);\
		idx++;\
	}\
	return 1;\
}\
\
int Name##_replace(Name *self, size_t idx, size_t len, Type *cstr, size_t cstr_len)\
{\
	size_t i;\
	size_t size;\
	assert(self && "String_replace");\
	assert(self->magic == self && "String_replace");\
	assert(cstr && "String_replace");\
	size = Name##_size(self);\
	assert(size >= idx && "String_replace");\
	if (len > size - idx) {\
		len = size - idx;\
	}\
	if (cstr_len == NPOS) {\
		cstr_len = Name##my_strlen(cstr);\
	}\
	if (cstr_len <= len) {\
		/* 拡張必要なし */\
		for (i = 0; i < len; i++) {\
			if (i < cstr_len) {\
				*Name##_at(self, i + idx) = cstr[i];\
			} else {\
				size_t j = len - cstr_len;\
				if (j > Name##_size(self) - (cstr_len + idx)) {\
					j = Name##_size(self) - (cstr_len + idx);\
				}\
				Name##_erase(self, cstr_len + idx, j);\
				break;\
			}\
		}\
	} else {\
		/* 拡張必要あり */\
		if (!Name##_reserve(self, Name##_capacity(self) + (cstr_len - len))) {\
			return 0;\
		}\
		for (i = 0; i < len; i++) {\
			*Name##_at(self, i + idx) = cstr[i];\
		}\
		Name##_insert(self, len + idx, &cstr[len], cstr_len - len);\
	}\
	return 1;\
}\
\
int Name##_replace_c(Name *self, size_t idx, size_t len, size_t n, Type c)\
{\
	size_t i;\
	size_t size;\
	assert(self && "String_replace_c");\
	assert(self->magic == self && "String_replace_c");\
	size = Name##_size(self);\
	assert(size >= idx && "String_replace_c");\
	if (len > size - idx) {\
		len = size - idx;\
	}\
	assert(Name##_size(self) >= idx + len && "String_replace_c");\
	if (n <= len) {\
		/* 拡張必要なし */\
		for (i = 0; i < len; i++) {\
			if (i < n) {\
				*Name##_at(self, i + idx) = c;\
			} else {\
				size_t j = len - n;\
				if (j > Name##_size(self) - (n + idx)) {\
					j = Name##_size(self) - (n + idx);\
				}\
				Name##_erase(self, n + idx, j);\
				break;\
			}\
		}\
	} else {\
		/* 拡張必要あり */\
		if (!Name##_reserve(self, Name##_capacity(self) + (n - len))) {\
			return 0;\
		}\
		for (i = 0; i < len; i++) {\
			*Name##_at(self, i + idx) = c;\
		}\
		Name##_insert_c(self, len + idx, n - len, c);\
	}\
	return 1;\
}\
\
static size_t Name##_brute_force_search(Type *str, size_t str_len, Type *ptn, size_t ptn_len)\
{\
	size_t i = 0;\
	size_t j = 0;\
\
	if (str_len < ptn_len) return NPOS;\
	while (i < str_len && j < ptn_len) {\
		if (str[i] == ptn[j]) {\
			i++;\
			j++;\
		} else {\
			i += 1 - j;\
			j = 0;\
		}\
	}\
	return (j == ptn_len) ? i - j : NPOS;\
}\
\
static size_t Name##_brute_force_search_r(Type *str, size_t str_len, Type *ptn, size_t ptn_len)\
{\
	size_t i = str_len;\
	size_t j = ptn_len;\
\
	if (str_len < ptn_len) return NPOS;\
	while (i > 0 && j > 0) {\
		if (str[i-1] == ptn[j-1]) {\
			i--;\
			j--;\
		} else {\
			i += ptn_len - j - 1;\
			j = ptn_len;\
		}\
	}\
	return (j == 0) ? i : NPOS;\
}\
\
size_t Name##_find(Type *x, Type *cstr, size_t idx, size_t cstr_len)\
{\
	size_t i;\
	size_t size;\
	assert(x && "String_find");\
	assert(cstr && "String_find");\
	if (cstr_len == NPOS) {\
		cstr_len = Name##my_strlen(cstr);\
	}\
	if (cstr_len == 0) {\
		return idx;\
	}\
	size = Name##my_strlen(x);\
	if (size < idx) return NPOS;\
	i = Name##_brute_force_search(&x[idx], size - idx, cstr, cstr_len);\
	if (i == NPOS) return i;\
	return i + idx;\
}\
\
size_t Name##_find_c(Type *x, Type c, size_t idx)\
{\
	assert(x && "String_find_c");\
	return Name##_find(x, &c, idx, 1);\
}\
\
size_t Name##_rfind(Type *x, Type *cstr, size_t idx, size_t cstr_len)\
{\
	size_t size;\
	assert(x && "String_rfind");\
	assert(cstr && "String_rfind");\
	if (cstr_len == NPOS) {\
		cstr_len = Name##my_strlen(cstr);\
	}\
	size = Name##my_strlen(x);\
	if (size - cstr_len < idx) {\
		idx = size - cstr_len;\
	}\
	return Name##_brute_force_search_r(&x[0], idx + cstr_len, cstr, cstr_len);\
}\
\
size_t Name##_rfind_c(Type *x, Type c, size_t idx)\
{\
	assert(x && "String_rfind_c");\
	return Name##_rfind(x, &c, idx, 1);\
}\
\
size_t Name##_find_first_of(Type *x, Type *cstr, size_t idx, size_t cstr_len)\
{\
	size_t i, j;\
	size_t size;\
	assert(x && "String_find_first_of");\
	assert(cstr && "String_find_first_of");\
	size = Name##my_strlen(x);\
	if (size < idx) return NPOS;\
	if (cstr_len == NPOS) {\
		cstr_len = Name##my_strlen(cstr);\
	}\
	for (i = idx; i < size; i++) {\
		for (j = 0; j < cstr_len; j++) {\
			if (x[i] == cstr[j]) {\
				return i;\
			}\
		}\
	}\
	return NPOS;\
}\
\
size_t Name##_find_first_of_c(Type *x, Type c, size_t idx)\
{\
	assert(x && "String_find_first_of_c");\
	return Name##_find_first_of(x, &c, idx, 1);\
}\
\
size_t Name##_find_last_of(Type *x, Type *cstr, size_t idx, size_t cstr_len)\
{\
	size_t i, j;\
	size_t size;\
	assert(x && "String_find_last_of");\
	assert(cstr && "String_find_last_of");\
	size = Name##my_strlen(x);\
	if (size <= idx) {\
		idx = size -1;\
	}\
	if (cstr_len == NPOS) {\
		cstr_len = Name##my_strlen(cstr);\
	}\
	for (i = idx +1; i > 0; i--) {\
		for (j = 0; j < cstr_len; j++) {\
			if (x[i-1] == cstr[j]) {\
				return i-1;\
			}\
		}\
	}\
	return NPOS;\
}\
\
size_t Name##_find_last_of_c(Type *x, Type c, size_t idx)\
{\
	assert(x && "String_find_last_of_c");\
	return Name##_find_last_of(x, &c, idx, 1);\
}\
\
size_t Name##_find_first_not_of(Type *x, Type *cstr, size_t idx, size_t cstr_len)\
{\
	size_t i, j;\
	size_t size;\
	assert(x && "String_find_first_not_of");\
	assert(cstr && "String_find_first_not_of");\
	size = Name##my_strlen(x);\
	if (size < idx) return NPOS;\
	if (cstr_len == NPOS) {\
		cstr_len = Name##my_strlen(cstr);\
	}\
	if (cstr_len == 0) {\
		return idx;\
	}\
	for (i = idx; i < size; i++) {\
		for (j = 0; j < cstr_len; j++) {\
			if (x[i] != cstr[j]) {\
				if (j == cstr_len - 1) return i;\
			} else {\
				break;\
			}\
		}\
	}\
	return NPOS;\
}\
\
size_t Name##_find_first_not_of_c(Type *x, Type c, size_t idx)\
{\
	assert(x && "String_find_first_not_of_c");\
	return Name##_find_first_not_of(x, &c, idx, 1);\
}\
\
size_t Name##_find_last_not_of(Type *x, Type *cstr, size_t idx, size_t cstr_len)\
{\
	size_t i, j;\
	size_t size;\
	assert(x && "String_find_last_not_of");\
	assert(cstr && "String_find_last_not_of");\
	size = Name##my_strlen(x);\
	if (size <= idx) {\
		idx = size -1;\
	}\
	if (cstr_len == NPOS) {\
		cstr_len = Name##my_strlen(cstr);\
	}\
	if (cstr_len == 0) {\
		return idx;\
	}\
	for (i = idx +1; i > 0; i--) {\
		for (j = 0; j < cstr_len; j++) {\
			if (x[i-1] != cstr[j]) {\
				if (j == cstr_len - 1) return i-1;\
			} else {\
				break;\
			}\
		}\
	}\
	return NPOS;\
}\
\
size_t Name##_find_last_not_of_c(Type *x, Type c, size_t idx)\
{\
	assert(x && "String_find_last_not_of_c");\
	return Name##_find_last_not_of(x, &c, idx, 1);\
}\
\


#endif /* CSTL_STRING_H_INCLUDED */
