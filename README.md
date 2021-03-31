# MD5
Simple implementation of the MD5 hash algorithm in C. This implementation was written so that it will hash a string of arbitrary length, but the entire string must be loaded into memory before the hash can be performed.

## Requirements
This code was written and tested in Windows using MinGW; however, it can easily be modified to run on Linux.

## Usage
<ul>
  <li><code>mingw32-make md5hash</code> to build</li>
  <li><code>./md5hash.exe "some-string-to-hash"</code> to run</li>
</ul>

### Additional Usage
If you are downloading the code only to use the MD5 hash code, you have two options:
<ol>
  <li>Use the in-place hash function: if you go with this route, you will want to provide the length of the string being hashed to the utility function inside MD5.h which will take a string length and return the number of 64-byte chunks that will be required to perform the hash; you will need to allocate and array of 64 * number of chunks for your string even if the string doesn't take up that much space; the in-place hash function will need to add padding to the end of that array. (<em>Note: the string you provide to the hash function will be modified</em>) You will also need to provide the length of the string being hashed as well as a 128-bit array in the form of 16 unsigned 8-bit integers into which the result of the hash calculation will be placed.</li><br>
  <li>Use the regular hash function: <em>not yet implemented</em></li>
</ol>
