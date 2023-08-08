#include "Test.h"

#include <iostream>
#include <functional>
#include <unordered_map>
#include <vector>

#include "util/Logger.h"
#include "util/Timer.h"
#include "util/Util.h"

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
    void AddCollection(A item) { printf("AddCollection: %p\n", &item); Collection.push_back(std::move(item)); }
    std::vector<A> Collection;
};

void PrintAddress(const std::vector<A>& collection)
{
    for (const A& a : collection)
    {
        printf("Address: %p\n", &a);
    }
}

namespace A1
{
    enum class DerivedID
    {
        DERIVED_1,
        DERIVED_11,
        DERIVED_12,
        DERIVED_13,
        DERIVED_14,
        DERIVED_15,
        DERIVED_16,
        DERIVED_2,
        COUNT
    };

    // Base class template
    template <typename Derived>
    struct Base {
        void foo() { static_cast<Derived*>(this)->fooImpl(); }
        int count = 0;
    };

    // Derived classes
    struct Derived1 : public Base<Derived1> {
        void fooImpl() { count++; } //std::cout << "Derived1\n"; }
    };

    struct Derived2 : public Base<Derived2> {
        void fooImpl() { count++; } //std::cout << "Derived2\n"; }
    };

    // Our functor that wraps member function call
    struct Functor {
        Functor() {};
        template <typename T>
        Functor(T* obj) : call([obj](){ obj->foo(); }) {}

        void operator()() { call(); }

    private:
        std::function<void()> call;
    };

    struct ShaderManager
    {
        static Derived1* d1;
        static Derived2* d2;
    };
    // template<size_t... I, typename Tuple>
    // void callFoo(std::index_sequence<I...>, Tuple& objects, DerivedID id) {
    //     using call_func_t = void (Base<Derived1>::*)();
    //     static const call_func_t funcs[] = { &std::get<I>(objects).foo... };
    //     (std::get<static_cast<size_t>(id)>(objects).*funcs[static_cast<size_t>(id)])();
    // } 
    // void callFoo(std::index_sequence<I...>, Tuple& objects, DerivedID id) {
    // using call_func_t = void (Base<Derived1>::*)();
    // static const call_func_t funcs[] = { &Base<std::tuple_element_t<I, Tuple>>::foo... };
    // ((std::get<static_cast<size_t>(id)>(objects)).*funcs[static_cast<size_t>(id)])();
}

A1::Derived1* A1::ShaderManager::d1 = new A1::Derived1();
A1::Derived2* A1::ShaderManager::d2 = new A1::Derived2();
#define CALL_MEMBER_FUNC(id, func) \
    if (id == A1::DerivedID::DERIVED_1) A1::ShaderManager::d1->func(); \
    else if (id == A1::DerivedID::DERIVED_11) A1::ShaderManager::d1->func(); \
    else if (id == A1::DerivedID::DERIVED_12) A1::ShaderManager::d1->func(); \
    else if (id == A1::DerivedID::DERIVED_13) A1::ShaderManager::d1->func(); \
    else if (id == A1::DerivedID::DERIVED_14) A1::ShaderManager::d1->func(); \
    else if (id == A1::DerivedID::DERIVED_15) A1::ShaderManager::d1->func(); \
    else if (id == A1::DerivedID::DERIVED_16) A1::ShaderManager::d1->func(); \
    else if (id == A1::DerivedID::DERIVED_2) A1::ShaderManager::d2->func();

namespace A2
{
    class Base {
    public:
        virtual ~Base() {}
        virtual void foo() = 0;
        int count = 0;
    };

    class Derived1 : public Base {
    public:
        void foo() override {
            count++; //std::cout << "Derived1\n";
        }
    };

    class Derived2 : public Base {
    public:
        void foo() override {
            count++; //std::cout << "Derived2\n";
        }
    };
}

TEST(CollectionTest1)
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

TEST(CollectionTest2)
{
    ACollection ac;
    ac.Collection.reserve(5);
    ac.AddCollection(A(1));
    ac.AddCollection(A(2));
    ac.AddCollection(A(3));
    LOGT("A size: %d", sizeof(A));

    for (const A& a : ac.Collection)
    {
        LOGT("Address: %p", &a);
    }

    // PrintAddress(ac.Collection);
    // PrintAddress(ac.GetCollection());
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

TEST(FunctionCalls)
{
    // std::unordered_map<int, std::unique_ptr<A2::Base>> objects;
    // objects[1] = std::make_unique<A2::Derived1>();
    // objects[2] = std::make_unique<A2::Derived2>();
    std::vector<std::unique_ptr<A2::Base>> objects;
    objects.push_back(std::make_unique<A2::Derived1>());
    objects.push_back(std::make_unique<A2::Derived2>());
    {
        ScopeTimer t("Virtual");
        for (int i = 0; i < 100000; i++)
        {
            objects[1]->foo();
        }
    }


    A1::Derived1 d1;
    A1::Derived2 d2;

    // std::unordered_map<int, A1::Functor> funcs;
    // funcs[1] = A1::Functor(&d1);
    // funcs[2] = A1::Functor(&d2);

    // auto f1 = A1::Base<A1::Derived1>();
    {
        ScopeTimer t("Functor");
        for (int i = 0; i < 100000; i++)
        {
            // funcs[1]();
            // funcs[2]();
            d2.foo();
        }
    }
    
    {
        std::tuple<A1::Derived1, A1::Derived2> objects;

        ScopeTimer t("Functor");
        int i = 6;
        for (int i = 0; i < 100000; i++)
        {
            // funcs[1]();
            // funcs[2]();
            // if (i == 1) d1.foo();
            // else if (i == 2) d1.foo();
            // else if (i == 3) d1.foo();
            // else if (i == 4) d1.foo();
            // else if (i == 5) d1.foo();
            // else if (i == 6) d2.foo();
            // A1::callFoo(std::make_index_sequence<static_cast<size_t>(A1::DerivedID::COUNT)>(), objects, A1::DerivedID::DERIVED_2);
            CALL_MEMBER_FUNC(A1::DerivedID::DERIVED_2, foo)
            
        }
    }
}

TEST(AssignmentOperator)
{
    A a(1);
}
