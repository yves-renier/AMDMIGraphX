#include <migraphx/serialize.hpp>
#include <migraphx/functional.hpp>
#include <test.hpp>

struct reflectable_type
{
    std::vector<std::size_t> ints = {};
    std::string name              = "";
    float fvalue                  = 0.0;

    struct nested_type
    {
        int value;
        template <class Self, class F>
        static auto reflect(Self& self, F f)
        {
            return migraphx::pack(f(self.value, "value"));
        }
    };
    std::vector<nested_type> nested_types = {};

    template <class Self, class F>
    static auto reflect(Self& self, F f)
    {
        return migraphx::pack(f(self.ints, "ints"),
                              f(self.name, "name"),
                              f(self.fvalue, "fvalue"),
                              f(self.nested_types, "nested_types"));
    }
};

TEST_CASE(serialize1)
{
    reflectable_type t1{{1, 2}, "hello", 1.0, {{1}, {2}}};
    migraphx::value v1  = migraphx::to_value(t1);
    reflectable_type t2 = migraphx::from_value<reflectable_type>(v1);
    migraphx::value v2  = migraphx::to_value(t2);
    migraphx::value v3  = migraphx::to_value(reflectable_type{});

    EXPECT(v1 == v2);
    EXPECT(v1 != v3);
    EXPECT(v2 != v3);
}

int main(int argc, const char* argv[]) { test::run(argc, argv); }
