from ctypes import *
giftilib = CDLL("libgiftiio.so")

#  GiftiIntentCode contains the intent codes as defined in nifti1.h
#  Dictionary access as well as direct access to the values in order to
#  provide as many accessor methods as possible.
class GiftiIntentCode:
    intents = {}
    intents["NIFTI_INTENT_NONE"] = 0
    intents["NIFTI_INTENT_TTEST"] = 3
    intents["NIFTI_INTENT_FTEST"] = 4
    intents["NIFTI_INTENT_ZSCORE"] = 5
    intents["NIFTI_INTENT_CHISQ"] = 6
    intents["NIFTI_INTENT_BETA"] = 7
    intents["NIFTI_INTENT_BINOM"] = 8
    intents["NIFTI_INTENT_GAMMA"] = 9
    intents["NIFTI_INTENT_POISSON"] = 10
    intents["NIFTI_INTENT_NORMAL"] = 11
    intents["NIFTI_INTENT_FTEST_NONC"] = 12
    intents["NIFTI_INTENT_CHISQ_NONC"] = 13
    intents["NIFTI_INTENT_LOGISTIC"] = 14
    intents["NIFTI_INTENT_LAPLACE"] = 15
    intents["NIFTI_INTENT_UNIFORM"] = 16
    intents["NIFTI_INTENT_TTEST_NONC"] = 17
    intents["NIFTI_INTENT_WEIBULL"] = 18
    intents["NIFTI_INTENT_CHI"] = 19
    intents["NIFTI_INTENT_INVGAUSS"] = 20
    intents["NIFTI_INTENT_EXTVAL"] = 21
    intents["NIFTI_INTENT_PVAL"] = 22
    intents["NIFTI_INTENT_LOGPVAL"] = 23
    intents["NIFTI_INTENT_LOG10PVAL"] = 24
    intents["NIFTI_FIRST_STATCODE"] = 2
    intents["NIFTI_LAST_STATCODE"] = 24
    intents["NIFTI_INTENT_ESTIMATE"] = 1001
    intents["NIFTI_INTENT_LABEL"] = 1002
    intents["NIFTI_INTENT_NEURONAME"] = 1003
    intents["NIFTI_INTENT_GENMATRIX"] = 1004
    intents["NIFTI_INTENT_SYMMATRIX"] = 1005
    intents["NIFTI_INTENT_DISPVECT"] = 1006
    intents["NIFTI_INTENT_VECTOR"] = 1007
    intents["NIFTI_INTENT_POINTSET"] = 1008
    intents["NIFTI_INTENT_TRIANGLE"] = 1009
    intents["NIFTI_INTENT_QUATERNION"] = 1010
    intents["NIFTI_INTENT_DIMLESS"] = 1011
    intents["NIFTI_INTENT_TIMESERIES"] = 2001
    intents["NIFTI_INTENT_NODE_INDEX"] = 2002
    intents["NIFTI_INTENT_RGB_VECTOR"] = 2003
    intents["NIFTI_INTENT_RGBA_VECTOR"] = 2004
    intents["NIFTI_INTENT_SHAPE"] = 2005

    NIFTI_INTENT_NONE = 0
    NIFTI_INTENT_TTEST = 3
    NIFTI_INTENT_FTEST = 4
    NIFTI_INTENT_ZSCORE = 5
    NIFTI_INTENT_CHISQ = 6
    NIFTI_INTENT_BETA = 7
    NIFTI_INTENT_BINOM = 8
    NIFTI_INTENT_GAMMA = 9
    NIFTI_INTENT_POISSON = 10
    NIFTI_INTENT_NORMAL = 11
    NIFTI_INTENT_FTEST_NONC = 12
    NIFTI_INTENT_CHISQ_NONC = 13
    NIFTI_INTENT_LOGISTIC = 14
    NIFTI_INTENT_LAPLACE = 15
    NIFTI_INTENT_UNIFORM = 16
    NIFTI_INTENT_TTEST_NONC = 17
    NIFTI_INTENT_WEIBULL = 18
    NIFTI_INTENT_CHI = 19
    NIFTI_INTENT_INVGAUSS = 20
    NIFTI_INTENT_EXTVAL = 21
    NIFTI_INTENT_PVAL = 22
    NIFTI_INTENT_LOGPVAL = 23
    NIFTI_INTENT_LOG10PVAL = 24
    NIFTI_FIRST_STATCODE = 2
    NIFTI_LAST_STATCODE = 24
    NIFTI_INTENT_ESTIMATE = 1001
    NIFTI_INTENT_LABEL = 1002
    NIFTI_INTENT_NEURONAME = 1003
    NIFTI_INTENT_GENMATRIX = 1004
    NIFTI_INTENT_SYMMATRIX = 1005
    NIFTI_INTENT_DISPVECT = 1006
    NIFTI_INTENT_VECTOR = 1007
    NIFTI_INTENT_POINTSET = 1008
    NIFTI_INTENT_TRIANGLE = 1009
    NIFTI_INTENT_QUATERNION = 1010
    NIFTI_INTENT_DIMLESS = 1011
    NIFTI_INTENT_TIMESERIES = 2001
    NIFTI_INTENT_NODE_INDEX = 2002
    NIFTI_INTENT_RGB_VECTOR = 2003
    NIFTI_INTENT_RGBA_VECTOR = 2004
    NIFTI_INTENT_SHAPE = 2005

