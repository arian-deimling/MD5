# MD5
Simple implementation of the MD5 hash algorithm in C. This implementation was written so that it will hash a string of arbitrary length, but the entire string must be loaded into memory before the hash can be performed.

## Requirements
This code was written and tested in Windows using MinGW; however, it can easily be modified to run on Linux.

## Usage
<ul>
  <li><code>mingw32-make main.exe</code> to build</li>
  <li><code>./main.exe</code> to run</li>
  <li>Follow the prompt which instructs the user to enter a string</li>
</ul>

### Additional Usage
If you are downloading the code only to use the MD5 hash code, the array containing your string that is being hashed must be large enough to contain the padding that will be added to the end of the string; at most, this will be your string's length plus 72 bytes, but is recommended to simply add several extra kilobytes to the size of your array to avoid any clobbering issues. (<em>Note: the string you provide to the hash function will be modified</em>). You will also need to provide the length of the string being hashed as well as a 128-bit array in the form of 16 unsigned 8-bit integers into which the result of the hash calculation will be placed.
