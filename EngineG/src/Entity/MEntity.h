#pragma once
namespace MG
{
	class MEntitySystem;
	class MEntity
	{
	public:
		MEntity();
		virtual ~MEntity();

		virtual void onCreate() {}

		void release();

	protected:
		size_t m_id = 0;
		MEntitySystem* m_entitySystem = nullptr;

		friend class MEntitySystem;
	};
}