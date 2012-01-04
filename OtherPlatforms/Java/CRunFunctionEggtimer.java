import java.awt.*;
import java.io.*;
import Extensions.*;
import Services.*;
import RunLoop.*;
import Conditions.*;
import Actions.*;
import Expressions.*;
import java.util.ArrayList;

public class CRunFunctionEggtimer extends CRunExtension
{
    // <editor-fold defaultstate="collapsed" desc=" A/C/E Constants ">
    public static final int CONONFUNCTION = 0;
    public static final int CONANYFUNCTION = 1;
    public static final int CND_LAST = 2;

    public static final int ACDELAYFUNCTION = 0;
    public static final int ACTCANCELFUNCTION = 1;
    public static final int ACSETREPEAT = 2;
    public static final int ACSETPRIVATEDATA = 3;
    public static final int ACUSESUCCESSFLAG = 4;
    public static final int ACFUNCTIONSUCCESSFULL = 5;
    public static final int ACIMMEDIATEFUNCTION = 6;
    public static final int ACSTOPALL = 7;

    public static final int EXPGETFUNCTIONSINQUEUE = 0;
    public static final int EXTLASTFUNCTIONNAME = 1;
    public static final int EXTPRIVATEDATA = 2;
    public static final int EXTREMAININGREPEATS = 3;

    // </editor-fold>

    public CRunFunctionEggtimer()
    {
    }
    public @Override int getNumberOfConditions()
    {
	    return CND_LAST;
    }
    public @Override boolean createRunObject(CBinaryFile file, CCreateObjectInfo cob, int version)
    {
        return false;
    }
    public @Override void destroyRunObject(boolean bFast)
    {
    }


    public @Override int handleRunObject()
    {
        for( int i = 0; i < functionList.size(); i++)
        {
            currentFunction = functionList.get(i);
            if(currentFunction == null)
                continue;

            //Delete the function if it was marked as successful or if function
            //is immediate but not asked to loop
            if(currentFunction.deleteMe == true ||
                    (currentFunction.immediateFunc
                    && !currentFunction.immediateApproved))
            {
                functionList.remove(i);
                i--;
                continue;
            }

            //When the time expires, trigger the functions if
            //function is not set to repeat.
            if( currentFunction.time <= 0 )
            {
                boolean deleteMe = true;

                //Update the lastFunctionName string
                lastFunctionName = currentFunction.name;

                //Handle repeatable functions
                if( currentFunction.repeat > 1 || currentFunction.repeat == -1)
                {
                    currentFunction.time = currentFunction.initialTime;
                    currentFunction.repeat = Math.max(currentFunction.repeat-1, -1);
                    deleteMe = false;
                }

                //Triggers the 'on function' events.
                ho.generateEvent(CONONFUNCTION, 0);
                ho.generateEvent(CONANYFUNCTION, 0);

                //Delete the function from the queue
                if(deleteMe && currentFunction.waitForSuccellfull == false)
                {
                    functionList.remove(i);
                    i--;
                    continue;
                }
            }
            else
                currentFunction.time--;
        }


        return 0;
    }


    public @Override void displayRunObject(Graphics2D g2)
    {
    }
    public @Override boolean saveRunObject(DataOutputStream stream)
    {
        return true;
    }
    public @Override boolean loadRunObject(DataInputStream stream)
    {
        return true;
    }
   

    // Conditions
    // -------------------------------------------------
    public @Override boolean condition(int num, CCndExtension cnd)
    {
        switch (num)
        {
            case CONONFUNCTION:
                return conOnFunction(cnd);
            case CONANYFUNCTION:
                return conAnyFunction(cnd);
        }
        return false;
    }

    // Actions
    // -------------------------------------------------
    public @Override void action(int num, CActExtension act)
    {
        switch (num)
        {
            case ACDELAYFUNCTION:
                acDelayFunction(act);
                break;
            case ACTCANCELFUNCTION:
                actCancelFunction(act);
                break;
            case ACSETREPEAT:
                acSetRepeat(act);
                break;
            case ACSETPRIVATEDATA:
                acSetPrivateData(act);
                break;
            case ACUSESUCCESSFLAG:
                acUseSuccessFlag(act);
                break;
            case ACFUNCTIONSUCCESSFULL:
                acFunctionSuccessfull(act);
                break;
            case ACIMMEDIATEFUNCTION:
                acImmediateFunction(act);
                break;
            case ACSTOPALL:
                acStopAll(act);
                break;
        }
    }

