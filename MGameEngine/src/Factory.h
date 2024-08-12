#pragma once

#ifndef FACTORY_H
#define FACTORY_H

#include <map>
#include <functional>

namespace MG
{
	class NullType {};
	struct EmptyType {};
	
	template< class T, class U >
	struct TypeList
	{
		typedef T Head;
		typedef U Tail;
	};

#define TYPELIST_1(T1) TypeList<T1, NullType>

	template <class TList, unsigned int index> struct TypeAt;

	template <class Head, class Tail>
	struct TypeAt< TypeList<Head, Tail>, 0>
	{
		typedef Head Result;
	};

	template <class Head, class Tail, unsigned int i>
	struct TypeAt< TypeList<Head, Tail>, i>
	{
		typedef typename TypeAt<Tail, i - 1>::Result Result;
	};

	template < 
		class AbstractProduct,
		class TList,
		typename IdentifierType,
		typename ProductCreator> // AbstractProduct* (*PointerFunction)()

	class Factory
	{
		typedef typename TypeAt<TList, 0>::Result
			Parm1;

		typedef std::map<IdentifierType, ProductCreator> AssocMap;
		AssocMap m_association;

		Factory() {};
		Factory(const Factory&);
		Factory& operator =(const Factory&);
	public:

		static Factory& getInstance()
		{
			static Factory obj;
			return obj;
		}

		bool registerProduct(const IdentifierType& id, ProductCreator creator)
		{
			return m_association.insert(AssocMap::value_type(id, creator)).second;
		}

		bool unregisterProduct(const IdentifierType& id)
		{
			return m_association.erase(id) == 1; // number of elements erased
		}

		AbstractProduct* createObject(const IdentifierType& id, Parm1 p1)
		{
			auto itt = m_association.find(id);
			
			if (itt != m_association.end())
			{
				return (itt->second)(p1);
			}

			return nullptr; // TODO add policy to handle erros out of the Factory
		}
	};
}

#endif // FACTORY