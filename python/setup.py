from distutils.core import setup, Extension
from Cython.Build import cythonize

from subprocess import Popen, PIPE

def get(cmd):
    proc = Popen(cmd, stdout=PIPE, encoding='utf-8')
    out, _ = proc.communicate()
    return out.strip()[2:]

extenstions = [
    Extension('darr', ['darr.pyx'],
        include_dirs=[get(['pkg-config', '--cflags', 'libdarr'])],
        libraries=[get(['pkg-config', '--libs-only-l', 'libdarr'])],
        library_dirs=[get(['pkg-config', '--libs-only-L', 'libdarr'])],
        extra_compile_args=['-std=c++0x'],
        language='c++')
]

setup(
    name = 'darr',
    ext_modules = cythonize(extenstions),
)
