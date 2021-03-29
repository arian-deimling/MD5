#include <Windows.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "MD5.h"  // function to perform MD5 hash on string

#define MAX_CHUNKS 1000
#define MAX_STRING_LENGTH MAX_CHUNKS*64-9

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

  // string to be hashed
  uint8_t a_string[64 * MAX_CHUNKS];

  // prompt the user to enter a string
  printf(
    "Enter a string of no more than %d characters to be hashed: ",
    MAX_STRING_LENGTH
  );
  fflush(stdout);

  // get user input; max string len does not include null terminating character
  // so add one, to max length to create room for that character
  get_input(a_string, MAX_STRING_LENGTH + 1);

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

  printf(
    "The result of the hash of %s is %s.",
    (char*) a_string, 
    hash_result_string
  );

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
