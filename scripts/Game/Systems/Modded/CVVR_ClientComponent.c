[ComponentEditorProps(category: "GameScripted/UI", description: "CSI Player Component for RPC", color: "0 0 255 255")]
class CVVR_ClientComponentClass : ScriptComponentClass {};

class CVVR_ClientComponent : ScriptComponent
{
	protected CVVR_AuthorityComponent m_AuthorityComponent;
	protected SCR_VONController m_VONController;
	protected int m_iLocalVoiceRange = 3;

	//------------------------------------------------------------------------------------------------

	// Override/static functions

	//------------------------------------------------------------------------------------------------
	
	static CVVR_ClientComponent GetInstance() 
	{
		if (GetGame().GetPlayerController())
			return CVVR_ClientComponent.Cast(GetGame().GetPlayerController().FindComponent(CVVR_ClientComponent));
		else
			return null;
	}

	//------------------------------------------------------------------------------------------------
	override protected void OnPostInit(IEntity owner) 
	{
		super.OnPostInit(owner);
		m_AuthorityComponent = CVVR_AuthorityComponent.GetInstance();
		ChangeVoiceRange(0);
	}
	
	//------------------------------------------------------------------------------------------------

	// Replication Functions

	//------------------------------------------------------------------------------------------------
	
	int ReturnLocalVoiceRange() 
	{
		return m_iLocalVoiceRange;
	}


	void ChangeVoiceRange(int voiceRangeChange) 
	{
		int voiceRangeSum = m_iLocalVoiceRange + voiceRangeChange;
		
		if (voiceRangeSum >= 1 && voiceRangeSum <= 5 && m_iLocalVoiceRange != voiceRangeSum) {
			m_iLocalVoiceRange = voiceRangeSum;
			
			Rpc(RpcAsk_UpdatePlayerArray, GetGame().GetPlayerController().GetPlayerId(), m_iLocalVoiceRange); // Tell all clients that this client has changed their voice range
		
			m_VONController = SCR_VONController.Cast(GetGame().GetPlayerController().FindComponent(SCR_VONController));
		
			if(m_VONController)
				GetGame().GetCallqueue().CallLater(m_VONController.ReloadVONForRangeChange, 350, false); //Need to give the server time to propigate the change
		};
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_UpdatePlayerArray(int playerID, int value) 
	{
		if (m_AuthorityComponent)
			m_AuthorityComponent.UpdatePlayerArray(playerID, value);
	}
}