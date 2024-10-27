#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define MAX_INPUT_LENGTH 1024

// Function 1 Milestone 1
int is_ascii(char str[]) {
    for (int i = 0; str[i] != '\0'; i++) {
        if ((unsigned char)str[i] > 127) {
            return 0;
        }
    }
    return 1;
}

// Function 2 Milestone 1
int32_t capitalize_ascii(char str[]) {
    int32_t count = 0;
    int32_t i = 0;
    
    while (str[i] != '\0') {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i] - 32;
            count++;
        }
        i++;
    }
    
    return count;
}



// Function 3 Milestone 2
int32_t width_from_start_byte(char start_byte) {
    unsigned char byte = (unsigned char)start_byte;

    if (byte <= 127) {
        return 1;
    } else if (byte >= 192 && byte <= 223) {
        return 2; 
    } else if (byte >= 224 && byte <= 239) {
        return 3;
    } else if (byte >= 240 && byte <= 247) {
        return 4;
    } else {
        return -1;
    }
}

// Function 4 Milestone 2
int32_t utf8_strlen(char str[]) {
    int32_t length = 0;
    int32_t i = 0;
    while (str[i] != '\0') {
        int32_t width = width_from_start_byte(str[i]);
        if (width < 0) {
            return -1; 
        }
        length++;
        i += width; 
    }
    return length;
}

// Function 5 Milestone 2
int32_t codepoint_index_to_byte_index(char str[], int32_t cpi) {
    if (str == NULL || cpi < 0) {
        return -1; 
    }
    int32_t byte_index = 0;
    for (int32_t codepoint_count = 0; str[byte_index] != '\0';) {
        unsigned char c = (unsigned char)str[byte_index];
        int num_bytes;

        if (c < 128) {
            num_bytes = 1; 
        } else if (c < 192) {
            return -1; 
        } else if (c < 224) {
            num_bytes = 2; 
        } else if (c < 240) {
            num_bytes = 3; 
        } else if (c < 248) {
            num_bytes = 4; 
        } else {
            return -1;
        }
        if (codepoint_count == cpi) {
            return byte_index; 
        }
        byte_index += num_bytes;
        codepoint_count++;
    }
    return -1;
}

// Function 6 Milestone 2
void utf8_substring(char str[], int32_t cpi_start, int32_t cpi_end, char result[]) {
    int32_t start_byte_index = codepoint_index_to_byte_index(str, cpi_start);
    int32_t end_byte_index = codepoint_index_to_byte_index(str, cpi_end);

    if (cpi_start < 0 || cpi_end < 0 || cpi_start >= cpi_end || 
    start_byte_index == -1 || end_byte_index == -1) {
        result[0] = '\0';
        return;
    }

    int32_t length = end_byte_index - start_byte_index;

    for (int32_t i = 0; i < length; i++) {
        result[i] = str[start_byte_index + i];
    }

    result[length] = '\0';
}

// Function 7 Milestone 3
int32_t codepoint_at(char str[], int32_t cpi) {
        int32_t byte_index = codepoint_index_to_byte_index(str, cpi);
    if (byte_index == -1) return -1;

    int width = width_from_start_byte(str[byte_index]);

    int32_t codepoint = 0;
    if (width == 1) {
        return str[byte_index];
    } else if (width == 2) {
        codepoint = (str[byte_index] & 0x1F) << 6;
        codepoint |= (str[byte_index + 1] & 0x3F);
    } else if (width == 3) {
        codepoint = (str[byte_index] & 0x0F) << 12;
        codepoint |= (str[byte_index + 1] & 0x3F) << 6;
        codepoint |= (str[byte_index + 2] & 0x3F);
    } else if (width == 4) {
        codepoint = (str[byte_index] & 0x07) << 18;
        codepoint |= (str[byte_index + 1] & 0x3F) << 12;
        codepoint |= (str[byte_index + 2] & 0x3F) << 6;
        codepoint |= (str[byte_index + 3] & 0x3F);
    }

    return codepoint;
}

