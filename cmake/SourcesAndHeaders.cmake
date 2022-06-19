set(sources
        src/details/param_type.cpp
        src/details/writter.cpp
        src/details/reader.cpp
        src/details/deserialize.cpp
        src/error.cpp
        )

set(exe_sources
        src/main.cpp
        ${sources}
        )

set(headers
        include/${PROJECT_NAME}/ethabi.hpp
        include/${PROJECT_NAME}/details/param_type.hpp
        )

set(test_sources
        src/main.cpp
        src/test_param_type.cpp
        )
