Gifticlib for Python in Windows. Tested with XP.

- Copy libexpat.dll, libgiftiio.so and zlib.dll to \Windows\System32
- Now you can use the gifticlib in Python like in pygiftiio.py

More information on:
http://www.nitrc.org/projects/gifti/

If you want to compile it on your own:
(from http://www.nitrc.org/forum/forum.php?forum_id=281 by Stephan Gerhard)
-----

Goal:

Compile the giftilibc in Windows XP in order to use it in Python with the ctypes CDLL command.

Important:

All this tools might have some problems with pathnames containing spaces. So I created a folder "Brain" in C:\ where I installed all the libraries (zlib, expat, nifti).

Requirements:

* Compiler MinGW32: http://www.mingw.org/
-> Go to "Downloads", then "Automated MinGW Installer". Install it to C:\MinGW\
* Zlib library from GNUWin32: http://gnuwin32.sourceforge.net/packages.html
-> Install it into C:\Brain\Zlib
* Expat: http://sourceforge.net/projects/expat/
-> Download and install it to C:\Brain\Expat (without the 2.0.1 suffix!)
* Nifti: http://niftilib.sourceforge.net/
-> Download source file and extract it to C:\Brain\nifti (so you have e.g. C:\Brain\nifti\bin)
* Gifti: http://www.nitrc.org/frs/?group_id=75&release_id=92
-> of course the library we are interested in compiling. Extract into C:\Brain\gifti (e.g. you have C:\Brain\gifti\gifti_io.h etc.)

Now, everything is in place!


Having nifti ready:

In order to compile successfully, the compiler (we use mingw32-gcc) has to find all the includes (the *.h files) and the libraries.

In this post, I assume that you have been able to successully make the niftilib. This means you have:
* nifti1_io.h znzlib.h and some more files in the folder C:\Brain\nifti\include
* libniftiio.a libznz.a and some more file in the folder C:\Brain\nifti\lib


Compilation:

* Run a commandprompt (Start -> Run -> cmd -> OK)
* Change to the gifticlib folder: cd C:\Brain\gifti
* Execute the following commands to compile the library, there should be no error in each and
afterwards, the library file (libgiftiio.so) should be available! (You can copy-paste!)
mingw32-gcc -v -O3 -DHAVE_ZLIB -I..\nifti\include -IC:\Brain\Zlib\include -IC:\Brain\Expat\Source\lib -o gifti_io.lo -c gifti_io.c
mingw32-gcc -v -O3 -DHAVE_ZLIB -I..\nifti\include -IC:\Brain\Zlib\include -IC:\Brain\Expat\Source\lib -o gifti_xml.lo -c gifti_xml.c
mingw32-gcc -v -shared -fPIC -DPIC -DHAVE_ZLIB -o libgiftiio.so gifti_io.lo gifti_xml.lo -LC:\Brain\Zlib\lib -LC:\Brain\Expat\Bin -L..\nifti\lib -lexpat -lniftiio -lznz -lz -lm

Testing:

So far, I didn't managed to create a static library, but for the shared library we've got, we have to ship with libexpat.dll (from C:\Brain\Expat\Bin)!
You need to copy these two files to the System folder (e.g. C:\Windows\System32) to make them available.

Now, create a little python file test.py with the contents:
from cytpes import *
giftilib = CDLL("libgiftiio.so")
and run it (python test.py).

You should see no error and we can happily start using gifti to understand the brain with python :-D
