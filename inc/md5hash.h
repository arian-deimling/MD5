#include <stdint.h>
#include <unistd.h>

/**
 * Convert 128-bit MD5 hash result to a 32-character hexadecimal string.
 * 
 * Provided an array of 16 bytes and an array of 33 characters, this
 * function will read the byte array and convert each byte into two
 * hexadecimal characters and store them in the provided character array.

 * @param hash_array Array of 16 bytes which represents ouput of MD5 hash
 * @param output_string Array of characters into which output string will
 * be placed; must be large enough to hold at least 33 characters
 */
void md5_hash_to_hex_str(const uint8_t hash_array[], char output_string[]);

/**
 * Get string from the user via stdin.
 * 
 * Reads a string from stdin into a provided buffer; the string is
 * truncated after `max_length - 1` characters to allow for space for the
 * null terminating character. Additionally, the newline character is removed
 * from the buffer.
 * 
 * @param buffer Character array into which the string from stdin is stored
 * @param max_length Number of characters (including null terminating 
 * character) to store into the buffer
 */
void get_input(char* buffer, size_t max_length);

/**
 * Provides the size, in bytes, of a file.
 * 
 * @param file_name Name/path of the file to get the size of
 * @return The size of the file in bytes
 */
uint32_t get_file_size(const char* file_name);

size_t file_to_byte_array(const char* file_name, uint8_t* dst, size_t n);

char hex_to_byte_array(const char* hex_string, uint8_t* dst, size_t n);