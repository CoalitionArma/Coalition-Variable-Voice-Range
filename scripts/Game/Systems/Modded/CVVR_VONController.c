
modded class SCR_VONController : ScriptComponent
{
	void ReloadVONForRangeChange() {
		OnVONToggle(0,0);
	}
	
	//------------------------------------------------------------------------------------------------
	//! SCR_PlayerController Event
	//! Used to reinit VON when new entity is controlled
	override protected void OnControlledEntityChanged(IEntity from, IEntity to)
	{				
		if (m_bIsToggledDirect || m_bIsToggledChannel)
			OnVONToggle(0,0);
		
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
		CVVR_VoNMasterComponent vonMasterComponent = CVVR_VoNMasterComponent.Cast(SCR_PlayerController.Cast(GetOwner()).GetControlledEntity().FindComponent(CVVR_VoNMasterComponent));
		
		if(!vonMasterComponent)
			return false;
		
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
				case 1  : {CVVR_VoNComponentRangeOne.Cast(ent.FindComponent(CVVR_VoNComponentRangeOne)).SetCapture(true);     break;};
				case 2  : {CVVR_VoNComponentRangeTwo.Cast(ent.FindComponent(CVVR_VoNComponentRangeTwo)).SetCapture(true);     break;};
				case 3  : {CVVR_VoNComponentRangeThree.Cast(ent.FindComponent(CVVR_VoNComponentRangeThree)).SetCapture(true); break;};
				case 5  : {CVVR_VoNComponentRangeFive.Cast(ent.FindComponent(CVVR_VoNComponentRangeFive)).SetCapture(true);   break;};
				case 6  : {CVVR_VoNComponentRangeSix.Cast(ent.FindComponent(CVVR_VoNComponentRangeSix)).SetCapture(true);     break;};
				default : {CVVR_VoNComponentRangeFour.Cast(ent.FindComponent(CVVR_VoNComponentRangeFour)).SetCapture(true)};
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
		CVVR_VoNMasterComponent vonMasterComponent = CVVR_VoNMasterComponent.Cast(ent.FindComponent(CVVR_VoNMasterComponent));
		
		if (vonMasterComponent) {	
			switch (vonMasterComponent.GetLocalVoiceRange()) {
				case 1  : {CVVR_VoNComponentRangeOne.Cast(ent.FindComponent(CVVR_VoNComponentRangeOne)).SetCapture(false);     break;};
				case 2  : {CVVR_VoNComponentRangeTwo.Cast(ent.FindComponent(CVVR_VoNComponentRangeTwo)).SetCapture(false);     break;};
				case 3  : {CVVR_VoNComponentRangeThree.Cast(ent.FindComponent(CVVR_VoNComponentRangeThree)).SetCapture(false); break;};
				case 5  : {CVVR_VoNComponentRangeFive.Cast(ent.FindComponent(CVVR_VoNComponentRangeFive)).SetCapture(false);   break;};
				case 6  : {CVVR_VoNComponentRangeSix.Cast(ent.FindComponent(CVVR_VoNComponentRangeSix)).SetCapture(false);     break;};
				default : {CVVR_VoNComponentRangeFour.Cast(ent.FindComponent(CVVR_VoNComponentRangeFour)).SetCapture(false)};
			};
		};
		
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
					CVVR_VoNComponentRangeOne vonComp = CVVR_VoNComponentRangeOne.Cast(ent.FindComponent(CVVR_VoNComponentRangeOne));
					if (entry.GetVONMethod() == ECommMethod.SQUAD_RADIO)
					{
						vonComp.SetCommMethod(ECommMethod.SQUAD_RADIO);
						vonComp.SetTransmitRadio(SCR_VONEntryRadio.Cast(entry).GetTransceiver());
						SetEntryActive(entry);
					}
					else 
					{
						vonComp.SetCommMethod(ECommMethod.DIRECT);
						vonComp.SetTransmitRadio(null);
					}	
					break;
				};
				//------------------------------------------------------------------------------------------------
				case 2 : {
					CVVR_VoNComponentRangeTwo vonComp = CVVR_VoNComponentRangeTwo.Cast(ent.FindComponent(CVVR_VoNComponentRangeTwo));
					if (entry.GetVONMethod() == ECommMethod.SQUAD_RADIO)
					{
						vonComp.SetCommMethod(ECommMethod.SQUAD_RADIO);
						vonComp.SetTransmitRadio(SCR_VONEntryRadio.Cast(entry).GetTransceiver());
						SetEntryActive(entry);
					}
					else 
					{
						vonComp.SetCommMethod(ECommMethod.DIRECT);
						vonComp.SetTransmitRadio(null);
					}	
					break;
				};
				//------------------------------------------------------------------------------------------------
				case 3 : {
					CVVR_VoNComponentRangeThree vonComp = CVVR_VoNComponentRangeThree.Cast(ent.FindComponent(CVVR_VoNComponentRangeThree));
					if (entry.GetVONMethod() == ECommMethod.SQUAD_RADIO)
					{
						vonComp.SetCommMethod(ECommMethod.SQUAD_RADIO);
						vonComp.SetTransmitRadio(SCR_VONEntryRadio.Cast(entry).GetTransceiver());
						SetEntryActive(entry);
					}
					else 
					{
						vonComp.SetCommMethod(ECommMethod.DIRECT);
						vonComp.SetTransmitRadio(null);
					}	
					break;
				};
				//------------------------------------------------------------------------------------------------
				case 5 : {
					CVVR_VoNComponentRangeFive vonComp = CVVR_VoNComponentRangeFive.Cast(ent.FindComponent(CVVR_VoNComponentRangeFive));
					if (entry.GetVONMethod() == ECommMethod.SQUAD_RADIO)
					{
						vonComp.SetCommMethod(ECommMethod.SQUAD_RADIO);
						vonComp.SetTransmitRadio(SCR_VONEntryRadio.Cast(entry).GetTransceiver());
						SetEntryActive(entry);
					}
					else 
					{
						vonComp.SetCommMethod(ECommMethod.DIRECT);
						vonComp.SetTransmitRadio(null);
					}	
					break;
				};
				//------------------------------------------------------------------------------------------------
				case 6 : {
					CVVR_VoNComponentRangeSix vonComp = CVVR_VoNComponentRangeSix.Cast(ent.FindComponent(CVVR_VoNComponentRangeSix));
					if (entry.GetVONMethod() == ECommMethod.SQUAD_RADIO)
					{
						vonComp.SetCommMethod(ECommMethod.SQUAD_RADIO);
						vonComp.SetTransmitRadio(SCR_VONEntryRadio.Cast(entry).GetTransceiver());
						SetEntryActive(entry);
					}
					else 
					{
						vonComp.SetCommMethod(ECommMethod.DIRECT);
						vonComp.SetTransmitRadio(null);
					}	
					break;
				};
				//------------------------------------------------------------------------------------------------
				default : {
					CVVR_VoNComponentRangeFour vonComp = CVVR_VoNComponentRangeFour.Cast(ent.FindComponent(CVVR_VoNComponentRangeFour));
					if (entry.GetVONMethod() == ECommMethod.SQUAD_RADIO)
					{
						vonComp.SetCommMethod(ECommMethod.SQUAD_RADIO);
						vonComp.SetTransmitRadio(SCR_VONEntryRadio.Cast(entry).GetTransceiver());
						SetEntryActive(entry);
					}
					else 
					{
						vonComp.SetCommMethod(ECommMethod.DIRECT);
						vonComp.SetTransmitRadio(null);
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