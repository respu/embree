## ======================================================================== ##
## Copyright 2009-2015 Intel Corporation                                    ##
##                                                                          ##
## Licensed under the Apache License, Version 2.0 (the "License");          ##
## you may not use this file except in compliance with the License.         ##
## You may obtain a copy of the License at                                  ##
##                                                                          ##
##     http://www.apache.org/licenses/LICENSE-2.0                           ##
##                                                                          ##
## Unless required by applicable law or agreed to in writing, software      ##
## distributed under the License is distributed on an "AS IS" BASIS,        ##
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. ##
## See the License for the specific language governing permissions and      ##
## limitations under the License.                                           ##
## ======================================================================== ##

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SOURCE_GROUP("Source Files\\bvh4" "/bvh4/" )
SOURCE_GROUP("Source Files\\bvh8" "/bvh8/" )
SOURCE_GROUP("Source Files\\geometry" "/geometry/" )
SOURCE_GROUP("Source Files\\builders" "/builders.*/" )
SOURCE_GROUP("Source Files\\common" "/kernels/common/" )
SOURCE_GROUP("Source Files\\tasking" "/tasking/" )
SOURCE_GROUP("Source Files\\algorithms" "/algorithms/")
SOURCE_GROUP("" FILES embree.rc)

IF (TARGET_SSE41)
 ADD_DEFINITIONS(-D__TARGET_SSE41__)
ENDIF()

IF (TARGET_SSE42)
 ADD_DEFINITIONS(-D__TARGET_SSE42__)
ENDIF()

IF (TARGET_AVX)
 ADD_DEFINITIONS(-D__TARGET_AVX__)
ENDIF()

IF (TARGET_AVX2)
 ADD_DEFINITIONS(-D__TARGET_AVX2__)
ENDIF()

IF (TASKING_INTERNAL)
  ADD_DEFINITIONS(-DTASKING_TBB_INTERNAL)
ENDIF()

IF (TASKING_TBB)
  ADD_DEFINITIONS(-DTASKING_TBB)
  SET(EMBREE_LIBRARIES ${EMBREE_LIBRARIES} ${TBB_LIBRARIES})
ENDIF()

ADD_ISPC_LIBRARY(embree ${EMBREE_LIB_TYPE}

  embree.rc

  ../../common/tasking/taskscheduler_tbb.cpp
  ../../common/tasking/tasksys.cpp

  ../algorithms/parallel_for.cpp
  ../algorithms/parallel_reduce.cpp
  ../algorithms/parallel_prefix_sum.cpp
  ../algorithms/parallel_for_for.cpp
  ../algorithms/parallel_for_for_prefix_sum.cpp
  ../algorithms/sort.cpp
  ../algorithms/pset.cpp
  ../algorithms/pmap.cpp
  ../algorithms/prefix.cpp

  ../common/stat.cpp
  ../common/globals.cpp
  ../common/acceln.cpp
  ../common/accelset.cpp
  ../common/state.cpp
  ../common/rtcore.cpp
  ../common/rtcore_ispc.cpp
  ../common/rtcore_ispc.ispc
  ../common/buffer.cpp
  ../common/scene.cpp
  ../common/geometry.cpp
  ../common/scene_user_geometry.cpp
  ../common/scene_instance.cpp
  ../common/scene_triangle_mesh.cpp
  ../common/scene_bezier_curves.cpp
  ../common/scene_subdiv_mesh.cpp
  ../common/raystream_log.cpp
  ../common/subdiv/tessellation_cache.cpp
  ../common/subdiv/subdivpatch1base.cpp

  geometry/primitive.cpp
  geometry/instance_intersector1.cpp
  geometry/instance_intersector4.cpp
  geometry/subdivpatch1_intersector1.cpp
  geometry/subdivpatch1cached_intersector1.cpp		
  builders/primrefgen.cpp

  bvh4/bvh4.cpp
  bvh4/bvh4_statistics.cpp
  bvh4/bvh4_rotate.cpp
  bvh4/bvh4_refit.cpp
  bvh4/bvh4_builder_hair.cpp
  bvh4/bvh4_builder_morton.cpp
  bvh4/bvh4_builder_sah.cpp
  bvh4/bvh4_builder_twolevel.cpp
  bvh4/bvh4_builder_subdiv.cpp

  bvh4/bvh4_intersector1.cpp
  bvh4/bvh4_intersector4_single.cpp
  bvh4/bvh4_intersector4_chunk.cpp
)
SET_PROPERTY(TARGET embree PROPERTY FOLDER kernels)

