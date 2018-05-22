#
# Build AMICI python interface
#

# Find Python interpreter, libs and headers
find_package(PythonInterp 3.6 REQUIRED)# TODO to setup.py
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})

set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/amici)
#set(CMAKE_SWIG_FLAGS "")

set_source_files_properties(
    ${AMICI_SRC_LIST} ${AMICI_DIR}/swig/amici.i
    PROPERTIES CPLUSPLUS ON
    )

SWIG_ADD_LIBRARY(
    amici MODULE
    LANGUAGE python
    SOURCES ${AMICI_DIR}/swig/amici.i
    )

SWIG_LINK_LIBRARIES(amici 
    ${PYTHON_LIBRARIES}
    ${AMICI_DIR}/build/libamici${CMAKE_STATIC_LIBRARY_SUFFIX}
    ${SUNDIALS_LIB_DIR}/libsundials_nvecserial${CMAKE_STATIC_LIBRARY_SUFFIX}
    ${SUNDIALS_LIB_DIR}/libsundials_cvodes${CMAKE_STATIC_LIBRARY_SUFFIX}
    ${SUNDIALS_LIB_DIR}/libsundials_idas${CMAKE_STATIC_LIBRARY_SUFFIX}
    ${SUITESPARSE_DIR}/KLU/Lib/libklu${CMAKE_STATIC_LIBRARY_SUFFIX}
    ${SUITESPARSE_DIR}/COLAMD/Lib/libcolamd${CMAKE_STATIC_LIBRARY_SUFFIX}
    ${SUITESPARSE_DIR}/BTF/Lib/libbtf${CMAKE_STATIC_LIBRARY_SUFFIX}
    ${SUITESPARSE_DIR}/AMD/Lib/libamd${CMAKE_STATIC_LIBRARY_SUFFIX}
    ${SUITESPARSE_DIR}/SuiteSparse_config/libsuitesparseconfig${CMAKE_STATIC_LIBRARY_SUFFIX}
    ${HDF5_HL_LIBRARIES}
    ${HDF5_C_LIBRARIES}
    ${HDF5_CXX_LIBRARIES}
    ${BLAS_LIBRARIES}
    -ldl -lz -lm)

# put library into python package
# (SWIG_ADD_LIBRARY directory options don't work in current cmake version)
set_target_properties(_amici
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/amici"
    )

#SET(PYTHON_INSTALL_FILES
#    ${CMAKE_CURRENT_BINARY_DIR}/amici.py
#    $<TARGET_FILE_NAME:_amici>)