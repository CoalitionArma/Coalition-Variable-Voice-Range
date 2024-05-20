[ComponentEditorProps(category: "GameScripted/UI", description: "CVVR Master Component", color: "0 0 255 255")]
class CVVR_VoNMasterComponentClass: ScriptComponentClass {};

class CVVR_VoNMasterComponent: ScriptComponent {
	
	protected SCR_VONController m_VONController;
	
	[RplProp()]
	protected int m_iLocalVoiceRange = 4;
	
	int GetLocalVoiceRange() {
		return m_iLocalVoiceRange;
	}
	
	void SetLocalVoiceRange(int voiceRangeChange) {
		int voiceRangeSum = m_iLocalVoiceRange + voiceRangeChange;
		
		if (voiceRangeSum >= 1 && voiceRangeSum <= 6) {
			m_iLocalVoiceRange = voiceRangeSum;
			
			Replication.BumpMe();
		
			m_VONController = SCR_VONController.Cast(GetGame().GetPlayerController().FindComponent(SCR_VONController));
		
			if(m_VONController)
				m_VONController.ReloadVONForRangeChange();
		};
	}
};

[ComponentEditorProps(category: "GameComponent", description: "CVVR Range Component", color: "0 0 255 255")]
class CVVR_VoNComponentRangeOneClass: VoNComponentClass {};

class CVVR_VoNComponentRangeOne: VoNComponent {
	private const float TRANSMISSION_TIMEOUT_MS = 500;
	private float m_fTransmitingTimeout;
	private float m_fTransmitRadioTimeout;
	private SCR_VonDisplay m_VONDisplay;
	
	ref ScriptInvoker m_OnReceivedVON = new ScriptInvoker();
	ref ScriptInvoker m_OnCaptureVON = new ScriptInvoker();
	
