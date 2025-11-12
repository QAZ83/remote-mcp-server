cmake_minimum_required(VERSION 3.16)

project(AIForgeStudio VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags
if(MSVC)
    add_compile_options(/W4 /WX)
    add_compile_options(/arch:AVX2)  # Use AVX2 instructions
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    add_compile_options(-march=native)  # Optimize for current CPU
endif()

# Qt6 configuration
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Qml
    Quick
    QuickControls2
    Charts
)

# Find CUDA (optional but recommended)
find_package(CUDA QUIET)
if(CUDA_FOUND)
    enable_language(CUDA)
    message(STATUS "CUDA found: ${CUDA_VERSION}")
    set(CMAKE_CUDA_STANDARD 17)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)
    add_compile_definitions(CUDA_AVAILABLE)
else()
    message(WARNING "CUDA not found - AI features will be limited")
endif()

# Find Vulkan (optional)
find_package(Vulkan QUIET)
if(Vulkan_FOUND)
    message(STATUS "Vulkan found: ${Vulkan_VERSION}")
    add_compile_definitions(VULKAN_AVAILABLE)
else()
    message(WARNING "Vulkan not found - graphics features will be limited")
endif()

# Find Python3 (for Python bridge)
find_package(Python3 COMPONENTS Interpreter Development QUIET)
if(Python3_FOUND)
    message(STATUS "Python found: ${Python3_VERSION}")
    add_compile_definitions(PYTHON_AVAILABLE)
else()
    message(WARNING "Python not found - Python bridge disabled")
endif()

# Find pybind11 (optional)
find_package(pybind11 QUIET)
if(pybind11_FOUND)
    message(STATUS "pybind11 found")
    add_compile_definitions(PYBIND11_AVAILABLE)
endif()

# TensorRT (optional)
# Uncomment when TensorRT is available
# find_package(TensorRT QUIET)
# if(TensorRT_FOUND)
#     message(STATUS "TensorRT found")
#     add_compile_definitions(TENSORRT_AVAILABLE)
# endif()

# NVML (NVIDIA Management Library)
# On Linux: typically in /usr/lib/x86_64-linux-gnu or /usr/local/cuda/lib64
# On Windows: in CUDA toolkit installation
if(CUDA_FOUND)
    if(WIN32)
        set(NVML_LIBRARY "${CUDA_TOOLKIT_ROOT_DIR}/lib/x64/nvml.lib")
    else()
        find_library(NVML_LIBRARY NAMES nvidia-ml PATHS /usr/lib/x86_64-linux-gnu /usr/local/cuda/lib64)
    endif()

    if(NVML_LIBRARY)
        message(STATUS "NVML library found: ${NVML_LIBRARY}")
        add_compile_definitions(NVML_AVAILABLE)
    else()
        message(WARNING "NVML library not found")
    endif()
endif()

# Source files
set(CORE_SOURCES
    core/logger.cpp
    core/hardware_monitor.cpp
    core/ai_engine.cpp
    core/render_engine.cpp
)

set(CORE_HEADERS
    core/logger.h
    core/hardware_monitor.h
    core/ai_engine.h
    core/render_engine.h
)

# Python bridge sources (conditional)
if(Python3_FOUND AND pybind11_FOUND)
    set(PYTHON_BRIDGE_SOURCES
        python_bridge/bridge.cpp
    )
    set(PYTHON_BRIDGE_HEADERS
        python_bridge/bridge.h
    )
endif()

# QML files
set(QML_SOURCES
    ui/main.qml
    ui/NavButton.qml
    ui/dashboard.qml
    ui/model_manager.qml
    ui/render_view.qml
    ui/settings.qml
    ui/GlassCard.qml
    ui/MetricRow.qml
    ui/GlowButton.qml
)

# Create main executable
add_executable(AIForgeStudio
    main.cpp
    ${CORE_SOURCES}
    ${CORE_HEADERS}
    ${PYTHON_BRIDGE_SOURCES}
    ${PYTHON_BRIDGE_HEADERS}
)

# Link Qt libraries
target_link_libraries(AIForgeStudio PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Charts
)

# Link CUDA libraries
if(CUDA_FOUND)
    target_link_libraries(AIForgeStudio PRIVATE
        ${CUDA_LIBRARIES}
        ${CUDA_CUBLAS_LIBRARIES}
    )

    if(NVML_LIBRARY)
        target_link_libraries(AIForgeStudio PRIVATE ${NVML_LIBRARY})
    endif()

    target_include_directories(AIForgeStudio PRIVATE ${CUDA_INCLUDE_DIRS})
endif()

# Link Vulkan libraries
if(Vulkan_FOUND)
    target_link_libraries(AIForgeStudio PRIVATE Vulkan::Vulkan)
endif()

# Link Python libraries
if(Python3_FOUND AND pybind11_FOUND)
    target_link_libraries(AIForgeStudio PRIVATE
        Python3::Python
        pybind11::embed
    )
endif()

# Platform-specific libraries
if(WIN32)
    target_link_libraries(AIForgeStudio PRIVATE
        ws2_32  # Windows sockets
        psapi   # Process API
    )
elseif(UNIX AND NOT APPLE)
    target_link_libraries(AIForgeStudio PRIVATE
        pthread
        dl
    )
endif()

# Include directories
target_include_directories(AIForgeStudio PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/python_bridge
)

# Copy QML files to build directory
foreach(QML_FILE ${QML_SOURCES})
    configure_file(${QML_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${QML_FILE} COPYONLY)
endforeach()

# Copy Python scripts to build directory
if(Python3_FOUND)
    file(GLOB PYTHON_SCRIPTS "python_bridge/*.py")
    foreach(PYTHON_SCRIPT ${PYTHON_SCRIPTS})
        get_filename_component(SCRIPT_NAME ${PYTHON_SCRIPT} NAME)
        configure_file(${PYTHON_SCRIPT}
                      ${CMAKE_CURRENT_BINARY_DIR}/python_bridge/${SCRIPT_NAME}
                      COPYONLY)
    endforeach()
endif()

# Create output directories
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/models)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/ui/assets)

# Installation rules
install(TARGETS AIForgeStudio
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY ui/
    DESTINATION share/AIForgeStudio/ui
    FILES_MATCHING PATTERN "*.qml"
)

if(Python3_FOUND)
    install(DIRECTORY python_bridge/
        DESTINATION share/AIForgeStudio/python_bridge
        FILES_MATCHING PATTERN "*.py"
    )
endif()

# Tests (optional)
option(BUILD_TESTS "Build test suite" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Package configuration
set(CPACK_PACKAGE_NAME "AIForgeStudio")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "AI Forge Studio - RTX 50-Series Edition")
set(CPACK_PACKAGE_VENDOR "AI Forge Studio")

if(WIN32)
    set(CPACK_GENERATOR "NSIS;ZIP")
else()
    set(CPACK_GENERATOR "TGZ;DEB")
endif()

include(CPack)

# Print configuration summary
message(STATUS "")
message(STATUS "==================================================")
message(STATUS "AI Forge Studio Configuration Summary")
message(STATUS "==================================================")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Qt6 version: ${Qt6_VERSION}")
message(STATUS "CUDA: ${CUDA_FOUND}")
message(STATUS "Vulkan: ${Vulkan_FOUND}")
message(STATUS "Python: ${Python3_FOUND}")
message(STATUS "pybind11: ${pybind11_FOUND}")
message(STATUS "==================================================")
message(STATUS "")
