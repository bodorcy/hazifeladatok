#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cdecl_wrap(void *target_function, int argc, void *return_value, ...);

struct TestResult {
    int contents;           // 1: ok, 0: fail, -1: skip
    int return_value;       // 1: ok, 0: fail, -1: skip
    int overflow;           // 1: ok, 0: fail, -1: skip
    int cdecl_registers;    // 1: ok, 0: fail, -1: skip     // cdecl or arm registers to save
};

enum TestOptions {
    TEST_OPTION_CONTENTS = 1 << 0,
    TEST_OPTION_RETURN_VALUE = 1 << 1,
    TEST_OPTION_OVERFLOW = 1 << 2,
    TEST_OPTION_CDECL_REGISTERS = 1 << 3,
};

#ifndef PREFIX
#define PREFIX
#endif

#define FILL (0xFF)
#define MAX_ARRAY_SIZE (40)

static int overflow_check(char *data, int length, char check);
static void parse_arguments(int argc, char** argv, enum TestOptions* test_options, int* test_start, int* test_count);
static void snprint_array(char *output, int output_size, int array[], int length);
static void print_test_results(int test_idx, struct TestResult results);

int rangeNormalizer(int input[], int midValue, int epsilon, int output[]);

static struct TestCase {
    struct {
        int input[100];
        int length;
        int midValue;
        int epsilon;
    } input;
    struct {
        int output[100];
        int length;
    } expected;
} tests[] = {
    { { { 1, 2, 3, 4, 5, 0 }, 6, 3, 1 },
      { { 0 }, 1, } },

    { { { }, 0, 0, 0 },
      { { }, 0, } },

    { { { 1, 2, 3, 4, 0 }, 5, 4, 2 },
      { { -1, 0 }, 2, } },

    { { { -4, -2, -3, 0 }, 4, -4, 2 },
      { { 0, 1 }, 2, } },

    { { { -4, -3, -1, 1, 2, 3, 4, 0 }, 8, 0, 3 },
      { { -1, 1, 2 }, 3, } },

    { { { -4, -3, -1, 1, 2, 3, 4, 0 }, 8, 50, 3 },
      { { }, 0, } },

    { { { 2, 10, 30, 3, 1, 0 }, 6, 0, 40 },
      { { 2, 10, 30, 3, 1 }, 5, } },

    { { { -2, 10, -30, -3, 1, 0 }, 6, 0, 40 },
      { { -2, 10, -30, -3, 1 }, 5, } },

};

//
int TestCall(struct TestCase *test_case, int *return_value, int *actual_output_array) {
    int cdecl_failed_regs = cdecl_wrap(rangeNormalizer, 4, return_value,
                                       test_case->input.input,
                                       test_case->input.midValue,
                                       test_case->input.epsilon,
                                       actual_output_array);

    return cdecl_failed_regs;
}

int TestContents(struct TestCase *test_case, int return_value, int *actual_output_array) {
    int contents = 0;

    if (memcmp(actual_output_array, test_case->expected.output, test_case->expected.length * sizeof(int)) == 0) {
        contents = 1;
    } else {
        contents = 0;
    }

    char input_array_text[1024];
    snprint_array(input_array_text, sizeof(input_array_text),
                  test_case->input.input, test_case->input.length);

    char output_array_text[1024];
    snprint_array(output_array_text, sizeof(output_array_text),
                  actual_output_array, test_case->expected.length);

    char expected_array_text[1024];
    snprint_array(expected_array_text, sizeof(expected_array_text),
                  test_case->expected.output, test_case->expected.length);

    printf(PREFIX "﻿rangeNormalize([ %s ], %d, %d, output_array)\n",
           input_array_text, test_case->input.midValue, test_case->input.epsilon);

    printf(PREFIX " |-> output_array: [ %s ]\n", output_array_text);
    printf(PREFIX " |-> return value: %d\n", return_value);
    printf(PREFIX " expected:\n");
    printf(PREFIX "  |-> expected output: [ %s ]\n", expected_array_text);
    printf(PREFIX "  |-> expected return value: %d\n", test_case->expected.length);

    return contents;
}

int TestReturnValue(struct TestCase *test_case, int return_value, int *actual_output_array) {
    return return_value == test_case->expected.length;
}


