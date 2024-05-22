[ComponentEditorProps(category: "GameScripted/Callsign", description: "")]
class CVVR_AuthorityComponentClass : SCR_BaseGameModeComponentClass {};

class CVVR_AuthorityComponent : SCR_BaseGameModeComponent
{
	// A hashmap that is modified only on each client by a .BumpMe from the authority.
	protected ref map<int, int> m_mPlayerRangeMap = new map<int, int>;

	// A array we use primarily for replication of m_mPlayerRangeMap.
	[RplProp(onRplName: "UpdatePlayerRangeMap")]
	protected ref array<string> m_aPlayerRangeArray = new array<string>;

	//------------------------------------------------------------------------------------------------

	// Override/static functions

	//------------------------------------------------------------------------------------------------
	
	static CVVR_AuthorityComponent GetInstance() {
		BaseGameMode gameMode = GetGame().GetGameMode();
		if (gameMode)
			return CVVR_AuthorityComponent.Cast(gameMode.FindComponent(CVVR_AuthorityComponent));
		else
			return null;
	}

	//------------------------------------------------------------------------------------------------
	override protected void OnPostInit(IEntity owner) {
		super.OnPostInit(owner);

		if (Replication.IsServer()) 
			GetGame().GetCallqueue().CallLater(CleanUpPlayerRangeArray, 960000, true); // Cleanup every 16min (960000ms)
	}

	//------------------------------------------------------------------------------------------------
	override protected void OnGameEnd() {
		super.OnGameEnd();
		
		if (Replication.IsServer())
			GetGame().GetCallqueue().Remove(CleanUpPlayerRangeArray);
	}

	//------------------------------------------------------------------------------------------------

	// Player Range Replication And Storage Functions

	//------------------------------------------------------------------------------------------------
	
	int ReturnPlayerRange(int playerID) {
		return m_mPlayerRangeMap.Get(playerID);
	}

	//------------------------------------------------------------------------------------------------
	protected void UpdatePlayerRangeMap() {
		foreach (string playerKeyAndValueToSplit : m_aPlayerRangeArray)
		{
			array<string> playerKeyAndValueArray = {};
			playerKeyAndValueToSplit.Split("╪", playerKeyAndValueArray, false);
			m_mPlayerRangeMap.Set(playerKeyAndValueArray[0].ToInt(), playerKeyAndValueArray[1].ToInt());
			
			//debugging
			Print("----------------------");
			PrintFormat("Player ID: %1", playerKeyAndValueArray[0]);
			PrintFormat("Voice Range: %1", playerKeyAndValueArray[1]);
			Print("----------------------");
		};
	}

	//------------------------------------------------------------------------------------------------
	void UpdatePlayerArray(int playerID, int value) {
		
		int previousValue = m_mPlayerRangeMap.Get(playerID);
		m_aPlayerRangeArray.RemoveItem(string.Format("%1╪%2", playerID, previousValue));

		m_aPlayerRangeArray.Insert(string.Format("%1╪%2", playerID, value));		
		UpdatePlayerRangeMap();
		
		Replication.BumpMe();
	}
	
	//------------------------------------------------------------------------------------------------
	protected void CleanUpPlayerRangeArray() {
		array<string> tempArray = new array<string>;
		
		foreach (string playerKeyAndValueToSplit : m_aPlayerRangeArray)
		{
			array<string> playerKeyAndValueArray = {};
			playerKeyAndValueToSplit.Split("╪", playerKeyAndValueArray, false);
			if(GetGame().GetPlayerManager().IsPlayerConnected((playerKeyAndValueArray[0]).ToInt()))
				tempArray.Insert(playerKeyAndValueToSplit);
		};
		m_aPlayerRangeArray = tempArray;
		Replication.BumpMe();
	}
};