#pragma once
#include "opengl.h"
#include "renderer/ring_buffer.h"

namespace profiling {

    class SimpleProfiler final {

        double gpu_time_elapsed_;
        double cpu_time_elapsed_;
        QueryIdsRingBuffer<GPU_QUERY_IDS_COUNT, DEFAULT_RINGBUFFER_STRIDE> query_ids_ring_buffer_;

    public:
        SimpleProfiler();

        void begin_gpu_profiling_query();
        void end_gpu_profiling_query();
        // void begin_cpu_profiling_query();
        // void end_cpu_profiling_query();

        double gpu_elapsed_time() const;
        // double cpu_elapsed_time() const;
    };
} // namespace profiling