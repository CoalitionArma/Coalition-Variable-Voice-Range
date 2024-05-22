class CVVR_HUD: SCR_InfoDisplay {
	protected CVVR_VoNMasterComponent m_VoNMasterComponent;
	protected ProgressBarWidget m_wVoiceRangeSlider;
	protected TextWidget m_wVoiceRangeText;
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
			m_wVoiceRangeText = TextWidget.Cast(m_wRoot.FindWidget("VoiceRangeText"));
			m_VoNMasterComponent = CVVR_VoNMasterComponent.Cast(SCR_PlayerController.Cast(m_Owner).GetControlledEntity().FindComponent(CVVR_VoNMasterComponent));
		};
		
		float currentSliderOpacity = m_wVoiceRangeSlider.GetOpacity();
		float currentTextOpacity = m_wVoiceRangeText.GetOpacity();
		
		if (currentSliderOpacity < 0.65 && currentTextOpacity < 0.65) {
			m_wVoiceRangeSlider.SetOpacity(currentSliderOpacity + 0.025);
			m_wVoiceRangeText.SetOpacity(currentTextOpacity + 0.025);
		};
		
		int actionValue = m_InputManager.GetActionValue("CVVR_VoiceRangeAnalog");
		
		if (actionValue != 0) {
			m_VoNMasterComponent.SetLocalVoiceRange(actionValue);
		};
		
		m_wVoiceRangeSlider.SetCurrent(m_VoNMasterComponent.GetLocalVoiceRange());
		
		// Color
		switch (m_VoNMasterComponent.GetLocalVoiceRange())
		{
			case 1: 
			{ 
				m_wVoiceRangeSlider.SetColor(Color.SpringGreen);
				m_wVoiceRangeText.SetText("Whisper"); 
				m_wVoiceRangeText.SetColor(Color.SpringGreen);
				break; 
			};
			case 2:
			{
				m_wVoiceRangeSlider.SetColor(Color.Green); 
				m_wVoiceRangeText.SetText("Close Contact"); 
				m_wVoiceRangeText.SetColor(Color.Green);
				break; 
			};
			case 3:
			{
				m_wVoiceRangeSlider.SetColor(Color.Yellow); 
				m_wVoiceRangeText.SetText("Normal"); 
				m_wVoiceRangeText.SetColor(Color.Yellow);
				break; 
			};
			case 4:
			{
				m_wVoiceRangeSlider.SetColor(Color.Red); 
				m_wVoiceRangeText.SetText("Yelling"); 
				m_wVoiceRangeText.SetColor(Color.Red);
				break; 
			};
			case 5:
			{
				m_wVoiceRangeSlider.SetColor(Color.DarkRed);
				m_wVoiceRangeText.SetText("May I Speak To Your Manager");  
				m_wVoiceRangeText.SetColor(Color.DarkRed);
				break; 
			}
			default: { m_wVoiceRangeSlider.SetColor(Color.Yellow); m_wVoiceRangeText.SetColor(Color.Yellow); break; };
		};
	};
	
	//------------------------------------------------------------------------------------------------
	protected void HideVoiceRangeSlider()
	{
		GetGame().GetCallqueue().CallLater(HideBar, 0, true);
	};


	//------------------------------------------------------------------------------------------------
	protected void HideBar()
	{
		float currentSliderOpacity = m_wVoiceRangeSlider.GetOpacity();
		float currentTextOpacity = m_wVoiceRangeText.GetOpacity();
		
		if (currentSliderOpacity <= 0 && currentTextOpacity <= 0) {
			GetGame().GetCallqueue().Remove(HideBar);
			return;
		};
		
		m_wVoiceRangeSlider.SetOpacity(currentSliderOpacity - 0.025);
		m_wVoiceRangeText.SetOpacity(currentTextOpacity - 0.025);
	}
};