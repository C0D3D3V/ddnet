#ifndef GAME_CLIENT_COMPONENTS_PLAYERBARS_H
#define GAME_CLIENT_COMPONENTS_PLAYERBARS_H
#include <game/client/component.h>

class CPlayerBars : public CComponent
{
	void RenderPlayerBars(const int ClientID);
	void RenderFreezeBarPos(const vec2 *Pos, float Progress, const float Alpha = 1.0f);
	void RenderNinjaBarPos(const vec2 *Pos, float Progress, const float Alpha = 1.0f);
	bool IsPlayerInfoAvailable(int ClientID) const;

public:
	virtual int Sizeof() const override { return sizeof(*this); }
	virtual void OnRender() override;
};

#endif
