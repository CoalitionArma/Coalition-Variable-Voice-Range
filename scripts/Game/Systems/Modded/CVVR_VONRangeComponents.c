//------------------------------------------------------------------------------------------------

// Range One

//------------------------------------------------------------------------------------------------

[ComponentEditorProps(category: "GameComponent", description: "CVVR Range Component", color: "0 0 255 255")]
class CVVR_VoNComponentRangeOneClass: SCR_VoNComponentClass {};

class CVVR_VoNComponentRangeOne: SCR_VoNComponent {
	override protected event void OnReceive(int playerId, BaseTransceiver receiver, int frequency, float quality)
	{
		super.OnReceive(playerId, receiver, frequency, quality);
		
		Print("Range One");
	}
};

//------------------------------------------------------------------------------------------------

// Range Two

//------------------------------------------------------------------------------------------------

[ComponentEditorProps(category: "GameComponent", description: "CVVR Range Component", color: "0 0 255 255")]
class CVVR_VoNComponentRangeTwoClass: SCR_VoNComponentClass {};

class CVVR_VoNComponentRangeTwo: SCR_VoNComponent {
	override protected event void OnReceive(int playerId, BaseTransceiver receiver, int frequency, float quality)
	{
		super.OnReceive(playerId, receiver, frequency, quality);
		
		Print("Range Two");
	}
};

//------------------------------------------------------------------------------------------------

// Range Three

//------------------------------------------------------------------------------------------------

[ComponentEditorProps(category: "GameComponent", description: "CVVR Range Component", color: "0 0 255 255")]
class CVVR_VoNComponentRangeThreeClass: SCR_VoNComponentClass {};

class CVVR_VoNComponentRangeThree: SCR_VoNComponent {
	override protected event void OnReceive(int playerId, BaseTransceiver receiver, int frequency, float quality)
	{
		super.OnReceive(playerId, receiver, frequency, quality);
		
		Print("Range Three");
	}
};

//------------------------------------------------------------------------------------------------

// Range Four

//------------------------------------------------------------------------------------------------

[ComponentEditorProps(category: "GameComponent", description: "CVVR Range Component", color: "0 0 255 255")]
class CVVR_VoNComponentRangeFourClass: SCR_VoNComponentClass {};

class CVVR_VoNComponentRangeFour: SCR_VoNComponent {
	override protected event void OnReceive(int playerId, BaseTransceiver receiver, int frequency, float quality)
	{
		super.OnReceive(playerId, receiver, frequency, quality);
		
		Print("Range Four");
	}
};

//------------------------------------------------------------------------------------------------

// Range Five

//------------------------------------------------------------------------------------------------

[ComponentEditorProps(category: "GameComponent", description: "CVVR Range Component", color: "0 0 255 255")]
class CVVR_VoNComponentRangeFiveClass: SCR_VoNComponentClass {};

class CVVR_VoNComponentRangeFive: SCR_VoNComponent {
	private SCR_VONController m_VONController;
	
	override event protected void OnCapture(BaseTransceiver transmitter) {
		super.OnCapture(transmitter);
		Print("test");
	}
	
	override protected event void OnReceive(int playerId, BaseTransceiver receiver, int frequency, float quality)
	{
		super.OnReceive(playerId, receiver, frequency, quality);
		
		Print("Range Five");
	}
};