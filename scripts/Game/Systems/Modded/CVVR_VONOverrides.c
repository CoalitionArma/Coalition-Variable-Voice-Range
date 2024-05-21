//------------------------------------------------------------------------------------------------

//SCR_NameTagData

//------------------------------------------------------------------------------------------------

modded class SCR_NameTagData : Managed
{	
	//------------------------------------------------------------------------------------------------
	//! Init nametag data class, the widget is held and reused, data has to be reinitialized 
	//! \param display is nametag info display
	//! \param entity is nametag subject
	//! \return false if nametag is not valid and should be cleaned up
	override bool InitTag(SCR_NameTagDisplay display, IEntity entity, SCR_NameTagConfig config, bool IsCurrentPlayer = false)
	{		
		m_bIsCurrentPlayer = IsCurrentPlayer;
		m_Entity = entity;
		m_NTDisplay = display;
		
		ChimeraCharacter ent = ChimeraCharacter.Cast(entity);
		if (ent)
		{
			m_CharController = SCR_CharacterControllerComponent.Cast(ent.FindComponent(SCR_CharacterControllerComponent));
			if (!m_CharController || m_CharController.IsDead())
				return false;	
			
			m_CharController.m_OnLifeStateChanged.Insert(OnLifeStateChanged);
			
			if (m_bIsCurrentPlayer)		// we only need VON received event for current player to set VON status icons
			{
				CVVR_VoNMasterComponent vonMasterComponent = CVVR_VoNMasterComponent.Cast(ent.FindComponent(CVVR_VoNMasterComponent));
			
				if (vonMasterComponent) {	
					switch (vonMasterComponent.GetLocalVoiceRange()) {
						case 1  : {CVVR_VoNComponentRangeOne.Cast(ent.FindComponent(CVVR_VoNComponentRangeOne)).m_OnReceivedVON.Insert(OnReceivedVON);     break;};
						case 2  : {CVVR_VoNComponentRangeTwo.Cast(ent.FindComponent(CVVR_VoNComponentRangeTwo)).m_OnReceivedVON.Insert(OnReceivedVON);     break;};
						case 3  : {CVVR_VoNComponentRangeThree.Cast(ent.FindComponent(CVVR_VoNComponentRangeThree)).m_OnReceivedVON.Insert(OnReceivedVON); break;};
						case 4  : {CVVR_VoNComponentRangeFour.Cast(ent.FindComponent(CVVR_VoNComponentRangeFour)).m_OnReceivedVON.Insert(OnReceivedVON);  break;}
						case 5  : {CVVR_VoNComponentRangeFive.Cast(ent.FindComponent(CVVR_VoNComponentRangeFive)).m_OnReceivedVON.Insert(OnReceivedVON);   break;};
						//case 6  : {CVVR_VoNComponentRangeSix.Cast(ent.FindComponent(CVVR_VoNComponentRangeSix)).m_OnReceivedVON.Insert(OnReceivedVON);     break;};
						default : {CVVR_VoNComponentRangeFour.Cast(ent.FindComponent(CVVR_VoNComponentRangeFour)).m_OnReceivedVON.Insert(OnReceivedVON); };
					};
				};
			}
		}
			
		InitDefaults();			
		InitData(config);
				
		if (!UpdateEntityStateFlags())
			return false;
	
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Uninitialize class when its being moved to unsued tags in order to be reused
	override void ResetTag()
	{
		if (m_CharController)
			m_CharController.m_OnLifeStateChanged.Remove(OnLifeStateChanged);
		
		if (m_bIsCurrentPlayer && m_Entity)
		{
			CVVR_VoNMasterComponent vonMasterComponent = CVVR_VoNMasterComponent.Cast(m_Entity.FindComponent(CVVR_VoNMasterComponent));
			
			if (vonMasterComponent) {	
				switch (vonMasterComponent.GetLocalVoiceRange()) {
					case 1  : {CVVR_VoNComponentRangeOne.Cast(m_Entity.FindComponent(CVVR_VoNComponentRangeOne)).m_OnReceivedVON.Remove(OnReceivedVON);     break;};
					case 2  : {CVVR_VoNComponentRangeTwo.Cast(m_Entity.FindComponent(CVVR_VoNComponentRangeTwo)).m_OnReceivedVON.Remove(OnReceivedVON);     break;};
					case 3  : {CVVR_VoNComponentRangeThree.Cast(m_Entity.FindComponent(CVVR_VoNComponentRangeThree)).m_OnReceivedVON.Remove(OnReceivedVON); break;};
					case 4  : {CVVR_VoNComponentRangeFour.Cast(m_Entity.FindComponent(CVVR_VoNComponentRangeFour)).m_OnReceivedVON.Insert(OnReceivedVON);  	break;}
					case 5  : {CVVR_VoNComponentRangeFive.Cast(m_Entity.FindComponent(CVVR_VoNComponentRangeFive)).m_OnReceivedVON.Remove(OnReceivedVON);   break;};
					//case 6  : {CVVR_VoNComponentRangeSix.Cast(m_Entity.FindComponent(CVVR_VoNComponentRangeSix)).m_OnReceivedVON.Remove(OnReceivedVON);     break;};
					default : {CVVR_VoNComponentRangeFour.Cast(m_Entity.FindComponent(CVVR_VoNComponentRangeFour)).m_OnReceivedVON.Remove(OnReceivedVON); };
				};
			};
		}
		
		if (ChimeraCharacter.Cast(m_Entity))
		{				
			// Vehicle enter/leave event
			SCR_CompartmentAccessComponent accessComp = SCR_CompartmentAccessComponent.Cast( m_Entity.FindComponent(SCR_CompartmentAccessComponent) );
			if (accessComp)
			{
				accessComp.GetOnCompartmentEntered().Remove(OnVehicleEntered);
				accessComp.GetOnCompartmentLeft().Remove(OnVehicleLeft);
			}
		}
	}
}

//------------------------------------------------------------------------------------------------

//SCR_AvailableActionsConditionData

//------------------------------------------------------------------------------------------------

modded class SCR_AvailableActionsConditionData
{		
	//------------------------------------------------------------------------------------------------
	//! Fetches data from the provided entity
	//! Sets the validity of the data which can be received via IsValid()
	override void FetchData(IEntity controlledEntity, float timeSlice)
	{
		// Detect when character changes
		ChimeraCharacter character = ChimeraCharacter.Cast(controlledEntity);
		if (character != m_Character)
		{
			m_Character = character;

			if (character)
			{
				m_CharacterController = character.GetCharacterController();
				m_GadgetManager = SCR_GadgetManagerComponent.GetGadgetManager(character);
			}
		}

		if (!m_Character)
		{
			m_CharacterController = null;
			m_GadgetManager = null;

			m_bIsValid = false;
			return;
		}

		if (!m_CharacterController)
		{
			m_bIsValid = false;
			return;
		}

		// Add inventory listeners for efficient magazine/grenade count updates
		SCR_InventoryStorageManagerComponent storageManager = SCR_InventoryStorageManagerComponent.Cast(m_CharacterController.GetInventoryStorageManager());
		if (storageManager != m_StorageManager)
		{
			if (m_StorageManager)
			{
				m_StorageManager.m_OnItemAddedInvoker.Remove(OnItemAddedListener);
				m_StorageManager.m_OnItemRemovedInvoker.Remove(OnItemRemovedListener);
				m_StorageManager.m_OnInventoryOpenInvoker.Remove(OnInventoryOpen);
				m_StorageManager.m_OnQuickBarOpenInvoker.Remove(OnQuickSlotOpen);
			}

			m_StorageManager = storageManager;

			if (m_StorageManager)
			{
				m_StorageManager.m_OnItemAddedInvoker.Insert(OnItemAddedListener);
				m_StorageManager.m_OnItemRemovedInvoker.Insert(OnItemRemovedListener);
				m_StorageManager.m_OnInventoryOpenInvoker.Insert(OnInventoryOpen);
				m_StorageManager.m_OnQuickBarOpenInvoker.Insert(OnQuickSlotOpen);
			}
		}

		// Invalidates and clears any data prior to following collection
		Clear();

		// Current character stance
		m_eCharacterStance = m_CharacterController.GetStance();
		// Is character ADS?
		m_bIsCharacterADS = m_CharacterController.IsWeaponADS();
		// Is character sprinting?
		m_bIsCharacterSprinting = m_CharacterController.IsSprinting();
		// Is character swimming?
		m_bIsCharacterSwimming = m_CharacterController.IsSwimming();
		// Is character falling? (is airborne?)
		m_bIsCharacterFalling = m_CharacterController.IsFalling();
		// Is character in vehicle?
		m_bIsCharacterInVehicle = m_Character.IsInVehicle();
		// Can character get out?
		m_bCanCharacterGetOutVehicle = m_CharacterController.CanGetOutVehicleScript();
		// Is character weapon raised?
		m_bIsCharacterWeaponRaised = m_CharacterController.IsWeaponRaised();
		// Can character fire weapon
		m_bCanCharacterFireWeapon = m_CharacterController.CanFire();
		// Is character currently using an item?
		m_bIsCharacterUsingItem = m_CharacterController.IsUsingItem();
		// Item that character is holding in his right hand
		m_CurrentItemEntity = m_CharacterController.GetAttachedGadgetAtLeftHandSlot();

		// Temporary sprinting time tracking
		if (m_bIsCharacterSprinting && !m_CharacterController.GetIsSprintingToggle())
			m_fSprintingTime += timeSlice;
		else
			m_fSprintingTime = 0;

		// Vehicle compartment and controls
		CompartmentAccessComponent compartmentAccess = m_Character.GetCompartmentAccessComponent();

		if (compartmentAccess)
		{
			m_bCanCharacterGetOutVehicle = m_bCanCharacterGetOutVehicle && compartmentAccess.CanGetOutVehicle();
			m_bIsCharacterGettingIn = compartmentAccess.IsGettingIn();
			m_bIsCharacterGettingOut = compartmentAccess.IsGettingOut();

			// Vehicle compartment
			BaseCompartmentSlot slot = compartmentAccess.GetCompartment();
			if (slot)
			{
				// Vehicle
				IEntity vehicle = slot.GetOwner();
				
				// In case the slot is actually in the injected compartment, like the backseats of Jeep
				vehicle = GetVehicle(vehicle);	
				
				if (vehicle != m_CurrentVehicle)
				{
					m_CurrentVehicle = vehicle;

					if (vehicle)
					{
						m_CurrentVehicleSignals = SignalsManagerComponent.Cast(vehicle.FindComponent(SignalsManagerComponent));
					}
				}

				// Temporary turbo time tracking
				// TODO: Condition activation time
				m_eCompartmentType = slot.GetType();
				if (m_eCompartmentType == ECompartmentType.Pilot && GetGame().GetInputManager().GetActionTriggered("CarTurbo"))
					m_fTurboTime += timeSlice;
				else
					m_fTurboTime = 0;

				// Turret controls
				m_CurrentVehicleController = slot.GetController();
				TurretControllerComponent turretController = TurretControllerComponent.Cast(m_CurrentVehicleController);
				if (turretController)
				{
					BaseWeaponManagerComponent weaponManager = turretController.GetWeaponManager();
					if (weaponManager)
						m_CurrentVehicleWeapon = weaponManager.GetCurrentWeapon();
				}
			}
			else
			{
				m_CurrentVehicle = null;
			}
		}

		// Clear vehicle variables
		if (!m_CurrentVehicle)
		{
			m_CurrentVehicleSignals = null;
			m_CurrentVehicleController = null;
		}

		// Current character weapon manager
		// Current character weapon
		BaseWeaponManagerComponent weaponManager = m_CharacterController.GetWeaponManagerComponent();
		if (weaponManager)
		{
			// Weapon slot -> weapon entity
			WeaponSlotComponent currentSlot = weaponManager.GetCurrentSlot();
			if (currentSlot)
				m_CurrentWeaponEntity = currentSlot.GetWeaponEntity();

			// BaseWeaponComponent
			BaseWeaponComponent currentWeapon = weaponManager.GetCurrent();
			if (currentWeapon != m_CurrentWeapon)
			{
				m_CurrentWeapon = currentWeapon;
				m_bCanFetchInventory = true;
			}

			// Muzzle and magazine
			if (m_CurrentWeapon)
			{
				m_CurrentMuzzle = m_CurrentWeapon.GetCurrentMuzzle();
				m_CurrentMagazine = m_CurrentWeapon.GetCurrentMagazine();
			}

			// Is character reloading
			m_bIsCharacterReloading = m_CharacterController.IsReloading();
		}

		// Does character have additional mags for current weapon?
		if (m_bCanFetchInventory && m_StorageManager)
		{
			if (m_CurrentWeapon)
				m_iAdditionalMagazines = m_StorageManager.GetMagazineCountByWeapon(m_CurrentWeapon);

			m_iMedicalItemCount = m_StorageManager.GetHealthComponentCount();

			// Check medical items in quick slots
			SCR_CharacterInventoryStorageComponent characterStorage = m_StorageManager.GetCharacterStorage();
			if (characterStorage)
			{
				m_iMedicalItemCountInQuickSlots = 0;

				array<IEntity> items = characterStorage.GetQuickSlotItems();
				foreach (IEntity item : items)
				{
					if (item && item.FindComponent(SCR_ConsumableItemComponent))
						m_iMedicalItemCountInQuickSlots++;
				}
			}

			m_bCanFetchInventory = false;
		}

		// Camera handler to check focus mode
		CameraHandlerComponent cameraHandler = m_CharacterController.GetCameraHandlerComponent();
		if (cameraHandler)
			m_fFocusMode = cameraHandler.GetFocusMode();

		// Fetch available gadgets
		m_bIsGadgetSelection = GetGame().GetInputManager().GetActionTriggered("GadgetSelection");
		m_bIsWeaponManipulation = GetGame().GetInputManager().GetActionTriggered("WeaponManipulation");

		// VON status
		if (m_VONController)
			m_bCharacterRadiosCount = m_VONController.GetVONEntryCount();

		// VON usage
		CVVR_VoNMasterComponent vonMasterComponent = CVVR_VoNMasterComponent.Cast(character.FindComponent(CVVR_VoNMasterComponent));
			
		if (vonMasterComponent) {	
			switch (vonMasterComponent.GetLocalVoiceRange()) {
				case 1  : {m_bCharacterIsUsingRadio = CVVR_VoNComponentRangeOne.Cast(character.FindComponent(CVVR_VoNComponentRangeOne)).IsTransmitingRadio();     break;};
				case 2  : {m_bCharacterIsUsingRadio = CVVR_VoNComponentRangeTwo.Cast(character.FindComponent(CVVR_VoNComponentRangeTwo)).IsTransmitingRadio();     break;};
				case 3  : {m_bCharacterIsUsingRadio = CVVR_VoNComponentRangeThree.Cast(character.FindComponent(CVVR_VoNComponentRangeThree)).IsTransmitingRadio(); break;};
				case 4  : {m_bCharacterIsUsingRadio = CVVR_VoNComponentRangeFour.Cast(character.FindComponent(CVVR_VoNComponentRangeFour)).IsTransmitingRadio(); 	break;};
				case 5  : {m_bCharacterIsUsingRadio = CVVR_VoNComponentRangeFive.Cast(character.FindComponent(CVVR_VoNComponentRangeFive)).IsTransmitingRadio();   break;};
				//case 6  : {m_bCharacterIsUsingRadio = CVVR_VoNComponentRangeSix.Cast(character.FindComponent(CVVR_VoNComponentRangeSix)).IsTransmitingRadio();     break;};
				default : {m_bCharacterIsUsingRadio = CVVR_VoNComponentRangeFour.Cast(character.FindComponent(CVVR_VoNComponentRangeFour)).IsTransmitingRadio()};
			};
			
			if (m_bCharacterIsUsingRadio)
				m_bIsRadioLongRange = m_VONController.IsUsingLRR();
			
			m_bOwnsLongRangeRadio = m_VONController.IsLRRAvailable();
		};
		
		// Addition data
		FetchHealthData(timeSlice);
		
		// Map data
		if (m_MapEntity && m_MapEntity.IsOpen())
			FetchMapData(timeSlice);
	}
}