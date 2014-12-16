LINK_DIRECTORIES(
    ${multiarray_BINARY_DIR}/basic
    ${multiarray_BINARY_DIR}/test
    /usr/local/lib
)

INCLUDE_DIRECTORIES(
    ${multiarray_SOURCE_DIR}/source
    /usr/local/include
)

ADD_EXECUTABLE(
    arraytests
    
    iteratortest.cpp
    arraytest.cpp
    multiarraytest.cpp
)

TARGET_LINK_LIBRARIES( 
    arraytests

    gtest
    gtest_main
)
