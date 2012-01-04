package Extensions
{
	public class CRunFunctionEggtimerFunction
	{
		public var name:String;
		public var time:int;
		public var initialTime:int;
		public var repeat:int;
		public var privateData:int;
		public var waitForSuccellfull:Boolean;
		public var deleteMe:Boolean;
		public var immediateFunc:Boolean;
		public var immediateApproved:Boolean;
		
		public var next:CRunFunctionEggtimerFunction = null;
        public var prev:CRunFunctionEggtimerFunction = null;
	}
}