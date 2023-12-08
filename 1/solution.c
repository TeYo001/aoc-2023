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

#define MAX_PREFIX_TREE_BRANCH_COUNT 32
#define PT_NO_VALUE -1
typedef struct PrefixTree_WordNumbers {
    struct PrefixTree_WordNumbers* parent;
    unsigned int branch_count;
    char branch_keys[MAX_PREFIX_TREE_BRANCH_COUNT];
    struct PrefixTree_WordNumbers* branch_values[MAX_PREFIX_TREE_BRANCH_COUNT];
    int value;
} PrefixTree_WordNumbers;

PrefixTree_WordNumbers* pt_next(const PrefixTree_WordNumbers* pt, const char ch)
{
    for (unsigned int i = 0; i < pt->branch_count; i++) {
        if (pt->branch_keys[i] == ch) {
            return pt->branch_values[i];
        }
    }
    return NULL;
}

PrefixTree_WordNumbers* pt_base(const PrefixTree_WordNumbers* pt)
{
    PrefixTree_WordNumbers* base = pt;
    while (base->parent != NULL) {
        base = base->parent;
    }
    return base;
}

PrefixTree_WordNumbers* pt_add_node(PrefixTree_WordNumbers* pt, const char ch)
{
    if (pt_next(pt, ch) == NULL) {
        PrefixTree_WordNumbers* branch = (PrefixTree_WordNumbers*)malloc(sizeof(PrefixTree_WordNumbers));
        branch->parent = pt;
        branch->branch_count = 0;
        branch->value = PT_NO_VALUE;
        pt->branch_keys[pt->branch_count] = ch;
        pt->branch_values[pt->branch_count] = branch;
        pt->branch_count++;
        return branch;
    }
    return NULL;
}

// This function assumes that you CAN add the word
void pt_add_word(PrefixTree_WordNumbers* pt, const Str* word, int value)
{
    PrefixTree_WordNumbers* curr = pt;
    for (unsigned int i = 0; i < word->size; i++) {
        PrefixTree_WordNumbers* new = pt_add_node(curr, word->c_str[i]);
        if (new == NULL) {
            curr = pt_next(curr, word->c_str[i]);
        } else {
            curr = new;
        }
    }
    curr->value = value;
}

Str* init_str(Str* ptr, char* c_str)
{
    unsigned int i = 0;
    while (c_str[i] != '\0') {
        i++;
    }
    ptr->size = i;
    ptr->c_str = c_str;
    return ptr;
}

PrefixTree_WordNumbers* init_forward_pt()
{
    PrefixTree_WordNumbers* base = (PrefixTree_WordNumbers*)malloc(sizeof(PrefixTree_WordNumbers));
    base->parent = NULL;
    base->branch_count = 0;
    base->value = PT_NO_VALUE;
    
    pt_add_word(base, init_str((Str*)malloc(sizeof(Str)), "zero"), 0);
    pt_add_word(base, init_str((Str*)malloc(sizeof(Str)), "one"), 1);
    pt_add_word(base, init_str((Str*)malloc(sizeof(Str)), "two"), 2);
    pt_add_word(base, init_str((Str*)malloc(sizeof(Str)), "three"), 3);
    pt_add_word(base, init_str((Str*)malloc(sizeof(Str)), "four"), 4);
    pt_add_word(base, init_str((Str*)malloc(sizeof(Str)), "five"), 5);
    pt_add_word(base, init_str((Str*)malloc(sizeof(Str)), "six"), 6);
    pt_add_word(base, init_str((Str*)malloc(sizeof(Str)), "seven"), 7);
    pt_add_word(base, init_str((Str*)malloc(sizeof(Str)), "eight"), 8);
    pt_add_word(base, init_str((Str*)malloc(sizeof(Str)), "nine"), 9);
    
    return base;
}

