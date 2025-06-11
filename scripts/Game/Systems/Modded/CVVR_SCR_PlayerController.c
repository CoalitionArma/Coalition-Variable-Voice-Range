modded class SCR_PlayerController
{
	void ChangeVoiceRange(float input)
	{
		CVVR_VoNSoundComponent vonComp = CVVR_VoNSoundComponent.Cast(GetLocalControlledEntity().FindComponent(CVVR_VoNSoundComponent));
		if ((vonComp.m_fVoiceRange >= 1 && input == 1) || (vonComp.m_fVoiceRange <= 0 && input == -1))
			return;
		Rpc(RpcDo_ChangeVoiceRange, input, GetPlayerId());
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void RpcDo_ChangeVoiceRange(float input, int playerId)
	{
		CVVR_VoNSoundComponent.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId).GetControlledEntity().FindComponent(CVVR_VoNSoundComponent)).ChangeVoiceRange(input);
	}
}