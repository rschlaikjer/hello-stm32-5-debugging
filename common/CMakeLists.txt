enable_language(C CXX)

project(common C CXX)

set(SRCS
    ${PROJECT_SOURCE_DIR}/src/arm/mpu.cpp
    ${PROJECT_SOURCE_DIR}/src/arm/scb.cpp
    ${PROJECT_SOURCE_DIR}/src/instrumentation/fault_handler.cpp
)

set(${PROJECT_NAME}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

set(${PROJECT_NAME}_SOURCES
    ${SRCS}
    CACHE INTERNAL "${PROJECT_NAME}: Sources" FORCE)