PrefixTree_WordNumbers* init_backward_pt()
{
    PrefixTree_WordNumbers* base = (PrefixTree_WordNumbers*)malloc(sizeof(PrefixTree_WordNumbers));
    base->parent = NULL;
    base->branch_count = 0;
    base->value = PT_NO_VALUE;
    
    pt_add_word(base, init_str((Str*)malloc(sizeof(Str)),"orez"), 0);
    pt_add_word(base, init_str((Str*)malloc(sizeof(Str)),"eno"), 1);
    pt_add_word(base, init_str((Str*)malloc(sizeof(Str)),"owt"), 2);
    pt_add_word(base, init_str((Str*)malloc(sizeof(Str)),"eerht"), 3);
    pt_add_word(base, init_str((Str*)malloc(sizeof(Str)),"ruof"), 4);
    pt_add_word(base, init_str((Str*)malloc(sizeof(Str)),"evif"), 5);
    pt_add_word(base, init_str((Str*)malloc(sizeof(Str)),"xis"), 6);
    pt_add_word(base, init_str((Str*)malloc(sizeof(Str)),"neves"), 7);
    pt_add_word(base, init_str((Str*)malloc(sizeof(Str)),"thgie"), 8);
    pt_add_word(base, init_str((Str*)malloc(sizeof(Str)),"enin"), 9);
    
    return base;
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
    for (int i = str->end - 1; i >= str->start; i--)
    {
        if (i < 0) break;
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

int parse_word_num(const PrefixTree_WordNumbers* in_pt, PrefixTree_WordNumbers** out_next_pt, const char ch)
{
    PrefixTree_WordNumbers* out;
    out = pt_next(in_pt, ch);
    if (out == NULL) {
        if (in_pt->parent != NULL) {
            *out_next_pt = pt_base(in_pt);
            return parse_word_num(*out_next_pt, out_next_pt, ch);
        }

        *out_next_pt = pt_base(in_pt);
        return PT_NO_VALUE;
    }
    *out_next_pt = out;
    return out->value;
}

int num_to_word_len(int num)
{
    const int arr[10] = {
        strlen("zero"),
        strlen("one"),
        strlen("two"),
        strlen("three"),
        strlen("four"),
        strlen("five"),
        strlen("six"),
        strlen("seven"),
        strlen("eight"),
        strlen("nine")
    };

    return arr[num];
}

int str_findl_word(const PrefixTree_WordNumbers* forward, const StrSlice* str, int* out_value)
{
    PrefixTree_WordNumbers* curr = forward;
    for (int i = str->start; i < str->end; i++)
    {
        int value = parse_word_num(curr, &curr, str->ref_str->c_str[i]);
        if (value != PT_NO_VALUE) {
            *out_value = value;
            return i;
        }
    }
    
    *out_value = PT_NO_VALUE;
    return -1;
}

int str_findr_word(const PrefixTree_WordNumbers* backward, const StrSlice* str, int* out_value)
{
    PrefixTree_WordNumbers* curr = backward; 
    for (int i = str->end - 1; i >= str->start; i--)
    {
        if (i < 0) break;
        int value = parse_word_num(curr, &curr, str->ref_str->c_str[i]);
        if (value != PT_NO_VALUE) {
            *out_value = value;
            return i;
        }
    }
    *out_value = PT_NO_VALUE;
    return -1;
}

int main(int argc, char argv[])
{
    int result = 0;
    
    Str input = {0};
    init_str(&input, read_file("input.txt"));
    
    PrefixTree_WordNumbers* forward = init_forward_pt();
    PrefixTree_WordNumbers* backward = init_backward_pt();

    unsigned int last_line_end = 0;
    for (unsigned int i = 0; i < input.size; i++)
    {
        if (input.c_str[i] == '\n') {
            StrSlice slice = init_str_slice(&input, last_line_end, i);
            int left_num_idx = str_findl(&slice, is_num);
            int left_num_word_value;
            int left_num_word_idx = str_findl_word(forward, &slice, &left_num_word_value);
            int right_num_idx = str_findr(&slice, is_num);
            int right_num_word_value;
            int right_num_word_idx = str_findr_word(backward, &slice, &right_num_word_value);

            if (left_num_idx == -1 && left_num_word_idx == -1) {
                printf("2 ERROR\n");
                exit(1);
            }

            if (right_num_idx == -1 && right_num_word_idx == -1) {
                printf("1 ERROR\n");
                exit(1);
            }
            
            if (right_num_idx == right_num_word_idx ||
                left_num_idx == left_num_word_idx) {
                printf("cringe\n");
                printf("ERROR\n");
                exit(1);
            }
            char* num_str = (char*)malloc(sizeof(char) * 3);
            char* word_str_buffer = (char*)malloc(sizeof(char) * 2);

            bool is_left_num = left_num_idx < left_num_word_idx;
            is_left_num |= left_num_word_idx == -1;
            is_left_num &= left_num_idx != -1;
            
            bool is_right_num = right_num_idx > right_num_word_idx;
            is_right_num |= right_num_word_idx == -1;
            is_right_num &= right_num_idx != -1;

            if (is_left_num) {
                num_str[0] = slice.ref_str->c_str[left_num_idx];
            } else {
                num_str[0] = itoa(left_num_word_value, word_str_buffer, 10)[0];
            }
            if (is_right_num) {
                num_str[1] = slice.ref_str->c_str[right_num_idx];
            } else {
                num_str[1] = itoa(right_num_word_value, word_str_buffer, 10)[0];
            }
            
            num_str[2] = '\0';
            result += atoi(num_str);
            last_line_end = i;
            
            free(num_str);
            free(word_str_buffer);
        }
    }

    printf("%d\n", result);

    return 0;
}
