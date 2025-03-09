
//========= Copyright Valve Corporation, All rights reserved. ============//
//
// TF Rocket
//
//=============================================================================
#include "cbase.h"
#include "tf_weaponbase.h"
#include "tf_projectile_bouncyrocket.h"
#include "tf_player.h"
#include "tf_weaponbase_gun.h"

//=============================================================================
//
// TF Rocket functions (Server specific).
//
#define ROCKET_MODEL "models/weapons/w_models/w_rocket.mdl"

LINK_ENTITY_TO_CLASS(tf_projectile_bouncyrocket, CTFProjectile_BouncyRocket);
PRECACHE_REGISTER(tf_projectile_bouncyrocket);

IMPLEMENT_NETWORKCLASS_ALIASED(TFProjectile_BouncyRocket, DT_TFProjectile_BouncyRocket)

BEGIN_NETWORK_TABLE(CTFProjectile_BouncyRocket, DT_TFProjectile_BouncyRocket)
SendPropBool(SENDINFO(m_bCritical)),
END_NETWORK_TABLE()

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
CTFProjectile_BouncyRocket* CTFProjectile_BouncyRocket::Create(CBaseEntity* pLauncher, const Vector& vecOrigin, const QAngle& vecAngles, CBaseEntity* pOwner, CBaseEntity* pScorer)
{
	CTFProjectile_BouncyRocket* pRocket = static_cast<CTFProjectile_BouncyRocket*>(CTFBaseRocket::Create(pLauncher, "tf_projectile_bouncyrocket", vecOrigin, vecAngles, pOwner));

	if (pRocket)
	{
		pRocket->SetScorer(pScorer);
		pRocket->SetEyeBallRocket(false);
		pRocket->SetSpell(false);

		CTFWeaponBase* pWeapon = dynamic_cast<CTFWeaponBase*>(pLauncher);
		pRocket->SetMoveType(MOVETYPE_FLYGRAVITY, MOVECOLLIDE_FLY_BOUNCE);
		pRocket->SetGravity(2.0f);
		pRocket->SetElasticity(69420);		//basically infinite lol
		pRocket->SetFriction(0.0f);
	}

	return pRocket;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFProjectile_BouncyRocket::Spawn()
{
	SetModel(ROCKET_MODEL);
	BaseClass::Spawn();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFProjectile_BouncyRocket::Precache()
{
	int iModel = PrecacheModel(ROCKET_MODEL);
	PrecacheGibsForModel(iModel);
	PrecacheParticleSystem("critical_rocket_blue");
	PrecacheParticleSystem("critical_rocket_red");
	PrecacheParticleSystem("eyeboss_projectile");
	PrecacheParticleSystem("rockettrail");
	PrecacheParticleSystem("rockettrail_RocketJumper");
	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFProjectile_BouncyRocket::SetScorer(CBaseEntity* pScorer)
{
	m_Scorer = pScorer;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
CBasePlayer* CTFProjectile_BouncyRocket::GetScorer(void)
{
	return dynamic_cast<CBasePlayer*>(m_Scorer.Get());
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int	CTFProjectile_BouncyRocket::GetDamageType()
{
	int iDmgType = BaseClass::GetDamageType();
	if (m_bCritical)
	{
		iDmgType |= DMG_CRITICAL;
	}

	return iDmgType;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int	CTFProjectile_BouncyRocket::GetDamageCustom()
{
	return BaseClass::GetDamageCustom();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFProjectile_BouncyRocket::RocketTouch(CBaseEntity* pOther)
{
	RecalculateDamage();
	BaseClass::RocketTouch(pOther);

	if (m_bCritical && pOther && pOther->IsPlayer())
	{
		CTFPlayer* pHitPlayer = ToTFPlayer(pOther);
		int iHitPlayerTeamNumber = pHitPlayer->GetTeamNumber();
		int iRocketTeamNumber = BaseClass::GetTeamNumber();

		if (pHitPlayer->IsPlayerClass(TF_CLASS_HEAVYWEAPONS) && !pHitPlayer->m_Shared.InCond(TF_COND_INVULNERABLE)
			&& pHitPlayer->IsAlive() && iHitPlayerTeamNumber != iRocketTeamNumber)
		{
			pHitPlayer->AwardAchievement(ACHIEVEMENT_TF_HEAVY_SURVIVE_CROCKET);
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Rocket was deflected.
//-----------------------------------------------------------------------------
void CTFProjectile_BouncyRocket::Deflected(CBaseEntity* pDeflectedBy, Vector& vecDir)
{
	CTFPlayer* pTFDeflector = ToTFPlayer(pDeflectedBy);
	if (!pTFDeflector)
		return;

	ChangeTeam(pTFDeflector->GetTeamNumber());
	SetLauncher(pTFDeflector->GetActiveWeapon());

	CTFPlayer* pOldOwner = ToTFPlayer(GetOwnerEntity());
	SetOwnerEntity(pTFDeflector);

	if (pOldOwner)
	{
		pOldOwner->SpeakConceptIfAllowed(MP_CONCEPT_DEFLECTED, "projectile:1,victim:1");
	}

	if (pTFDeflector->m_Shared.IsCritBoosted())
	{
		SetCritical(true);
	}

	CTFWeaponBase::SendObjectDeflectedEvent(pTFDeflector, pOldOwner, GetWeaponID(), this);

	IncrementDeflected();
	m_nSkin = (GetTeamNumber() == TF_TEAM_BLUE) ? 1 : 0;
}

/*// for some reason i can't get this to work. i'll check back on this later.
float CTFProjectile_BouncyRocket::GetDamage() {
	float flBase = BaseClass::GetDamage();
	float flRatio = GetAbsVelocity().Length() / 1000.0f;	//base velocity of rocket is 1000 but im not in the mood to unmagicnumber this
	return flBase;
}*/

//so i guess i've fixed the above issue by just calling this function in RocketTouch, but it feels cheaty. i'll leave it here for now.
void CTFProjectile_BouncyRocket::RecalculateDamage() {
	float flBase = BaseClass::GetDamage();
	float flRatio = GetAbsVelocity().Length() / 1000.0f;	//base velocity of rocket is 1000 but im still not in the mood to unmagicnumber this
	SetDamage(flBase * flRatio);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int	CTFProjectile_BouncyRocket::GetSpeedCustom()
{
	CTFWeaponBaseGun* ownerWep = dynamic_cast<CTFWeaponBaseGun*>(GetLauncher());
	if (ownerWep)
	{
		return ownerWep->GetProjectileSpeed();
	}
	return BaseClass::GetSpeedCustom();
}