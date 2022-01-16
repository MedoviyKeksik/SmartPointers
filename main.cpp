#include <iostream>
#include <functional>
#include "UniquePtr.h"
#include "SharedPtr.h"

template <typename T>
void test(T value, T expected, std::string message) {
    std::cout << message << ":      ";
    if (value == expected)  {
        std::cout << "Ok... Test passed\n";
    } else {
        std::cerr << "Error. Expected: " << expected << ", but " << value << " found.\n";
    }
}

int main() {
    std::cout << "Testing UniquePtr...\n";
    UniquePtr<int> ptr = MakeUnique<int>(42);
    test(*ptr, 42, "Create ptr to number 42");
    UniquePtr<int> another(std::move(ptr));
    test((bool) ptr, false, "Check if after move old ptr is false");
    test(*another, 42, "Check if in new ptr number is 42");
    *another = 33;
    test(*another, 33, "Try to put another number");

    bool deleted = false;
    {
        UniquePtr<float, std::function<void(float*)>> withDeleter(new float(14.3), [&](float * ptr) {
            delete ptr;
            deleted = true;
        });
    }
    test(deleted, true, "Check if custom deleter executes");

    UniquePtr<long[], std::function<void(long*)>> array(new long[10], [](long *ptr){
        delete []ptr;
    });
    for (int i = 0; i < 10; i++) {
        array[i] = i * i;
    }
    test(array[9], 81L, "Test array ptr");

    std::cout << "\nTesting SharedPtr...\n";
    SharedPtr<int> intPtr = MakeShared<int>(42);
    test(*intPtr, 42, "Create ptr to number 42");
    {
        SharedPtr<int> copy = intPtr;
        test(*copy, 42, "Copy ptr and check number");
        test(copy.getRefCount(), 2UL, "Check ref count");

        SharedPtr<int> other(std::move(copy));
        test(*other, 42, "Move ptr and check number");
        test(other.getRefCount(), 2UL, "Check ref count in new ptr after move");
        test(copy.getRefCount(), 0UL, "Check ref count in old ptr after move");
        test(copy.get(), (int*) nullptr, "Check if in old ptr reference is null");
    }
    test(intPtr.getRefCount(), 1UL, "Check ref count after exit from scope");
    deleted = false;
    {
        SharedPtr<int[], std::function<void(int *)>> intArray(new int[2]{21, 21}, [&](int *ptr) {
            delete[]ptr;
            deleted = true;
        });
        test(intArray[0] + intArray[1], 42, "Check array ptr");
        test(intArray.getRefCount(), 1UL, "Check array ptr ref count");
    }
    test(deleted, true, "Check custom deleter");
    return 0;
}
