//------------------------------------------------------------------------------------------------

// Range One

//------------------------------------------------------------------------------------------------

[ComponentEditorProps(category: "GameComponent", description: "CVVR Range Component", color: "0 0 255 255")]
class CVVR_VoNComponentRangeOneClass: SCR_VoNComponentClass {};

class CVVR_VoNComponentRangeOne: SCR_VoNComponent {
	override protected event void OnReceive(int playerId, BaseTransceiver receiver, int frequency, float quality)
	{
		super.OnReceive(playerId, receiver, frequency, quality);
		
		//IEntity playerEntity = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerId);
		int voiceRange = CVVR_AuthorityComponent.GetInstance().ReturnPlayerRange(playerId);
		Print(voiceRange);
		
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
		
		//IEntity playerEntity = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerId);
		int voiceRange = CVVR_AuthorityComponent.GetInstance().ReturnPlayerRange(playerId);
		Print(voiceRange);
		
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
				
		//IEntity playerEntity = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerId);
		int voiceRange = CVVR_AuthorityComponent.GetInstance().ReturnPlayerRange(playerId);
		Print(voiceRange);
		
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
				
		//IEntity playerEntity = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerId);
		int voiceRange = CVVR_AuthorityComponent.GetInstance().ReturnPlayerRange(playerId);
		Print(voiceRange);
		
		Print("Range Four");
	}
};

//------------------------------------------------------------------------------------------------

// Range Five

//------------------------------------------------------------------------------------------------

[ComponentEditorProps(category: "GameComponent", description: "CVVR Range Component", color: "0 0 255 255")]
class CVVR_VoNComponentRangeFiveClass: SCR_VoNComponentClass {};

class CVVR_VoNComponentRangeFive: SCR_VoNComponent {
	override protected event void OnReceive(int playerId, BaseTransceiver receiver, int frequency, float quality)
	{
		super.OnReceive(playerId, receiver, frequency, quality);
				
		//IEntity playerEntity = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerId);
		int voiceRange = CVVR_AuthorityComponent.GetInstance().ReturnPlayerRange(playerId);
		Print(voiceRange);
		
		Print("Range Five");
	}
};