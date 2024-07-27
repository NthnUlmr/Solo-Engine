
#include <mutex>

template <typename T>
struct ThreadSafe
{
    std::mutex mut;
    T data;

    void operator=(const T& other)
    {
        mut.lock();
        data = other;
        mut.unlock();
    }

    T& operator&()
    {
        return data;
    }

    operator T() const 
    {
        return data;
    }
};