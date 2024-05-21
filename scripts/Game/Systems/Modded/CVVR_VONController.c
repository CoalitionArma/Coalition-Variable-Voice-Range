
modded class SCR_VONController : ScriptComponent
{
	protected CVVR_VoNComponentRangeOne m_VoNComponentRangeOne;    
	protected CVVR_VoNComponentRangeTwo m_VoNComponentRangeTwo;    
	protected CVVR_VoNComponentRangeThree m_VoNComponentRangeThree;    
	protected CVVR_VoNComponentRangeFour m_VoNComponentRangeFour;    
	protected CVVR_VoNComponentRangeFive m_VoNComponentRangeFive;    
	//protected CVVR_VoNComponentRangeSix m_VoNComponentRangeSix;
	protected CVVR_VoNMasterComponent m_VoNMasterComponent; 
	
	void ReloadVONForRangeChange() {
		DeactivateVON(EVONTransmitType.DIRECT);
		OnVONToggle(0,0);
	}
	
	//------------------------------------------------------------------------------------------------
	//! SCR_PlayerController Event
	//! Used to reinit VON when new entity is controlled
	override protected void OnControlledEntityChanged(IEntity from, IEntity to)
	{				
		if (m_bIsToggledDirect || m_bIsToggledChannel)
			OnVONToggle(0,0);
		
		m_VoNComponentRangeOne   = CVVR_VoNComponentRangeOne.Cast(to.FindComponent(CVVR_VoNComponentRangeOne));    
		m_VoNComponentRangeTwo   = CVVR_VoNComponentRangeTwo.Cast(to.FindComponent(CVVR_VoNComponentRangeTwo));    
		m_VoNComponentRangeThree = CVVR_VoNComponentRangeThree.Cast(to.FindComponent(CVVR_VoNComponentRangeThree));    
		m_VoNComponentRangeFour  = CVVR_VoNComponentRangeFour.Cast(to.FindComponent(CVVR_VoNComponentRangeFour));    
		m_VoNComponentRangeFive  = CVVR_VoNComponentRangeFive.Cast(to.FindComponent(CVVR_VoNComponentRangeFive));    
		//m_VoNComponentRangeSix   = CVVR_VoNComponentRangeSix.Cast(to.FindComponent(CVVR_VoNComponentRangeSix));
		m_VoNMasterComponent     = CVVR_VoNMasterComponent.Cast(to.FindComponent(CVVR_VoNMasterComponent));
		
		m_sLocalEncryptionKey = string.Empty;	
		
		if (from)
		{
			ChimeraCharacter character = ChimeraCharacter.Cast(from);
			if (character)
			{	
				SCR_CharacterControllerComponent controller = SCR_CharacterControllerComponent.Cast(character.GetCharacterController());
				if (controller)
					controller.m_OnLifeStateChanged.Remove(OnLifeStateChanged);
			}
		}
		
		bool unconsciousVONEnabled;
		SCR_BaseGameMode baseGameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		if (!baseGameMode)
			return;
		
		SCR_GameModeHealthSettings healthSettingsComp = SCR_GameModeHealthSettings.Cast(baseGameMode.GetGameModeHealthSettings());
		if (healthSettingsComp)
			unconsciousVONEnabled = healthSettingsComp.IsUnconsciousVONPermitted();
		
		if (unconsciousVONEnabled)
			return;
		
		if (to)		
		{
			ChimeraCharacter character = ChimeraCharacter.Cast(to);
			if (!character)
				return;
			
			SCR_CharacterControllerComponent controller = SCR_CharacterControllerComponent.Cast(character.GetCharacterController());
			if (controller)
			{
				m_bIsUnconscious = controller.IsUnconscious();
				controller.m_OnLifeStateChanged.Insert(OnLifeStateChanged);
			}
			
			UpdateSystemState();
		}
	}
	
	//------------------------------------------------------------------------------------------------
	//! Assign VON comp by fetching it from controlled entity
	override bool AssignVONComponent()
	{
		IEntity ent = SCR_PlayerController.Cast(GetOwner()).GetControlledEntity();
		m_VoNMasterComponent = CVVR_VoNMasterComponent.Cast(SCR_PlayerController.Cast(GetOwner()).GetControlledEntity().FindComponent(CVVR_VoNMasterComponent));
		
		if(!m_VoNMasterComponent || !ent)
			return false;
		
		m_VoNComponentRangeOne   = CVVR_VoNComponentRangeOne.Cast(ent.FindComponent(CVVR_VoNComponentRangeOne));    
		m_VoNComponentRangeTwo   = CVVR_VoNComponentRangeTwo.Cast(ent.FindComponent(CVVR_VoNComponentRangeTwo));    
		m_VoNComponentRangeThree = CVVR_VoNComponentRangeThree.Cast(ent.FindComponent(CVVR_VoNComponentRangeThree));    
		m_VoNComponentRangeFour  = CVVR_VoNComponentRangeFour.Cast(ent.FindComponent(CVVR_VoNComponentRangeFour));    
		m_VoNComponentRangeFive  = CVVR_VoNComponentRangeFive.Cast(ent.FindComponent(CVVR_VoNComponentRangeFive));    
		//m_VoNComponentRangeSix   = CVVR_VoNComponentRangeSix.Cast(ent.FindComponent(CVVR_VoNComponentRangeSix));
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	//! VON activation
	//! \param entry is the subject VON entry
 	override protected void ActivateVON(EVONTransmitType transmitType)
	{				
		m_eVONType = transmitType;
		SCR_VONEntry entry = GetEntryByTransmitType(transmitType);
				
		if (!GetGame().GetVONCanTransmitCrossFaction())		// is cross faction transmit disabled
		{
			if (!m_sLocalEncryptionKey)
				InitEncryptionKey();
		
			SCR_VONEntryRadio radioEntry = SCR_VONEntryRadio.Cast(entry);
			if (radioEntry && m_sLocalEncryptionKey != string.Empty && radioEntry.GetTransceiver().GetRadio().GetEncryptionKey() != m_sLocalEncryptionKey)
			{
				m_VONDisplay.ShowSelectedVONDisabledHint(true);
				return;
			}
		}
					
		SetActiveTransmit(entry);
		
		IEntity ent = SCR_PlayerController.Cast(GetOwner()).GetControlledEntity();
		CVVR_VoNMasterComponent vonMasterComponent = CVVR_VoNMasterComponent.Cast(ent.FindComponent(CVVR_VoNMasterComponent));
		
		if (vonMasterComponent) {	
			switch (vonMasterComponent.GetLocalVoiceRange()) {
				case 1  : {m_VoNComponentRangeOne.SetCapture(true);   break;};
				case 2  : {m_VoNComponentRangeTwo.SetCapture(true);   break;};
				case 3  : {m_VoNComponentRangeThree.SetCapture(true); break;};
				case 4  : {m_VoNComponentRangeFour.SetCapture(true);  break;};
				case 5  : {m_VoNComponentRangeFive.SetCapture(true);  break;};
				default : {m_VoNComponentRangeThree.SetCapture(true);};
			};
			m_bIsActive = true;
		};
	}
	
	//------------------------------------------------------------------------------------------------
	//! Current VON deactivation
	override protected void DeactivateVON(EVONTransmitType transmitType = EVONTransmitType.NONE)
	{		
		if (transmitType != EVONTransmitType.NONE && transmitType != m_eVONType)	// only deactivate target type, in case other type was activated already
			return;
	
		m_eVONType = EVONTransmitType.NONE;
		m_bIsActive = false;
		
		IEntity ent = SCR_PlayerController.Cast(GetOwner()).GetControlledEntity();
		
		m_VoNComponentRangeOne.SetCapture(false);     
		m_VoNComponentRangeTwo.SetCapture(false);     
		m_VoNComponentRangeThree.SetCapture(false); 
		m_VoNComponentRangeFour.SetCapture(false);   
		m_VoNComponentRangeFive.SetCapture(false);     
		//m_VoNComponentRangeSix.SetCapture(false);
		
		m_sActiveHoldAction = string.Empty;
					
		if (m_bIsToggledDirect) 		// direct toggle is active so ending VON should not end capture
			ActivateVON(EVONTransmitType.DIRECT);
		else if (m_bIsToggledChannel)	// channel toggle is active so ending VON should not end capture
			ActivateVON(EVONTransmitType.CHANNEL);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Set transmission method depending on entry type when starting VON transmit
	override protected void SetActiveTransmit(notnull SCR_VONEntry entry)
	{		
		IEntity ent = SCR_PlayerController.Cast(GetOwner()).GetControlledEntity();
		CVVR_VoNMasterComponent vonMasterComponent = CVVR_VoNMasterComponent.Cast(ent.FindComponent(CVVR_VoNMasterComponent));
		
		if (vonMasterComponent) {	
			switch (vonMasterComponent.GetLocalVoiceRange()) {
				case 1 : {
					if (entry.GetVONMethod() == ECommMethod.SQUAD_RADIO)
					{
						m_VoNComponentRangeOne.SetCommMethod(ECommMethod.SQUAD_RADIO);
						m_VoNComponentRangeOne.SetTransmitRadio(SCR_VONEntryRadio.Cast(entry).GetTransceiver());
						SetEntryActive(entry);
					}
					else 
					{
						m_VoNComponentRangeOne.SetCommMethod(ECommMethod.DIRECT);
						m_VoNComponentRangeOne.SetTransmitRadio(null);
					}	
					break;
				};
				//------------------------------------------------------------------------------------------------
				case 2 : {
					if (entry.GetVONMethod() == ECommMethod.SQUAD_RADIO)
					{
						m_VoNComponentRangeTwo.SetCommMethod(ECommMethod.SQUAD_RADIO);
						m_VoNComponentRangeTwo.SetTransmitRadio(SCR_VONEntryRadio.Cast(entry).GetTransceiver());
						SetEntryActive(entry);
					}
					else 
					{
						m_VoNComponentRangeTwo.SetCommMethod(ECommMethod.DIRECT);
						m_VoNComponentRangeTwo.SetTransmitRadio(null);
					}	
					break;
				};
				//------------------------------------------------------------------------------------------------
				case 3 : {
					if (entry.GetVONMethod() == ECommMethod.SQUAD_RADIO)
					{
						m_VoNComponentRangeThree.SetCommMethod(ECommMethod.SQUAD_RADIO);
						m_VoNComponentRangeThree.SetTransmitRadio(SCR_VONEntryRadio.Cast(entry).GetTransceiver());
						SetEntryActive(entry);
					}
					else 
					{
						m_VoNComponentRangeThree.SetCommMethod(ECommMethod.DIRECT);
						m_VoNComponentRangeThree.SetTransmitRadio(null);
					}	
					break;
				};
				//------------------------------------------------------------------------------------------------
				case 5 : {
					if (entry.GetVONMethod() == ECommMethod.SQUAD_RADIO)
					{
						m_VoNComponentRangeFive.SetCommMethod(ECommMethod.SQUAD_RADIO);
						m_VoNComponentRangeFive.SetTransmitRadio(SCR_VONEntryRadio.Cast(entry).GetTransceiver());
						SetEntryActive(entry);
					}
					else 
					{
						m_VoNComponentRangeFive.SetCommMethod(ECommMethod.DIRECT);
						m_VoNComponentRangeFive.SetTransmitRadio(null);
					}	
					break;
				};
				//------------------------------------------------------------------------------------------------
				/*case 6 : {
					if (entry.GetVONMethod() == ECommMethod.SQUAD_RADIO)
					{
						m_VoNComponentRangeSix.SetCommMethod(ECommMethod.SQUAD_RADIO);
						m_VoNComponentRangeSix.SetTransmitRadio(SCR_VONEntryRadio.Cast(entry).GetTransceiver());
						SetEntryActive(entry);
					}
					else 
					{
						m_VoNComponentRangeSix.SetCommMethod(ECommMethod.DIRECT);
						m_VoNComponentRangeSix.SetTransmitRadio(null);
					}	
					break;
				};*/
				//------------------------------------------------------------------------------------------------
				default : {
					if (entry.GetVONMethod() == ECommMethod.SQUAD_RADIO)
					{
						m_VoNComponentRangeThree.SetCommMethod(ECommMethod.SQUAD_RADIO);
						m_VoNComponentRangeThree.SetTransmitRadio(SCR_VONEntryRadio.Cast(entry).GetTransceiver());
						SetEntryActive(entry);
					}
					else 
					{
						m_VoNComponentRangeThree.SetCommMethod(ECommMethod.DIRECT);
						m_VoNComponentRangeThree.SetTransmitRadio(null);
					}	
					break;
				};
			};
		};
		
		if (entry == m_SavedEntry)
			m_SavedEntry = null;
	}
	
	//------------------------------------------------------------------------------------------------
	//! VON toggle   val 0 = off / 1 = direct / 2 = channel
 	override protected void OnVONToggle(float value, EActionTrigger reason)
	{
		if (value == 0)
		{
			m_fToggleOffDelay = 0;
			m_bIsToggledDirect = false;
			m_bIsToggledChannel = false;
			DeactivateVON();
		}
		else if (value == 1)
		{
			m_fToggleOffDelay = TOGGLE_OFF_DELAY;
			m_bIsToggledDirect = !m_bIsToggledDirect;
			m_bIsToggledChannel = false;
			
			if (m_bIsToggledDirect)
				ActivateVON(EVONTransmitType.DIRECT);
			else
				DeactivateVON();
		}
		else 
		{
			SetGamepadLRRMode(false);
			
			if (!m_ActiveEntry || !m_ActiveEntry.IsUsable())
				return;
			
			m_fToggleOffDelay = TOGGLE_OFF_DELAY;
			m_bIsToggledDirect = false;
			m_bIsToggledChannel = !m_bIsToggledChannel;
			
			if (m_bIsToggledChannel)
				ActivateVON(EVONTransmitType.CHANNEL);
			else 
				DeactivateVON();
		}
						
		m_OnVONActiveToggled.Invoke(m_bIsToggledDirect, m_bIsToggledChannel);
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void VONDirect(bool activate)
	{
		if (m_bIsToggledDirect && m_fToggleOffDelay <= 0)	// direct speech toggle is active, cancel it
			OnVONToggle(0,0);
		
		if (!m_DirectSpeechEntry.IsUsable())
			return;
		
		m_sActiveHoldAction = VON_DIRECT_HOLD;
		
		if (activate)
			ActivateVON(EVONTransmitType.DIRECT);
		else
			DeactivateVON(EVONTransmitType.DIRECT);
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void VONChannel(bool activate)
	{
		if (m_bIsToggledChannel && m_fToggleOffDelay <= 0) // channel speech toggle is active, cancel it
			OnVONToggle(0,0);
		
		if (!m_ActiveEntry || !m_ActiveEntry.IsUsable())	// if active entry disabled, use direct instead
		{
			VONDirect(activate);
			m_VONDisplay.ShowSelectedVONDisabledHint();
			return;
		}
		
		m_sActiveHoldAction = VON_CHANNEL_HOLD;
		
		if (activate)
			ActivateVON(EVONTransmitType.CHANNEL);
		else
			DeactivateVON(EVONTransmitType.CHANNEL);
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void VONLongRange(bool activate)
	{
		if ((m_bIsToggledChannel || m_bIsToggledDirect) && m_fToggleOffDelay <= 0) 
			OnVONToggle(0,0);
		
		if (!m_LongRangeEntry || !m_LongRangeEntry.IsUsable())	// if active entry disabled, use direct instead
		{
			VONDirect(activate);
			m_VONDisplay.ShowSelectedVONDisabledHint();
			return;
		}
		
		m_sActiveHoldAction = VON_LONG_RANGE_HOLD;
		
		if (activate)
		{
			if (!m_SavedEntry)	// saved entry to switch back to after using long range mode
				m_SavedEntry = m_ActiveEntry;
			
			m_bIsActiveModeDirect = false;
			
			ActivateVON(EVONTransmitType.LONG_RANGE);
		}
		else
		{
			DeactivateVON(EVONTransmitType.LONG_RANGE);	
			
			if (m_SavedEntry && !m_bIsUsingGamepad)	// restore last saved entry if there is one
				SetActiveTransmit(m_SavedEntry);
		}
	}
};