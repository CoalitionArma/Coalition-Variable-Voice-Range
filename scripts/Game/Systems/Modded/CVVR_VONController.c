
modded class SCR_VONController : ScriptComponent
{
	/*
	CVVR_AuthorityComponent m_AuthorityComponent;
	protected int m_iPlayerID;
	
	//------------------------------------------------------------------------------------------------
	// Reset VON input states and stop transmission
	override protected void ResetVON()
	{
		if (m_eVONType == EVONTransmitType.DIRECT)
		{
			DeactivateVON(EVONTransmitType.DIRECT);
			ActivateVON(EVONTransmitType.DIRECT);
		} else {
			SetVONProximityToggle(false);
			SetVONLongRange(false, false);
			DeactivateVON();
		};
	}
	
	//------------------------------------------------------------------------------------------------
	//! Set component for transmit of VON
	//! \param VONComp is the subject
	void UpdateVONComponent()
	{		
		m_AuthorityComponent = CVVR_AuthorityComponent.GetInstance();
			
		if (m_AuthorityComponent && GetGame().GetPlayerController()) {	
			m_iPlayerID          = GetGame().GetPlayerController().GetPlayerId();
			IEntity playerEnt    = GetGame().GetPlayerManager().GetPlayerControlledEntity(m_iPlayerID);
			
			if(!playerEnt)
				return;
			
			switch (m_AuthorityComponent.ReturnPlayerRange(m_iPlayerID)) {
				case 1  : {SetVONComponent(CVVR_VoNComponentRangeOne.Cast(playerEnt.FindComponent(CVVR_VoNComponentRangeOne)));     break;};
				case 2  : {SetVONComponent(CVVR_VoNComponentRangeTwo.Cast(playerEnt.FindComponent(CVVR_VoNComponentRangeTwo)));     break;};
				case 3  : {SetVONComponent(CVVR_VoNComponentRangeThree.Cast(playerEnt.FindComponent(CVVR_VoNComponentRangeThree))); break;};
				case 4  : {SetVONComponent(CVVR_VoNComponentRangeFour.Cast(playerEnt.FindComponent(CVVR_VoNComponentRangeFour)));   break;};
				case 5  : {SetVONComponent(CVVR_VoNComponentRangeFive.Cast(playerEnt.FindComponent(CVVR_VoNComponentRangeFive)));   break;};
				default : {SetVONComponent(CVVR_VoNComponentRangeThree.Cast(playerEnt.FindComponent(CVVR_VoNComponentRangeThree)));       };
			};
		} else {
			SetVONComponent(null);	
		};
		
		Rpc(RpcBroadcast_UpdateVONComponent, m_iPlayerID);
		Print(m_VONComp);
	};
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RpcBroadcast_UpdateVONComponent(int playerID)
	{
		CVVR_AuthorityComponent authorityComponent = CVVR_AuthorityComponent.GetInstance();
		if (authorityComponent && playerID != 0) {	
			IEntity playerEnt = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerID);
			
			if(!playerEnt)
				return;
			
			switch (authorityComponent.ReturnPlayerRange(playerID)) {
				case 1  : {SetVONComponent(CVVR_VoNComponentRangeOne.Cast(playerEnt.FindComponent(CVVR_VoNComponentRangeOne)));     break;};
				case 2  : {SetVONComponent(CVVR_VoNComponentRangeTwo.Cast(playerEnt.FindComponent(CVVR_VoNComponentRangeTwo)));     break;};
				case 3  : {SetVONComponent(CVVR_VoNComponentRangeThree.Cast(playerEnt.FindComponent(CVVR_VoNComponentRangeThree))); break;};
				case 4  : {SetVONComponent(CVVR_VoNComponentRangeFour.Cast(playerEnt.FindComponent(CVVR_VoNComponentRangeFour)));   break;};
				case 5  : {SetVONComponent(CVVR_VoNComponentRangeFive.Cast(playerEnt.FindComponent(CVVR_VoNComponentRangeFive)));   break;};
				default : {SetVONComponent(CVVR_VoNComponentRangeThree.Cast(playerEnt.FindComponent(CVVR_VoNComponentRangeThree)));       };
			};

			Print(m_VONComp);
		};
	}
	
	//------------------------------------------------------------------------------------------------
	//! SCR_PlayerController Event
	//! Used to reinit VON when new entity is controlled
	override protected void OnControlledEntityChanged(IEntity from, IEntity to)
	{				
		ResetVON();
		
		m_sLocalEncryptionKey = string.Empty;
		
		UpdateVONComponent();
		
		ChimeraCharacter previous = ChimeraCharacter.Cast(from);
		if (previous)
		{
			SCR_CharacterControllerComponent controller = SCR_CharacterControllerComponent.Cast(previous.GetCharacterController());
			if (controller)
				controller.m_OnLifeStateChanged.Remove(OnLifeStateChanged);
		}
		
		ChimeraCharacter character = ChimeraCharacter.Cast(to);
		if (character)
		{
			SCR_CharacterControllerComponent controller = SCR_CharacterControllerComponent.Cast(character.GetCharacterController());
			if (controller)
			{
				ECharacterLifeState lifeState = controller.GetLifeState();
				OnLifeStateChanged(lifeState, lifeState);
				controller.m_OnLifeStateChanged.Insert(OnLifeStateChanged);
			}
		}

		UpdateSystemState();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Assign VON comp by fetching it from controlled entity
	override bool AssignVONComponent()
	{
		ResetVON();
		
		UpdateVONComponent();
		
		if(!m_VONComp)
			return false;
		
		return true;			
	}
	
	//------------------------------------------------------------------------------------------------
	//! Current VON deactivation
	override protected void DeactivateVON(EVONTransmitType transmitType = EVONTransmitType.NONE)
	{
		if (!m_VONComp)
			return; 
		
		if (transmitType != EVONTransmitType.NONE && transmitType != m_eVONType)	// only deactivate target type, in case other type was activated already
			return;
	
		m_eVONType = EVONTransmitType.NONE;
		m_bIsActive = false;
		m_sActiveHoldAction = string.Empty;
		
		IEntity controlledEnt = SCR_PlayerController.Cast(GetOwner()).GetControlledEntity();
		
		if (controlledEnt) {	
			CVVR_VoNComponentRangeOne.Cast(controlledEnt.FindComponent(CVVR_VoNComponentRangeOne)).SetCapture(false);
			CVVR_VoNComponentRangeTwo.Cast(controlledEnt.FindComponent(CVVR_VoNComponentRangeTwo)).SetCapture(false);
			CVVR_VoNComponentRangeThree.Cast(controlledEnt.FindComponent(CVVR_VoNComponentRangeThree)).SetCapture(false);
			CVVR_VoNComponentRangeFour.Cast(controlledEnt.FindComponent(CVVR_VoNComponentRangeFour)).SetCapture(false);
			CVVR_VoNComponentRangeFive.Cast(controlledEnt.FindComponent(CVVR_VoNComponentRangeFive)).SetCapture(false);
		};
					
		// direct toggle is active so ending VON should not end capture
		if (m_bIsToggledDirect)
			ActivateVON(EVONTransmitType.DIRECT);
	}
	
	void ReloadVONForRangeChange()
	{	
		UpdateVONComponent();
		
		ResetVON();
		DisconnectFromHandleUpdateVONControllersSystem();
		ConnectToHandleUpdateVONControllersSystem();
	}
	*/
};