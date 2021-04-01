#include <Windows.h>
#include <stdint.h>

#ifndef MD5_H
#define MD5_H

/**
 * MD5 algorithm is performed on 64-byte chunks of the input and padding is
 * added to the end of any input to make it's lenght a multiple of 64-bytes.
 */
static const uint32_t MD5_CHUNK_BYTES = 64;

/**
 * MD5 algorithm specification requires padding of a single 1-bit after the
 * input; this implementation simply appends a 0b10000000 byte to fulfill this
 * requirement. The final 8-bytes of the last chunk must be filled with the
 * length of the input. In total, a minimum of 9 bytes are needed for padding.
 */
static const uint32_t MD5_MIN_PADDING_BYTES = 9;

/**
 * Calculates the MD5 hash of an array of characters. 
 * 
 * *** THIS FUNCTION HAS NOT BEEN TESTED ON BIG ENDIAN SYSTEMS ***
 * 
 * Given an array of 8-bit characters, the length of the character string,
 * and the array into which the result should be written, this function
 * will calculate the MD5 Hash of the given string. The string can be of any
 * length, but the array holding it should be a multiple of 64-bytes in length
 * to allow the function to add the padding as required by the MD5 hash
 * algorithm to be added to the end of the string (see get_md5_chunk_count()).
 * 
 * See https://en.wikipedia.org/wiki/MD5 for more information.
 * 
 * @param string A byte array with length that is a multiple of 64 bytes 
 * containing a string of characters; array will be modified by the function
 * @param string_len Number of characters in the string to be hashed
 * @param hash_result A 16-byte array to hold/return the hash calculation
 */
void inplace_md5_sum(uint8_t* string,
                     const uint32_t string_len,
                     uint8_t* hash_result);

/**
 * Get the number of 512-bit MD5 chunks are needed to hash a string of the
 * given length in place.
 * 
 * Used in conjunction with the inplace MD5 hash function; use this function
 * to determine how big the character array needs to be to allow the inplace
 * hash function to add the necessary padding before performing the hash.
 * 
 * @param string_len Length of the string to be hashed (in bytes)
 * @return the number of 64-byte chunks that will be used to perform the hash
 */
uint32_t get_md5_chunk_count(uint32_t string_len);

#endif /* MD5_H */