class GiftiEncoding:
    encodings = {}
    encodings["GIFTI_ENCODING_UNDEF"]  = 0
    encodings["GIFTI_ENCODING_ASCII"]  = 1
    encodings["GIFTI_ENCODING_B64BIN"] = 2
    encodings["GIFTI_ENCODING_B64GZ"]  = 3
    encodings["GIFTI_ENCODING_EXTBIN"] = 4
    encodings["GIFTI_ENCODING_MAX"]    = 4

    GIFTI_ENCODING_UNDEF  = 0
    GIFTI_ENCODING_ASCII  = 1
    GIFTI_ENCODING_B64BIN = 2
    GIFTI_ENCODING_B64GZ  = 3
    GIFTI_ENCODING_EXTBIN = 4
    GIFTI_ENCODING_MAX    = 4

class GiftiEndian:
    endian = {}
    endian["GIFTI_ENDIAN_UNDEF"] = 0
    endian["GIFTI_ENDIAN_BIG"]   = 1
    endian["GIFTI_ENDIAN_LITTLE"]= 2
    endian["GIFTI_ENDIAN_MAX"]   = 2

    GIFTI_ENDIAN_UNDEF = 0
    GIFTI_ENDIAN_BIG   = 1
    GIFTI_ENDIAN_LITTLE= 2
    GIFTI_ENDIAN_MAX   = 2
    
class GiftiDataType:
    datatypes = {}
    datatypes["NIFTI_TYPE_UINT8"]      = 2
    datatypes["NIFTI_TYPE_INT16"]      = 4
    datatypes["NIFTI_TYPE_INT32"]      = 8
    datatypes["NIFTI_TYPE_FLOAT32"]    = 16
    datatypes["NIFTI_TYPE_COMPLEX64"]  = 32
    datatypes["NIFTI_TYPE_FLOAT64"]    = 64
    datatypes["NIFTI_TYPE_RGB24"]      = 128
    datatypes["NIFTI_TYPE_INT8"]       = 256
    datatypes["NIFTI_TYPE_UINT16"]     = 512
    datatypes["NIFTI_TYPE_UINT32"]     = 768
    datatypes["NIFTI_TYPE_INT64"]      = 1024
    datatypes["NIFTI_TYPE_UINT64"]     = 1280
    datatypes["NIFTI_TYPE_FLOAT128"]   = 1536   #  Python cannot handle 128-bit floats
    datatypes["NIFTI_TYPE_COMPLEX128"] = 1792
    datatypes["NIFTI_TYPE_COMPLEX256"] = 2048   #  Python cannot handle 128-bit floats
    datatypes["NIFTI_TYPE_RGBA32"]     = 2304

    NIFTI_TYPE_UINT8      = 2
    NIFTI_TYPE_INT16      = 4
    NIFTI_TYPE_INT32      = 8
    NIFTI_TYPE_FLOAT32    = 16
    NIFTI_TYPE_COMPLEX64  = 32
    NIFTI_TYPE_FLOAT64    = 64
    NIFTI_TYPE_RGB24      = 128
    NIFTI_TYPE_INT8       = 256
    NIFTI_TYPE_UINT16     = 512
    NIFTI_TYPE_UINT32     = 768
    NIFTI_TYPE_INT64      = 1024
    NIFTI_TYPE_UINT64     = 1280
    NIFTI_TYPE_FLOAT128   = 1536   #  Python cannot handle 128-bit floats
    NIFTI_TYPE_COMPLEX128 = 1792
    NIFTI_TYPE_COMPLEX256 = 2048   #  Python cannot handle 128-bit floats
    NIFTI_TYPE_RGBA32     = 2304