int main(int argc, char** argv) {
    int test_count = sizeof(tests) / sizeof(tests[0]);

    enum TestOptions test_options = TEST_OPTION_CONTENTS | TEST_OPTION_RETURN_VALUE | TEST_OPTION_OVERFLOW | TEST_OPTION_CDECL_REGISTERS;

    int requested_test_idx = -1;
    parse_arguments(argc, argv, &test_options, &requested_test_idx, &test_count);

    int test_start = 0;
    if (requested_test_idx != -1) {
        if (requested_test_idx <= 0 || requested_test_idx > test_count) {
            printf("Unknown test index %d (valid range: [%d;%d]\n", requested_test_idx, 1, test_count);
            return 0;
        }

        test_start = requested_test_idx - 1;
        test_count = test_start + 1;
    }

    for (int test_idx = test_start; test_idx < test_count; test_idx++) {
        printf(PREFIX "\nTest %d:\n", test_idx + 1);

        int actual_output[MAX_ARRAY_SIZE];
        memset(actual_output, FILL, sizeof(actual_output));

        int return_value = -1;
        int cdecl_failed_regs = TestCall(&tests[test_idx], &return_value, actual_output);
        int actual_output_length = return_value;

        struct TestResult results = { -1, -1, -1, -1 };
        if (test_options & TEST_OPTION_CONTENTS) {
            results.contents = TestContents(&tests[test_idx], return_value, actual_output);
        }

        if (test_options & TEST_OPTION_RETURN_VALUE) {
            results.return_value = TestReturnValue(&tests[test_idx], return_value, actual_output);
        }

        if (test_options & TEST_OPTION_OVERFLOW) {
            int expected_length = tests[test_idx].expected.length;
            results.overflow = (overflow_check((char*)(actual_output + expected_length), MAX_ARRAY_SIZE - expected_length - 1, FILL) == 0);
        }

        if (test_options & TEST_OPTION_CDECL_REGISTERS) {
            results.cdecl_registers = (cdecl_failed_regs == 0);
            printf(PREFIX "Number of arm registers save/restore failed: %d (0 => ok)\n", cdecl_failed_regs);
        }

        print_test_results(test_idx + 1, results);
    }

    return 0;
}

const char* resultnum_to_str(int result_num) {
    switch (result_num) {
        case -1: return "SKIP";
        case 0: return "FAIL";
        case 1: return "PASS";
        default: return "????";
    }
}

static void print_test_results(int test_idx, struct TestResult results) {
    const char* result_str_contents = resultnum_to_str(results.contents);
    const char* result_str_return = resultnum_to_str(results.return_value);
    const char* result_str_overflow = resultnum_to_str(results.overflow);
    const char* result_str_cdecl_registers = resultnum_to_str(results.cdecl_registers);

    char* result_str_overall = "FAIL";

    if ((results.contents & results.return_value & results.overflow & results.cdecl_registers) == -1) {
        result_str_overall = "SKIP";
    } else if ((results.contents * results.return_value * results.overflow * results.cdecl_registers) != 0) {
        result_str_overall = "PASS";
    }

    printf(PREFIX "Test %d: %s | Contents: %s | Return value: %s | Overflow check: %s | calling convention registers check: %s\n",
            test_idx,
            result_str_overall,
            result_str_contents,
            result_str_return,
            result_str_overflow,
            result_str_cdecl_registers
    );
}

static void snprint_array(char *output, int output_size, int array[], int length) {
    int pos = 0;

    memset(output, 0, output_size);

    if (length < 0) {
        sprintf(output, " < invalid array length found: %d >", length);
        return;
    }

    for (int idx = 0; idx < length; idx++) {
        pos += sprintf(output + pos, "%d, ", array[idx]);
    }
}

static int overflow_check(char* data, int length, char check) {
    for (int idx = 0; idx < length; idx++) {
        if (data[idx] != check) {
            return 1;
        }
    }

    return 0;
}

