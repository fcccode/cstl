#include "../cstl/string.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <string>

#include "../heap.h"
#ifdef MY_MALLOC
extern Heap heap;
#define malloc(s)		Heap_alloc(&heap, s)
#define realloc(p, s)	Heap_realloc(&heap, p, s)
#define free(p)			Heap_free(&heap, p)
#endif

STRING_INTERFACE(String, char)
STRING_IMPLEMENT(String, char)


using namespace std;


void StringTest_test_1_1(void)
{
	String *x;
	String *y;
	string s;
	size_t i;
	printf("***** test_1_1 *****\n");
	/* 初期状態 */
	x = String_new();
	assert(String_empty(x));
	assert(String_size(x) == 0);
	String_delete(x);
	/* new_cstr */
	x = String_new_cstr("", NPOS);
	assert(String_empty(x));
	assert(String_size(x) == 0);
	String_delete(x);
	x = String_new_cstr("abcdef", NPOS);
	assert(!String_empty(x));
	assert(String_size(x) == 6);
	assert(String_size(x) == String_length(x));
	String_delete(x);
	x = String_new_cstr("abcdef", 0);
	assert(strcmp(String_c_str(x), "") == 0);
	String_delete(x);
	x = String_new_cstr("abcdefghijklmn", 7);
	assert(strcmp(String_c_str(x), "abcdefg") == 0);
	/* new_copy */
	y = String_new_copy(x);
	assert(strcmp(String_c_str(y), "abcdefg") == 0);
	assert(!String_empty(y));
	assert(String_size(y) == 7);
	String_delete(y);
	/* new_c */
	y = String_new_c(10, 'a');
	assert(strcmp(String_c_str(y), "aaaaaaaaaa") == 0);
	assert(String_size(y) == 10);
	String_delete(y);
	y = String_new_c(0, 'b');
	assert(strcmp(String_c_str(y), "") == 0);
	assert(String_size(y) == 0);
	String_delete(y);
	y = String_new_c(1, 'c');
	assert(strcmp(String_c_str(y), "c") == 0);
	assert(String_size(y) == 1);
	/* c_str */
//    printf("%s\n", String_c_str(x));
	s = String_c_str(x);
	assert(strcmp(s.c_str(), String_c_str(x)) == 0);
	/* at */
	for (i = 0; i < String_length(x); i++) {
		assert(s.at(i) == *String_at(x, i));
	}
	/* clear */
	String_clear(x);
	assert(String_empty(x));
	assert(String_size(x) == 0);

	String_delete(x);
	String_delete(y);
}

