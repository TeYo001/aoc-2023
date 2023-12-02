#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    unsigned int size;
    char* c_str;
} Str;

typedef struct {
    unsigned int start, end;
    const Str* ref_str;
} StrSlice;

typedef struct {} PrefixTree_WordNumbers;

#define MAX_PREFIX_TREE_BRANCH_COUNT 32
typedef struct {
    unsigned int branch_count;
    char branch_keys[MAX_PREFIX_TREE_BRANCH_COUNT];
    PrefixTree_WordNumbers* branch_values[MAX_PREFIX_TREE_BRANCH_COUNT];
    bool is_end;
} PrefixTree_WordNumbers;

PrefixTree_WordNumbers* pt_next(PrefixTree_WordNumbers* pt, const char ch)
{
    for (unsigned int i = 0; i < branch_count; i++)
    {
        // TODO(TeYo): Continue from here
    }
}

Str init_str(char* c_str)
{
    unsigned int i = 0;
    while (c_str[i] != '\0') {
        i++;
    }
    Str ret = {0};
    ret.size = i + 1;
    ret.c_str = c_str;
    return ret;
}

StrSlice init_str_slice(const Str* ref_str, unsigned int start, unsigned int end)
{
    StrSlice slice = {0};
    slice.ref_str = ref_str;
    slice.start = start;
    slice.end = end;
    return slice;
}

void print_str_slice(const StrSlice* str)
{
    printf("%.*s", str->end - str->start, &str->ref_str->c_str[str->start]);
}

int str_reduce_int(const Str* str, int initial_value, int(*func)(int, const char))
{
    int result = initial_value;
    for(unsigned int i = 0; i < str->size; i++) {
        result = func(result, str->c_str[i]);
    }
    return result;
}

int str_findl(const StrSlice* str, bool(*func)(const char))
{
    for (unsigned int i = str->start; i < str->end; i++)
    {
        bool test = func(str->ref_str->c_str[i]);
        if (test) {
            return i;
        }
    }
    
    return -1;
}

int str_findr(const StrSlice* str, bool(*func)(const char))
{
    for (unsigned int i = str->end; i >= str->start; i--)
    {
        if (func(str->ref_str->c_str[i])) {
            return i;
        }
    }

    return -1;
}

char* read_file(char* file_name) 
{
    FILE *file = fopen(file_name, "r");
    char *code;
    size_t n = 0;
    int c;

    if (file == NULL) return NULL; //could not open file
    fseek(file, 0, SEEK_END);
    long f_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    code = malloc(f_size);

    while ((c = fgetc(file)) != EOF) {
        code[n++] = (char)c;
    }

    code[n] = '\0';

    return code;
}

bool is_num(const char ch)
{
    return (ch >= 48 && ch <= 57);
}

bool is_word_num(const PrefixTree* in_pt, PrefixTree* out_next_pt)
{
    
}

int main(int argc, char argv[])
{
    int result = 0;

    Str input = init_str(read_file("input.txt"));
    
    unsigned int last_line_end = 0;
    for (unsigned int i = 0; i < input.size; i++)
    {
        if (input.c_str[i] == '\n') {
            StrSlice slice = init_str_slice(&input, last_line_end, i);
            int left_num_idx = str_findl(&slice, is_num);
            int right_num_idx = str_findr(&slice, is_num);

            if (left_num_idx == -1 || right_num_idx == -1) {
                printf("ERROR\n");
                exit(1);
            }
            char* num_str = (char*)malloc(sizeof(char) * 3);
            num_str[0] = slice.ref_str->c_str[left_num_idx];
            num_str[1] = slice.ref_str->c_str[right_num_idx];
            num_str[2] = '\0';
            result += atoi(num_str);

            last_line_end = i;
        }
    }

    printf("%d\n", result);

    return 0;
}
