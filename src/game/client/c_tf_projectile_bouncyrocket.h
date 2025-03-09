//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================

#ifndef C_TF_PROJECTILE_BOUNCYROCKET_H
#define C_TF_PROJECTILE_BOUNCYROCKET_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_weaponbase_rocket.h"

#define CTFProjectile_BouncyRocket C_TFProjectile_BouncyRocket

//-----------------------------------------------------------------------------
// Purpose: Rocket projectile.
//-----------------------------------------------------------------------------
class C_TFProjectile_BouncyRocket : public C_TFBaseRocket
{
	DECLARE_CLASS(C_TFProjectile_BouncyRocket, C_TFBaseRocket);
public:
	DECLARE_NETWORKCLASS();

	C_TFProjectile_BouncyRocket();
	~C_TFProjectile_BouncyRocket();

	virtual void	OnDataChanged(DataUpdateType_t updateType);

	virtual void	CreateTrails(void);
	virtual const char* GetTrailParticleName(void);
	bool			IsCritical() const { return m_bCritical; }

private:
	bool	m_bCritical;

	CNewParticleEffect* pEffect;
};

#endif // C_TF_PROJECTILE_BOUNCYROCKET_H
