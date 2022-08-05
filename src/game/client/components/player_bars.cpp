#include <game/client/gameclient.h>
#include <game/generated/client_data.h>

#include "player_bars.h"

void CPlayerBars::RenderPlayerBars(const int ClientID)
{
	CCharacterCore *pCharacter = &m_pClient->m_aClients[ClientID].m_Predicted;

	// Freeze Bar
	bool ShowFreezeBar = true;
	if(pCharacter->m_FreezeEnd <= 0.0f || pCharacter->m_FreezeStart == 0 || !m_pClient->m_Snap.m_aCharacters[ClientID].m_HasExtendedDisplayInfo || (pCharacter->m_IsInFreeze && g_Config.m_ClFreezeBarsAlphaInsideFreeze == 0))
	{
		ShowFreezeBar = false;
	}

	if(ShowFreezeBar)
	{
		const int Max = pCharacter->m_FreezeEnd - pCharacter->m_FreezeStart;
		float FreezeProgress = clamp(Max - (Client()->GameTick(g_Config.m_ClDummy) - pCharacter->m_FreezeStart), 0, Max) / (float)Max;
		if(FreezeProgress <= 0.0f)
		{
			ShowFreezeBar = false;
		}
		else
		{
			vec2 Position = m_pClient->m_aClients[ClientID].m_RenderPos;
			// Center the bar under the tee body
			Position.x -= 32;
			Position.y += 26;

			float Alpha = m_pClient->IsOtherTeam(ClientID) ? g_Config.m_ClShowOthersAlpha / 100.0f : 1.0f;
			if(pCharacter->m_IsInFreeze)
			{
				Alpha *= g_Config.m_ClFreezeBarsAlphaInsideFreeze / 100.0f;
			}

			RenderFreezeBarPos(&Position, FreezeProgress, Alpha);
		}
	}

	// Ninja Bar
	if(pCharacter->m_Ninja.m_ActivationTick > 0.0f && m_pClient->m_Snap.m_aCharacters[ClientID].m_HasExtendedDisplayInfo)
	{
		const int Max = g_pData->m_Weapons.m_Ninja.m_Duration * Client()->GameTickSpeed() / 1000;
		float NinjaProgress = clamp(pCharacter->m_Ninja.m_ActivationTick + g_pData->m_Weapons.m_Ninja.m_Duration * Client()->GameTickSpeed() / 1000 - Client()->GameTick(g_Config.m_ClDummy), 0, Max) / (float)Max;
		if(NinjaProgress > 0.0f)
		{
			vec2 Position = m_pClient->m_aClients[ClientID].m_RenderPos;
			// Center the bar under the tee body
			Position.x -= 32;
			Position.y += 26;
			if(ShowFreezeBar)
			{
				Position.y += 16;
			}
			float Alpha = m_pClient->IsOtherTeam(ClientID) ? g_Config.m_ClShowOthersAlpha / 100.0f : 1.0f;
			RenderNinjaBarPos(&Position, NinjaProgress, Alpha);
		}
	}
}

void CPlayerBars::RenderFreezeBarPos(const vec2 *Pos, float Progress, const float Alpha)
{
	Progress = clamp(Progress, 0.0f, 1.0f);

	static const float BarWidth = 64.0f;
	static const float BarHeight = 16.0f;

	static const float EndRestPct = 0.125f;
	static const float ProgPct = 0.75f;

	static const float EndRestWidth = BarWidth * EndRestPct;
	static const float ProgressBarWidth = BarWidth - (EndRestWidth * 2.0f);

	// Full
	Graphics()->WrapClamp();
	Graphics()->TextureSet(m_pClient->m_HudSkin.m_SpriteHudFreezeBarFull);
	Graphics()->QuadsBegin();
	Graphics()->SetColor(1.f, 1.f, 1.f, Alpha);
	// Subset: top_l, top_m, btm_m, btm_l
	Graphics()->QuadsSetSubsetFree(0, 0, EndRestPct + ProgPct * Progress, 0, EndRestPct + ProgPct * Progress, 1, 0, 1);
	IGraphics::CQuadItem QuadFullBeginning(Pos->x, Pos->y, EndRestWidth + ProgressBarWidth * Progress, BarHeight);
	Graphics()->QuadsDrawTL(&QuadFullBeginning, 1);
	Graphics()->QuadsEnd();

	// Empty
	Graphics()->WrapClamp();
	Graphics()->TextureSet(m_pClient->m_HudSkin.m_SpriteHudFreezeBarEmpty);
	Graphics()->QuadsBegin();
	Graphics()->SetColor(1.f, 1.f, 1.f, Alpha);
	// Subset: top_m, top_r, btm_r, btm_m
	Graphics()->QuadsSetSubsetFree(EndRestPct + ProgPct * Progress, 0, 1, 0, 1, 1, EndRestPct + ProgPct * Progress, 1);
	IGraphics::CQuadItem QuadEmptyBeginning(Pos->x + EndRestWidth + ProgressBarWidth * Progress, Pos->y, EndRestWidth + ProgressBarWidth * (1.0f - Progress), BarHeight);
	Graphics()->QuadsDrawTL(&QuadEmptyBeginning, 1);
	Graphics()->QuadsEnd();

	Graphics()->QuadsSetSubset(0, 0, 1, 1);
	Graphics()->SetColor(1.f, 1.f, 1.f, 1.f);
	Graphics()->WrapNormal();
}

