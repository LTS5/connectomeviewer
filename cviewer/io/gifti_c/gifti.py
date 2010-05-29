import pygiftiio
import numpy
import ctypes, os
from pygiftiio import GiftiCoordSystem, GiftiDataArray, GiftiDataType, GiftiEncoding, GiftiEndian, GiftiImage, GiftiIntentCode, GiftiLabelTable, GiftiMetaData, GiftiNVPairs

__doc__ = """ Allows to manipulate Gifti Image files conveniently

This module provides functions to read and write Gifti files, manipulate
Gifti Structures such as DataArrays, Coordinate Systems or MetaData,
using standards numpy array as much as possible.

Quickstart :
You can load a gifti file with 'loadImage', or wrap an existing numpy array
with the GiftiImage_fromarray function. Individual object manipulation
functions are then available as method of the relevant classes.

Note that the standard 'gifti_*' lower-level functions are still directly
available with docstrings through the pygiftiio submodule."""

__all__ = ["GiftiCoordSystem", "GiftiDataArray", "GiftiDataType", "GiftiEncoding", "GiftiEndian", "GiftiImage", "GiftiImage_fromTriangles", "GiftiImage_fromarray", "GiftiIntentCode", "GiftiLabelTable", "GiftiMetaData", "GiftiNVPairs", "loadImage", "freeImage", "saveImage", "pygiftiio"]

# First, move some potentially dangerous or inconvenient fields
GiftiCoordSystem._xform = GiftiCoordSystem.xform; del GiftiCoordSystem.xform
GiftiImage._meta = GiftiImage.meta; del GiftiImage.meta
GiftiDataArray._coordsys = GiftiDataArray.coordsys; del GiftiDataArray.coordsys
GiftiDataArray._data = GiftiDataArray.data; del GiftiDataArray.data
GiftiDataArray._meta = GiftiDataArray.meta; del GiftiDataArray.meta
GiftiImage._darray = GiftiImage.darray; del GiftiImage.darray
GiftiImage._labeltable = GiftiImage.labeltable; del GiftiImage.labeltable

# Metadata as dictionary : read, updateFrom, and clear
def _getMeta(s):
	""" return a copy of the header fields as a dictionary """
	return dict(zip(s._meta.name[:s._meta.length], s._meta.value[:s._meta.length]))
GiftiImage.getMeta = GiftiDataArray.getMeta = _getMeta

def _updateMetaFromDict(s, h):
	""" update the meta header from a dictionary, already-defined fields are overwritten """
	for key, val in h.items():
		pygiftiio.gifti_add_to_meta(s._meta, key, str(val), True)
	return s
GiftiImage.updateMetaFromDict = GiftiDataArray.updateMetaFromDict = _updateMetaFromDict

def _clearMeta(s): s._meta = GiftiMetaData()
GiftiImage.clearMeta = GiftiDataArray.clearMeta = _clearMeta

def freeImage(self):
	"""recursively desallocate all the c-allocated memory.
After that, all the instance data is meaningless and the variable
should be deleted"""
	pygiftiio.gifti_free_image(self)

GiftiImage.__del__ = freeImage
GiftiImage.free = freeImage # because __del__ doesn't seems to be called automatically

# Convenient access to lists of structures
GiftiImage.getArrays = lambda s : tuple(x.contents for x in s._darray[:s.numDA])
GiftiImage.arrays = property(GiftiImage.getArrays)
GiftiDataArray.getCoordSystems = lambda s : tuple(x.contents for x in s._coordsys[:s.numCS])
GiftiDataArray.coordsystems = property(GiftiDataArray.getCoordSystems)

GiftiImage.getArraysFromIntent = lambda s, it : [x for x in s.arrays if x.intent == it]

# xform property access as a read&write numpy array
GiftiCoordSystem.getXForm = lambda s : numpy.frombuffer(s._xform, s._xform._type_, s._xform._length_ ).reshape(4,-1)
GiftiCoordSystem.xform = property(GiftiCoordSystem.getXForm)

