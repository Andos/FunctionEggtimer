// Object identifier "FuET"
#define IDENTIFIER	MAKEID(F,u,E,T)

// ------------------------------
// DEFINITION OF CONDITIONS CODES
// ------------------------------
#define	CND_CONDITION				0
#define	CND_LAST					2

// ---------------------------
// DEFINITION OF ACTIONS CODES
// ---------------------------
#define	ACT_ACTION					0
#define	ACT_LAST					8

// -------------------------------
// DEFINITION OF EXPRESSIONS CODES
// -------------------------------
#define	EXP_EXPRESSION				0
#define	EXP_LAST                    4




#include "Alist.h"


struct FuncStruct
{
	char *	FuncName;
	long	Time;
	long	InitialTime;
	long	Repeat;
	long	PrivateData;
	bool	WaitForSuccellfull;
	bool	DeleteMe;
	bool	ImmediateFunc;
	bool	ImmediateApproved;
};




// --------------------------------
// EDITION OF OBJECT DATA STRUCTURE
// --------------------------------
// These values let you store data in your extension that will be saved in the CCA.
// You would use these with a dialog...

typedef struct tagEDATA_V1
{
	extHeader		eHeader;
//	short			swidth;
//	short			sheight;

} EDITDATA;
typedef EDITDATA _far *			LPEDATA;

// Object versions
#define	KCX_CURRENT_VERSION			1

// --------------------------------
// RUNNING OBJECT DATA STRUCTURE
// --------------------------------
// If you want to store anything between actions/conditions/expressions
// you should store it here. Also, some OEFLAGS require you to add
// structures to this structure.

typedef struct tagRDATA
{
	headerObject	rHo;					// Header
	AList	<FuncStruct>	FunctionList;			// List of queued functions
	char *					LastFuncName;			// For condition comparisons
	long					CurrentFuncIndex;		// For expressions so they can receive data from current triggered function


} RUNDATA;
typedef	RUNDATA	_far *			LPRDATA;

// Size when editing the object under level editor
// -----------------------------------------------
#define	MAX_EDITSIZE			sizeof(EDITDATA)

// Default flags
// -------------
#define	OEFLAGS      			0
#define	OEPREFS      			0

/* See OEFLAGS.txt for more info on these useful things! */


// If to handle message, specify the priority of the handling procedure
// 0= low, 255= very high. You should use 100 as normal.                                                
// --------------------------------------------------------------------
#define	WINDOWPROC_PRIORITY		100
