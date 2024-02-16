#pragma once
namespace MG
{
	class MEntity
	{
	public:
		MEntity();
		virtual ~MEntity();

		virtual void onCreate() {}
	};
}