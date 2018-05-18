LINK_DIRECTORIES(
    ${multiarray_BINARY_DIR}/basic
    ${multiarray_BINARY_DIR}/test
    /usr/local/lib
)

INCLUDE_DIRECTORIES(
    ${multiarray_SOURCE_DIR}/source
    ${multiarray_SOURCE_DIR}/thirdparty
    /usr/local/include
)

ADD_EXECUTABLE(
    arraytests
    
    iteratortest.cpp
    arraytest.cpp
    multiarraytest.cpp
)
