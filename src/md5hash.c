#include <Windows.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>

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

/**
 * Provides the size, in bytes, of a file.
 * 
 * @param file_name Name/path of the file to get the size of
 * @return The size of the file in bytes
 */
uint32_t get_file_size(const char* file_name);

int main(int argc, char* argv[]) {

  // disable debugging messages from the getopt() function in getopt.h
  opterr = 0;

  // usage string
  const char* usage = "usage: %s {-t text | -f file}\n";
  const char* executable_file = "md5hash.exe";

  // command line argument value
  char* string_to_hash = NULL;
  char* file_to_hash = NULL;

  // length of the string that is being hashed in bytes
  uint32_t string_to_hash_length;

  // number of md5 512-bit (64-byte) chunks used by the hash input
  uint32_t chunk_count;

  // character array containing hash input message
  uint8_t* a_string;


  // read command line arguments until none remain
  int c = 0;
  while ((c = getopt(argc, argv, "t:f:")) != -1) {

    switch (c) {

      // case where the user is providing a string to hash directly as a
      // command line argument
      case 't':

        // get the string provided by the user
        string_to_hash = optarg;
        if (string_to_hash == NULL) {
          fprintf(stderr, usage, executable_file);
          exit(EXIT_FAILURE);
        }

        string_to_hash_length = strlen(string_to_hash);

        // get number of md5 512-bit blocks are needed to perform the hash
        chunk_count = get_md5_chunk_count(string_to_hash_length);

        // create an array to hold the string to be hashed (plus padding)
        a_string = malloc(MD5_CHUNK_BYTES * chunk_count * sizeof(uint8_t));
        if (a_string == NULL) {
          fprintf(stderr, "malloc failed at %s:%d", __FILE__, __LINE__ - 2);
          exit(EXIT_FAILURE);
        }

        // copy the string provided into the hash input array
        strncpy(
          (char*) a_string, 
          string_to_hash, 
          MD5_CHUNK_BYTES * chunk_count
        );

        break;

      // case where user is providing the name of a file to hash
      case 'f':

        // get the file name provided by the user
        file_to_hash = optarg;
        if (file_to_hash == NULL) {
          fprintf(stderr, usage, executable_file);
          exit(EXIT_FAILURE);
        }

        // check whether the file provided is accessible
        if (access(file_to_hash, R_OK) != 0) {
          fprintf(stderr, "%s is not available to be read", file_to_hash);
          exit(EXIT_FAILURE);
        }

        // get the lenght of the file in bytes (ascii characters)
        string_to_hash_length = get_file_size(file_to_hash);

        // number of md5 512-bit chunks needed to perform hash
        chunk_count = get_md5_chunk_count(string_to_hash_length);

        // create an array to hold the string to be hashed (plus padding)
        a_string = malloc(MD5_CHUNK_BYTES * chunk_count * sizeof(uint8_t));
        if (a_string == NULL) {
          fprintf(stderr, "malloc failed at %s:%d", __FILE__, __LINE__ - 2);
          exit(EXIT_FAILURE);
        }

        // open the file and read contents into the hash input array
        FILE* hash_file_ptr = fopen(file_to_hash, "rb");
        uint32_t bytes_read = fread(
          a_string, 1, string_to_hash_length, hash_file_ptr);

        // make sure the entire file was read
        if (string_to_hash_length != bytes_read) {
          fclose(hash_file_ptr);
          fprintf(stderr, "read from file %s failed", file_to_hash);
          exit(EXIT_FAILURE);
        }

        // close the file 
        fclose(hash_file_ptr);

        break;

      // case where the option provided was not recognized
      case '?':
      default:
        fprintf(stderr, usage, executable_file);
        exit(EXIT_FAILURE);
    }
  }

  // create an array to hold the hash result
  uint8_t hash_result[16];

  // create an array to hold hex string of the hash result
  char hash_result_string[33];

  // calculate the MD5 hash of the string
  inplace_md5_sum(a_string, string_to_hash_length, hash_result);

  // convert the MD5 hash to a string of hex characters
  md5_hash_to_hex_str(hash_result, hash_result_string);

  printf("%s\n", hash_result_string);

  free(a_string);

  exit(EXIT_SUCCESS);

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

uint32_t get_file_size(const char* file_name) {

  uint32_t file_size;

  // open the file
  FILE* file_ptr = fopen(file_name, "rb");

  // seek to the end of the file and get the size of the file in bytes
  fseek(file_ptr, 0L, SEEK_END);
  file_size = ftell(file_ptr);

  fclose(file_ptr);
  return file_size;

}
