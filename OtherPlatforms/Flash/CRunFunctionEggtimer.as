package Extensions
{
    import Actions.*;
    
    import Conditions.*;
    
    import Expressions.*;
    
    import RunLoop.*;
    
    import Services.*;
    
    import Sprites.*;

    public class CRunFunctionEggtimer extends CRunExtension
    {
        private static var CONONFUNCTION:int = 0;
        private static var CONANYFUNCTION:int = 1;
        private static var CND_LAST:int = 2;

        private static var ACDELAYFUNCTION:int = 0;
        private static var ACTCANCELFUNCTION:int = 1;
        private static var ACSETREPEAT:int = 2;
        private static var ACSETPRIVATEDATA:int = 3;
        private static var ACUSESUCCESSFLAG:int = 4;
        private static var ACFUNCTIONSUCCESSFULL:int = 5;
        private static var ACIMMEDIATEFUNCTION:int = 6;
        private static var ACSTOPALL:int = 7;

        private static var EXPGETFUNCTIONSINQUEUE:int = 0;
        private static var EXTLASTFUNCTIONNAME:int = 1;
        private static var EXTPRIVATEDATA:int = 2;
        private static var EXTREMAININGREPEATS:int = 3;

        //Linked list for holding the functions
        private var head:CRunFunctionEggtimerFunction = null;
        private var tail:CRunFunctionEggtimerFunction = null;
        private var itemCount:int = 0;

		private var lastFunctionName:String = "";
	    private var currentFunction:CRunFunctionEggtimerFunction;
	    
        public function CRunFunctionEggtimer()
        {
        }
        
        public override function getNumberOfConditions():int
        {
            return CND_LAST;
        }
        
        public override function createRunObject(file:CBinaryFile, cob:CCreateObjectInfo, version:int):Boolean
        {
            return false;
        }
        
        public override function destroyRunObject(bFast:Boolean):void
        {
        	head = tail = null;
        }
        
        public override function handleRunObject():int
        {
        	for(var it:CRunFunctionEggtimerFunction = head; it != null; it = it.next)
        	{
        		currentFunction = it;
        		
 		        //Delete the function if it was marked as successful or if function
	            //is immediate but not asked to loop
	            if(it.deleteMe == true || (it.immediateFunc && !it.immediateApproved))
	            {
	                DeleteItem(it);
	                continue;
	            }
	
	            //When the time expires, trigger the functions if
	            //function is not set to repeat.
	            if( it.time <= 0 )
	            {
	                var deleteMe:Boolean = true;
	
	                //Update the lastFunctionName string
	                lastFunctionName = it.name;
	
	                //Handle repeatable functions
	                if( it.repeat > 1 || it.repeat == -1)
	                {
	                    it.time = it.initialTime;
	                    it.repeat = Math.max(it.repeat-1, -1);
	                    deleteMe = false;
	                }
	
	                //Triggers the 'on function' events.
	                ho.generateEvent(CONONFUNCTION, 0);
	                ho.generateEvent(CONANYFUNCTION, 0);
	
	                //Delete the function from the queue
	                if(deleteMe && it.waitForSuccellfull == false)
	                {
	                    DeleteItem(it);
	                    continue;
	                }
	            }
	            else
	                it.time--;
	        }

            return 0;
        }


        // Conditions
        // -------------------------------------------------
        public override function condition(num:int, cnd:CCndExtension):Boolean
        {
            switch (num)
            {
            case CONONFUNCTION:
                return conOnFunction(cnd.getParamExpString(rh,0));
            case CONANYFUNCTION:
                return conAnyFunction();
            }
            return false;
        }

        // Actions
        // -------------------------------------------------
        public override function action(num:int, act:CActExtension):void
        {
            switch (num)
            {
            case ACDELAYFUNCTION:
                acDelayFunction(act.getParamExpString(rh,0), act.getParamExpression(rh,1));
                break;
            case ACTCANCELFUNCTION:
                actCancelFunction(act.getParamExpString(rh,0));
                break;
            case ACSETREPEAT:
                acSetRepeat(act.getParamExpression(rh,0));
                break;
            case ACSETPRIVATEDATA:
                acSetPrivateData(act.getParamExpression(rh,0));
                break;
            case ACUSESUCCESSFLAG:
                acUseSuccessFlag();
                break;
            case ACFUNCTIONSUCCESSFULL:
                acFunctionSuccessfull();
                break;
            case ACIMMEDIATEFUNCTION:
                acImmediateFunction(act.getParamExpString(rh,0), act.getParamExpression(rh,1));
                break;
            case ACSTOPALL:
                acStopAll();
                break;
            }
        }

        // Expressions
        // -------------------------------------------------
        public override function expression(num:int):CValue
        {
            switch (num)
            {
            case EXPGETFUNCTIONSINQUEUE:
                return expGetFunctionsInQueue();
            case EXTLASTFUNCTIONNAME:
                return extLastFunctionName();
            case EXTPRIVATEDATA:
                return extPrivateData();
            case EXTREMAININGREPEATS:
                return extRemainingRepeats();
            }
            return new CValue(0);
        }

        private function DeleteItem(it:CRunFunctionEggtimerFunction):void
        {
        	//Deleting this object and fixing pointers
	    	if(it.prev != null)
	    		it.prev.next = it.next;
	    	else
	    		head = it.next;	
	    		
	    	if(it.next != null)
	    		it.next.prev = it.prev;
	    	else
	    		tail = it.prev;
	    		
	    	itemCount--;
		}
		
		private function AddItem(it:CRunFunctionEggtimerFunction):void
		{
			if(tail == null)
			{
				head = tail = it;
				it.prev = it.next = null;
			}
			else
			{
				tail.next = it;
				it.prev = tail;
				it.next = null;
				tail = it;
			}
			itemCount++;
		}




        private function conOnFunction(functionName:String):Boolean
        {
        	if(functionName == lastFunctionName)
        		return true;
            return false;
        }

        private function conAnyFunction():Boolean
        {
            return true;
        }

        private function acDelayFunction(functionName:String, delay:int):void
        {
	        var newfunc:CRunFunctionEggtimerFunction = new CRunFunctionEggtimerFunction();
	        newfunc.name = functionName;
	        newfunc.initialTime = delay;
	        newfunc.time = delay;
	        newfunc.deleteMe = false;
	        newfunc.privateData = 0;
	        newfunc.repeat = 0;
	        newfunc.waitForSuccellfull = false;
	        newfunc.immediateFunc = false;
	        newfunc.immediateApproved = false;
	
	        AddItem(newfunc);
        }

        private function actCancelFunction(functionName:String):void
        {
	        for(var it:CRunFunctionEggtimerFunction = head; it != null; it = it.next)
	            if( functionName == it.name )
	                DeleteItem(it);
        }

        private function acSetRepeat(repetitions:int):void
        {
	        if(tail == null)
	            return;
	
	        var func:CRunFunctionEggtimerFunction = tail;
	
	        if(func.immediateFunc == false)
	            func.repeat = Math.max(repetitions,-1);
	        else
	            if( repetitions >= 0 )
	                func.repeat = Math.max(repetitions-1, 0);
	            else
	                func.repeat = -1;
	
	        func.immediateApproved = true;
        }

        private function acSetPrivateData(privateData:int):void
        {
        	if(tail == null)
	        	return;
	        tail.privateData = privateData;
        }

        private function acUseSuccessFlag():void
        {
        	if(tail == null)
	        	return;
	        tail.waitForSuccellfull = true;
        }

        private function acFunctionSuccessfull():void
        {
        	if(currentFunction != null)
        		currentFunction.deleteMe = true;
        }

        private function acImmediateFunction(functionName:String, delay:int):void
        {
	        var newfunc:CRunFunctionEggtimerFunction = new CRunFunctionEggtimerFunction();
	        newfunc.name = functionName;
	        newfunc.initialTime = delay;
	        newfunc.time = delay;
	        newfunc.deleteMe = false;
	        newfunc.privateData = 0;
	        newfunc.repeat = 0;
	        newfunc.waitForSuccellfull = false;
	        newfunc.immediateFunc = true;
	        newfunc.immediateApproved = false;
	
	        AddItem(newfunc);
	        
	        lastFunctionName = functionName;
	        ho.generateEvent(CONONFUNCTION, 0);
	        ho.generateEvent(CONANYFUNCTION, 0);
        }

        private function acStopAll():void
        {
        	head = tail = null;
        }

        private function expGetFunctionsInQueue():CValue
        {
            return new CValue(itemCount);
        }

        private function extLastFunctionName():CValue
        {
        	var ret:CValue = new CValue(0);
        	ret.forceString(lastFunctionName)
            return ret;
        }

        private function extPrivateData():CValue
        {
        	if(currentFunction != null)
        		return new CValue(currentFunction.privateData);
        	else
        		return new CValue(0);
        }

        private function extRemainingRepeats():CValue
        {
        	if(currentFunction != null)
        		return new CValue(currentFunction.repeat);
        	else
        		return new CValue(0);
        }

    }
}