#  The following classes are the wrappers as defined by the
#  struct definitions in gifti_io.h
class GiftiMetaData(Structure):
    _fields_ = [("length", c_int),
                ("name",  POINTER(c_char_p)),
                ("value", POINTER(c_char_p))]

class GiftiNVPairs(Structure):
    _fields_ = [("length", c_int),
                ("name",  POINTER(c_char_p)),
                ("value", POINTER(c_char_p))]

class GiftiLabelTable(Structure):
    _fields_ = [("length", c_int),
                ("index", POINTER(c_int)),
                ("label", POINTER(c_char_p))]

class GiftiCoordSystem(Structure):
    _fields_ = [("dataspace", c_char_p),
                ("xformspace", c_char_p),
                ("xform", c_double*16)]

class GiftiDataArray(Structure):
    _fields_ = [("intent", c_int),
                ("datatype", c_int),
                ("ind_ord", c_int),
                ("num_dim", c_int),
                ("dims", c_int*6),
                ("encoding", c_int),
                ("endian", c_int),
                ("ext_fname", c_char_p),
                ("ext_offset", c_longlong),
                ("meta", GiftiMetaData),
                ("coordsys", POINTER(GiftiCoordSystem)),
                ("data", c_void_p),
                ("nvals", c_longlong),
                ("nbyper", c_int),
                ("ex_atrs", GiftiNVPairs)]

class GiftiImage(Structure):
    _fields_ = [("numDA", c_int),
                ("version", c_char_p),
                ("meta", GiftiMetaData),
                ("labeltable", GiftiLabelTable),
                ("darray", POINTER(POINTER(GiftiDataArray))),
                ("swapped", c_int),
                ("compressed", c_int),
                ("ex_atrs", GiftiNVPairs)]

class GiftiGlobals(Structure):
    _fields_ = [("verb", c_int)]

class GiftiTypeEle(Structure):
    _fields_ = [("type", c_int),
                ("nbyper", c_int),
                ("swapsize", c_int),
                ("name", c_char_p)]

#  Some helper functions
def get_endianness():
    import array
    end =  ord(array.array("i",[1]).tostring()[0])
    
    if end:
        # this is little endian
        return GiftiEndian.GIFTI_ENDIAN_LITTLE
    else:
        # this is big endian
        return GiftiEndian.GIFTI_ENDIAN_BIG

## Begin Main API routines ##
def gifti_read_image(fname, read_data):
    read_func = giftilib.gifti_read_image
    read_func.restype = POINTER(GiftiImage)
    image = read_func(fname, read_data)
    return image.contents

def gifti_read_da_list(fname, read_data, dalist, len):
    read_func = giftilib.gifti_read_data_list
    read_func.restype = POINTER(GiftiImage)
    image = read_func(fname, read_data, pointer(dalist), len)
    return image.contents

def gifti_free_image(image):
    free_func = giftilib.gifti_free_image
    ret = free_func(pointer(image))
    return ret

def gifti_valid_gifti_image(image, whine):
    val_func = giftilib.gifti_valid_gifti_image
    ret = val_func(image, whine)
    return ret
    
