class CVVR_HUD: SCR_InfoDisplay {
	protected CVVR_VoNMasterComponent m_VoNMasterComponent;
	protected ProgressBarWidget m_wVoiceRangeSlider;
	protected InputManager m_InputManager;
	protected IEntity m_Owner;

	//------------------------------------------------------------------------------------------------

	// Override/static functions

	//------------------------------------------------------------------------------------------------
	
	override event void OnStartDraw(IEntity owner)
	{
		super.OnStartDraw(owner);
		m_Owner = owner;
		GetGame().GetInputManager().AddActionListener("CVVR_ShowVoiceRangeSlider", EActionTrigger.PRESSED, ShowVoiceRangeSlider);
		GetGame().GetInputManager().AddActionListener("CVVR_ShowVoiceRangeSlider", EActionTrigger.UP, HideVoiceRangeSlider);
	}
	
	//------------------------------------------------------------------------------------------------
	override event void OnStopDraw(IEntity owner)
	{
		super.OnStopDraw(owner);
		GetGame().GetInputManager().RemoveActionListener("CVVR_ShowVoiceRangeSlider", EActionTrigger.PRESSED, ShowVoiceRangeSlider);
		GetGame().GetInputManager().AddActionListener("CVVR_ShowVoiceRangeSlider", EActionTrigger.UP, HideVoiceRangeSlider);
	}
	
	//------------------------------------------------------------------------------------------------

	// UI functions

	//------------------------------------------------------------------------------------------------
	
	protected void ShowVoiceRangeSlider()
	{
		if (!m_InputManager || !m_wVoiceRangeSlider) 
		{
			m_InputManager = GetGame().GetInputManager();
			m_wVoiceRangeSlider = ProgressBarWidget.Cast(m_wRoot.FindWidget("VoiceRangeSlider"));
			m_VoNMasterComponent = CVVR_VoNMasterComponent.Cast(SCR_PlayerController.Cast(m_Owner).GetControlledEntity().FindComponent(CVVR_VoNMasterComponent));
			return;
		};
		
		float currentOpacity = m_wVoiceRangeSlider.GetOpacity();	
		
		if (currentOpacity < 0.65) {
			m_wVoiceRangeSlider.SetOpacity(currentOpacity + 0.025);
		};
		
		int actionValue = m_InputManager.GetActionValue("CVVR_VoiceRangeAnalog");
		
		if (actionValue == -1 || actionValue == 1) {
			m_VoNMasterComponent.SetLocalVoiceRange(actionValue);
		};
		
		m_wVoiceRangeSlider.SetCurrent(m_VoNMasterComponent.GetLocalVoiceRange());
	};
	
	//------------------------------------------------------------------------------------------------
	protected void HideVoiceRangeSlider()
	{
		GetGame().GetCallqueue().CallLater(HideBar, 0, true);
	};


	//------------------------------------------------------------------------------------------------
	protected void HideBar()
	{
		float currentOpacity = m_wVoiceRangeSlider.GetOpacity();
		
		if (currentOpacity <= 0) {
			GetGame().GetCallqueue().Remove(HideBar);
			return;
		};
		
		m_wVoiceRangeSlider.SetOpacity(currentOpacity - 0.025);
	}
};