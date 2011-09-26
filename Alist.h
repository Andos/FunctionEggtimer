#ifndef ANDOSALIST
#define ANDOSALIST
template <typename T> class AList
{

public:
	
	template <typename T> struct _AListStruct
	{
		T Value;
		_AListStruct<T> * ListPrevious;
		_AListStruct<T> * ListNext;
	};
	typedef _AListStruct<T> AListStruct;

	AList()
	{
		ListStart = 0;
		ListEnd = 0;
		ListSize = 0;
	};

	~AList()
	{
		Clear();
	};
	
	bool	InitializeList()
	{
		AList();
		return true;
	}

	inline T operator [](long index)
	{
		 return GetItem(index);
	}

	bool	Add( T value )
			{
				AListStruct * ListItem = new AListStruct;
				if(ListItem == 0)
					return false;

				ListItem->Value = value;				//Store the value into the struct
				ListItem->ListNext = 0;					//This is the end of the tail
				ListItem->ListPrevious = ListEnd;		//The old last element now becomes the new previous object.
				
				if(ListEnd == 0)
					ListStart = ListItem;				//Update the list start position if it is the first item in the list
				else
					ListEnd->ListNext = ListItem;

				ListEnd = ListItem;						//Update the list tail
				
				ListSize++;								//Our list just got 1 bigger!
				return true;							//Action was successfull
			};

	bool	Delete( long index )
			{
				AListStruct * ListItem = ListStart;

				if( ListSize == 1 && index == 0)
				{
					delete ListItem;
					ListStart = 0;
					ListEnd = 0;
					ListSize = 0;
					
					return true;
				}


				for( long i=0; i<ListSize ;i++ )
				{
					if( i == index )
					{


						if( ListItem->ListPrevious == 0 )
						{
							AListStruct * NextListItem = ListItem->ListNext;
							NextListItem->ListPrevious = 0;
							delete ListItem;
							ListSize--;
							ListStart = NextListItem;
						}
						else
						{
							if( ListItem->ListNext == 0 )
							{
								AListStruct * PreviousListItem = ListItem->ListPrevious;
								PreviousListItem->ListNext = 0;
								delete ListItem;
								ListSize--;
								ListEnd = PreviousListItem;
							}
							else
							{
								AListStruct * NextListItem = ListItem->ListNext;
								AListStruct * PreviousListItem = ListItem->ListPrevious;
								
								PreviousListItem->ListNext = ListItem->ListNext;
								NextListItem->ListPrevious = ListItem->ListPrevious;
								
								delete ListItem;
								ListSize--;
							}
						
						}
						return true;	//Index found and deleted properly, break iteration
					}
					else
					{
						ListItem = ListItem->ListNext;
					}

				}
				return false;			//No index found, haven't deleted
			};

	bool	Clear()
			{
				AListStruct * ListItem = ListStart;
				AListStruct * NextList = 0;

				if( ListStart != 0 )
				while( ListItem->ListNext != 0 )
				{
					NextList = ListItem->ListNext;
					
					delete ListItem;
					
					ListItem = NextList;
				}

				ListStart = 0;
				ListEnd = 0;
				ListSize = 0;

				return true;
			};

	long	Size(){	return ListSize; };

	T *	GetItem( long index )
		{
			AListStruct * ListItem = ListStart;

			for( long i=0; i<ListSize ;i++ )
			{
				if( i == index )
				{
					return &ListItem->Value;
				}
				else
				{
					ListItem = ListItem->ListNext;
				}

			}

			return 0;
		};

	T *	GetFirst()
		{
			AListStruct * ListItem = ListStart;

			if(ListSize != 0)
				return &ListItem->Value;

			return 0;
		};

	T *	GetLast()
		{
			AListStruct * ListItem = ListEnd;

			if(ListSize != 0)
				return &ListItem->Value;

			return 0;
		};

private:

	AListStruct * ListStart;
	AListStruct * ListEnd;
	long ListSize;
};

#endif