from numpy import uint8
class rgb:
	""" mini class to handle access to NIFTI RGB* typed arrays. You can
 switch from fields-based (1D) indexation to shape-based (2D) indexation
 with as1D and as2D"""
	uint8s_rgb = numpy.dtype('(3,)uint8')
	uint8s_rgba = numpy.dtype('(4,)uint8')
	fields_rgb = numpy.dtype([('r',uint8), ('g',uint8), ('b',uint8) ])
	fields_rgba = numpy.dtype([('r',uint8),('g',uint8),('b',uint8),('a',uint8)])

_as2D = lambda a : numpy.frombuffer(a, (rgb.uint8s_rgb, rgb.uint8s_rgba)[(a.itemsize==1) and (a.shape[-1] - 3) or (a.itemsize - 3)])
_as1D = lambda a : numpy.frombuffer(a, (rgb.fields_rgb, rgb.fields_rgba)[(a.itemsize==1) and (a.shape[-1] - 3) or (a.itemsize - 3)])
rgb.as2D = staticmethod(_as2D)
rgb.as1D = staticmethod(_as1D)


# Access to the "_data"-pointed data as an rw numpy array
# _giiType2npyType : a dictionary of NIFTI->Numpy Types
_giiType2npyType = \
{GiftiDataType.NIFTI_TYPE_COMPLEX128 : numpy.dtype('complex128'),
 GiftiDataType.NIFTI_TYPE_COMPLEX64 : numpy.dtype('complex64'),
 GiftiDataType.NIFTI_TYPE_FLOAT32 : numpy.dtype('float32'),
 GiftiDataType.NIFTI_TYPE_FLOAT64 : numpy.dtype('float64'),
 GiftiDataType.NIFTI_TYPE_INT16 : numpy.dtype('int16'),
 GiftiDataType.NIFTI_TYPE_INT32 : numpy.dtype('int32'),
 GiftiDataType.NIFTI_TYPE_INT64 : numpy.dtype('int64'),
 GiftiDataType.NIFTI_TYPE_INT8 : numpy.dtype('int8'),
 GiftiDataType.NIFTI_TYPE_UINT16 : numpy.dtype('uint16'),
 GiftiDataType.NIFTI_TYPE_UINT32 : numpy.dtype('uint32'),
 GiftiDataType.NIFTI_TYPE_UINT64 : numpy.dtype('uint64'),
 GiftiDataType.NIFTI_TYPE_UINT8 : numpy.dtype('uint8'),
 GiftiDataType.NIFTI_TYPE_RGB24 : rgb.fields_rgb,
 GiftiDataType.NIFTI_TYPE_RGBA32 : rgb.fields_rgba
 }
del uint8

GiftiDataArray.getData = lambda s : numpy.frombuffer((ctypes.c_char*s.nvals*_giiType2npyType[s.datatype].itemsize).from_address(s._data), _giiType2npyType[s.datatype], s.nvals).reshape(tuple(s.dims[:s.num_dim]), order = 'C' if s.ind_ord < 2 else 'F')
GiftiDataArray.data = property(GiftiDataArray.getData)


# allows using intent & datatype as strings
def _setIntentString(s, string):
	s.intent = GiftiIntentCode.intents[string]
GiftiDataArray.intentString = property(lambda s: pygiftiio.gifti_intent_to_string(s.intent), _setIntentString)

def _setDatatypeString(s, string):
	s.datatype = GiftiDataType.datatypes[string]
GiftiDataArray.datatypeString = property(lambda s: pygiftiio.gifti_datatype2str(s.datatype), _setDatatypeString)

# label tables
def _setLabels(s, labels, indices = None):
	""" Set the optional label table associated with this image.
 labels (str list) defines the names associated with the label-array values.
 indices is an optional list of (integers) values which maps to these labels;
 otherwise, indices are assumed to be ordered integers."""
	pygiftiio.gifti_clear_LabelTable(s._labeltable)
	t = GiftiLabelTable(len(labels))
	t.index, t.label = (c_int*len(labels))(), (c_char_p*len(labels))()
	for i, label in zip(indices if indices else xrange(len(labels)), labels):
		t.index[i], t.label[i] = i, label
	s._labeltable = t
