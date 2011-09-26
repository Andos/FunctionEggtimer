// ============================================================================
//
// This file are where the Conditions/Actions/Expressions are defined.
// You can manually enter these, or use CICK (recommended)
// See the Extension FAQ in this SDK for more info and where to download it
//
// ============================================================================

// Common Include
#include	"common.h"

// Quick memo: content of the eventInformations arrays
// ---------------------------------------------------
// Menu ID
// String ID
// Code
// Flags
// Number_of_parameters
// Parameter_type [Number_of_parameters]
// Parameter_TitleString [Number_of_parameters]

// Definitions of parameters for each condition
short conditionsInfos[]=
	{
	CID_conOnFunction,	CID_conOnFunction,	0,	0,	1,	PARAM_EXPSTRING,	CP0ID_conOnFunction,
	CID_conAnyFunction,	CID_conAnyFunction,	1,	0,	0,
	0
	};

// Definitions of parameters for each action
short actionsInfos[]=
	{ 
	AID_acDelayFunction,		AID_acDelayFunction,		0,	0,	2,	PARAM_EXPSTRING,	PARAM_EXPRESSION,	AP0ID_acDelayFunction,	AP1ID_acDelayFunction,
	AID_actCancelFunction,		AID_actCancelFunction,		1,	0,	1,	PARAM_EXPSTRING,	AP0ID_actCancelFunction,
	AID_acSetRepeat,			AID_acSetRepeat,			2,	0,	1,	PARAM_EXPRESSION,	AP0ID_acSetRepeat,
	AID_acSetPrivateData,		AID_acSetPrivateData,		3,	0,	1,	PARAM_EXPRESSION,	AP0ID_acSetPrivateData,
	AID_acUseSuccessFlag,		AID_acUseSuccessFlag,		4,	0,	0,
	AID_acFunctionSuccessfull,	AID_acFunctionSuccessfull,	5,	0,	0,
	AID_acImmediateFunction,	AID_acImmediateFunction,	6,	0,	2,	PARAM_EXPSTRING,	PARAM_EXPRESSION,	AP0ID_acImmediateFunction,	AP1ID_acImmediateFunction,
	AID_acStopAll,				AID_acStopAll,				7,	0,	0,
	0 };

// Definitions of parameters for each expression
short expressionsInfos[]=
	{ 
	EID_expGetFunctionsInQueue,	EID_expGetFunctionsInQueue,	0,	0,	0,
	EID_extLastFunctionName,	EID_extLastFunctionName,	1,	EXPFLAG_STRING,	0,
	EID_extPrivateData,			EID_extPrivateData,			2,	0,	0,
	EID_extRemainingRepeats,	EID_extRemainingRepeats,	3,	0,	0,
	0 };



// ============================================================================
//
// CONDITION ROUTINES
// 
// ============================================================================

long WINAPI DLLExport conOnFunction(LPRDATA rdPtr, long param1, long param2)
{
	char * p1=(LPSTR)param1;

	if( strcmp( rdPtr->LastFuncName , p1 ) == 0 )
		return true;

	return false;
}

long WINAPI DLLExport conAnyFunction(LPRDATA rdPtr, long param1, long param2)
{
	return true;
}



// ============================================================================
//
// ACTIONS ROUTINES
// 
// ============================================================================

short WINAPI DLLExport acDelayFunction(LPRDATA rdPtr, long param1, long param2)
{
	char * p1=(LPSTR)param1;
	FuncStruct NewTimer;
	int strl = strlen(p1)+1;
	NewTimer.FuncName = new char[strl];

	if(NewTimer.FuncName)
		strcpy_s(NewTimer.FuncName, strl, p1);
	else return 0;

	NewTimer.Time = param2;
	NewTimer.InitialTime = param2;

	NewTimer.DeleteMe = false;
	NewTimer.PrivateData = 0;
	NewTimer.Repeat = 0;
	NewTimer.WaitForSuccellfull = 0;
	NewTimer.ImmediateFunc = false;
	NewTimer.ImmediateApproved = false;

	rdPtr->FunctionList.Add( NewTimer );

	return 0;
}

short WINAPI DLLExport actCancelFunction(LPRDATA rdPtr, long param1, long param2)
{
	char * p1=(LPSTR)param1;
	
	for( int i = 0; i<rdPtr->FunctionList.Size();i++)
	{
		if( strcmp( p1, rdPtr->FunctionList.GetItem(i)->FuncName) == 0 )
		{
			delete [] rdPtr->FunctionList.GetItem(i)->FuncName;
			rdPtr->FunctionList.Delete(i);
			i = max( i-1 , 0); //Doesn't this make it ignore the second item??? FIXME! (Delete the line after test)
		}
	}
	

	return 0;
}

short WINAPI DLLExport acSetRepeat(LPRDATA rdPtr, long param1, long param2)
{
	FuncStruct * CurrentFunc = rdPtr->FunctionList.GetLast();

	if(!CurrentFunc)
		return 0;

	if( CurrentFunc->ImmediateFunc == false )
			CurrentFunc->Repeat = max(param1,-1);
	else 
	{
		if( param1 >= 0 )
			CurrentFunc->Repeat = max( param1 - 1 ,0);
		else
			CurrentFunc->Repeat = -1;
	}

	CurrentFunc->ImmediateApproved = true;

	return 0;
}