    // Expressions
    // -------------------------------------------------
    public @Override CValue expression(int num)
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
        return null;
    }

    String lastFunctionName = "";
    DelayedFunction currentFunction;
    ArrayList<DelayedFunction> functionList = new ArrayList<DelayedFunction>();

    private boolean conOnFunction(CCndExtension cnd)
    {
        String param0 = cnd.getParamExpString(rh,0);
        if(lastFunctionName.equals(param0))
            return true;
        return false;
    }

    private boolean conAnyFunction(CCndExtension cnd)
    {
        return true;
    }

    private void acDelayFunction(CActExtension act)
    {
        String functionName = act.getParamExpString(rh,0);
        int delay = act.getParamExpression(rh,1);

        DelayedFunction newfunc = new DelayedFunction();
        newfunc.name = functionName;
        newfunc.initialTime = delay;
        newfunc.time = delay;
        newfunc.deleteMe = false;
        newfunc.privateData = 0;
        newfunc.repeat = 0;
        newfunc.waitForSuccellfull = false;
        newfunc.immediateFunc = false;
        newfunc.immediateApproved = false;

        functionList.add(newfunc);
    }

    private void actCancelFunction(CActExtension act)
    {
        String functionName = act.getParamExpString(rh,0);
        for(int i = 0; i<functionList.size(); i++)
        {
            if( functionName.equals(functionList.get(i).name) )
            {
                functionList.remove(i);
                i--;
            }
        }
    }

    private void acSetRepeat(CActExtension act)
    {
        int repetitions = act.getParamExpression(rh,0);
        if(functionList.isEmpty())
            return;

        DelayedFunction func = functionList.get(functionList.size()-1);

        if(func.immediateFunc == false)
            func.repeat = Math.max(repetitions,-1);
        else
            if( repetitions >= 0 )
                func.repeat = Math.max(repetitions-1, 0);
            else
                func.repeat = -1;

        func.immediateApproved = true;
    }

    private void acSetPrivateData(CActExtension act)
    {
        int privatedata = act.getParamExpression(rh,0);
        if(functionList.isEmpty())
            return;

        DelayedFunction func = functionList.get(functionList.size()-1);
        func.privateData = privatedata;
    }

    private void acUseSuccessFlag(CActExtension act)
    {
        if(functionList.isEmpty())
            return;

        DelayedFunction func = functionList.get(functionList.size()-1);
        func.waitForSuccellfull = true;
    }

    private void acFunctionSuccessfull(CActExtension act)
    {
        if(currentFunction != null)
            currentFunction.deleteMe = true;
    }

    private void acImmediateFunction(CActExtension act)
    {
        String functionName = act.getParamExpString(rh,0);
        int delay = act.getParamExpression(rh,1);

        DelayedFunction newfunc = new DelayedFunction();
        newfunc.name = functionName;
        newfunc.initialTime = delay;
        newfunc.time = delay;
        newfunc.deleteMe = false;
        newfunc.privateData = 0;
        newfunc.repeat = 0;
        newfunc.waitForSuccellfull = false;
        newfunc.immediateFunc = true;
        newfunc.immediateApproved = false;

        functionList.add(newfunc);
        
        lastFunctionName = functionName;
        ho.generateEvent(CONONFUNCTION, 0);
        ho.generateEvent(CONANYFUNCTION, 0);
    }

    private void acStopAll(CActExtension act)
    {
        functionList.clear();
    }

    //-----------------------------
    //EXPRESSIONS//
    //-----------------------------

    private CValue expGetFunctionsInQueue()
    {
        return new CValue(functionList.size());
    }

    private CValue extLastFunctionName()
    {
        return new CValue(lastFunctionName);
    }

    private CValue extPrivateData()
    {
        return new CValue(currentFunction.privateData);
    }

    private CValue extRemainingRepeats()
    {
        return new CValue(currentFunction.repeat);
    }

}
