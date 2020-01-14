#ifndef MIGRAPHX_GUARD_RTGLIB_DEVICE_REDUCE_SUM_SQUARE_HPP
#define MIGRAPHX_GUARD_RTGLIB_DEVICE_REDUCE_SUM_SQUARE_HPP

#include <migraphx/argument.hpp>
#include <migraphx/config.hpp>
#include <hip/hip_runtime_api.h>

namespace migraphx {
inline namespace MIGRAPHX_INLINE_NS {
namespace gpu {
namespace device {

void reduce_sum_square(hipStream_t stream, const argument& result, const argument& arg);

} // namespace device
} // namespace gpu
} // namespace MIGRAPHX_INLINE_NS
} // namespace migraphx

#endif
