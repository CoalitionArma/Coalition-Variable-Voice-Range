class CVVR_VoNSoundComponentClass: ScriptComponentClass
{

}

class CVVR_VoNSoundComponent: ScriptComponent
{
	[RplProp(onRplName: "OnVoiceRangeChanged")]
    float        m_fVoiceRange = 1.0;
    int          m_iVoiceRangeSignalIndex = -1;
	
	SignalsManagerComponent m_SignalsManager;

	protected void OnVoiceRangeChanged()
    {
		if (!m_SignalsManager)
			m_SignalsManager = SignalsManagerComponent.Cast(GetOwner().FindComponent(SignalsManagerComponent));
		if (m_iVoiceRangeSignalIndex == -1)
		{
			m_iVoiceRangeSignalIndex = m_SignalsManager.AddOrFindSignal("VoiceRange");
			m_SignalsManager.SetSignalValue(m_iVoiceRangeSignalIndex, m_fVoiceRange);
		}
		
        if (m_SignalsManager)
           m_SignalsManager.SetSignalValue(m_iVoiceRangeSignalIndex, m_fVoiceRange);
		Print(m_SignalsManager.GetSignalValue(m_iVoiceRangeSignalIndex));
    }
	
	void ChangeVoiceRange(float input)
	{
		float step = 0.2 * input;
		m_fVoiceRange += step;
		Replication.BumpMe();
		OnVoiceRangeChanged();
	}
}