GiftiImage.setLabels = _setLabels
def _getLabels(s):
	""" Return the (indices, labels) pair associated with this image """
	return (s._labeltable.index[:s._labeltable.length], s._labeltable.label[:s._labeltable.length])
GiftiImage.getLabels = _getLabels
GiftiImage.labels = property(GiftiImage.getLabels, GiftiImage.setLabels)

# convenient for a quick overview
def _getArraysIntentList(self, asStrings = True):
	if asStrings:
		return [x.intentString for x in self.arrays]
	else:
		return [x.intent for x in self.arrays]
GiftiImage.getArraysIntentList = _getArraysIntentList

def _printSummary(self, meta = False):
	if meta:
		print ("\n".join([" '%s': '%s'" % c for c in self.getMeta().items()]))
	print "%d DataArrays :" % self.numDA
	for x in self.arrays:
		m = x.getMeta()
		if m.has_key('Name'):
			print "%s" % m['Name']
		print "%s\t%s, %s" % (x.intentString, x.data.shape, x.datatypeString)
		if x.numCS > 0:
			print "\t%d Coord System defined:" % x.numCS
			print "\n".join(["\t%s - %s" % (i.dataspace, i.xformspace) for i in x.coordsystems])
		if meta == True and len(m):
			print "\n".join(["  %s :\n\t%s" % v for v in m.items()]), '\n'

GiftiImage.showSummary = _printSummary

# Copy routines
def _copy_da(self, shared_data):
	"""copy a DataArray structure. If shared_data is True, actual data are
only a view on original ones. In this case, be careful not to delete the
original DA array, or you will get a dangling pointer after that."""
	if shared_data == False:
		da = pygiftiio.gifti_copy_DataArray(self, True)
	else:
		da = pygiftiio.gifti_copy_DataArray(self, False)
		da._meta = self._meta
	return da

GiftiDataArray.copy = _copy_da

GiftiCoordSystem.copy = lambda self : pygiftiio.gifti_copy_CoordSystem(self)

def _removeLastCS(self):
	""" remove the last coordinate system from the list """
	if self.numCS == 0:
		raise IndexError, "No coordsystem to delete"
	cs = self._coordsys[self.numCS-1].contents
	pygiftiio.gifti_clear_CoordSystem(cs)
	pygiftiio.gifti_free_CoordSystem(cs)
	self.numCS -= 1

GiftiDataArray.removeLastCoordSystem = _removeLastCS

def _removeLastArray(self):
	""" remove the last DataArray from the list.
	If the Gifti Image owns the array, it will be deallocated.
	"""
	if self.numDA == 0:
		raise IndexError, "No array to delete"
	da = self._darray[self.numDA-1].contents
	if self._refsnpy.has_key(da._data):
		self._refsnpy[da._data].pop()
		if len(self._refsnpy[da._data]) == 0:
			del self._refsnpy[da._data]
	else:
		pygiftiio.gifti_free_DataArray(da)
	self.numDA -= 1
	
GiftiImage.removeLastArray = _removeLastArray

# convenient loading & saving of images
def loadImage(filename):
	""" Load a Gifti image from a file """
	if not os.path.exists(filename):
		raise IOError("No such file or directory: '%s'" % filename)
	img = pygiftiio.gifti_read_image(filename, 1)
	img.filename = filename
	img._refsnpy = {}
	return img

def saveImage(image, filename):
	""" Save the current image to a new file """
	if not image.version:
		t = pygiftiio.gifticlib_version()
		versionstr = t[t.find("version ")+8:t.find(", ")]
		float(versionstr) # raise an exception should the format change in the future :-)
		image.version = versionstr
	pygiftiio.gifti_write_image(image, filename, 1)

