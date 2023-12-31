#pragma once

class delegate
{
public:
    delegate()
        : object_ptr(0)
        , stub_ptr(0)
    {}

    template <class T, void (T::* TMethod)(float)>
    static delegate from_method(T* object_ptr)
    {
        delegate d;
        d.object_ptr = object_ptr;
        d.stub_ptr = &method_stub<T, TMethod>; // #1
        return d;
    }

    void operator()(float a1) const
    {
        return (*stub_ptr)(object_ptr, a1);
    }

private:
    typedef void (*stub_type)(void* object_ptr, float);

    void* object_ptr;
    stub_type stub_ptr;

    template <class T, void (T::* TMethod)(float)>
    static void method_stub(void* object_ptr, float a1)
    {
        T* p = static_cast<T*>(object_ptr);
        return (p->*TMethod)(a1); // #2
    }
};