// check:
//  * value{=[0/1]}
//  * value
static int check_arg(const char* arg, const char* target, int* out_value) {
    if (strncmp(arg, target, strlen(target)) == 0) {
        char *equal_sign = strstr(arg, "=");

        if (equal_sign != NULL) {
            int arg_value = (int)strtol(equal_sign + 1, NULL, 10);

             *out_value = arg_value; //(equal_sign[1] != '0');
        } else {
            *out_value = 1;
        }

        return 1;
    }

    return 0;
}

static void handle_test_count(int value, enum TestOptions* test_options, int* test_start, int* test_count, void* user_p) {
    printf(PREFIX "Available tests: %d\n", *test_count);
    exit(0);
}

static void handle_test(int value, enum TestOptions* test_options, int* test_start, int* test_count, void* user_p) {
    printf(PREFIX "Running single test: %d\n", value);
    *test_start = value;
}

static void handle_check_bit(int value, enum TestOptions* test_options, int* test_start, int* test_count, void* user_p) {
    enum TestOptions set_option = (enum TestOptions)user_p;

    if (value) {
        *test_options |= set_option;
    } else {
        *test_options &= ~set_option;
    }
}

static void parse_arguments(int argc, char** argv, enum TestOptions* test_options, int* test_start, int* test_count) {
    if (argc <= 1) {
        return;
    }

    struct {
        const char* key;
        void (*handler)(int value, enum TestOptions* test_options, int* test_start, int* test_count, void* user_p);
        void* user_p;
    } arguments[] = {
        { "test_count", handle_test_count, NULL },
        { "test", handle_test, NULL },
        { "check_return", handle_check_bit, (void*)TEST_OPTION_RETURN_VALUE },
        { "check_overflow", handle_check_bit, (void*)TEST_OPTION_OVERFLOW },
        { "check_contents", handle_check_bit, (void*)TEST_OPTION_CONTENTS },
        { "check_cdecl", handle_check_bit, (void*)TEST_OPTION_CDECL_REGISTERS },
    };

    for (int idx = 0; idx < argc; idx++) {
        const char* arg = argv[idx];
        int value = 0;

        for (int option_idx = 0; option_idx < (sizeof(arguments) / sizeof(arguments[0])); option_idx++) {
            if (check_arg(arg, arguments[option_idx].key, &value) == 1) {
                arguments[option_idx].handler(value, test_options, test_start, test_count, arguments[option_idx].user_p);
            }
        }
    }
}