void StringTest_test_1_2(void)
{
	String *x;
	String *y;
	printf("***** test_1_2 *****\n");
	x = String_new();
	/* capacity */
	/* reserve */
	String_reserve(x, 100);
	assert(String_size(x) == 0);
	assert(String_capacity(x) == 100);
	String_reserve(x, 99);
	assert(String_capacity(x) == 100);
	String_reserve(x, 101);
	assert(String_capacity(x) == 101);
	/* shrink */
	String_shrink(x, 10);
	assert(String_capacity(x) == 10);
	String_shrink(x, 11);
	assert(String_capacity(x) == 10);
	String_shrink(x, 9);
	assert(String_capacity(x) == 9);
	/* resize */
	String_resize(x, 10, 'a');
	assert(String_size(x) == 10);
	assert(strcmp("aaaaaaaaaa", String_c_str(x)) == 0);
	String_resize(x, 15, 'b');
	assert(String_size(x) == 15);
	assert(strcmp("aaaaaaaaaabbbbb", String_c_str(x)) == 0);
	String_resize(x, 5, 'b');
	assert(String_size(x) == 5);
	assert(strcmp("aaaaa", String_c_str(x)) == 0);
	String_resize(x, 0, 'b');
	assert(String_size(x) == 0);
	assert(strcmp("", String_c_str(x)) == 0);
	/* assign */
	String_assign(x, "abcdefghijklmnopqrstuvwxyz", NPOS);
	assert(String_size(x) == 26);
	assert(strcmp("abcdefghijklmnopqrstuvwxyz", String_c_str(x)) == 0);
	/* erase */
	String_erase(x, 0, 3);
	assert(String_size(x) == 23);
	assert(strcmp("defghijklmnopqrstuvwxyz", String_c_str(x)) == 0);
	String_erase(x, 0, 0);
	assert(String_size(x) == 23);
	assert(strcmp("defghijklmnopqrstuvwxyz", String_c_str(x)) == 0);
	String_erase(x, 1, 1);
	assert(String_size(x) == 22);
	assert(strcmp("dfghijklmnopqrstuvwxyz", String_c_str(x)) == 0);
	String_erase(x, 5, 10);
	assert(String_size(x) == 12);
	assert(strcmp("dfghituvwxyz", String_c_str(x)) == 0);
	String_erase(x, 11, 1);
	assert(String_size(x) == 11);
	assert(strcmp("dfghituvwxy", String_c_str(x)) == 0);
	String_erase(x, 5, NPOS);
	assert(String_size(x) == 5);
	assert(strcmp("dfghi", String_c_str(x)) == 0);
	String_erase(x, 0, NPOS);
	assert(String_size(x) == 0);
	assert(strcmp("", String_c_str(x)) == 0);
	/* compare */
	String_assign(x, "abcdefg", NPOS);
	y = String_new_cstr("abcdefgh", NPOS);
	assert(String_compare(x, y) < 0);
	String_erase(y, String_size(y)-2, 2);
	assert(String_compare(x, y) > 0);
	String_erase(x, String_size(x)-1, 1);
	assert(String_compare(x, y) == 0);

	String_delete(x);
	String_delete(y);
}

