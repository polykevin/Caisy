add_library(glad STATIC 
    ./glad/glad.h  
    ./glad/glad.c )
set_target_properties(glad PROPERTIES LINKER_LANGUAGE C++)
target_include_directories(glad PUBLIC ./)

