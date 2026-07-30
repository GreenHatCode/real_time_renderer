#pragma once
#include "opengl.h"
#include <array>

namespace profiling
{
    inline constexpr size_t GPU_QUERY_IDS_COUNT = 5;
    inline constexpr size_t DEFAULT_RINGBUFFER_STRIDE = 3;

    template <size_t BufferSize, size_t Stride>
    class QueryIdsRingBuffer final {

        std::array<GLuint, BufferSize> query_ids_; // array of ids for ring buffer
        size_t current_value_idx_;

    public:
        // the buffer is not a copyable or movealbe object
        QueryIdsRingBuffer(const QueryIdsRingBuffer& rhs) = delete;
        QueryIdsRingBuffer(QueryIdsRingBuffer&& rhs) = delete;
        QueryIdsRingBuffer& operator=(const QueryIdsRingBuffer& rhs) = delete;
        QueryIdsRingBuffer& operator=(QueryIdsRingBuffer&& rhs) = delete;

        QueryIdsRingBuffer()
            : current_value_idx_(0)
        {
            static_assert(BufferSize > Stride);
            glGenQueries(static_cast<GLsizei>(BufferSize), query_ids_.data());
        }

        ~QueryIdsRingBuffer() {
            glDeleteQueries(static_cast<GLsizei>(BufferSize), query_ids_.data());
        }

        GLuint next_id() {
            GLuint id = query_ids_[current_value_idx_];
            current_value_idx_ = (current_value_idx_ + 1) % BufferSize;
            return id;
        }

        GLuint strided_id() {
            return query_ids_[(current_value_idx_ + Stride) % BufferSize];
        }
    
    };

} // namespace profiling
