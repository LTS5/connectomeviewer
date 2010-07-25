To make
	from ctypes import *
	giftilib = CDLL("libgiftiio.so")
work, you need to copy the libgiftiio.so file to
a path accessible by the python interpreter, such as

	/usr/lib or
	/epd_py25-4.3.0/lib

libgiftiio-1.0.0.so:
Compiled on a Fedora 11 x86_64 system with -m32 for 32bit

libgiftiio.so:
Compiled on a Ubuntu 8.10 x86 system

libgiftiio64-dyn.so:
Compiled on 64bit Mandriva. Linked to /usr/lib/libexpat.so.1.5.2 

libgiftiio.so:
Static linked for 64bit. Thanks to Benjamin.