def gifti_write_image(image, fname, write_data):
    write_func = giftilib.gifti_write_image
    ret = write_func(pointer(image), fname, write_data)
    return ret

def gifti_create_image(numDA, intent, dtype, ndim, dims, alloc_data):
    create_func = giftilib.gifti_create_image
    create_func.restype = POINTER(GiftiImage)
    dim_ar = (c_int*ndim)()
    dim_ar = cast(dim_ar, POINTER(c_int))
    for i in xrange(ndim):
        dim_ar[i] = dims[i]
    image = create_func(numDA, intent, dtype, ndim, dim_ar, alloc_data)
    return image.contents

## End main API functions ##

## Begin state set/get routines ##
def gifti_get_b64_check():
    return giftilib.gifti_get_b64_check()

def gifti_set_b64_check(lvl):
    func = giftilib.gifti_set_b64_check
    ret = func(lvl)
    return ret

def gifti_get_indent():
    return giftilib.gifti_get_indent()

def gifti_set_indent(lvl):
    func = giftilib.gifti_set_indent
    ret = func(lvl)
    return ret

def gifti_get_verb():
    return giftilib.gifti_get_verb()
    
def gifti_set_verb(lvl):
    verb_func = giftilib.gifti_set_verb
    ret = verb_func(lvl)
    return ret

def gifti_get_update_ok():
    return giftilib.gifti_get_update_ok()

def gifti_set_update_ok(lvl):
    func = giftilib.gifti_set_update_ok
    ret = func(lvl)
    return ret

def gifti_get_zlevel():
    return giftilib.gifti_get_zlevel()

def gifti_set_zlevel(lvl):
    func = giftilib.gifti_set_zlevel
    ret = func(lvl)
    return ret

## End state set/get routines ##

## Begin data copy routines ##
def gifti_convert_to_float(image):
    func = giftilib.gifti_convert_to_float
    return func(pointer(image))

def gifti_copy_nvpairs(dest, src):
    func = giftilib.gifti_copy_nvpairs
    return func(pointer(dest), pointer(src))

def gifti_copy_char_list(str_list, length):
    func = giftilib.gifti_copy_char_list
    func.restype = POINTER(c_char_p)
    str_ar = (c_char_p*length)()
    str_ar = cast(str_ar, POINTER(c_char_p))
    for i in xrange(length):
        str_ar[i] = str_list[i]
    ret = func(str_ar, length)
    out_list = []
    for i in xrange(length):
        out_list.append(ret[i])
    return out_list

def gifti_copy_LabelTable(dest, src):
    func = giftilib.gifti_copy_LabelTable
    return func(pointer(dest), pointer(src))

def gifti_strdup(src):
    func = giftilib.gifti_strdup
    func.restype = c_char_p
    return func(src).value

def gifti_copy_gifti_image(old, copy_data):
    func = giftilib.gifti_copy_gifti_image
    func.restype = POINTER(GiftiImage)
    return func(pointer(old), copy_data).contents

def gifti_copy_CoordSystem(src):
    func = giftilib.gifti_copy_CoordSystem
    func.restype = POINTER(GiftiCoordSystem)
    return func(pointer(src)).contents

def gifti_copy_DataArray(orig, get_data):
    func = giftilib.gifti_copy_DataArray
    func.restype = POINTER(GiftiDataArray)
    return func(pointer(orig), get_data).contents

def gifti_darray_nvals(da):
    func = giftilib.gifti_darray_nvals
    func.restype = c_longlong
    return func(pointer(da)).value

def gifti_gim_DA_size(image, in_mb):
    func = giftilib.gifti_gim_DA_size
    func.restype = c_longlong
    return func(pointer(image), in_mb).value

def gifti_check_swap(data, endian, nsets):
    check_func = giftilib.gifti_check_swap
    ret = check_func(pointer(data), endian, c_longlong(nsets))
    return ret

def gifti_datatype_sizes(datatype, nbyper, swapsize):
    func = giftilib.gifti_datatype_sizes
    return func(datatype, pointer(nbyper), pointer(swapsize))

def gifti_datatype2str(type):
    func = giftilib.gifti_datatype2str
    func.restype = c_char_p
    return func(type).value

