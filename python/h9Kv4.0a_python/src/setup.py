#setup.py
#!/usr/bin/env python

"""
setup.py file for SWIG C++ /Python example
%% python setup.py build_ext --inplace

"""
from distutils.core import setup, Extension

py_rdb_test_module = Extension('_py_rdb',
    sources=['py_rdb.c', 'py_rdb_wrap.c',],
    extra_link_args = ['/NODEFAULTLIB:libcmtd.lib','/EDITANDCONTINUE']
)
py_rdb_datatype_module = Extension('_py_data_type',
    sources=['py_h9kv4_datatype_wrap.c',  ],
    extra_link_args = ['/NODEFAULTLIB:libcmtd.lib','/EDITANDCONTINUE']
)

setup (
    name = 'py_rdb_test',
    version = '0.1',
    author = "Chi.Hailong",
    description = """Simple swig C\+\+/Python example""",
    ext_modules = [py_rdb_test_module,py_rdb_datatype_module],
    py_modules = ["py_rdb_test","py_rdb_data_type.py"],
    include_dirs = ["C:\Tcl\include"],
)