#include "Test.h"

#include <vector>

#include "logger.h"

class A
{
public:
    int ID;
    A(int id): ID(id) { LOGT("Constructor"); }
    A(const A& other): ID(other.ID) { LOGT("Copy Constructor"); }
    A(A&& other) noexcept: ID(other.ID) { LOGT("Move Constructor"); }
    A& operator=(const A& other) {
        LOGT("Copy operator");
        if (this != &other) {
            ID = other.ID;
        }
        return *this;
    }
};

class ACollection
{
public:
    std::vector<A>& GetCollection() { return Collection; }
    std::vector<A> Collection;
};

void PrintAddress(const std::vector<A>& collection)
{
    for (const A& a : collection)
    {
        printf("Address: %p\n", &a);
    }
}

TEST(CollectionTest)
{
    ACollection ac;
    ac.Collection = { A(1), A(2), A(3) };

    LOGT("A size: %d", sizeof(A));

    for (const A& a : ac.Collection)
    {
        LOGT("Address: %p", &a);
    }

    PrintAddress(ac.Collection);

    PrintAddress(ac.GetCollection());
}

TEST(PointerTest)
{
    int a = 5;
    int b = 4;
    int c = 3;
    int d = 2;
    int e = 1;

    const int* last = nullptr;
    std::vector<const int*> v = { &a, &b, &c, &d, &e };
    for (const int* p : v)
    {
        if (last != nullptr) LOGT("last: %p", last);

        LOGT("p: %p", p);
        last = p;
    }
}