def _save(self, filename = None):
	""" Save the current image on disk.
	If the image was created using array data (not loaded from a file) one
	has to specify a filename
	
	Note that the Gifti spec suggests using the following suffixes to your
	filename when saving each specific type of data:
		Generic GIFTI File    .gii
		Coordinates           .coord.gii
		Functional            .func.gii
		Labels                .label.gii
		RGB or RGBA           .rgba.gii
		Shape                 .shape.gii
		Surface               .surf.gii
		Tensors               .tensor.gii
		Time Series           .time.gii
		Topology              .topo.gii	"""
	if filename == None:
		filename = self.filename
	saveImage(self, filename)
GiftiImage.save = _save


def GiftiImage_fromarray(data, intent = GiftiIntentCode.NIFTI_INTENT_NONE, encoding=GiftiEncoding.GIFTI_ENCODING_B64GZ, endian = GiftiEndian.GIFTI_ENDIAN_LITTLE):
	""" Create a new Gifti Image containing one array, made from a numpy array.
	Note that the actual data pointer is shared. """
	K = GiftiImage()
	K.addDataArray_fromarray(data, intent, encoding, endian)
	return K

def GiftiImage_fromTriangles(vertices, triangles, cs = None, encoding=GiftiEncoding.GIFTI_ENCODING_B64GZ, endian = GiftiEndian.GIFTI_ENDIAN_LITTLE):
	""" Create a new Gifti Image from vertices and triangles arrays
	Note that the actual data pointer is shared. 
	An optional coordinate system (cs) can be specified, either as a 3-tuple
	(dataspace_string, xformspace_string, transformMatrix) or as a CS object"""
	K = GiftiImage()
	K.addDataArray_fromarray(vertices, GiftiIntentCode.NIFTI_INTENT_POINTSET, encoding, endian)
	K.addDataArray_fromarray(triangles, GiftiIntentCode.NIFTI_INTENT_TRIANGLE, encoding, endian)
	if cs:
		K.arrays[0].addCoordSystem(*cs if type(cs) is tuple else (cs.dataspace,cs.xformspace,cs.xform))
	return K

def _add_fromarray(self, data, intent = GiftiIntentCode.NIFTI_INTENT_NONE, encoding=GiftiEncoding.GIFTI_ENCODING_B64GZ, endian = GiftiEndian.GIFTI_ENDIAN_LITTLE):
	""" Add an entry to the list of DataArray, from a numpy array
	Note that the actual data pointer is shared """
	print_optional_DA_warnings(data, intent) # check and print some hints
	if data.flags['C_CONTIGUOUS'] == False and data.flags['F_CONTIGUOUS'] == False:
		raise ValueError, "Need a contigous array as input"
	typ = [k for k, v in _giiType2npyType.items() if v == data.dtype][0]
	dims = list(data.shape) + [0]*(6-data.ndim)
	pygiftiio.gifti_add_empty_darray(self, 1)
	da = self._darray[self.numDA-1].contents
	da.intent = intent
	da._data = data.ctypes._data
	da.num_dim = data.ndim
	da.nbyper = data.dtype.itemsize
	da.ind_ord = 1 if data.flags['C_CONTIGUOUS'] else 2
	da.encoding = encoding
	da.endian = endian
	da.dims[:data.ndim] = data.shape
	da.nvals = data.size
	# add a ref to the NumpyArray in the Image structure:
	if getattr(self, '_refsnpy', None) == None:
		self._refsnpy = {}
	self._refsnpy.setdefault(da._data, []).append(data)
	da.datatype = [k for k, v in _giiType2npyType.items() if v == data.dtype][0]

GiftiImage.addDataArray_fromarray = _add_fromarray

def _addCoordSys(self, dataspace = None, xformspace = None, transformMatrix = None):
	""" Add a new coordinate system to the list """
	pygiftiio.gifti_add_empty_CS(self)
	cs = self._coordsys[self.numCS-1].contents
	if transformMatrix != None:
		cs.xform.flat = transformMatrix
	cs.dataspace, cs.xformspace = dataspace, xformspace

GiftiDataArray.addCoordSystem = _addCoordSys


