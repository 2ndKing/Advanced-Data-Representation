#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

//A2 functions
void oct_to_bin(const char *octal_text, char *output);
void oct_to_hex(const char *octal_text, char *output);
void hex_to_bin(const char *hex_text, char *output);
void to_sign_magnitude(int32_t number, char *output);
void to_ones_complement(int32_t number, char *output);
void to_twos_complement(int32_t number, char *output);

//(A1 carryover, not used by A2 tests but kept)
void div_convert(uint32_t number, int base, char *output);
void sub_convert(uint32_t number, int base, char *output);
void print_tables(uint32_t number);

static void remove_newline(char *s) {
    s[strcspn(s, "\r\n")] = '\0';
}

int main(void) {

    FILE *tests = fopen("a2_test.txt", "r");
    if (!tests) {
        printf("Error: could not open a2_test.txt\n");
        return 1;
    }

    FILE *out = fopen("output.txt", "w");
    if (!out) {
        printf("Error: could not create output.txt\n");
        fclose(tests);
        return 1;
    }

    char line[512];
    int line_number = 0;
    int total = 0, passed = 0;

    while (fgets(line, sizeof(line), tests)) {

        line_number++;
        remove_newline(line);
        if (line[0] == '\0' || line[0] == '#') continue;

        // Format per test file: function_name input expected
        char function_name[64], input_text[256], expected_text[256];
        function_name[0] = input_text[0] = expected_text[0] = '\0';

        // Some inputs may look like "-5" or "A3". Read as strings.
        if (sscanf(line, "%63s %255s %255s", function_name, input_text, expected_text) < 3) {
            // ignore lines that don't match
            continue;
        }

        char actual[1024];
        actual[0] = '\0';

        if (strcmp(function_name, "oct_to_bin") == 0) {
            oct_to_bin(input_text, actual);
            total++;
            int ok = (strcmp(actual, expected_text) == 0);
            if (ok) passed++;
            fprintf(out, "Test (line %d): oct_to_bin(\"%s\") -> Expected: \"%s\", Got: \"%s\" [%s]\n",
                    line_number, input_text, expected_text, actual, ok ? "PASS" : "FAIL");

        } else if (strcmp(function_name, "oct_to_hex") == 0) {
            oct_to_hex(input_text, actual);
            total++;
            int ok = (strcmp(actual, expected_text) == 0);
            if (ok) passed++;
            fprintf(out, "Test (line %d): oct_to_hex(\"%s\") -> Expected: \"%s\", Got: \"%s\" [%s]\n",
                    line_number, input_text, expected_text, actual, ok ? "PASS" : "FAIL");

        } else if (strcmp(function_name, "hex_to_bin") == 0) {
            hex_to_bin(input_text, actual);
            total++;
            int ok = (strcmp(actual, expected_text) == 0);
            if (ok) passed++;
            fprintf(out, "Test (line %d): hex_to_bin(\"%s\") -> Expected: \"%s\", Got: \"%s\" [%s]\n",
                    line_number, input_text, expected_text, actual, ok ? "PASS" : "FAIL");

        } else if (strcmp(function_name, "to_sign_magnitude") == 0) {
            int32_t value = (int32_t)strtol(input_text, NULL, 10);
            to_sign_magnitude(value, actual);
            total++;
            int ok = (strcmp(actual, expected_text) == 0);
            if (ok) passed++;
            fprintf(out, "Test (line %d): to_sign_magnitude(%d) -> Expected: \"%s\", Got: \"%s\" [%s]\n",
                    line_number, value, expected_text, actual, ok ? "PASS" : "FAIL");

        } else if (strcmp(function_name, "to_ones_complement") == 0) {
            int32_t value = (int32_t)strtol(input_text, NULL, 10);
            to_ones_complement(value, actual);
            total++;
            int ok = (strcmp(actual, expected_text) == 0);
            if (ok) passed++;
            fprintf(out, "Test (line %d): to_ones_complement(%d) -> Expected: \"%s\", Got: \"%s\" [%s]\n",
                    line_number, value, expected_text, actual, ok ? "PASS" : "FAIL");

        } else if (strcmp(function_name, "to_twos_complement") == 0) {
            int32_t value = (int32_t)strtol(input_text, NULL, 10);
            to_twos_complement(value, actual);
            total++;
            int ok = (strcmp(actual, expected_text) == 0);
            if (ok) passed++;
            fprintf(out, "Test (line %d): to_twos_complement(%d) -> Expected: \"%s\", Got: \"%s\" [%s]\n",
                    line_number, value, expected_text, actual, ok ? "PASS" : "FAIL");
        } else {
        }
    }

    fprintf(out, "\nSummary: %d/%d tests passed\n", passed, total);

    fclose(tests);
    fclose(out);
    printf("All results written to output.txt\n");
    return 0;
}