#if __arm__
asm(""
".syntax unified\n"
".section .data\n"
"TMP_FP: .int 0\n"
".section .text\n"
"cdecl_wrap:\n"
    "push {r4-r12, lr}\n" // 4*10 bytes saved regs
    "mov fp, sp\n"
    "sub sp, sp, #4*4\n"  // extra local vars: 4*4 bytes

    "str r0, [fp, #-4]\n" // r0: target_func
                          // r1: argument count
    "str r2, [fp, #-8]\n" // r2: output address ofreturn_value
    "str r3, [fp, #-12]\n" // r3: first arg

    "ldr r12, =TMP_FP\n" // [TMP] is used for original fp value
    "str fp, [r12]\n"    // save fp

    "sub r1, r1, #4\n"
    "cmp r1, #0\n"       // if (argc - 4) <= 0 goto argend
    "ble argend\n"
    "lsl r0, r1, #2\n"  // r0 := (argc - 4) * 4
    //"sub sp, sp, r0\n"

    "mov r3, #4*10 + 4*2\n"  // 4*10 (saved regs) + 4*2 extra args
    "add r3, r3, r0\n"
    //"add r3, r11, r12\n"
  "argloop:\n"
    "cmp r1, #0\n"
    "ble argend\n"
    "ldr r0, [fp, r3]\n"
    //"lsl r2, r1, #2\n"
    //"add r2, r2, #0\n"
    //"str r0, [sp, r2]\n"
    "push {r0}\n"
    "sub r3, r3, #4\n"
    "sub r1, r1, #1\n"
    "b argloop\n"
  "argend:\n"

    // prepare args
    "ldr r0, [fp, #-12]\n" // arg1
    "ldr r1, [fp, #4*10 + 4*0]\n" // copy arg2 from stack
    "ldr r2, [fp, #4*10 + 4*1]\n"
    "ldr r3, [fp, #4*10 + 4*2]\n"
    // TODO: copy extra args to stack


    "ldr r12, [fp, #-4]\n"  // load target func
    "blx r12\n"

    "ldr r3, =TMP_FP\n"     // restore fp
    "ldr r3, [r3]\n"        // r3 <= fp
    "ldr r2, [r3, #-8]\n"   // load return_value address
    "str r0, [r2]\n"        // save return_value

    "mov r0, #8\n"          // r0 := 8 (8 registerst to check)

    // check r4-r11 register
    "ldr r2, [r3, #4*0]\n"
    "cmp r4, r2\n"
    "subeq r0, r0, #1\n"

    "ldr r2, [r3, #4*1]\n"
    "cmp r5, r2\n"
    "subeq r0, r0, #1\n"

    "ldr r2, [r3, #4*2]\n"
    "cmp r6, r2\n"
    "subeq r0, r0, #1\n"

    "ldr r2, [r3, #4*3]\n"
    "cmp r7, r2\n"
    "subeq r0, r0, #1\n"

    "ldr r2, [r3, #4*4]\n"
    "cmp r8, r2\n"
    "subeq r0, r0, #1\n"

    "ldr r2, [r3, #4*5]\n"
    "cmp r9, r2\n"
    "subeq r0, r0, #1\n"

    "ldr r2, [r3, #4*6]\n"
    "cmp r10, r2\n"
    "subeq r0, r0, #1\n"

    // fp check for saved fp
    "cmp r11, r3\n"
    "subeq r0, r0, #1\n"

    "mov sp, r3\n"  // restore fp
    "pop {r4-r12, pc}\n"
"");
#else
asm(""
".intel_syntax noprefix\n"
".section .data\n"
"TMP_EBP: .int 0\n"
".section .text\n"
"cdecl_wrap:\n"
    "push ebp\n"
    "mov ebp, esp\n"
    "mov [TMP_EBP], ebp\n"      // save start ebp

    "sub esp, 4*5\n"            // allocate save for 5 int32_t values
    "mov [ebp - 4*1], ebx\n"
    "mov [ebp - 4*2], esi\n"
    "mov [ebp - 4*3], edi\n"

    "mov eax, [ebp + 4*4]\n"    // eax := return_value (3rd arg)
    "cmp eax, 0\n"              // if return_value == nullptr goto LSK
    "je .LSK\n"
    "mov eax, [eax]\n"          // ???  eax := *eax
  ".LSK:\n"
    "mov ecx, [ebp + 4*3]\n"    // ecx := argc (2nd arg)
  // prepare target function arguments (copy to stack)
  ".LP:\n"
    "cmp ecx, 0\n"              // if argc <= 0 goto LE
    "jle .LE\n"
    "push dword ptr [ebp + 4*ecx + 4*4]\n"  // push extra args [argc]
    "dec ecx\n"                 // --argc
    "jmp .LP\n"
  ".LE:\n"
    "call [ebp + 4*2]\n"        // target_function(...args)

    "xchg TMP_EBP, ebp\n"       // restore ebp and save tainted one

    "mov ecx, [ebp + 4*4]\n"    // ecx := return_value
    "cmp ecx, 0\n"              // if return_value != nullptr { *return_value = eax; }
    "je .LSK2\n"
    "mov [ecx], eax\n"

  ".LSK2:\n"
    // check registers
    "mov eax, 4\n"              // failed_registers := 4

    "cmp TMP_EBP, ebp\n"        // check original ebp and possibly tainted one
    "sete cl\n"                 // cl := (TMP_EBP - ebp) == 0 ? 1 : 0
    "sub al, cl\n"              // failed_register -= cl

    "cmp [ebp - 4*1], ebx\n"
    "sete cl\n"
    "sub al, cl\n"

    "cmp [ebp - 4*2], esi\n"
    "sete cl\n"
    "sub al, cl\n"

    "cmp [ebp - 4*3], edi\n"
    "sete cl\n"
    "sub al, cl\n"

    "mov ebx, [ebp - 4*1]\n"
    "mov esi, [ebp - 4*2]\n"
    "mov edi, [ebp - 4*3]\n"

    "mov esp, ebp\n"
    "pop ebp\n"
    "ret\n"
"\n"
".att_syntax\n"
"");
#endif
