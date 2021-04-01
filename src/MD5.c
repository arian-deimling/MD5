#include "MD5.h"

uint32_t left_rotate(uint32_t x, uint8_t bits) {
  return (x << bits) | (x >> (32 - bits));
}

uint32_t int_ceiling(uint32_t numerator, uint32_t denominator) {
  return numerator / denominator  // integer division
      + (numerator % denominator != 0 ? 1 : 0);  // ceiling
}

void inplace_md5_sum(uint8_t* string,
                     const uint32_t string_length,
                     uint8_t* hash_result) {

  // see https://en.wikipedia.org/wiki/MD5#Pseudocode for MD5 implementation
  // information this implementation only handles hashes of strings of up to
  // 55 8-bit characters

  // per round shift amounts - array contains 64 entries because there are 64
  // rounds per 512-bit chunk of input
  static uint8_t s[] = {
    7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
    5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
    4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
    6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,
  };

  // precomupted values to be used in the hash algorithm - the formula for
  // calculating is: floor( 2^32 * abs( sin( i + 1 ) ) ) for i = 0 ... 63
  static uint32_t K[] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391,
  };

  // use these values to initialize A, B, C, and D; after the values are
  // manipulated for a given 512-bit chunk, add A, B, C, D back into their 
  // respective values below
  uint32_t A0 = 0x67452301;
  uint32_t B0 = 0xefcdab89;
  uint32_t C0 = 0x98badcfe;
  uint32_t D0 = 0x10325476;

  // hash must occur on the input in 512-bit (64-byte) chunks; padding
  // is required, and 0-bits fill empty space to fill final chunk
  uint32_t chunks = get_md5_chunk_count(string_length);

  // add 1-bit padding followed by the appropriate amount of 0 bit padding
  string[string_length] = 0x80;

  // after accounting for the string and the addition byte added above, add
  // enough padding to fill out to the end of the final 512-bit chunk
  memset(
    &string[string_length + 1],
    0,
    chunks * MD5_CHUNK_BYTES - (string_length + 1)
  );  

  // reinterpret the last 8 bytes of the padding as a unsigned int; per the MD5
  // specification, this should hold the message length in bits mod 2^64
  uint64_t* string_bit_length = 
    (uint64_t*) &string[MD5_CHUNK_BYTES * chunks - 8];

  // set the string bit length
  *string_bit_length = string_length * 8;

  // iterate over each chunk in the padded message
  for (int chunk_number = 0; chunk_number < chunks; chunk_number++) {

    // reinterpret the current 512-bit chunk as 16 32-bit unsigned integers
    uint32_t* M = (uint32_t*) &string[chunk_number * MD5_CHUNK_BYTES];
 
    // initialize the values of A, B, C, D
    uint32_t A = A0;
    uint32_t B = B0;
    uint32_t C = C0;
    uint32_t D = D0;

    // there are 64 rounds per 512-bit block in the MD5 hashing algorithm
    for (int i = 0; i < 64; i++) {

      // value F and index g are computer and used to modify the value of B
      uint32_t F, g;

      // for rounds 0 - 15, this is the calculation for F and g
      if (i < 16) {
        F = (B & C) | ((~B) & D);
        g = i;

      // for rounds 16 - 31, this is the calculation for F and g
      } else if (i < 32) {
        F = (D & B) | ((~D) & C);
        g = (5 * i + 1) % 16;

      // for rounds 32 - 47, this is the calculation for F and g
      } else if (i < 48) {
        F = B ^ C ^ D;
        g = (3 * i + 5) % 16;

      // for rounds 48 - 63, this is the calculation for F and g
      } else {
        F = C ^ (B | (~D));
        g = (7 * i) % 16;

      }

      // for each round, use the following equations to compute the new values
      // of A, B, C, D
      F = F + A + K[i] + M[g];
      A = D;
      D = C;
      C = B;
      B = B + left_rotate(F, s[i]);
  
    }

    // add the values of A, B, C, D into A0, B0, C0, D0 after each round
    A0 += A;
    B0 += B;
    C0 += C;
    D0 += D;

  }

  // reinterpret the hash_result array as an array of 4 32-bit unsigned ints 
  // and use A0, B0, C0, D0 to set the value of the output hash result
  ((uint32_t*) hash_result)[0] = A0;
  ((uint32_t*) hash_result)[1] = B0;
  ((uint32_t*) hash_result)[2] = C0;
  ((uint32_t*) hash_result)[3] = D0;

  return;

}

uint32_t get_md5_chunk_count(uint32_t string_len) {
  return int_ceiling(string_len + MD5_MIN_PADDING_BYTES, MD5_CHUNK_BYTES);
}