void CPlayerBars::RenderNinjaBarPos(const vec2 *Pos, float Progress, const float Alpha)
{
	Progress = clamp(Progress, 0.0f, 1.0f);

	static const float BarWidth = 64.0f;
	static const float BarHeight = 16.0f;

	static const float EndRestPct = 0.125f;
	static const float ProgPct = 0.75f;

	static const float EndRestWidth = BarWidth * EndRestPct;
	static const float ProgressBarWidth = BarWidth - (EndRestWidth * 2.0f);

	// Full
	Graphics()->WrapClamp();
	Graphics()->TextureSet(m_pClient->m_HudSkin.m_SpriteHudNinjaBarFull);
	Graphics()->QuadsBegin();
	Graphics()->SetColor(0.6f, 0.6f, 0.6f, Alpha);
	// Subset: top_l, top_m, btm_m, btm_l
	Graphics()->QuadsSetSubsetFree(0, 0, EndRestPct + ProgPct * Progress, 0, EndRestPct + ProgPct * Progress, 1, 0, 1);
	IGraphics::CQuadItem QuadFullBeginning(Pos->x, Pos->y, EndRestWidth + ProgressBarWidth * Progress, BarHeight);
	Graphics()->QuadsDrawTL(&QuadFullBeginning, 1);
	Graphics()->QuadsEnd();

	// Empty
	Graphics()->WrapClamp();
	Graphics()->TextureSet(m_pClient->m_HudSkin.m_SpriteHudNinjaBarEmpty);
	Graphics()->QuadsBegin();
	Graphics()->SetColor(0.6f, 0.6f, 0.6f, Alpha);
	// Subset: top_m, top_r, btm_r, btm_m
	Graphics()->QuadsSetSubsetFree(EndRestPct + ProgPct * Progress, 0, 1, 0, 1, 1, EndRestPct + ProgPct * Progress, 1);
	IGraphics::CQuadItem QuadEmptyBeginning(Pos->x + EndRestWidth + ProgressBarWidth * Progress, Pos->y, EndRestWidth + ProgressBarWidth * (1.0f - Progress), BarHeight);
	Graphics()->QuadsDrawTL(&QuadEmptyBeginning, 1);
	Graphics()->QuadsEnd();

	Graphics()->QuadsSetSubset(0, 0, 1, 1);
	Graphics()->SetColor(1.f, 1.f, 1.f, 1.f);
	Graphics()->WrapNormal();
}

inline bool CPlayerBars::IsPlayerInfoAvailable(int ClientID) const
{
	const void *pPrevInfo = Client()->SnapFindItem(IClient::SNAP_PREV, NETOBJTYPE_PLAYERINFO, ClientID);
	const void *pInfo = Client()->SnapFindItem(IClient::SNAP_CURRENT, NETOBJTYPE_PLAYERINFO, ClientID);
	return pPrevInfo && pInfo;
}

void CPlayerBars::OnRender()
{
	if(!g_Config.m_ClShowFreezeBars && !g_Config.m_ClShowNinjaBars)
	{
		return;
	}
	// get screen edges to avoid rendering offscreen
	float ScreenX0, ScreenY0, ScreenX1, ScreenY1;
	Graphics()->GetScreen(&ScreenX0, &ScreenY0, &ScreenX1, &ScreenY1);
	// expand the edges to prevent popping in/out onscreen
	//
	// it is assumed that the tee with the freeze bar fit into a 240x240 box centered on the tee
	// this may need to be changed or calculated differently in the future
	float BorderBuffer = 120;
	ScreenX0 -= BorderBuffer;
	ScreenX1 += BorderBuffer;
	ScreenY0 -= BorderBuffer;
	ScreenY1 += BorderBuffer;

	int LocalClientID = m_pClient->m_Snap.m_LocalClientID;

	// render everyone else's freeze bar, then our own
	for(int ClientID = 0; ClientID < MAX_CLIENTS; ClientID++)
	{
		if(ClientID == LocalClientID || !m_pClient->m_Snap.m_aCharacters[ClientID].m_Active || !IsPlayerInfoAvailable(ClientID))
		{
			continue;
		}

		//don't render if the tee is offscreen
		vec2 *pRenderPos = &m_pClient->m_aClients[ClientID].m_RenderPos;
		if(pRenderPos->x < ScreenX0 || pRenderPos->x > ScreenX1 || pRenderPos->y < ScreenY0 || pRenderPos->y > ScreenY1)
		{
			continue;
		}

		RenderPlayerBars(ClientID);
	}
	if(LocalClientID != -1 && m_pClient->m_Snap.m_aCharacters[LocalClientID].m_Active && IsPlayerInfoAvailable(LocalClientID))
	{
		RenderPlayerBars(LocalClientID);
	}
}