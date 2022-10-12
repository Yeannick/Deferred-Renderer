#include "stdafx.h"
#include "PostBrightness.h"

PostBrightness::PostBrightness():
	PostProcessingMaterial(L"Effects/Post/Brightness.fx")
{
}

void PostBrightness::UpdateBaseEffectVariables(const SceneContext& , RenderTarget* pSource)
{
	const auto pSourceSRV = pSource->GetColorShaderResourceView();
	m_pBaseEffect->GetVariableByName("gTexture")->AsShaderResource()->SetResource(pSourceSRV);
	//const auto pSourceSRV = pSource->GetColorShaderResourceView();
	m_pBaseEffect->GetVariableByName("g_Brightness")->AsScalar()->SetFloat(m_Multiplier);


}