# Docstrings :
# Add pygiftiio.gifti_io.c-extracted docstrings to corresponding python functions
# docstrings.py should exist (it can be generated from the src with: docstring_generates.py ./gifti_io.c )
try:
	from docstrings import docstrings as funcdict
except ImportError:
	print "docstrings.py not found : no docstring added"
	print "you should have run docstring_generates.py at install time"
else:
	import pygiftiio
	def update_docstrings(funcdict):
		for fun in pygiftiio.__dict__:
			if fun.startswith('gifti_'):
				if funcdict.has_key(fun):
					pygiftiio.__dict__[fun].__doc__ = "(original gifticlib comment)\n" + funcdict[fun]
	update_docstrings(funcdict)
	del funcdict
	del update_docstrings

GiftiImage.__doc__ = """
GiftiImage represents a whole Gifti content.

Quickstart :
- Use showSummary() to display a quick summary of the current content.
- Manipulate meta dictionary with getMeta and updateMetaFromDict
- Access individual 'DataArray' substructures through the 'arrays' tuple,
  or get them from their associated intent-code with getArraysFromIntent.
- Add or remove from the DataArray list with addDataArray_fromarray and
  removeLastArray.
- Manage the label list trough get/setLabels or the 'labels' property
- Save to a file with 'save'
"""
GiftiDataArray.__doc__ = """
GiftiDataArray represents one Gifti 'DataArray' (or DA) structure, ie. an
array of values, with optional meta-data and coordinate system.

Quickstart :
 - Access the actual stored values as a numpy array trough the 'data' field
 - Read/Set the intent-code as number or string with intent or intentString
 - Coordinate System list is available through the coordsystems property
 - Manage DA meta-data dictionary with getMeta and updateMetaFromDict (refer
   to chapter 3 of the Gifti spec for a list of standardized DA-metadata)
"""
GiftiCoordSystem.__doc__ = """
GiftiCoordSystem represents an affine mapping from one space to another.

From the spec:
"For a DataArray with an Intent NIFTI_INTENT_POINTSET, this element
describes the stereotaxic space of the data before and after the
application of a transformation matrix.
The most common stereotaxic space is the Talairach Space
which has its origin at the anterior commissure and the negative X, Y,
and Z axes correspond to left, posterior, and inferior respectively."

Here the affine matrix is available as a (4,4) numpy array named 'xform'.
The names of the spaces are available as *string* in 'dataspace' and
'xformspace'. Standard string (defined in nifti1.h) includes
"NIFTI_XFORM_UNKNOWN", "NIFTI_XFORM_SCANNER_ANAT", "NIFTI_XFORM_ALIGNED_ANAT"
"NIFTI_XFORM_TALAIRACH", "NIFTI_XFORM_MNI_152"
"""

def print_optional_DA_warnings(data, intent):
	if intent == GiftiIntentCode.NIFTI_INTENT_POINTSET:
		if data.dtype != numpy.float32:
			print ("""Warning : NIFTI_INTENT_POINTSET requested, but array has no float32 dtype.
(some software requires a pointset to be float32)""")
		if len(data.shape) != 2 or data.shape[1] != 3:
			print ("Warning : NIFTI_INTENT_POINTSET requested, but shape(array) != (n,3)")
	if intent == GiftiIntentCode.NIFTI_INTENT_TRIANGLE:
		if len(data.shape) != 2 or data.shape[1] != 3:
			print ("Warning : NIFTI_INTENT_TRIANGLE requested, but shape(array) != (n,3)")
	if (intent == GiftiIntentCode.NIFTI_INTENT_RGB_VECTOR and data.dtype != rgb.fields_rgb) or \
	(intent == GiftiIntentCode.NIFTI_INTENT_RGBA_VECTOR and data.dtype != rgb.fields_rgba):
		print ("""Warning : NIFTI_INTENT_RGB*_VECTOR requested, but array has no rgb dtype.
(you can view an (3,n)-shaped uint8 array as an rgb dtype with rgb.as1D())""")