	//------------------------------------------------------------------------------------------------
	//! Get Display Info script
	//! \return SCR_VonDisplay reference
	SCR_VonDisplay GetDisplay()
	{
		if (m_VONDisplay)
			return m_VONDisplay;
		
		SCR_VONController vonContr = SCR_VONController.Cast(GetGame().GetPlayerController().FindComponent(SCR_VONController));
		if (vonContr)
		{
			m_VONDisplay = vonContr.GetDisplay();
			return m_VONDisplay;
		}
		
		return null;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Simple getter for when transmission was recently active, we assume that player's avatar
	//! should not produce various sounds while he's talking.
	//! \return true if recently active
	bool IsTransmiting()
	{
		return m_fTransmitingTimeout > GetGame().GetWorld().GetWorldTime();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Radio transmission was recently active
	//! \return true if recently active
	bool IsTransmitingRadio()
	{
		return m_fTransmitRadioTimeout > GetGame().GetWorld().GetWorldTime();
	}
	
	//------------------------------------------------------------------------------------------------
	override protected event void OnCapture(BaseTransceiver transmitter)
	{
		SCR_VonDisplay display = GetDisplay();
		
		if (display)
			display.OnCapture(transmitter);
		
		m_OnCaptureVON.Invoke(transmitter);
		
		m_fTransmitingTimeout = GetGame().GetWorld().GetWorldTime() + TRANSMISSION_TIMEOUT_MS;
		
		if (transmitter)
			m_fTransmitRadioTimeout = GetGame().GetWorld().GetWorldTime() + TRANSMISSION_TIMEOUT_MS;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected event void OnReceive(int playerId, BaseTransceiver receiver, int frequency, float quality)
	{
		SCR_VonDisplay display = GetDisplay();
		
		if (display)
			display.OnReceive(playerId, receiver, frequency, quality);

		m_OnReceivedVON.Invoke(playerId, receiver, frequency, quality);
	}
};

[ComponentEditorProps(category: "GameComponent", description: "CVVR Range Component", color: "0 0 255 255")]
class CVVR_VoNComponentRangeTwoClass: VoNComponentClass {};

class CVVR_VoNComponentRangeTwo: VoNComponent {
	private const float TRANSMISSION_TIMEOUT_MS = 500;
	private float m_fTransmitingTimeout;
	private float m_fTransmitRadioTimeout;
	private SCR_VonDisplay m_VONDisplay;
	
	ref ScriptInvoker m_OnReceivedVON = new ScriptInvoker();
	ref ScriptInvoker m_OnCaptureVON = new ScriptInvoker();
	
	//------------------------------------------------------------------------------------------------
	//! Get Display Info script
	//! \return SCR_VonDisplay reference
	SCR_VonDisplay GetDisplay()
	{
		if (m_VONDisplay)
			return m_VONDisplay;
		
		SCR_VONController vonContr = SCR_VONController.Cast(GetGame().GetPlayerController().FindComponent(SCR_VONController));
		if (vonContr)
		{
			m_VONDisplay = vonContr.GetDisplay();
			return m_VONDisplay;
		}
		
		return null;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Simple getter for when transmission was recently active, we assume that player's avatar
	//! should not produce various sounds while he's talking.
	//! \return true if recently active
	bool IsTransmiting()
	{
		return m_fTransmitingTimeout > GetGame().GetWorld().GetWorldTime();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Radio transmission was recently active
	//! \return true if recently active
	bool IsTransmitingRadio()
	{
		return m_fTransmitRadioTimeout > GetGame().GetWorld().GetWorldTime();
	}
	
	//------------------------------------------------------------------------------------------------
	override protected event void OnCapture(BaseTransceiver transmitter)
	{
		SCR_VonDisplay display = GetDisplay();
		
		if (display)
			display.OnCapture(transmitter);
		
		m_OnCaptureVON.Invoke(transmitter);
		
		m_fTransmitingTimeout = GetGame().GetWorld().GetWorldTime() + TRANSMISSION_TIMEOUT_MS;
		
		if (transmitter)
			m_fTransmitRadioTimeout = GetGame().GetWorld().GetWorldTime() + TRANSMISSION_TIMEOUT_MS;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected event void OnReceive(int playerId, BaseTransceiver receiver, int frequency, float quality)
	{
		SCR_VonDisplay display = GetDisplay();
		
		if (display)
			display.OnReceive(playerId, receiver, frequency, quality);

		m_OnReceivedVON.Invoke(playerId, receiver, frequency, quality);
	}
};

[ComponentEditorProps(category: "GameComponent", description: "CVVR Range Component", color: "0 0 255 255")]
class CVVR_VoNComponentRangeThreeClass: VoNComponentClass {};

class CVVR_VoNComponentRangeThree: VoNComponent {
	private const float TRANSMISSION_TIMEOUT_MS = 500;
	private float m_fTransmitingTimeout;
	private float m_fTransmitRadioTimeout;
	private SCR_VonDisplay m_VONDisplay;
	
	ref ScriptInvoker m_OnReceivedVON = new ScriptInvoker();
	ref ScriptInvoker m_OnCaptureVON = new ScriptInvoker();
	
	//------------------------------------------------------------------------------------------------
	//! Get Display Info script
	//! \return SCR_VonDisplay reference
	SCR_VonDisplay GetDisplay()
	{
		if (m_VONDisplay)
			return m_VONDisplay;
		
		SCR_VONController vonContr = SCR_VONController.Cast(GetGame().GetPlayerController().FindComponent(SCR_VONController));
		if (vonContr)
		{
			m_VONDisplay = vonContr.GetDisplay();
			return m_VONDisplay;
		}
		
		return null;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Simple getter for when transmission was recently active, we assume that player's avatar
	//! should not produce various sounds while he's talking.
	//! \return true if recently active
	bool IsTransmiting()
	{
		return m_fTransmitingTimeout > GetGame().GetWorld().GetWorldTime();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Radio transmission was recently active
	//! \return true if recently active
	bool IsTransmitingRadio()
	{
		return m_fTransmitRadioTimeout > GetGame().GetWorld().GetWorldTime();
	}
	
	//------------------------------------------------------------------------------------------------
	override protected event void OnCapture(BaseTransceiver transmitter)
	{
		SCR_VonDisplay display = GetDisplay();
		
		if (display)
			display.OnCapture(transmitter);
		
		m_OnCaptureVON.Invoke(transmitter);
		
		m_fTransmitingTimeout = GetGame().GetWorld().GetWorldTime() + TRANSMISSION_TIMEOUT_MS;
		
		if (transmitter)
			m_fTransmitRadioTimeout = GetGame().GetWorld().GetWorldTime() + TRANSMISSION_TIMEOUT_MS;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected event void OnReceive(int playerId, BaseTransceiver receiver, int frequency, float quality)
	{
		SCR_VonDisplay display = GetDisplay();
		
		if (display)
			display.OnReceive(playerId, receiver, frequency, quality);

		m_OnReceivedVON.Invoke(playerId, receiver, frequency, quality);
	}
};

[ComponentEditorProps(category: "GameComponent", description: "CVVR Range Component", color: "0 0 255 255")]
class CVVR_VoNComponentRangeFourClass: VoNComponentClass {};

class CVVR_VoNComponentRangeFour: VoNComponent {
	private const float TRANSMISSION_TIMEOUT_MS = 500;
	private float m_fTransmitingTimeout;
	private float m_fTransmitRadioTimeout;
	private SCR_VonDisplay m_VONDisplay;
	
	ref ScriptInvoker m_OnReceivedVON = new ScriptInvoker();
	ref ScriptInvoker m_OnCaptureVON = new ScriptInvoker();
	
	//------------------------------------------------------------------------------------------------
	//! Get Display Info script
	//! \return SCR_VonDisplay reference
	SCR_VonDisplay GetDisplay()
	{
		if (m_VONDisplay)
			return m_VONDisplay;
		
		SCR_VONController vonContr = SCR_VONController.Cast(GetGame().GetPlayerController().FindComponent(SCR_VONController));
		if (vonContr)
		{
			m_VONDisplay = vonContr.GetDisplay();
			return m_VONDisplay;
		}
		
		return null;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Simple getter for when transmission was recently active, we assume that player's avatar
	//! should not produce various sounds while he's talking.
	//! \return true if recently active
	bool IsTransmiting()
	{
		return m_fTransmitingTimeout > GetGame().GetWorld().GetWorldTime();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Radio transmission was recently active
	//! \return true if recently active
	bool IsTransmitingRadio()
	{
		return m_fTransmitRadioTimeout > GetGame().GetWorld().GetWorldTime();
	}
	
	//------------------------------------------------------------------------------------------------
	override protected event void OnCapture(BaseTransceiver transmitter)
	{
		SCR_VonDisplay display = GetDisplay();
		
		if (display)
			display.OnCapture(transmitter);
		
		m_OnCaptureVON.Invoke(transmitter);
		
		m_fTransmitingTimeout = GetGame().GetWorld().GetWorldTime() + TRANSMISSION_TIMEOUT_MS;
		
		if (transmitter)
			m_fTransmitRadioTimeout = GetGame().GetWorld().GetWorldTime() + TRANSMISSION_TIMEOUT_MS;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected event void OnReceive(int playerId, BaseTransceiver receiver, int frequency, float quality)
	{
		SCR_VonDisplay display = GetDisplay();
		
		if (display)
			display.OnReceive(playerId, receiver, frequency, quality);

		m_OnReceivedVON.Invoke(playerId, receiver, frequency, quality);
	}
};

[ComponentEditorProps(category: "GameComponent", description: "CVVR Range Component", color: "0 0 255 255")]
class CVVR_VoNComponentRangeFiveClass: VoNComponentClass {};

class CVVR_VoNComponentRangeFive: VoNComponent {
	private const float TRANSMISSION_TIMEOUT_MS = 500;
	private float m_fTransmitingTimeout;
	private float m_fTransmitRadioTimeout;
	private SCR_VonDisplay m_VONDisplay;
	
	ref ScriptInvoker m_OnReceivedVON = new ScriptInvoker();
	ref ScriptInvoker m_OnCaptureVON = new ScriptInvoker();
	
	//------------------------------------------------------------------------------------------------
	//! Get Display Info script
	//! \return SCR_VonDisplay reference
	SCR_VonDisplay GetDisplay()
	{
		if (m_VONDisplay)
			return m_VONDisplay;
		
		SCR_VONController vonContr = SCR_VONController.Cast(GetGame().GetPlayerController().FindComponent(SCR_VONController));
		if (vonContr)
		{
			m_VONDisplay = vonContr.GetDisplay();
			return m_VONDisplay;
		}
		
		return null;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Simple getter for when transmission was recently active, we assume that player's avatar
	//! should not produce various sounds while he's talking.
	//! \return true if recently active
	bool IsTransmiting()
	{
		return m_fTransmitingTimeout > GetGame().GetWorld().GetWorldTime();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Radio transmission was recently active
	//! \return true if recently active
	bool IsTransmitingRadio()
	{
		return m_fTransmitRadioTimeout > GetGame().GetWorld().GetWorldTime();
	}
	
	//------------------------------------------------------------------------------------------------
	override protected event void OnCapture(BaseTransceiver transmitter)
	{
		SCR_VonDisplay display = GetDisplay();
		
		if (display)
			display.OnCapture(transmitter);
		
		m_OnCaptureVON.Invoke(transmitter);
		
		m_fTransmitingTimeout = GetGame().GetWorld().GetWorldTime() + TRANSMISSION_TIMEOUT_MS;
		
		if (transmitter)
			m_fTransmitRadioTimeout = GetGame().GetWorld().GetWorldTime() + TRANSMISSION_TIMEOUT_MS;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected event void OnReceive(int playerId, BaseTransceiver receiver, int frequency, float quality)
	{
		SCR_VonDisplay display = GetDisplay();
		
		if (display)
			display.OnReceive(playerId, receiver, frequency, quality);

		m_OnReceivedVON.Invoke(playerId, receiver, frequency, quality);
	}
};

[ComponentEditorProps(category: "GameComponent", description: "CVVR Range Component", color: "0 0 255 255")]
class CVVR_VoNComponentRangeSixClass: VoNComponentClass {};

class CVVR_VoNComponentRangeSix: VoNComponent {
	private const float TRANSMISSION_TIMEOUT_MS = 500;
	private float m_fTransmitingTimeout;
	private float m_fTransmitRadioTimeout;
	private SCR_VonDisplay m_VONDisplay;
	
	ref ScriptInvoker m_OnReceivedVON = new ScriptInvoker();
	ref ScriptInvoker m_OnCaptureVON = new ScriptInvoker();
	
	//------------------------------------------------------------------------------------------------
	//! Get Display Info script
	//! \return SCR_VonDisplay reference
	SCR_VonDisplay GetDisplay()
	{
		if (m_VONDisplay)
			return m_VONDisplay;
		
		SCR_VONController vonContr = SCR_VONController.Cast(GetGame().GetPlayerController().FindComponent(SCR_VONController));
		if (vonContr)
		{
			m_VONDisplay = vonContr.GetDisplay();
			return m_VONDisplay;
		}
		
		return null;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Simple getter for when transmission was recently active, we assume that player's avatar
	//! should not produce various sounds while he's talking.
	//! \return true if recently active
	bool IsTransmiting()
	{
		return m_fTransmitingTimeout > GetGame().GetWorld().GetWorldTime();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Radio transmission was recently active
	//! \return true if recently active
	bool IsTransmitingRadio()
	{
		return m_fTransmitRadioTimeout > GetGame().GetWorld().GetWorldTime();
	}
	
	//------------------------------------------------------------------------------------------------
	override protected event void OnCapture(BaseTransceiver transmitter)
	{
		SCR_VonDisplay display = GetDisplay();
		
		if (display)
			display.OnCapture(transmitter);
		
		m_OnCaptureVON.Invoke(transmitter);
		
		m_fTransmitingTimeout = GetGame().GetWorld().GetWorldTime() + TRANSMISSION_TIMEOUT_MS;
		
		if (transmitter)
			m_fTransmitRadioTimeout = GetGame().GetWorld().GetWorldTime() + TRANSMISSION_TIMEOUT_MS;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected event void OnReceive(int playerId, BaseTransceiver receiver, int frequency, float quality)
	{
		SCR_VonDisplay display = GetDisplay();
		
		if (display)
			display.OnReceive(playerId, receiver, frequency, quality);

		m_OnReceivedVON.Invoke(playerId, receiver, frequency, quality);
	}
};