// IoCpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace IoC;

class A { public:
	A(void){;} ~A(void){;}

	static A* Create(){return new A();}
};

class B { public:
	B(void){;} ~B(void){;}

	static B* Create(A* a){return new B();}
};

class C { public:
	C(void){;} ~C(void){;}

	static C* Create(B* b, A* a){return new C();}
};

class E;
class D { public:
	D(void){;} ~D(void){;}

	static D* Create(E* e){return new D();}
};

class E { public:
	E(void){;} ~E(void){;}

	static E* Create(D* d){return new E();}
};

int _tmain(int argc, _TCHAR* argv[])
{
	IoContainer<A>::Register(A::Create);
	IoContainer<B>::Register(B::Create);
	IoContainer<C>::Register(C::Create);
	IoContainer<D>::Register(D::Create);
	IoContainer<E>::Register(E::Create);

	A* resolve1 = IoContainer<A>::Resolve();
	B* resolve2 = IoContainer<B>::Resolve();
	C* resolve3 = IoContainer<C>::Resolve();

	try
	{
		D* resolve4 = IoContainer<D>::Resolve();
	}
	catch (void* e)
	{
		// catch circular dependency exception
	}

	try
	{
		E* resolve5 = IoContainer<E>::Resolve();
	}
	catch (void* e)
	{
		// catch circular dependency exception
	}


	return 0;
}

