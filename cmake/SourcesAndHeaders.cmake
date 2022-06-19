set(sources
        ${PROJECT_SOURCE_DIR}/src/details/writer.cpp
        ${PROJECT_SOURCE_DIR}/src/details/reader.cpp
        ${PROJECT_SOURCE_DIR}/src/details/deserialize.cpp
        ${PROJECT_SOURCE_DIR}/src/error.cpp
        )

set(headers
        # ethabi
        ${PROJECT_SOURCE_DIR}/include/ethabi/ethabi.hpp
        ${PROJECT_SOURCE_DIR}/include/ethabi/error.hpp

        # ethabi::param_type
        ${PROJECT_SOURCE_DIR}/include/ethabi/details/param_type/param_type.hpp
        ${PROJECT_SOURCE_DIR}/include/ethabi/details/param_type/reader.hpp
        ${PROJECT_SOURCE_DIR}/include/ethabi/details/param_type/writer.hpp
        ${PROJECT_SOURCE_DIR}/include/ethabi/details/param_type/deserialize.hpp
        ${PROJECT_SOURCE_DIR}/include/ethabi/details/param_type.hpp

        # ethcore::meta
        ${PROJECT_SOURCE_DIR}/include/ethcore/details/meta/is_any_of.hpp

        # ethcore::rva
        ${PROJECT_SOURCE_DIR}/include/ethcore/details/rva/rva.hpp

        # ethcore::st
        ${PROJECT_SOURCE_DIR}/include/ethcore/details/st/st.hpp
        ${PROJECT_SOURCE_DIR}/include/ethcore/details/st/is_strong_type.hpp
        ${PROJECT_SOURCE_DIR}/include/ethcore/details/st/traits.hpp
        ${PROJECT_SOURCE_DIR}/include/ethcore/details/st/type.hpp
        ${PROJECT_SOURCE_DIR}/include/ethcore/details/st/unwrap.hpp
        )

set(test_sources
        ${PROJECT_SOURCE_DIR}/test/src/main.cpp
        ${PROJECT_SOURCE_DIR}/test/src/test_param_type.cpp
        )