def gifti_get_this_endian():
    return giftilib.gifti_get_this_endian()

def gifti_intent_from_string(name):
    return giftilib.gifti_intent_from_string(c_char_p(name))

def gifti_intent_is_valid(code):
    return giftilib.gifti_intent_is_valid(code)

def gifti_intent_to_string(code):
    func = giftilib.gifti_intent_to_string
    func.restype = c_char_p
    return func(code).value

def gifti_list_index2string(str_list, index):
    func = giftilib.gifti_list_index2string
    func.restype = c_char_p
    str_ar = (c_char_p*len(str_list))()
    str_ar = cast(str_ar, POINTER(c_char_p))
    for i in xrange(len(str_list)):
        str_ar[i] = str_list[i]
    return func(str_ar, index).value

def gifti_get_xml_buf_size():
    return giftilib.gifti_get_xml_buf_size()

def gifti_set_xml_buf_size(size):
    return giftilib.gifti_set_xml_buf_size(size)

def gifti_str2attr_gifti(image, attr, val):
    return giftilib.gifti_str2attr_gifti(pointer(image), attr, val)

def gifti_str2attr_darray(da, attr, val):
    return giftilib.gifti_str2attr_darray(pointer(da), attr, val)

def gifti_str2ind_ord(cstr):
    return giftilib.gifti_str2ind_ord(cstr)

def gifti_str2dataloc(cstr):
    return giftilib.gifti_str2dataloc(cstr)

def gifti_str2encoding(cstr):
    return giftilib.gifti_str2encoding(cstr)

def gifti_str2endian(cstr):
    return giftilib.gifti_str2endian(cstr)

def gifti_str2datatype(cstr):
    return giftilib.gifti_str2datatype(cstr)

def gifti_swap_2bytes(data, nsets):
    return giftilib.gifti_swap_2bytes(cast(pointer(data), c_void_p), c_longlong(nsets))

def gifti_swap_4bytes(data, nsets):
    return giftilib.gifti_swap_4bytes(cast(pointer(data), c_void_p), c_longlong(nsets))

def gifti_swap_Nbytes(data, nsets, swapsize):
    swap_func = giftilib.gifti_swap_Nbytes
    return swap_func(cast(pointer(data), c_void_p), nsets, swapsize)

def gifti_alloc_DA_data(image, dalist, length):
    da_ar = (c_int*length)()
    da_ar = cast(da_ar, POINTER(c_int))
    for i in xrange(length):
        da_ar[i] = dalist[i]
    return giftilib.gifti_alloc_DA_data(pointer(image), da_ar, length)

def gifti_add_empty_darray(image, num_to_add):
    return giftilib.gifti_add_empty_darray(pointer(image), num_to_add)

def gifti_add_to_meta(md, name, val, replace):
    add_func = giftilib.gifti_add_to_meta
    ret = add_func(pointer(md), name, val, replace)
    return ret

def gifti_add_to_nvpairs(p, name, val):
    return giftilib.gifti_add_to_nvpairs(pointer(p), name, val)

def gifti_free_CoordSystem(cs):
    return giftilib.gifti_free_CoordSystem(pointer(cs))

def gifti_free_DataArray_list(da_list, numDA):
    da_ar = (POINTER(GiftiDataArray)*numDA)()
    da_ar = cast(da_ar, POINTER(POINTER(GiftiDataArray)))
    for i in xrange(numDA):
        da_ar[i] = pointer(da_list[i])
    return giftilib.gifti_free_DataArray_list(da_ar, numDA)

def gifti_free_DataArray(da):
    return giftilib.gifti_free_DataArray(pointer(da))

def gifti_free_LabelTable(t):
    return giftilib.gifti_free_LabelTable(pointer(t))

def gifti_free_nvpairs(p):
    return giftilib.gifti_free_nvpairs(pointer(p))
        
def gifti_read_dset_numDA(fname):
    return giftilib.gifti_read_dset_numDA(fname)

