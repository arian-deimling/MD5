#include <Windows.h>

#include <stdint.h>

#define MAX_TARGET_STR_LEN 55

/**
 * Calculates the MD5 hash of an array of characters. 
 * 
 * *** THIS FUNCTION HAS NOT BEEN TESTED ON BIG ENDIAN SYSTEMS ***
 * 
 * Given an array of 8-bit characters, the length of the character string,
 * and the array into which the result should be written, this function
 * will calculate the MD5 Hash of the given string. The string should
 * contain no more than 55 characters, but should be 64-bytes in length
 * to allow the function to add the padding as required by the MD5 hash
 * algorithm to be added to the end of the string.
 * 
 * See https://en.wikipedia.org/wiki/MD5 for more information.
 * 
 * @param string A 64-byte array of 55 or less characters; bytes after the
 * characters will be modified by this function
 * @param string_len Number of characters in the string to be hashed
 * @param hash_result A 16-byte array to hold/return the hash calculation
 * @return Void; hash result is returned into an array provided by caller
 */
void md5_sum(uint8_t* string, uint8_t string_len, uint8_t* hash_result);