void StringTest_test_1_3(void)
{
	String *x;
	printf("***** test_1_3 *****\n");
	x = String_new();
	/* assign */
	String_assign(x, "abcdefghijklmnopqrstuvwxyz", NPOS);
	assert(String_size(x) == 26);
	assert(strcmp("abcdefghijklmnopqrstuvwxyz", String_c_str(x)) == 0);
	String_assign(x, "abcdefghijklmnopqrstuvwxyz", 10);
	assert(String_size(x) == 10);
	assert(strcmp("abcdefghij", String_c_str(x)) == 0);
	String_assign(x, "abcdefghijklmnopqrstuvwxyz", 0);
	assert(String_size(x) == 0);
	assert(strcmp("", String_c_str(x)) == 0);
	String_assign(x, "", 0);
	assert(String_size(x) == 0);
	assert(strcmp("", String_c_str(x)) == 0);
	/* assign_c */
	String_assign_c(x, 10, 'a');
	assert(String_size(x) == 10);
	assert(strcmp("aaaaaaaaaa", String_c_str(x)) == 0);
	String_assign_c(x, 1, 'b');
	assert(String_size(x) == 1);
	assert(strcmp("b", String_c_str(x)) == 0);
	String_assign_c(x, 0, 'c');
	assert(String_size(x) == 0);
	assert(strcmp("", String_c_str(x)) == 0);
	/* append */
	String_append(x, "abcdef", NPOS);
	assert(String_size(x) == 6);
	assert(strcmp("abcdef", String_c_str(x)) == 0);
	String_append(x, "gh", NPOS);
	assert(String_size(x) == 8);
	assert(strcmp("abcdefgh", String_c_str(x)) == 0);
	String_append(x, "ijkl", 3);
	assert(String_size(x) == 11);
	assert(strcmp("abcdefghijk", String_c_str(x)) == 0);
	String_append(x, "l", 1);
	assert(String_size(x) == 12);
	assert(strcmp("abcdefghijkl", String_c_str(x)) == 0);
	String_append(x, "mno", 0);
	assert(String_size(x) == 12);
	assert(strcmp("abcdefghijkl", String_c_str(x)) == 0);
	String_append(x, "m", NPOS);
	assert(String_size(x) == 13);
	assert(strcmp("abcdefghijklm", String_c_str(x)) == 0);
	String_clear(x);
	assert(String_size(x) == 0);
	/* append_c */
	String_append_c(x, 5, 'a');
	assert(String_size(x) == 5);
	assert(strcmp("aaaaa", String_c_str(x)) == 0);
	String_append_c(x, 2, 'b');
	assert(String_size(x) == 7);
	assert(strcmp("aaaaabb", String_c_str(x)) == 0);
	String_append_c(x, 4, 'c');
	assert(String_size(x) == 11);
	assert(strcmp("aaaaabbcccc", String_c_str(x)) == 0);
	String_append_c(x, 0, 'd');
	assert(String_size(x) == 11);
	assert(strcmp("aaaaabbcccc", String_c_str(x)) == 0);
	/* push_back */
	String_push_back(x, 'd');
	String_push_back(x, 'e');
	String_push_back(x, 'f');
	assert(String_size(x) == 14);
	assert(strcmp("aaaaabbccccdef", String_c_str(x)) == 0);
	String_clear(x);
	assert(String_size(x) == 0);
	/* insert */
	String_insert(x, 0, "abcdef", NPOS);
	assert(String_size(x) == 6);
	assert(strcmp("abcdef", String_c_str(x)) == 0);
	String_insert(x, 0, "gh", NPOS);
	assert(String_size(x) == 8);
	assert(strcmp("ghabcdef", String_c_str(x)) == 0);
	String_insert(x, 1, "ijk", NPOS);
	assert(String_size(x) == 11);
	assert(strcmp("gijkhabcdef", String_c_str(x)) == 0);
	String_insert(x, String_size(x), "lmn", NPOS);
	assert(String_size(x) == 14);
	assert(strcmp("gijkhabcdeflmn", String_c_str(x)) == 0);
	String_insert(x, String_size(x)-1, "opqr", 3);
	assert(String_size(x) == 17);
	assert(strcmp("gijkhabcdeflmopqn", String_c_str(x)) == 0);
	String_insert(x, 5, "rst", 0);
	assert(String_size(x) == 17);
	assert(strcmp("gijkhabcdeflmopqn", String_c_str(x)) == 0);
	String_clear(x);
	assert(String_size(x) == 0);
	/* insert_c */
	String_insert_c(x, 0, 5, 'a');
	assert(String_size(x) == 5);
	assert(strcmp("aaaaa", String_c_str(x)) == 0);
	String_insert_c(x, 0, 3, 'b');
	assert(String_size(x) == 8);
	assert(strcmp("bbbaaaaa", String_c_str(x)) == 0);
	String_insert_c(x, 1, 2, 'c');
	assert(String_size(x) == 10);
	assert(strcmp("bccbbaaaaa", String_c_str(x)) == 0);
	String_insert_c(x, 2, 1, 'd');
	assert(String_size(x) == 11);
	assert(strcmp("bcdcbbaaaaa", String_c_str(x)) == 0);
	String_insert_c(x, String_size(x), 3, 'e');
	assert(String_size(x) == 14);
	assert(strcmp("bcdcbbaaaaaeee", String_c_str(x)) == 0);
	String_insert_c(x, String_size(x)-1, 2, 'f');
	assert(String_size(x) == 16);
	assert(strcmp("bcdcbbaaaaaeeffe", String_c_str(x)) == 0);
	String_insert_c(x, 5, 0, 'g');
	assert(String_size(x) == 16);
	assert(strcmp("bcdcbbaaaaaeeffe", String_c_str(x)) == 0);
	String_insert_c(x, 0, 17, 'g');
	assert(String_size(x) == 33);
	assert(strcmp("gggggggggggggggggbcdcbbaaaaaeeffe", String_c_str(x)) == 0);
	String_clear(x);
	assert(String_size(x) == 0);


#if 0
	string s;
	s.insert(0, "abc", 3);
	printf("%s\n", s.c_str());
	s.insert(0, "def", 3);
	printf("%s\n", s.c_str());
	s.erase(0, string::npos);
	printf("%s\n", s.c_str());
#endif

	String_delete(x);
}

