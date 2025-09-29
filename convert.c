#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>


static void write_32bit_binary(uint32_t value, char *output) {
    for (int i = 31; i >= 0; i--) {
        output[31 - i] = ((value >> i) & 1u) ? '1' : '0';
    }
    output[32] = '\0';
}

static int hex_char_to_value(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
    if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
    return -1;
}

static char value_to_hex_char(int value) {
    if (value < 10) return (char)('0' + value);
    return (char)('A' + (value - 10));
}

//Part 1: direct mapping functions

// octal to binary
void oct_to_bin(const char *octal_text, char *output) {
    size_t length = strlen(octal_text);
    size_t position = 0;
    for (size_t i = 0; i < length; i++) {
        char c = octal_text[i];
        if (c < '0' || c > '7') { output[0] = '\0'; return; } /* simple validation */
        int digit = c - '0';
        output[position++] = ((digit >> 2) & 1) ? '1' : '0';
        output[position++] = ((digit >> 1) & 1) ? '1' : '0';
        output[position++] = ((digit >> 0) & 1) ? '1' : '0';
    }
    output[position] = '\0';
}

// hex to binary
void hex_to_bin(const char *hex_text, char *output) {
    size_t length = strlen(hex_text);
    size_t position = 0;
    for (size_t i = 0; i < length; i++) {
        int value = hex_char_to_value(hex_text[i]);
        if (value < 0) { output[0] = '\0'; return; }
        output[position++] = ((value >> 3) & 1) ? '1' : '0';
        output[position++] = ((value >> 2) & 1) ? '1' : '0';
        output[position++] = ((value >> 1) & 1) ? '1' : '0';
        output[position++] = ((value >> 0) & 1) ? '1' : '0';
    }
    output[position] = '\0';
}

// octal to hex
void oct_to_hex(const char *octal_text, char *output) {
    char binary_text[1024];
    oct_to_bin(octal_text, binary_text);
    if (binary_text[0] == '\0') { output[0] = '\0'; return; }

    // handle zero specially: if all binary are '0', result is "0"
    int all_zero = 1;
    for (size_t i = 0; binary_text[i]; i++) {
        if (binary_text[i] == '1') { all_zero = 0; break; }
    }
    if (all_zero) { strcpy(output, "0"); return; }

    // pad the left with zeros so length is a multiple of 4
    size_t bin_len = strlen(binary_text);
    int pad = (int)((4 - (bin_len % 4)) % 4);
    char padded[1024];
    for (int i = 0; i < pad; i++) padded[i] = '0';
    strcpy(padded + pad, binary_text);

    // convert each nibble to a hex digit
    char hex_text[1024];
    size_t write_pos = 0;
    for (size_t i = 0; i < strlen(padded); i += 4) {
        int v = (padded[i]   == '1' ? 8 : 0)
              + (padded[i+1] == '1' ? 4 : 0)
              + (padded[i+2] == '1' ? 2 : 0)
              + (padded[i+3] == '1' ? 1 : 0);
        hex_text[write_pos++] = value_to_hex_char(v);}
    hex_text[write_pos] = '\0';

    // trim leading zeros but keep one digit
    size_t start = 0;
    while (hex_text[start] == '0' && hex_text[start + 1] != '\0') start++;
    strcpy(output, hex_text + start);
}

//Part 2: signed number representations

// sign-magnitude: bit 31 is sign, bits 0..30 are magnitude
void to_sign_magnitude(int32_t number, char *output) {
    if (number >= 0) {
        write_32bit_binary((uint32_t)number, output);
        return;
    }

    // get magnitude as positive value (careful at INT32_MIN)
    uint32_t magnitude;
    if (number == INT32_MIN) {
        magnitude = 0x80000000u;
    } else {
        magnitude = (uint32_t)(-number);
    }

    // build bits: sign bit = 1, magnitude in the rest (mask to 31 bits just in case)
    uint32_t pattern = (1u << 31) | (magnitude & 0x7FFFFFFFu);
    write_32bit_binary(pattern, output);
}

//one's complement: positive unchanged; negative = bitwise NOT of positive magnitude pattern
void to_ones_complement(int32_t number, char *output) {
    if (number >= 0) {
        write_32bit_binary((uint32_t)number, output);
        return;
    }

    // represent the positive magnitude, then flip all bits
    uint32_t positive_bits;
    if (number == INT32_MIN) {
        positive_bits = 0x80000000u;
    } else {
        positive_bits = (uint32_t)(-number);
    }
    uint32_t flipped = ~positive_bits;
    write_32bit_binary(flipped, output);
}

// two's complement: positive unchanged; negative = flip all bits and add 1
void to_twos_complement(int32_t number, char *output) {
    // casting to uint32_t gives the exact two's complement bit pattern
    uint32_t pattern = (uint32_t)number;
    write_32bit_binary(pattern, output);
}

//Code from Assignment 1

void div_convert(uint32_t number, int base, char *output) {
    if (base < 2 || base > 16) { output[0] = '\0'; return; }
    if (number == 0) { output[0] = '0'; output[1] = '\0'; return; }
    char temporary[64];
    int position = 0;
    while (number > 0) {
        int remainder = (int)(number % (uint32_t)base);
        number = number / (uint32_t)base;
        temporary[position++] = (remainder < 10)
                                ? (char)('0' + remainder)
                                : (char)('A' + (remainder - 10));
    }
    temporary[position] = '\0';

    // reverse into output
    int left = 0, right = position - 1;
    while (left < right) {
        char t = temporary[left];
        temporary[left] = temporary[right];
        temporary[right] = t;
        left++; right--;
    }
    strcpy(output, temporary);
}

void sub_convert(uint32_t number, int base, char *output) {
    if (base < 2 || base > 16) { output[0] = '\0'; return; }
    if (number == 0) { output[0] = '0'; output[1] = '\0'; return; }
    uint32_t power = 1;
    while (power <= number / (uint32_t)base) power *= (uint32_t)base;
    char buffer[64];
    int position = 0;
    while (power > 0) {
        int digit = (int)(number / power);
        number -= (uint32_t)digit * power;
        power /= (uint32_t)base;
        buffer[position++] = (digit < 10)
                             ? (char)('0' + digit)
                             : (char)('A' + (digit - 10));
    }
    int start = 0;
    while (start < position - 1 && buffer[start] == '0') start++;
    memmove(buffer, buffer + start, (size_t)(position - start));
    buffer[position - start] = '\0';
    strcpy(output, buffer);
}

void print_tables(uint32_t number) {
    char binary_text[33], octal_text[12], hex_text[9];
    div_convert(number, 2, binary_text);
    div_convert(number, 8, octal_text);
    div_convert(number, 16, hex_text);
    printf("Original: Binary=%s Octal=%s Decimal=%u Hex=%s\n", binary_text, octal_text, number, hex_text);
    uint32_t left_shift = number << 3;
    div_convert(left_shift, 2, binary_text);
    div_convert(left_shift, 8, octal_text);
    div_convert(left_shift, 16, hex_text);
    printf("Left Shift by 3: Binary=%s Octal=%s Decimal=%u Hex=%s\n", binary_text, octal_text, left_shift, hex_text);
    uint32_t masked = number & 0xFFu;
    div_convert(masked, 2, binary_text);
    div_convert(masked, 8, octal_text);
    div_convert(masked, 16, hex_text);
    printf("AND with 0xFF: Binary=%s Octal=%s Decimal=%u Hex=%s\n", binary_text, octal_text, masked, hex_text);
}