IF (TARGET_SSE41)
  ADD_LIBRARY(embree_sse41 STATIC

    geometry/subdivpatch1_intersector1.cpp
    geometry/subdivpatch1cached_intersector1.cpp

    bvh4/bvh4_intersector1.cpp
    bvh4/bvh4_intersector4_single.cpp
    bvh4/bvh4_intersector4_chunk.cpp
  )
  SET_TARGET_PROPERTIES(embree_sse41 PROPERTIES COMPILE_FLAGS "${FLAGS_SSE41}")
  SET_PROPERTY(TARGET embree_sse41 PROPERTY FOLDER kernels)
  SET(EMBREE_LIBRARIES ${EMBREE_LIBRARIES} embree_sse41)
ENDIF ()

IF (TARGET_SSE42)
  ADD_LIBRARY(embree_sse42 STATIC
    bvh4/bvh4_intersector4_hybrid.cpp
  )
  SET_TARGET_PROPERTIES(embree_sse42 PROPERTIES COMPILE_FLAGS "${FLAGS_SSE42}")
  SET_PROPERTY(TARGET embree_sse42 PROPERTY FOLDER kernels)
  SET(EMBREE_LIBRARIES ${EMBREE_LIBRARIES} embree_sse42)
ENDIF ()

IF (TARGET_AVX)
  ADD_LIBRARY(embree_avx STATIC

    geometry/primitive.cpp
    geometry/instance_intersector1.cpp
    geometry/instance_intersector4.cpp
    geometry/instance_intersector8.cpp
    geometry/subdivpatch1_intersector1.cpp
    geometry/subdivpatch1cached_intersector1.cpp
    builders/primrefgen.avx.cpp

    bvh4/bvh4_rotate.cpp
    bvh4/bvh4_refit.avx.cpp
    bvh4/bvh4_builder_hair.avx.cpp
    bvh4/bvh4_builder_morton.avx.cpp
    bvh4/bvh4_builder_sah.avx.cpp
    bvh4/bvh4_builder_twolevel.avx.cpp
    bvh4/bvh4_builder_subdiv.avx.cpp

    bvh4/bvh4_intersector1.cpp
    bvh4/bvh4_intersector4_single.cpp
    bvh4/bvh4_intersector4_chunk.cpp
    bvh4/bvh4_intersector4_hybrid.cpp
    bvh4/bvh4_intersector8_single.cpp
    bvh4/bvh4_intersector8_chunk.cpp
    bvh4/bvh4_intersector8_hybrid.cpp

    bvh8/bvh8.cpp
    bvh8/bvh8_statistics.cpp
    bvh8/bvh8_builder_sah.avx.cpp

    bvh8/bvh8_intersector1.cpp
    bvh8/bvh8_intersector4_hybrid.cpp
    bvh8/bvh8_intersector8_chunk.cpp
    bvh8/bvh8_intersector8_hybrid.cpp
  )

  SET_TARGET_PROPERTIES(embree_avx PROPERTIES COMPILE_FLAGS "${FLAGS_AVX}")
  SET_PROPERTY(TARGET embree_avx PROPERTY FOLDER kernels)
  SET(EMBREE_LIBRARIES ${EMBREE_LIBRARIES} embree_avx)
 ENDIF()

