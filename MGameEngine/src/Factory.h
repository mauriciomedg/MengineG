#pragma once

#ifndef FACTORY_H
#define FACTORY_H

#include <map>

namespace MG
{
	template < 
		class AbstractProduct,
		typename IdentifierType,
		typename ProductCreator = AbstractProduct* (*)()> // AbstractProduct* (*PointerFunction)()

	class Factory
	{
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

		AbstractProduct* createObject(const IdentifierType& id)
		{
			auto itt = m_association.find(id);
			
			if (itt != m_association.end())
			{
				return (itt->second)();
			}

			return nullptr; // TODO add policy to handle erros out of the Factory
		}
	};
}

#endif // FACTORY