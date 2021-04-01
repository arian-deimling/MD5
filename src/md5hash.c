#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <getopt.h>

#include "MD5.h"  // function to perform MD5 hash on string
#include "md5hash.h"

int main(int argc, char* argv[]) {

  // disable debugging messages from the getopt() function in getopt.h
  opterr = 0;

  // usage string
  const char* usage = "usage: %s {-t text | -f file | -x hex}\n";
  const char* executable_file = "md5hash.exe";

  // command line argument value
  char* string_to_hash = NULL;
  char* file_to_hash = NULL;
  char* hex_to_hash = NULL;

  // read command line arguments until none remain
  int c = 0;
  while ((c = getopt(argc, argv, "t:f:x:")) != -1) {

    switch (c) {

      // case where the user is providing a string to hash directly as a
      // command line argument
      case 't':
        string_to_hash = optarg;
        break;

      // case where user is providing the name of a file to hash
      case 'f':
        file_to_hash = optarg;
        break;

      // case where user is providing bytes to hash as a hex string
      case 'x':
        hex_to_hash = optarg;
        break;

      // case where the option provided was not recognized
      case '?':
      default:
        break;

    }
  }

  // length of the string that is being hashed in bytes
  uint32_t hash_input_length;

  // number of md5 512-bit (64-byte) chunks used by the hash input
  uint32_t chunk_count;

  // character array containing hash input message
  uint8_t* hash_input = NULL;

  // if the user provided a string to hash via the cmdline
  if (string_to_hash != NULL) {

    // strlen() is safe here because string_to_hash was provided via cmdline
    hash_input_length = strlen(string_to_hash);

  // if the user provided a file to hash via the cmdline
  } else if (file_to_hash != NULL) {

    // check whether the file provided is accessible
    if (access(file_to_hash, R_OK) != 0) {
      fprintf(stderr, "%s is not available to be read\n", file_to_hash);
      exit(EXIT_FAILURE);
    }

    // get the lenght of the file in bytes (ascii characters)
    hash_input_length = get_file_size(file_to_hash);

  // if the user provided a hex string to be interpreted and hashed
  } else if (hex_to_hash != NULL) {
    
    // hex string must represent an integer number of bytes
    if (strlen(hex_to_hash) % 2 != 0) {
      fprintf(
        stderr,
        "%s\n", 
        "hex string must represent an integer number of bytes"
      );
      exit(EXIT_FAILURE);
    }

    // each hex character represents 1/2 byte or 4-bits of input
    hash_input_length = strlen(hex_to_hash) / 2;

  // if the user did not provide valid cmdline args, print usage
  } else {
    fprintf(stderr, usage, executable_file);
    exit(EXIT_FAILURE);
  }

  // get number of md5 512-bit blocks are needed to perform the hash
  chunk_count = get_md5_chunk_count(hash_input_length);

  // create an array to hold the string to be hashed (plus padding)
  hash_input = malloc(MD5_CHUNK_BYTES * chunk_count * sizeof(uint8_t));
  if (hash_input == NULL) {
    fprintf(stderr, "malloc failed at %s:%d\n", __FILE__, __LINE__ - 2);
    exit(EXIT_FAILURE);
  }

  // entire if-else below copies either string or file contents into the hash
  // input byte array
  if (string_to_hash != NULL) {

    // copy the string provided into the hash input array
    strncpy((char*) hash_input, string_to_hash, MD5_CHUNK_BYTES * chunk_count);

  } else if (file_to_hash != NULL) {

    // read file contents into hash input array and record bytes read from file
    uint32_t bytes_read = file_to_byte_array(
      file_to_hash, hash_input, hash_input_length);

    // make sure the entire file was read
    if (hash_input_length != bytes_read) {
      fprintf(stderr, "read from file %s failed\n", file_to_hash);
      exit(EXIT_FAILURE);
    }

  } else if (hex_to_hash != NULL) {

    // convert hex string to byte array and store whether an invalid character
    // was supplied in the hex string
    char invalid_char = hex_to_byte_array(
      hex_to_hash, hash_input, hash_input_length);

    // '0' is returned if function call was successful
    if (invalid_char != '0') {
      fprintf(
        stderr, 
        "invalid character %c provided in hex string\n", 
        invalid_char
      );
      exit(EXIT_FAILURE);
    }

  } else {
    fprintf(stderr, "unexpected error occured at %s:%d\n", __FILE__, __LINE__);
    exit(EXIT_FAILURE);
  }

  // create an array to hold the hash result
  uint8_t hash_result[16];

  // create an array to hold hex string of the hash result
  char hash_result_string[33];

  // calculate the MD5 hash of the string
  inplace_md5_sum(hash_input, hash_input_length, hash_result);

  // convert the MD5 hash to a string of hex characters
  md5_hash_to_hex_str(hash_result, hash_result_string);

  printf("%s\n", hash_result_string);

  free(hash_input);

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

size_t file_to_byte_array(const char* file_name, uint8_t* dst, size_t n) {

  // open the file and read contents into the hash input array
  FILE* file_ptr = fopen(file_name, "rb");
  if (file_ptr == NULL) {
    return 0;
  }

  // read the specified number of bytes from the file into the dst array
  uint32_t bytes_read = fread(dst, 1, n, file_ptr);

  fclose(file_ptr);

  return bytes_read;

}

char hex_to_byte_array(const char* hex_string, uint8_t* dst, size_t n) {

  // for each character in the input string (each half byte in dst array)
  for (int i = 0; i < n * 2; i++) {

    // holds the value of the character from 0x0 to 0xF (0 to 15)
    uint8_t hex_value = 0;
    
    // subtract ascii value of 0 from digit values to get their numerical value
    if (hex_string[i] >= '0' && hex_string[i] <= '9') {
      hex_value |= hex_string[i] - 48;
      
    // subtract ascii value of 'A' from letter values and add 10 to get their
    // numerical value (add 10 because hex 0xA = decimal 10)
    } else if (hex_string[i] >= 'A' && hex_string[i] <= 'F') {
      hex_value |= hex_string[i] - 55;

    // subtract ascii value of 'a' from letter values and add 10 to get their
    // numerical value (add 10 because hex 0xA = decimal 10)
    } else if (hex_string[i] >= 'a' && hex_string[i] <= 'f') {
      hex_value |= hex_string[i] - 87;

    // if an invalid character is provided, return it for the caller to handle
    } else {
      return hex_string[i];
    }
    
    // if the hex character represents the first half of the current byte
    if (i % 2 == 0) {

      // initialize each byte to 0
      dst[i / 2] = 0;

      // set the first half of the byte
      dst[i / 2] |= (hex_value << 4);

    // if the hex character represents the second half of the current byte
    } else {

      // set the second half of the byte
      dst[i / 2] |= hex_value;

    }
  }

  // return '0' character if the function executed correctly; since we are
  // returning invalid characters to indicate parsing failures, we must
  // return a valid character to indiciate successful parsing of the hex str
  return '0';

}