IF (TARGET_AVX2)
  ADD_LIBRARY(embree_avx2 STATIC
    geometry/instance_intersector1.cpp
    geometry/instance_intersector4.cpp
    geometry/instance_intersector8.cpp
    geometry/subdivpatch1_intersector1.cpp
    geometry/subdivpatch1cached_intersector1.cpp

    bvh4/bvh4_intersector1.cpp
    bvh4/bvh4_intersector4_single.cpp
    bvh4/bvh4_intersector4_chunk.cpp
    bvh4/bvh4_intersector4_hybrid.cpp
    bvh4/bvh4_intersector8_single.cpp
    bvh4/bvh4_intersector8_chunk.cpp
    bvh4/bvh4_intersector8_hybrid.cpp

    bvh8/bvh8_intersector1.cpp
    bvh8/bvh8_intersector4_hybrid.cpp
    bvh8/bvh8_intersector8_chunk.cpp
    bvh8/bvh8_intersector8_hybrid.cpp
  )

  SET_TARGET_PROPERTIES(embree_avx2 PROPERTIES COMPILE_FLAGS "${FLAGS_AVX2}")
  SET_PROPERTY(TARGET embree_avx2 PROPERTY FOLDER kernels)
  SET(EMBREE_LIBRARIES ${EMBREE_LIBRARIES} embree_avx2)
ENDIF()

IF (TARGET_AVX512)
  ADD_LIBRARY(embree_avx512 STATIC

    geometry/instance_intersector1.cpp
    geometry/instance_intersector4.cpp
    geometry/instance_intersector8.cpp

    bvh4/bvh4_intersector1.cpp
    bvh4/bvh4_intersector4_single.cpp
    bvh4/bvh4_intersector4_chunk.cpp
    bvh4/bvh4_intersector4_hybrid.cpp
    bvh4/bvh4_intersector8_single.cpp
    bvh4/bvh4_intersector8_chunk.cpp
    bvh4/bvh4_intersector8_hybrid.cpp

    bvh8/bvh8_intersector1.cpp
    bvh8/bvh8_intersector4_hybrid.cpp
    bvh8/bvh8_intersector8_chunk.cpp
    bvh8/bvh8_intersector8_hybrid.cpp
)
  SET_TARGET_PROPERTIES(embree_avx512 PROPERTIES COMPILE_FLAGS "${FLAGS_AVX512}")
  SET_PROPERTY(TARGET embree_avx512 PROPERTY FOLDER kernels)
  SET(EMBREE_LIBRARIES ${EMBREE_LIBRARIES} embree_avx512)
ENDIF()

TARGET_LINK_LIBRARIES(embree ${EMBREE_LIBRARIES} sys simd lexers)

IF (WIN32 AND ENABLE_ISPC_SUPPORT)
  LIST(GET ISPC_TARGETS -1 MAX_ISA)
  SET_TARGET_PROPERTIES(embree PROPERTIES LINK_FLAGS "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/embree_${MAX_ISA}.def")
ENDIF()

IF (ENABLE_INSTALLER)
  SET_TARGET_PROPERTIES(embree PROPERTIES VERSION ${EMBREE_VERSION} SOVERSION ${EMBREE_VERSION_MAJOR})
ELSE()
  SET_TARGET_PROPERTIES(embree PROPERTIES VERSION ${EMBREE_VERSION_MAJOR} SOVERSION ${EMBREE_VERSION_MAJOR})
ENDIF()

IF (WIN32)
  INSTALL(TARGETS embree DESTINATION lib COMPONENT libraries)
ELSE()
  INSTALL(TARGETS embree DESTINATION lib LIBRARY DESTINATION lib COMPONENT libraries NAMELINK_SKIP)
ENDIF()

IF (WIN32)
    INSTALL(TARGETS embree DESTINATION bin COMPONENT tutorials)
ENDIF()
