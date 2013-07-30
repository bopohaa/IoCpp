#pragma once

#include <vector>

namespace IoC{

	using namespace std;

#define FACTORY_FUNC(name) T*(*name)(void)
#define FACTORY_FUNC1(name) T*(*name)(T1*)
#define FACTORY_FUNC2(name) T*(*name)(T1*, T2*)
#define FACTORY_RESOLVER_FUNC(name, type_name) type_name*(*name)(vector<IResolver*> &stack)


	template <class T>
	class IoContainer;

	namespace Resolvers
	{
		class IResolver
		{
		protected:
			IResolver(){;}
			~IResolver(){;}

			virtual void* Resolve(vector<IResolver*> &stack){return NULL;}

			template <class T> friend class IoC::IoContainer;

			bool Contains(vector<IResolver*> &resolvers)
			{
				for(vector<IResolver*>::iterator it = resolvers.begin();it!=resolvers.end();it++)
				{
					if(*it==this)
						return true;
				}
				return false;
				//return find(resolvers.begin(),resolvers.end(), this)!=resolvers.end();
			}
		};

		template<class T> class Resolver:IResolver
		{
		private:
			FACTORY_FUNC(_factory);
		protected:
			Resolver(FACTORY_FUNC(factory))
				:_factory(factory)
			{;}
			~Resolver(){;}

			virtual void* Resolve(vector<IResolver*> &stack)
			{
				return _factory();
			}

			template <class Tt> friend class IoC::IoContainer;
		};

		template<class T, class T1> class Resolver1:IResolver
		{
		private:
			FACTORY_FUNC1(_factory);
			FACTORY_RESOLVER_FUNC(_factory1, T1);
		protected:
			Resolver1(FACTORY_FUNC1(factory), FACTORY_RESOLVER_FUNC(factory1, T1))
				:_factory(factory), _factory1(factory1)
			{;}
			~Resolver1(){;}

			virtual void* Resolve(vector<IResolver*> &stack)
			{
				T1* instance1 = _factory1(stack);

				return _factory(instance1);
			}

			template <class Tt> friend class IoC::IoContainer;

		};

		template<class T, class T1, class T2> class Resolver2:IResolver
		{
		private:
			FACTORY_FUNC2(_factory);
			FACTORY_RESOLVER_FUNC(_factory1, T1);
			FACTORY_RESOLVER_FUNC(_factory2, T2);
		protected:
			Resolver2(FACTORY_FUNC2(factory), FACTORY_RESOLVER_FUNC(factory1, T1), FACTORY_RESOLVER_FUNC(factory2, T2))
				:_factory(factory), _factory1(factory1), _factory2(factory2)
			{;}
			~Resolver2(){;}

			virtual void* Resolve(vector<IResolver*> &stack)
			{
				T1* instance1 = _factory1(stack);
				T2* instance2 = _factory2(stack);

				return _factory(instance1,instance2);
			}

			template <class Tt> friend class IoC::IoContainer;

		};
	}

	using namespace Resolvers;

	template <class T>
	class IoContainer
	{
	private:
		IoContainer(void){;}
		~IoContainer(void){;}
	public:

		static void Register(FACTORY_FUNC(factory))
		{
			IResolver* resolver = (IResolver*) new Resolver<T>(factory);
			vector<IResolver*>* stack = NULL;

			Translate(false, resolver,*stack);
		}

		template<class T1>
		static void Register(FACTORY_FUNC1(factory))
		{
			IResolver* resolver = (IResolver*) new Resolver1<T,T1>(factory, 
				&IoContainer<T1>::_Resolve);
			vector<IResolver*>* stack = NULL;

			Translate(false, resolver, *stack);
		}

		template<class T1, class T2>
		static void Register(FACTORY_FUNC2(factory))
		{
			IResolver* resolver = (IResolver*) new Resolver2<T,T1,T2>(factory, 
				&IoContainer<T1>::_Resolve,
				&IoContainer<T2>::_Resolve);
			vector<IResolver*>* stack = NULL;

			Translate(false, resolver, *stack);
		}


		static T* Resolve()
		{
			vector<IResolver*> stack;

			return Translate(true, NULL, stack);
		}

		static T* _Resolve(vector<IResolver*> &stack)
		{
			return Translate(true, NULL, stack);
		}
	private:

		static T* Translate(bool resolve, IResolver* resolver, vector<IResolver*>& stack)
		{
			static IResolver* _resolver = NULL;

			if(resolve)
			{
				if(_resolver->Contains(stack)) 
					throw "circular dependency";

				stack.push_back(_resolver);
				T* rval = (T*)_resolver->Resolve(stack);
				stack.pop_back();

				return rval;
			}
			else
				_resolver = resolver;

			return NULL;
		}
	};
}