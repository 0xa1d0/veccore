set(FLAGS_SSE   "-msse")
set(FLAGS_SSE2  "-msse2")
set(FLAGS_SSE3  "-msse3")
set(FLAGS_SSSE3 "-mssse3")
set(FLAGS_SSE41 "-msse4.1")
set(FLAGS_SSE42 "-msse4.2")
set(FLAGS_AVX   "-mavx")
set(FLAGS_AVX2  "-mf16c;-mavx2;-mfma;-mlzcnt;-mbmi;-mbmi2")
set(FLAGS_AVX512 "-mavx512")
set(FLAGS_NATIVE "-march=native")

if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.5)
  set(FLAGS_KNL "-march=knl")
endif()