void StringTest_test_1_4(void)
{
	String *x;
	printf("***** test_1_4 *****\n");
	x = String_new();
	String_assign(x, "abcdefghijklmnopqrstuvwxyz", NPOS);
	assert(String_size(x) == 26);
	assert(strcmp("abcdefghijklmnopqrstuvwxyz", String_c_str(x)) == 0);
	/* replace */
	String_replace(x, 0, 3, "ABC", NPOS);
	assert(String_size(x) == 26);
	assert(strcmp("ABCdefghijklmnopqrstuvwxyz", String_c_str(x)) == 0);
	String_replace(x, 6, 3, "GHI", NPOS);
	assert(String_size(x) == 26);
	assert(strcmp("ABCdefGHIjklmnopqrstuvwxyz", String_c_str(x)) == 0);
	String_replace(x, String_size(x)-3, NPOS, "XYZ", NPOS);
	assert(String_size(x) == 26);
	assert(strcmp("ABCdefGHIjklmnopqrstuvwXYZ", String_c_str(x)) == 0);
	String_shrink(x, 0);
	String_replace(x, 3, 0, "DEF", NPOS);
	assert(String_size(x) == 29);
	assert(strcmp("ABCDEFdefGHIjklmnopqrstuvwXYZ", String_c_str(x)) == 0);
	String_replace(x, 6, 3, "", NPOS);
	assert(String_size(x) == 26);
	assert(strcmp("ABCDEFGHIjklmnopqrstuvwXYZ", String_c_str(x)) == 0);
	String_replace(x, 0, 3, "hogehoge", 4);
	assert(String_size(x) == 27);
	assert(strcmp("hogeDEFGHIjklmnopqrstuvwXYZ", String_c_str(x)) == 0);
	String_replace(x, 0, NPOS, "", NPOS);
	assert(String_size(x) == 0);
	assert(strcmp("", String_c_str(x)) == 0);
	String_replace(x, 0, NPOS, "abc", NPOS);
	assert(String_size(x) == 3);
	assert(strcmp("abc", String_c_str(x)) == 0);
	/* replace_c */
	String_assign(x, "abcdefghijklmnopqrstuvwxyz", NPOS);
	assert(String_size(x) == 26);
	assert(strcmp("abcdefghijklmnopqrstuvwxyz", String_c_str(x)) == 0);
	String_replace_c(x, 0, 3, 3, 'A');
	assert(String_size(x) == 26);
	assert(strcmp("AAAdefghijklmnopqrstuvwxyz", String_c_str(x)) == 0);
	String_replace_c(x, 6, 3, 3, 'G');
	assert(String_size(x) == 26);
	assert(strcmp("AAAdefGGGjklmnopqrstuvwxyz", String_c_str(x)) == 0);
	String_replace_c(x, String_size(x)-3, NPOS, 3, 'X');
	assert(String_size(x) == 26);
	assert(strcmp("AAAdefGGGjklmnopqrstuvwXXX", String_c_str(x)) == 0);
	String_shrink(x, 0);
	String_replace_c(x, 3, 0, 3, 'D');
	assert(String_size(x) == 29);
	assert(strcmp("AAADDDdefGGGjklmnopqrstuvwXXX", String_c_str(x)) == 0);
	String_replace_c(x, 6, 3, 0, 'E');
	assert(String_size(x) == 26);
	assert(strcmp("AAADDDGGGjklmnopqrstuvwXXX", String_c_str(x)) == 0);
	String_replace_c(x, 0, 3, 4, '0');
	assert(String_size(x) == 27);
	assert(strcmp("0000DDDGGGjklmnopqrstuvwXXX", String_c_str(x)) == 0);
	String_replace_c(x, 0, NPOS, 0, '0');
	assert(String_size(x) == 0);
	assert(strcmp("", String_c_str(x)) == 0);
	String_replace_c(x, 0, NPOS, 10, '0');
	assert(String_size(x) == 10);
	assert(strcmp("0000000000", String_c_str(x)) == 0);

#if 0
	string s = "abcdefghijklmnopqrstuvwxyz";
	s.replace(1, string::npos, "hoge");
	printf("%s\n", s.c_str());
#endif

	String_delete(x);
}

