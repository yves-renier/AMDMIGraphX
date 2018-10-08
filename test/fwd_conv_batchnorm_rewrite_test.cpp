#include <migraph/fwd_conv_batchnorm_rewrite.hpp>
#include <migraph/program.hpp>
#include <migraph/cpu/cpu_target.hpp>
#include <migraph/operators.hpp>
#include <migraph/instruction.hpp>
#include <test.hpp>
#include <migraph/verify.hpp>

void fwd_conv_batchnorm_rewrite_test() {
    std::vector<float> xdata =
    {0.26485917, 0.61703885, 0.32762103, 0.2503367 , 0.6552712 ,
     0.07947932, 0.95442678, 0.70892651, 0.890563  , 0.80808088,
     0.89540492, 0.52657048, 0.94614791, 0.64371508, 0.0971229 ,
     0.2475562 , 0.47405955, 0.85538928, 0.05428386, 0.993078  ,
     0.72771973, 0.18312255, 0.3091522 , 0.51396558, 0.35158192,
     0.2419852 , 0.83691474, 0.36355352, 0.04769134, 0.08312604,
     0.61804092, 0.0508887 , 0.30987137, 0.81307629, 0.16398955,
     0.69886166, 0.02415926, 0.60608918, 0.81907569, 0.13208211,
     0.48303735, 0.87533734, 0.92998813, 0.65553674, 0.73223327,
     0.99401001, 0.09850688, 0.76972609, 0.11118327, 0.04392097,
     0.39252306, 0.91129653, 0.89078693, 0.60571206, 0.98410397,
     0.15290698, 0.86992609, 0.7575111 , 0.80583525, 0.23649562,
     0.7478029 , 0.62888878, 0.39886601, 0.37066793, 0.72627947,
     0.8745595 , 0.13568234, 0.7413787 , 0.5039495 , 0.18945697,
     0.87046838, 0.63970494, 0.01124038, 0.27459063, 0.65745586,
     0.69182619, 0.80470603, 0.58039348, 0.36950583, 0.43634225,
     0.01694425, 0.14099377, 0.77015849, 0.35809292, 0.40547674,
     0.46538817, 0.65835358, 0.2266954 , 0.39057646, 0.64642207,
     0.84491134, 0.20998067, 0.41074121, 0.73055221, 0.26424874,
     0.10612507, 0.24478521, 0.24091282, 0.52536754, 0.57292341,
     0.82190903, 0.51858515, 0.17162996, 0.52048114, 0.96624787,
     0.17527163, 0.56384485, 0.91991603};
    std::vector<float> wdata = 
    {-1.12125056,  0.50228441,  1.12719446, -2.61705068, -0.2027315 ,
     -0.82199441,  0.05337102, -0.62146691, -2.40572931, -1.47175612,
      1.49654601, -1.07070376, -0.65908074, -0.28457694,  1.60046717,
      0.20677642, -1.51844486,  0.41203847, -0.01285751,  0.07948031,
     -0.91507006, -1.59481079, -0.12856238,  0.39970482, -1.89015158,
      0.66969754,  0.10312618};
    migraph::shape xs{migraph::shape::float_type, {1, 3, 6, 6}};
    migraph::shape ws{migraph::shape::float_type, {1, 3, 3, 3}};
    migraph::shape vars{migraph::shape::float_type, {1}};
    migraph::program p1;
    migraph::program p2;
    {
        auto x        = p1.add_literal(xs, xdata);
        auto w        = p1.add_literal(ws, wdata);
        auto conv     = p1.add_instruction(migraph::op::convolution{{0, 0}, {1, 1}, {1, 1}}, x, w);
        auto scale    = p1.add_literal(migraph::literal{vars, {3.0f}});
        auto bias     = p1.add_literal(migraph::literal{vars, {8.1f}});
        auto mean     = p1.add_literal(migraph::literal{vars, {4.0f}});
        auto variance = p1.add_literal(migraph::literal{vars, {37.11f}});
        p1.add_instruction(migraph::op::batch_norm_inference{}, conv, scale, bias, mean, variance);    
    }
    {
        auto x        = p2.add_literal(xs, xdata);
        auto w        = p2.add_literal(ws, wdata);
        auto conv     = p2.add_instruction(migraph::op::convolution{{0, 0}, {1, 1}, {1, 1}}, x, w);
        auto scale    = p2.add_literal(migraph::literal{vars, {3.0f}});
        auto bias     = p2.add_literal(migraph::literal{vars, {8.1f}});
        auto mean     = p2.add_literal(migraph::literal{vars, {4.0f}});
        auto variance = p2.add_literal(migraph::literal{vars, {37.11f}});
        p2.add_instruction(migraph::op::batch_norm_inference{}, conv, scale, bias, mean, variance);    
    }
    std::cout << p1 << std::endl;
    migraph::fwd_conv_batchnorm_rewrite opt;
    opt.apply(p2);
    p1.compile(migraph::cpu::cpu_target{});
    p2.compile(migraph::cpu::cpu_target{});
    
    auto result1 = p1.eval({});
    auto result2 = p2.eval({});

    std::vector<float> results_vector1;
    std::vector<float> results_vector2;
    result1.visit([&](auto output) { results_vector1.assign(output.begin(), output.end()); });
    result2.visit([&](auto output) { results_vector2.assign(output.begin(), output.end()); });
    EXPECT(migraph::verify_range(results_vector1, results_vector2));
}

int main()
{
    fwd_conv_batchnorm_rewrite_test();
    return 0;
}
