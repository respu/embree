// ======================================================================== //
// Copyright 2009-2014 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#pragma once

#include "../kernels/common/default.h"
#include "embree2/rtcore_ray.h"
#include <iostream>
#include <fstream>
#include <pthread.h>

namespace embree
{
  class RayStreamLogger
  {
  private:

    pthread_mutex_t mutex;

    bool initialized;
    bool active;

    std::ofstream rayData;
    std::ofstream rayDataVerify;

    void openRayDataStream();

  public:

    enum { 
      RAY_INTERSECT = 0,
      RAY_OCCLUDED  = 1
    };

    RayStreamLogger();
    ~RayStreamLogger();

    struct __aligned(64) LogRay16  {
      unsigned int type;
      unsigned int m_valid;
      unsigned int dummy[14];
      RTCRay16 ray16;

      LogRay16() {
	memset(this,0,sizeof(LogRay16));
      }

      __forceinline void prefetchL2()
      {
#if defined(__MIC__)
	prefetch<PFHINT_L2>(&type);
	const size_t cl = sizeof(RTCRay16) / 64;
	const char *__restrict__ ptr = (char*)&ray16;
#pragma unroll(cl)
	for (size_t i=0;i<cl;i++,ptr+=64)
	  prefetch<PFHINT_L2>(ptr);
#endif
      }

      __forceinline void prefetchL1()
      {
#if defined(__MIC__)
	prefetch<PFHINT_NT>(&type);
	const size_t cl = sizeof(RTCRay16) / 64;
	const char *__restrict__ ptr = (char*)&ray16;
#pragma unroll(cl)
	for (size_t i=0;i<cl;i++,ptr+=64)
	  prefetch<PFHINT_NT>(ptr);
#endif
      }

      __forceinline void evict()
      {
#if defined(__MIC__)
	evictL2(&type);
	const size_t cl = sizeof(RTCRay16) / 64;
	const char *__restrict__ ptr = (char*)&ray16;
#pragma unroll(cl)
	for (size_t i=0;i<cl;i++,ptr+=64)
	  evictL2(ptr);
#endif
      }
    };

      
  static RayStreamLogger rayStreamLogger;

  void logRay16Intersect(const void* valid, void* scene, RTCRay16& start, RTCRay16& end);
  void logRay16Occluded (const void* valid, void* scene, RTCRay16& start, RTCRay16& end);
  void dumpGeometry(void* scene);

  __forceinline void deactivate() { active = false; };
  __forceinline bool isActive() { return active; };

  };
};