//========= Copyright Valve Corporation, All rights reserved. ============//
//
//
//=============================================================================
#include "cbase.h"
#include "tf_weapon_rifle.h"
#include "tf_fx_shared.h"
#include "in_buttons.h"
#include "tf_gamerules.h"

// Client specific.
#ifdef CLIENT_DLL
#include "c_tf_player.h"
#include "c_tf_gamestats.h"
// Server specific.
#else
#include "tf_player.h"
#include "tf_gamestats.h"
#include "ilagcompensationmanager.h"
#endif

//=============================================================================
//
// Weapon Pistol tables.
//
IMPLEMENT_NETWORKCLASS_ALIASED(TFRifle, DT_WeaponRifle)

BEGIN_NETWORK_TABLE(CTFRifle, DT_WeaponRifle)
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA(CTFRifle)
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS(tf_weapon_rifle, CTFRifle);
PRECACHE_WEAPON_REGISTER(tf_weapon_rifle);

// Server specific.
#ifndef CLIENT_DLL
BEGIN_DATADESC(CTFRifle)
END_DATADESC()
#endif