// Function 8 Milestone 3
char is_animal_emoji_at(char str[], int32_t cpi) {
    int32_t current_codepoint = codepoint_at(str, cpi);

    int32_t animal_s1 = codepoint_at("🐀", 0);
    int32_t animal_e1 = codepoint_at("🐿", 0);
    int32_t animal_s2 = codepoint_at("🦀", 0);
    int32_t animal_e2 = codepoint_at("🦮", 0);

    return (current_codepoint >= animal_s1 && current_codepoint <= animal_e1) ||
           (current_codepoint >= animal_s2 && current_codepoint <= animal_e2);
}

void next_utf8_char(char str[], int32_t cpi, char result[]) {
    int32_t current_codepoint = codepoint_at(str, cpi);
    if (current_codepoint == -1) {
        result[0] = '\0';
        return;
    }

    int32_t next_codepoint = current_codepoint + 1;

    if (next_codepoint <= 127) {
        result[0] = next_codepoint;
        result[1] = '\0';
    } else if (next_codepoint <= 2047) {
        result[0] = 192 + ((next_codepoint >> 6) & 31);
        result[1] = 128 + (next_codepoint & 63);
        result[2] = '\0';
    } else if (next_codepoint <= 65535) {
        result[0] = 224 + ((next_codepoint >> 12) & 15);
        result[1] = 128 + ((next_codepoint >> 6) & 63);
        result[2] = 128 + (next_codepoint & 63);
        result[3] = '\0';
    } else if (next_codepoint <= 1114111) {
        result[0] = 240 + ((next_codepoint >> 18) & 7);
        result[1] = 128 + ((next_codepoint >> 12) & 63);
        result[2] = 128 + ((next_codepoint >> 6) & 63);
        result[3] = 128 + (next_codepoint & 63);
        result[4] = '\0';
    } else {
        result[0] = '\0';
    }
}


// UTF-8 Analyzer in main
int main() {
    char input[MAX_INPUT_LENGTH];
    char original_input[MAX_INPUT_LENGTH];
    printf("Enter a UTF-8 encoded string: ");
    fgets(input, MAX_INPUT_LENGTH, stdin);

    // Remove newline character at the end of input
    input[strcspn(input, "\n")] = '\0';

    // Store a copy of the original input for later use
    strncpy(original_input, input, MAX_INPUT_LENGTH);

    // Function calls using the original input where needed
    int ascii_valid = is_ascii(input);
    printf("\n");
    printf("Valid ASCII: %s\n", ascii_valid ? "true" : "false");

    // Capitalize ASCII characters in input
    int32_t capitalized_count = capitalize_ascii(input);
    printf("Uppercased ASCII: \"%s\"\n", input);

    // Length in bytes and number of code points
    int32_t byte_length = strlen(input);
    printf("Length in bytes: %d\n", byte_length);

    int32_t code_point_count = utf8_strlen(input);
    printf("Number of code points: %d\n", code_point_count);

    // Bytes per code point
    printf("Bytes per code point: ");
    for (int32_t i = 0; i < code_point_count; i++) {
        int byte_index = codepoint_index_to_byte_index(input, i);
        if (byte_index != -1) {
            printf("%d ", width_from_start_byte(input[byte_index]));
        }
    }
    printf("\n");

    // Use original input to get substring
    char substring[MAX_INPUT_LENGTH];
    utf8_substring(original_input, 0, 6, substring);
    printf("Substring of the first 6 code points: \"%s\"\n", substring);

    // Code points as decimal numbers
    printf("Code points as decimal numbers: ");
    for (int32_t i = 0; i < code_point_count; i++) {
        printf("%d ", codepoint_at(input, i));
    }
    printf("\n");

    // Detect and print animal emojis
    printf("Animal emojis: ");
    for (int32_t i = 0; i < code_point_count; i++) {
        if (is_animal_emoji_at(input, i)) {
            int byte_index = codepoint_index_to_byte_index(input, i);
            int width = width_from_start_byte(input[byte_index]);
            printf("%.*s ", width, &input[byte_index]);
        }
    }
    
    char next_char[MAX_INPUT_LENGTH];
    next_utf8_char(input, 3, next_char);  // Example for codepoint index 3
    printf("Next Character of Codepoint at Index 3: \"%s\"\n", next_char);

    printf("\n");
    return 0;
}
