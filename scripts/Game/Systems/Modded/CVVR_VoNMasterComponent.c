[ComponentEditorProps(category: "GameScripted/UI", description: "CVVR Master Component", color: "0 0 255 255")]
class CVVR_VoNMasterComponentClass: ScriptComponentClass {};

class CVVR_VoNMasterComponent: ScriptComponent {
	
	protected SCR_VONController m_VONController;
	
	[RplProp()]
	protected int m_iLocalVoiceRange = 3; // 1-5 now
	
	int GetLocalVoiceRange() {
		return m_iLocalVoiceRange;
	}
	
	void SetLocalVoiceRange(int voiceRangeChange) {
		int voiceRangeSum = m_iLocalVoiceRange + voiceRangeChange;
		
		if (voiceRangeSum >= 1 && voiceRangeSum <= 5) {
			m_iLocalVoiceRange = voiceRangeSum;
			
			Print(m_iLocalVoiceRange);
			
			Replication.BumpMe();
		
			m_VONController = SCR_VONController.Cast(GetGame().GetPlayerController().FindComponent(SCR_VONController));
		
			if(m_VONController)
				m_VONController.ReloadVONForRangeChange();
		};
	}
};

[ComponentEditorProps(category: "GameComponent", description: "CVVR Range Component", color: "0 0 255 255")]
class CVVR_VoNComponentRangeOneClass: SCR_VoNComponentClass {};

class CVVR_VoNComponentRangeOne: SCR_VoNComponent {};

[ComponentEditorProps(category: "GameComponent", description: "CVVR Range Component", color: "0 0 255 255")]
class CVVR_VoNComponentRangeTwoClass: SCR_VoNComponentClass {};

class CVVR_VoNComponentRangeTwo: SCR_VoNComponent {};

[ComponentEditorProps(category: "GameComponent", description: "CVVR Range Component", color: "0 0 255 255")]
class CVVR_VoNComponentRangeThreeClass: SCR_VoNComponentClass {};

class CVVR_VoNComponentRangeThree: SCR_VoNComponent {};

[ComponentEditorProps(category: "GameComponent", description: "CVVR Range Component", color: "0 0 255 255")]
class CVVR_VoNComponentRangeFourClass: SCR_VoNComponentClass {};

class CVVR_VoNComponentRangeFour: SCR_VoNComponent {};

[ComponentEditorProps(category: "GameComponent", description: "CVVR Range Component", color: "0 0 255 255")]
class CVVR_VoNComponentRangeFiveClass: SCR_VoNComponentClass {};

class CVVR_VoNComponentRangeFive: SCR_VoNComponent {};

/*[ComponentEditorProps(category: "GameComponent", description: "CVVR Range Component", color: "0 0 255 255")]
class CVVR_VoNComponentRangeSixClass: SCR_VoNComponentClass {};

class CVVR_VoNComponentRangeSix: SCR_VoNComponent {};*/