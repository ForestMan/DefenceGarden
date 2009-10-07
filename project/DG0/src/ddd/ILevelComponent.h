#pragma once 

namespace ddd
{
	class ILevelComponent
	{
	public:
		ILevelComponent();
		virtual ~ILevelComponent();

		void create();
		void init();
		void release();

		inline const bool isCreate()const;
		inline const bool isInit()const; 

	private:
		
		virtual void onCreate() = 0;
		virtual void onInit() = 0;
		virtual void onRelease() = 0;
	
		inline void setCreate( const bool create );
		inline void setInit( const bool init );
	
	private:
	
		bool create_;
		bool init_;
	};
	
	//-------------------------------------------------------------------------
	
	inline const bool ILevelComponent::isCreate()const
	{
		return create_;
	}

	//-------------------------------------------------------------------------

	inline const bool ILevelComponent::isInit()const
	{
		return init_;
	}

	//-------------------------------------------------------------------------
	
	inline void ILevelComponent::setCreate( const bool create )
	{
		create_ = create;
	}

	//-------------------------------------------------------------------------

	inline void ILevelComponent::setInit( const bool init )
	{
		init_ = init;
	}

	//-------------------------------------------------------------------------
}