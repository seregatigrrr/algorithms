#pragma once
#include "TVector.h"
#include <cassert>

namespace TestTVector {
    void TestPushBack() {
        {
            TVector<int> v{};
            assert(v.Size() == 0);
            v.PushBack(3);
            v.PushBack(4);
            TVector<int> res = {3, 4};
            assert(v.Size() == 2);
            assert(v == res);
        }
        {
            TVector<int> v = {1, 2, 3};
            v.PushBack(4);
            v.PushBack(5);
            TVector<int> res = {1, 2, 3, 4, 5};
            assert(v.Size() == 5);
            assert(v == res);
        }
        {
            TVector<int> v = {1, 2, 3};
            int i = 4;
            v.PushBack(i);
            i = 5;
            v.PushBack(i);
            TVector<int> res = {1, 2, 3, 4, 5};
            assert(v == res);
        }
    }

    void TestSwap() {
        {
            TVector<int> v1 = {1, 2, 3, 4}, res1(v1);
            TVector<int> v2 = {5, 6, 7, 8}, res2(v2);
            v1.Swap(v2);
            assert(v1 == res2 && v2 == res1);
        }
        {
            TVector<int> v1 = {1, 2, 3, 4}, res1(v1);
            TVector<int> v2 = {}, res2(v2);
            v1.Swap(v2);
            assert(v1 == res2 && v2 == res1);
            v1.Swap(v2);
            assert(v1 == res1 && v2 == res2);
        }
    }

    void TestClear() {
        {
            TVector<int> v = {};
            v.Clear();
            TVector<int> res = {};
            assert(v == res);
        }
        {
            TVector<int> v = {1, 2, 3};
            size_t cp = v.Capacity();
            v.Clear();
            TVector<int> res = {};
            assert(v == res);
            assert(v.Capacity() == cp);
        }
    }

    void TestCopy() { //копирование и присваивание
        {
            TVector<int> v1 = {1, 2, 3};
            TVector<int> v2 = v1;
            assert(v2 == v1);

            TVector<int> v3;
            v3 = v2;
            assert(v3 == v2);
            v2 = {2, 3, 4, 5};
            v2 = *new TVector<int>;
            TVector<int> res = {};
            assert(v2 == res);
        }
        {
            TVector<int> v1 = {1, 2, 3};
            TVector<int> v2 = {3, 4, 5, 6};
            v1 = v2;
            assert(v1 == v2);
            v2 = v1;
            assert(v2 == v1);
        }
    }

    void TestEmplaceBack() {
        {
            TVector<int> v = {};
            v.EmplaceBack(3);
            v.EmplaceBack(4);
            TVector<int> res = {3, 4};
            assert(v == res);
        }
        {
            TVector<int> v = {1, 2, 3};
            v.EmplaceBack(4);
            v.EmplaceBack(5);
            TVector<int> res = {1, 2, 3, 4, 5};
            assert(v == res);
        }
    }

    void TestResize() {
        TVector<int> v = {1, 2, 3, 4, 5};
        int res = v.Size() - 1;
        v.PopBack();
        assert(v.Size() == res);
        v.Resize(6);
        v.PushBack(7);
        TVector<int> res2 = {1, 2, 3, 4, 0, 0, 7};
        assert(v == res2);
        v.Resize(5);
        TVector<int> res3 = {1, 2, 3, 4, 0};
        assert(v == res3);
    }

    void TestAll() {
        TestPushBack();
        TestSwap();
        TestClear();
        TestCopy();
        TestEmplaceBack();
        TestResize();
    }
}