short WINAPI DLLExport acSetPrivateData(LPRDATA rdPtr, long param1, long param2)
{
	if(rdPtr->FunctionList.Size() > 0)
		rdPtr->FunctionList.GetLast()->PrivateData = param1;
	return 0;
}

short WINAPI DLLExport acUseSuccessFlag(LPRDATA rdPtr, long param1, long param2)
{
	FuncStruct * CurrentFunc = rdPtr->FunctionList.GetLast();

	if(!CurrentFunc)
		return 0;

	CurrentFunc->WaitForSuccellfull = true;

	return 0;
}

short WINAPI DLLExport acFunctionSuccessfull(LPRDATA rdPtr, long param1, long param2)
{
	FuncStruct * CurrentStruct = rdPtr->FunctionList.GetItem( rdPtr->CurrentFuncIndex );

	if( CurrentStruct != 0 )
		CurrentStruct->DeleteMe = true;

	return 0;
}

	
	
short WINAPI DLLExport acImmediateFunction(LPRDATA rdPtr, long param1, long param2)
{
	char * p1=(LPSTR)param1;
	FuncStruct NewTimer;
	int strl = strlen(p1)+1;
	NewTimer.FuncName = new char[strl];

	if(NewTimer.FuncName)
		strcpy_s(NewTimer.FuncName, strl, p1);
	else return 0;

	NewTimer.Time = param2;
	NewTimer.InitialTime = param2;

	NewTimer.DeleteMe = false;
	NewTimer.PrivateData = 0;
	NewTimer.Repeat = 0;
	NewTimer.WaitForSuccellfull = 0;
	NewTimer.ImmediateFunc = true;
	NewTimer.ImmediateApproved = false;

	rdPtr->FunctionList.Add( NewTimer );

	// Update the 'Last function name' in the rdPtr structure
	delete [] rdPtr->LastFuncName;
	rdPtr->LastFuncName = new char[strl];
	strcpy_s(rdPtr->LastFuncName, strl, p1);

	//Trigger the 'On function' events
	callRunTimeFunction(rdPtr,RFUNCTION_GENERATEEVENT,0,0);
	callRunTimeFunction(rdPtr,RFUNCTION_GENERATEEVENT,1,0);
	return 0;
}
	
short WINAPI DLLExport acStopAll(LPRDATA rdPtr, long param1, long param2)
{
	while( rdPtr->FunctionList.Size() > 0 )
	{
		FuncStruct * CurrentStruct = rdPtr->FunctionList.GetFirst();

		if( !CurrentStruct )
			return 0;

		if( CurrentStruct->FuncName )
			delete [] CurrentStruct->FuncName;

		rdPtr->FunctionList.Delete( 0 );

	}

	return 0;
}


// ============================================================================
//
// EXPRESSIONS ROUTINES
// 
// ============================================================================

long WINAPI DLLExport expGetFunctionsInQueue(LPRDATA rdPtr, long param1)
{
	return rdPtr->FunctionList.Size();
}

long WINAPI DLLExport extLastFunctionName(LPRDATA rdPtr, long param1)
{
	rdPtr->rHo.hoFlags |= HOF_STRING;
	return (long)rdPtr->LastFuncName;
}




long WINAPI DLLExport extPrivateData(LPRDATA rdPtr, long param1)
{
	FuncStruct * CurrentStruct = rdPtr->FunctionList.GetItem( rdPtr->CurrentFuncIndex );
	
	if(CurrentStruct != 0)
		return CurrentStruct->PrivateData;


	return 0;
}

long WINAPI DLLExport extRemainingRepeats(LPRDATA rdPtr, long param1)
{
	FuncStruct * CurrentStruct = rdPtr->FunctionList.GetItem( rdPtr->CurrentFuncIndex );
	
	if(CurrentStruct != 0)
		return CurrentStruct->Repeat;

	return 0;
}



// ----------------------------------------------------------
// Condition / Action / Expression jump table
// ----------------------------------------------------------
// Contains the address inside the extension of the different
// routines that handle the action, conditions and expressions.
// Located at the end of the source for convinience
// Must finish with a 0
//
long (WINAPI * ConditionJumps[])(LPRDATA rdPtr, long param1, long param2) = 
	{
	conOnFunction,
	conAnyFunction,
	0
	};
	
short (WINAPI * ActionJumps[])(LPRDATA rdPtr, long param1, long param2) =
	{
	acDelayFunction,
	actCancelFunction,
	acSetRepeat,
	acSetPrivateData,
	acUseSuccessFlag,
	acFunctionSuccessfull,
	acImmediateFunction,
	acStopAll,
	0
	};

long (WINAPI * ExpressionJumps[])(LPRDATA rdPtr, long param) = 
	{
	expGetFunctionsInQueue,
	extLastFunctionName,
	extPrivateData,
	extRemainingRepeats,
	0
	};