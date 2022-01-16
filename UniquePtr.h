//
// Created by slonik on 1/16/22.
//

#ifndef SMARTPOINTERS_UNIQUEPTR_H
#define SMARTPOINTERS_UNIQUEPTR_H

// Умный указатель с уникальный владением

// Сделать свой шаблонный класс умного указателя unique_ptr.
// Требования:
//    Реализация должна быть максмально оптимальной как по времени работы основных операций,
//    так и по памяти, т.е. не должно храниться лишней информации или делаться лишних обращений.

// Усложнение 1. Реализовать аналог make_unique (опционально).
// Усложнение 2. Сделать возможным задавать пользовательский deleter (опционально).
// Усложнение 3. Реализовать работу с T[] (опционально).

#include "DefaultDelete.h"

// Умный указатель с уникальным владением для одного элемента
template <typename T, class Deleter = DefaultDelete<T>>
class UniquePtr {
protected:
    T *pT;
    Deleter deleter;
public:
    UniquePtr():
            pT(nullptr) {}

    explicit UniquePtr(T * obj):
            pT(obj) {}

    UniquePtr(T* obj, Deleter d):
            pT(obj),
            deleter(d) {}

    UniquePtr(UniquePtr &ptr) = delete;

    UniquePtr(UniquePtr &&ptr):
            pT(ptr.pT) {
        ptr.pT = nullptr;
    }

    virtual ~UniquePtr() {
        deleter(pT);
    }

    T& operator*() {
        return *pT;
    }

    T* operator->() {
        return pT;
    }

    explicit operator bool() const noexcept {
        return pT != nullptr;
    }

    T* get() {
        return pT;
    }
};

// умный указатель с уникальным владением для для массива
template <typename T, class Deleter>
class UniquePtr<T[], Deleter> {
protected:
    T *pT;
    Deleter deleter;
public:
    UniquePtr():
            pT(nullptr) {}

    explicit UniquePtr(T * obj):
            pT(obj) {}

    UniquePtr(T* obj, Deleter d):
            pT(obj),
            deleter(d) {}

    UniquePtr(UniquePtr &ptr) = delete;

    UniquePtr(UniquePtr &&ptr):
            pT(ptr.pT) {
        ptr.pT = nullptr;
    }

    virtual ~UniquePtr() {
        deleter(pT);
    }

    T& operator[](std::size_t i) {
        return pT[i];
    }

    explicit operator bool() const noexcept {
        return pT != nullptr;
    }

    T* get() {
        return pT;
    }
};

template <typename T, typename ...Args>
UniquePtr<T> MakeUnique(Args&&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

#endif //SMARTPOINTERS_UNIQUEPTR_H