def gifti_set_atr_in_DAs(image, name, val, dalist, length):
    da_ar = (c_int*length)()
    da_ar = cast(da_ar, POINTER(c_int))
    for i in xrange(length):
        da_ar[i] = dalist[i]

    return giftilib.gifti_set_atr_in_DAs(pointer(image), name, val, da_ar, length)

def gifti_set_DA_atrs(da, attr_list, length, add_to_extra):
    at_ar = (c_char_p*length)()
    at_ar = cast(at_ar, POINTER(c_char_p))
    for i in xrange(length):
        at_ar[i] = pointer(c_char_p(attr_list[i]))

    return giftilib.gifti_set_DA_atrs(pointer(da), at_ar, length, add_to_extra)

def gifti_set_DA_defaults(da):
    return giftilib.gifti_set_DA_defaults(pointer(da))

def gifti_set_DA_meta(image, name, val, dalist, length, replace):
    da_ar = (c_int*length)()
    da_ar = cast(da_ar, POINTER(c_int))
    for i in xrange(length):
        da_ar[i] = dalist[i]

    return giftilib.gifti_set_DA_meta(pointer(image), name, val, da_ar, length, replace)

def gifti_set_dims_all_DA(image, ndim, dim_list):
    dim_ar = (c_int*ndim)()
    dim_ar = cast(dim_ar, POINTER(c_int))
    for i in xrange(ndim):
        dim_ar[i] = dim_list[i]

    return giftilib.gifti_set_dims_all_DA(pointer(image), ndim, dim_ar)

def gifti_update_nbyper(image):
    return giftilib.gifti_update_nbyper(pointer(image))

def gifti_valid_DataArray(da, whine):
    return giftilib.gifti_valid_DataArray(pointer(da), whine)

def gifti_valid_datatype(dtype, whine):
    return giftilib.gifti_valid_datatype(dtype, whine)

def gifti_valid_dims(da, whine):
    return giftilib.gifti_valid_dims(pointer(da), whine)

def gifti_valid_int_list(int_list, length, min, max, whine):
    int_ar = (c_int*length)()
    int_ar = cast(int_ar, POINTER(c_int))
    for i in xrange(length):
        int_ar[i] = int_list[i]

    return giftilib.gifti_valid_int_list(int_ar, length, min, max, whine)
    
def gifti_valid_LabelTable(t, whine):
    return giftilib.gifti_valid_labelTable(pointer(t), whine)

def gifti_valid_nbyper(nbyper, whine):
    return giftilib.gifti_valid_nbyper(nbyper, whine)

def gifti_valid_num_dim(ndim, whine):
    return giftilib.gifti_valid_num_dim(ndim, whine)

def gifti_valid_nvpairs(nvp, whine):
    return giftilib.gifti_valid_nvpairs(pointer(nvp), whine)

def gifti_validate_dims(da, whine):
    return giftilib.gifti_validate_dims(pointer(da), whine)

## Display-based routines omitted ##
def gifti_clear_DataArray(da):
    return giftilib.gifti_clear_DataArra(pointer(da))

def gifti_clear_float_zeros(cstr):
    return giftilib.gifti_clear_float_zeros(c_char_p(cstr))

def gifti_clear_gifti_image(image):
    return giftilib.gifti_clear_gifti_image(pointer(image))

def gifti_clear_nvpairs(p):
    return giftilib.gifti_clear_nvpairs(pointer(p))

def gifti_clear_LabelTable(lt):
    return giftilib.gifti_clear_LabelTable(pointer(lt))

def gifti_clear_CoordSystem(cs):
    return giftilib.gifti_clear_CoordSystem(pointer(cs))

def gifti_find_DA(image, intent, index):
    func = giftilib.gifti_find_DA
    func.restype = POINTER(GiftiDataArray)
    return func(pointer(image), intent, index).contents

def gifti_DA_rows_cols(da, rows, cols):
    rows = c_int()
    cols = c_int()
    ret = giftilib.gifti_DA_rows_cols(pointer(da), pointer(rows), pointer(cols))
    return (rows, cols, ret)

def gifticlib_version():
    return giftilib.gifticlib_version().value
    