void print_find_test(const char *cstr, string &t, size_t i, size_t j, size_t k, size_t idx1, size_t idx2, const char *f)
{
	if (idx1 != idx2) {
		if (k == NPOS) {
			t.assign(cstr);
		} else {
			t.assign(cstr, k);
		}
		printf("cstr[%s]\ni[%3d], j[%3d], k[%3d], idx1[%3d], idx2[%3d] :%s\n", t.c_str(), i, j, k, idx1, idx2, f);
	}
}

void find_test(String *x, const string &s, size_t i, size_t j, size_t k, char *cstr)
{
	string t;
	size_t idx1, idx2;
	/* find */
	idx1 = String_find(String_c_str(x), &cstr[j], i, k);
	idx2 = (k == NPOS) ? s.find(&cstr[j], i) : s.find(&cstr[j], i, k);
	/* STLの実装によって動作が違うことがある */
	if (i == NPOS) {
		assert(idx1 == NPOS);
	} else {
		print_find_test(&cstr[j], t, i, j, k, idx1, idx2, "find");
		assert(idx1 == idx2);
	}
	/* find_c */
	idx1 = String_find_c(String_c_str(x), cstr[j], i);
	idx2 = s.find(cstr[j], i);
	print_find_test(&cstr[j], t, i, j, k, idx1, idx2, "find_c");
	assert(idx1 == idx2);
	assert(String_find_c(String_c_str(x), cstr[j], i) == String_find_first_of_c(String_c_str(x), cstr[j], i));

	/* rfind */
	idx1 = String_rfind(String_c_str(x), &cstr[j], i, k);
	idx2 = (k == NPOS) ? s.rfind(&cstr[j], i) : s.rfind(&cstr[j], i, k);
	print_find_test(&cstr[j], t, i, j, k, idx2, idx2, "rfind");
	assert(idx1 == idx2);
	/* rfind_c */
	idx1 = String_rfind_c(String_c_str(x), cstr[j], i);
	idx2 = s.rfind(cstr[j], i);
	print_find_test(&cstr[j], t, i, j, k, idx2, idx2, "rfind_c");
	assert(idx1 == idx2);
	assert(String_rfind_c(String_c_str(x), cstr[j], i) == String_find_last_of_c(String_c_str(x), cstr[j], i));

	/* find_first_of */
	idx1 = String_find_first_of(String_c_str(x), &cstr[j], i, k);
	idx2 = (k == NPOS) ? s.find_first_of(&cstr[j], i) : s.find_first_of(&cstr[j], i, k);
	print_find_test(&cstr[j], t, i, j, k, idx1, idx2, "find_first_of");
	assert(idx1 == idx2);
	/* find_first_of_c */
	idx1 = String_find_first_of_c(String_c_str(x), cstr[j], i);
	idx2 = s.find_first_of(cstr[j], i);
	print_find_test(&cstr[j], t, i, j, k, idx1, idx2, "find_first_of_c");
	assert(idx1 == idx2);

	/* find_last_of */
	idx1 = String_find_last_of(String_c_str(x), &cstr[j], i, k);
	idx2 = (k == NPOS) ? s.find_last_of(&cstr[j], i) : s.find_last_of(&cstr[j], i, k);
	print_find_test(&cstr[j], t, i, j, k, idx1, idx2, "find_last_of");
	assert(idx1 == idx2);
	/* find_last_of_c */
	idx1 = String_find_last_of_c(String_c_str(x), cstr[j], i);
	idx2 = s.find_last_of(cstr[j], i);
	print_find_test(&cstr[j], t, i, j, k, idx1, idx2, "find_last_of_c");
	assert(idx1 == idx2);

	/* find_first_not_of */
	idx1 = String_find_first_not_of(String_c_str(x), &cstr[j], i, k);
	idx2 = (k == NPOS) ? s.find_first_not_of(&cstr[j], i) : s.find_first_not_of(&cstr[j], i, k);
	print_find_test(&cstr[j], t, i, j, k, idx1, idx2, "find_first_not_of");
	assert(idx1 == idx2);
	/* find_first_not_of_c */
	idx1 = String_find_first_not_of_c(String_c_str(x), cstr[j], i);
	idx2 = s.find_first_not_of(cstr[j], i);
	print_find_test(&cstr[j], t, i, j, k, idx1, idx2, "find_first_not_of_c");
	assert(idx1 == idx2);

	/* find_last_not_of */
	idx1 = String_find_last_not_of(String_c_str(x), &cstr[j], i, k);
	idx2 = (k == NPOS) ? s.find_last_not_of(&cstr[j], i) : s.find_last_not_of(&cstr[j], i, k);
	print_find_test(&cstr[j], t, i, j, k, idx1, idx2, "find_last_not_of");
	assert(idx1 == idx2);
	/* find_last_not_of_c */
	idx1 = String_find_last_not_of_c(String_c_str(x), cstr[j], i);
	idx2 = s.find_last_not_of(cstr[j], i);
	print_find_test(&cstr[j], t, i, j, k, idx1, idx2, "find_last_not_of_c");
	assert(idx1 == idx2);
}

