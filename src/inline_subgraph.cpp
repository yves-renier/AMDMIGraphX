#include <migraphx/inline_subgraph.hpp>
#include <migraphx/program.hpp>
#include <migraphx/instruction.hpp>
#include <migraphx/make_op.hpp>

#include <migraphx/iterator_for.hpp>

namespace migraphx {
inline namespace MIGRAPHX_INLINE_NS {

void inline_subgraph::apply(module& p) const
{
    for(auto ins : iterator_for(p))
    {
        if (ins->name() != "if")
            continue;
            
        shape s = ins->get_shape();
        if(not s.standard() and s.elements() != 0)
        {
            auto c = p.insert_instruction(std::next(ins), make_op("contiguous"), ins);
            p.replace_instruction(ins, c);
        }
    }
}

} // namespace MIGRAPHX_INLINE_NS
} // namespace migraphx
