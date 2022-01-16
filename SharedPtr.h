//
// Created by slonik on 1/16/22.
//

#ifndef SMARTPOINTERS_SHAREDPTR_H
#define SMARTPOINTERS_SHAREDPTR_H

// Умный указатель с общим владением

// Сделать свой шаблонный класс умного указателя shared_ptr.
// Требования:
//    Реализация должна быть максмально оптимальной как по времени работы основных операций, так и по памяти,
//    т.е .не должно храниться лишней информации или делаться лишних обращений.

// Усложнение 1. Реализовать аналог make_shared (опционально).
// Усложнение 2. Сделать возможным задавать пользовательский deleter (опционально).
// Усложнение 3. Реализовать работу с T[] (опционально).

#include "DefaultDelete.h"

template <typename T, class Deleter = DefaultDelete<T>>
class SharedPtr {
protected:
    typedef std::remove_extent_t<T> ET;
    unsigned long* pRefCount;
    ET* pT;
    Deleter deleter;
public:
    SharedPtr():
        pT(nullptr),
        pRefCount(new unsigned long(0L)) {}

    explicit SharedPtr(ET *obj):
        pT(obj),
        pRefCount(new unsigned long(1L)) {}

    SharedPtr(ET *obj, Deleter d):
        pT(obj),
        pRefCount(new unsigned long(1L)),
        deleter(d) {}

    SharedPtr(SharedPtr &ptr):
        pT(ptr.pT),
        pRefCount(ptr.pRefCount),
        deleter(ptr.deleter) {
        (*pRefCount)++;
    }

    SharedPtr(SharedPtr &&ptr):
        pT(ptr.pT),
        pRefCount(ptr.pRefCount),
        deleter(ptr.deleter) {
        ptr.pT = nullptr;
        ptr.pRefCount = new unsigned long(0L);
    }

    ~SharedPtr() {
        (*pRefCount)--;
        if (*pRefCount == 0) deleter(pT);
    }

    ET& operator*() {
        return *pT;
    }

    ET* operator->() {
        return pT;
    }

    explicit operator bool() const noexcept {
        return pT != nullptr;
    }

    ET* get() {
        return pT;
    }

    ET& operator[](std::size_t i) {
        return pT[i];
    }

    unsigned long getRefCount() {
        return *pRefCount;
    }
};

template <class T, class... Args>
SharedPtr<T> MakeShared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

#endif //SMARTPOINTERS_SHAREDPTR_H
