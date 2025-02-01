[ComponentEditorProps(category: "GameComponent", description: "CVVR Range Component", color: "0 0 255 255")]
class CVVR_VoNComponentClass: SCR_VoNComponentClass {};

class CVVR_VoNComponent: SCR_VoNComponent {
	override protected event void OnReceive(int playerId, BaseTransceiver receiver, int frequency, float quality)
	{
		super.OnReceive(playerId, receiver, frequency, quality);
		
		Print("test");
	}
};