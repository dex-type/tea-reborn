#ifndef TF_WEAPON_RIFLE_H
#define TF_WEAPON_RIFLE_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_weaponbase_gun.h"

#if defined( CLIENT_DLL )
#define CTFRifle C_TFRifle
#endif


//=============================================================================
//
// Sniper Rifle class.
//
class CTFRifle : public CTFWeaponBaseGun
{
public:

	DECLARE_CLASS(CTFRifle, CTFWeaponBaseGun);
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();
	DECLARE_DATADESC();

	virtual int	GetWeaponID(void) const { return TF_WEAPON_RIFLE; };
};
#endif	// TF_WEAPON_RIFLE_H