void StringTest_test_1_5(void)
{
	String *x;
	String *y;
	string s;
	size_t i, j, k;
	printf("***** test_1_5 *****\n");
	x = String_new_cstr("abc abcd abcde abcdef abcdefg abcdefgh abcdefghi", NPOS);
	y = String_new_cstr("abcdefghijklmnopqrstuvwxyz", NPOS);
	s = String_c_str(x);
	assert(strcmp(s.c_str(), String_c_str(x)) == 0);
	for (i = 0; i < String_size(x); i++) {
		for (j = 0; j < String_size(x); j++) {
			for (k = 0; k < String_size(x) -j +1; k++) {
				find_test(x, s, i, j, k, String_c_str(x));
			}
			if (k == String_size(x) -j +1) {
				find_test(x, s, i, j, NPOS, String_c_str(x));
			}
			if (j <= String_size(y)) {
				for (k = 0; k < String_size(y) -j +1; k++) {
					find_test(x, s, i, j, k, String_c_str(y));
				}
				if (k == String_size(y) -j +1) {
					find_test(x, s, i, j, NPOS, String_c_str(y));
				}
			}
		}
	}
	for (j = 0; j < String_size(x); j++) {
		for (k = 0; k < String_size(x) -j +1; k++) {
			find_test(x, s, NPOS, j, k, String_c_str(x));
		}
		if (k == String_size(x) -j +1) {
			find_test(x, s, NPOS, j, NPOS, String_c_str(x));
		}
		if (j <= String_size(y)) {
			for (k = 0; k < String_size(y) -j +1; k++) {
				find_test(x, s, NPOS, j, k, String_c_str(y));
			}
			if (k == String_size(y) -j +1) {
				find_test(x, s, NPOS, j, NPOS, String_c_str(y));
			}
		}
	}

	String_delete(x);
	String_delete(y);
}


void StringTest_run(void)
{
	printf("\n===== string test =====\n");
	StringTest_test_1_1();
	StringTest_test_1_2();
	StringTest_test_1_3();
	StringTest_test_1_4();
	StringTest_test_1_5();
}

double buf[1024*1024/sizeof(double)];
Heap heap;

int main(void)
{
#ifdef MY_MALLOC
	Heap_init(&heap, buf, sizeof buf);
#endif
	StringTest_run();
#ifdef MY_MALLOC
	DUMP_MEMORY_LEAK(&heap, 0);
#endif
	return 0;
}
