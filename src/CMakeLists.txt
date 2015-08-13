# -----------------------------------------------
# BUILD THE PARTS BASED DETECTOR FROM SOURCE
# -----------------------------------------------
set(SRC_FILES
    DepthConsistency.cpp
    DynamicProgram.cpp
    FileStorageModel.cpp
    HOGFeatures.cpp
    PartsBasedDetector.cpp
    PoseEstimator.cpp
    SearchSpacePruning.cpp
    SpatialConvolutionEngine.cpp
    StereoCameraModel.cpp
    Visualize.cpp
    nms.cpp
    estimator.cpp
)

set(LIBS        ${Boost_LIBRARIES}
                ${OpenCV_LIBS}
)

# with cvmatio support
if (cvmatio_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWITH_MATLABIO")
    set(SRC_FILES ${SRC_FILES} MatlabIOModel.cpp)
    set(LIBS ${LIBS} ${ZLIB_LIBRARIES} ${cvmatio_LIBRARIES})
    add_executable(ModelTransfer ModelTransfer.cpp FileStorageModel.cpp MatlabIOModel.cpp)
    target_link_libraries(ModelTransfer ${LIBS})
    install(TARGETS ModelTransfer
            RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin
    )
endif()

# as a library (always)
add_library(${PROJECT_NAME}_lib SHARED ${SRC_FILES})
target_link_libraries(${PROJECT_NAME}_lib ${LIBS})
set_target_properties(${PROJECT_NAME}_lib PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME}_lib
        DESTINATION ${PROJECT_SOURCE_DIR}/lib
)

# as an executable
set(EXE1_FILES SRC_FILES)
set(EXE1_FILES pose_estimator_demo.c)
add_executable(${PROJECT_NAME}1_bin ${EXE1_FILES})
target_link_libraries(${PROJECT_NAME}1_bin ${LIBS} ${PROJECT_NAME}_lib)
set_target_properties(${PROJECT_NAME}1_bin PROPERTIES
                      OUTPUT_NAME ${PROJECT_NAME}1)
install(TARGETS ${PROJECT_NAME}1_bin
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin
)

set(EXE2_FILES SRC_FILES)
set(EXE2_FILES demo.cpp)
add_executable(${PROJECT_NAME}2_bin ${EXE2_FILES})
target_link_libraries(${PROJECT_NAME}2_bin ${LIBS} ${PROJECT_NAME}_lib)
set_target_properties(${PROJECT_NAME}2_bin PROPERTIES
                      OUTPUT_NAME ${PROJECT_NAME}2)
install(TARGETS ${PROJECT_NAME}2_bin
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin
)
