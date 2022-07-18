#pragma once
template<typename T>
class GetterSetter {
private:
    T& ref;
public:
    GetterSetter(T& _ref) : ref(_ref) {}

    T& get() const {
        return ref;
    }
    //присваивание lvalue
    template<typename R>
    GetterSetter<T>& operator= (R& rv) {
        ref = rv;
        return *this;
    }
    //присваивание const lvalue
    template<typename R>
    GetterSetter<T>& operator= (const R& rv) {
        ref = rv;
        return *this;
    }
    //присваивание lvalue
    template<typename R>
    GetterSetter<T>& operator= (R&& rv) {
        ref = rv;
        return *this;
    }
    //разыменование
    T* operator->() const {
        return &ref;
    }
    //когда тебе надо получиться значение напрямую
    operator T() {
        return ref;
    }
    //Си-шное приведение типов 
    template<typename R>
    operator R () {
        return (R)ref;
    }
};
//вывод значения хранимого внутри GetterSetter
template<typename T>
std::ostream& operator<< (std::ostream& os, const GetterSetter<T>& gs) {
    os << gs.get();
    return os;
}


/*
class A {
private:
    int _i = 1;
public:
    GetterSetter<int> i;

    A() : i(_i) {}
};

class ContainerOfA {
    A _a;
public:
    GetterSetter<A> a;

    ContainerOfA() : a(_a) {}
};
class B {
public:
    B() {
        A a;
        std::cout << a.i << "- prints ahead GetterSetter\n";
        a.i = 5;
        int Int = a.i;
        std::cout << Int << " - set other value and set GetterSetter to int\n";
        float fl = a.i; 
        std::cout << fl << " - float\n";

        ContainerOfA Coa; 
        std::cout << Coa.a->i << " -  обращение к именно ОБЪЕКТУ через GetterSetter";
    }
};*/