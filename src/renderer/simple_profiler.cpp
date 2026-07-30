#include "renderer/simple_profiler.h"
#include "spdlog/spdlog.h"

using namespace profiling;

SimpleProfiler::SimpleProfiler()
    : gpu_time_elapsed_(0.0)
{
}

void SimpleProfiler::begin_gpu_profiling_query()
{
    glBeginQuery(GL_TIME_ELAPSED, query_ids_ring_buffer_.next_id());
}

void SimpleProfiler::end_gpu_profiling_query()
{
    glEndQuery(GL_TIME_ELAPSED);

    GLint available = 0;
    glGetQueryObjectiv(query_ids_ring_buffer_.strided_id(), GL_QUERY_RESULT_AVAILABLE, &available);
    if (!available) {
        // if you receive available = false always, try to increase buffer size and stride. 
        // sometimes cpu is faster than gpu and reaches this code before gpu has finished query. 
        spdlog::error("Faield to get gpu profiling query result, query_id={}", query_ids_ring_buffer_.strided_id());
        return;
    }

    GLuint64 time_elapsed;
    glGetQueryObjectui64v(query_ids_ring_buffer_.strided_id(), GL_QUERY_RESULT, &time_elapsed);
    gpu_time_elapsed_ = time_elapsed / 1000000.0;

}

double SimpleProfiler::gpu_elapsed_time() const
{
    return gpu_time_elapsed_;
}
