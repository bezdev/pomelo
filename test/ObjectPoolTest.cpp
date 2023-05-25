#include "Test.h"

#include "ObjectPool.h"
#include "logger.h"

struct StructA
{
    char name[10];
    int id;
};

class ClassA
{
public:
    ClassA() {};
    ClassA(const char* name, int id)
    {
        name = name;
        id = id;
    };
    char name[10];
    int id;
};

TEST(BasicObjectPoolClassTest)
{
    const std::size_t N = 10;
    ObjectPool<ClassA, N> op;
    op.Create();
    ASSERT_ARE_EQUAL(op.Count(), 1);

    for (std::size_t i = 1; i < N; ++i)
    {
        op.Create("line", i);
    }

    ASSERT_ARE_EQUAL(op.Count(), 10);

    ASSERT_THROWS(op.Create());

    LOGT("sizeof(ClassA): %d", sizeof(ClassA));
    LOGT("sizeof(op): %d", sizeof(op));
}

TEST(BasicObjectPoolStructTest)
{
    const std::size_t N = 10;
    ObjectPool<StructA, N> op;
    op.Create();
    ASSERT_ARE_EQUAL(op.Count(), 1);

    for (std::size_t i = 1; i < N; ++i)
    {
        op.Create();
    }

    ASSERT_ARE_EQUAL(op.Count(), 10);

    ASSERT_THROWS(op.Create());
}