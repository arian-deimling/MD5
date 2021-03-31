#include <Windows.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "MD5.h"  // function to perform MD5 hash on string

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

int main(int argc, char* argv[]) {

  // if an invalid number of arguments are given, print usage instructions
  if (argc != 2) {
    puts("Usage: ./md5hash.exe {string-to-hash}");
    return 1;
  }

  // figure out how many md5 512-bit blocks are needed to perform the hash
  uint32_t chunk_count = get_md5_chunk_count(strlen(argv[1]));

  // create an array to hold the string to be hashed (plus padding)
  uint8_t *a_string = malloc(chunk_count * 64 * sizeof(uint8_t));
  strcpy((char*) a_string, argv[1]);

  // get the length of the inputted string
  uint32_t string_len = strlen((char*) a_string);

  // create an array to hold the hash result
  uint8_t hash_result[16];

  // create an array to hold hex string of the hash result
  char hash_result_string[33];

  // calculate the MD5 hash of the string
  inplace_md5_sum(a_string, string_len, hash_result);

  // convert the MD5 hash to a string of hex characters
  md5_hash_to_hex_str(hash_result, hash_result_string);

  // replace null terminating character which is removed by the hash function
  a_string[string_len] = 0x00;

  printf("%s is the hash of \"%s\"", hash_result_string, (char*) a_string);

  free(a_string);

}

void md5_hash_to_hex_str(const uint8_t hash_array[], char output_string[]) {

  static const char HEX_CHARACTERS[] = "0123456789abcdef";

  // for each byte in the hash array
  for (int i = 0; i < 16; i++) {

    // get the value of the first and second 4 bits of the byte
    uint8_t first_four_bits_value = hash_array[i] >> 4;
    uint8_t second_four_bits_value = hash_array[i] & 0x0f;

    // set the corresponding char for the first and second half of the byte
    output_string[2 * i] = HEX_CHARACTERS[first_four_bits_value];
    output_string[2 * i + 1] = HEX_CHARACTERS[second_four_bits_value];

  }

  // add null terminating character
  output_string[32] = '\0';

  return;

}

void get_input(char* buffer, size_t max_length) {

  if (max_length == 0) {
    return;
  }

  // get user input from stdin; if it is null, set buffer to an empty string
  if (fgets(buffer, max_length, stdin) == NULL) {
    buffer[0] = '\0';
  }

  // if the newline character was read into the buffer string, remove it
  if (buffer[strlen(buffer) - 1] == '\n') {
    buffer[strlen(buffer) - 1] = '\0';
  }

}
