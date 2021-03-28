# MD5
Very simplistic implementation of the MD5 hash algorithm in C. This implementation was written so that it will hash a single chunk which is defined by the MD5 hash algorithm to be 512 bits. Due to the padding requirements, this limits the input length to 440 bits or 55 8-bit ASCII characters.

## Requirements
This code was written and tested in Windows using MinGW; however, it can easily be modified to run on Linux.

## Usage
<ul>
  <li><code>mingw32-make main.exe</code> to build</li>
  <li><code>./main.exe</code> to run</li>
  <li>Follow the prompt which instructs the user to enter a string of less than 55 characters</li>
</ul>

### Additional Usage
If you are downloading the code only to use the MD5 hash code, you will need to provide the method a 64-byte (512-bit) string with the string you wish to hash contained within the first 55 bytes; a null terminating character is not required (<em>Note: the string you provide to the hash function will be modified</em>). You will also need to provide the length of the string being hashed as well as a 128-bit array in the form of 16 unsigned 8-bit integers into which the result of the hash calculation will be placed.
