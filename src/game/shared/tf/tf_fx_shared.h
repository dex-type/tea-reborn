//========= Copyright Valve Corporation, All rights reserved. ============//
//
//  
//
//=============================================================================
#ifndef TF_FX_SHARED_H
#define TF_FX_SHARED_H
#ifdef _WIN32
#pragma once
#endif

// Client specific.
#ifdef CLIENT_DLL
#include "c_tf_player.h"
// Server specific.
#else
#include "tf_player.h"
#endif

void FX_WeaponSound ( int iPlayer, WeaponSound_t soundType, const Vector &vecOrigin, CTFWeaponInfo *pWeaponInfo );
void StartGroupingSounds( void );
void EndGroupingSounds( void );
int GetWeaponSpreadType( CTFWeaponBase *pWeapon );

extern ConVar tea_spread_force_type;

// This runs on both the client and the server.
// On the server, it only does the damage calculations.
// On the client, it does all the effects.
void FX_FireBullets( CTFWeaponBase *pWpn, int iPlayer, const Vector &vecOrigin, const QAngle &vecAngles,
					 int iWeapon, int iMode, int iSeed, float flSpread, float flDamage = -1.0f, bool bCritical = false );

enum spreadType {
	TEA_SPREADMODE_CIRCLE_UNFILTERED,
	TEA_SPREADMODE_CIRCLE_GAUSSIAN,
	TEA_SPREADMODE_CIRCLE_SERIES,
	TEA_SPREADMODE_CIRCLE_FIXED,
	TEA_SPREADMODE_SQUARE_UNFILTERED,
	TEA_SPREADMODE_SQUARE_GAUSSIAN,
	TEA_SPREADMODE_SQUARE_SERIES,
	TEA_SPREADMODE_HORIZONTAL_UNFILTERED,
	TEA_SPREADMODE_HORIZONTAL_GAUSSIAN,
	TEA_SPREADMODE_HORIZONTAL_FIXED,
	TEA_SPREADMODE_HORIZONTAL_SERIES,
};

#endif // TF_FX_SHARED_H
