IoCpp
=====

Very simple static IoC for c++

Usage
---------

    // Declare
    class B { public:  B(void){;} ~B(void){;}
      static B* Create(){return new B();}
    };
    class A { public:  A(void){;} ~A(void){;}
      static A* Create(B* b){return new A();}
    };
    ...
    // Register
    IoContainer<A>::Register(A::Create)
    ...
    // Resolve
    A* resolve1 = IoContainer<A>::Resolve();
