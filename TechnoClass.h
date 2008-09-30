/*
	Base class for buildable objects
*/

#ifndef TECHNO_H
#define TECHNO_H

#include <RadioClass.h>
#include <TechnoTypeClass.h>
#include <HouseClass.h>

//forward declarations
class AirstrikeClass;
class AnimClass;
class BuildingClass;
class CaptureManagerClass;
class CellClass;
class FootClass;
class InfantryTypeClass;
class ObjectTypeClass;
class ParticleSystemClass;
class SlaveManagerClass;
class SpawnManagerClass;
class TemporalClass;
class WaveClass;

#include <TransitionTimer.h>

//also see FACING definitions
struct FacingStruct
{
	WORD Facing1; //current facing?
	WORD unused_2;
	WORD Facing2; //??
	WORD unused_6;
	TimerStruct Timer; //rotation?
	WORD ROT; //Rate of Turn. INI Value * 256
	WORD unused_16;
};

struct VeterancyStruct
{
	float Veterancy;
	DWORD unknown_4;
};

class PassengersClass
{
public:
	int NumPassengers;
	FootClass* FirstPassenger;

	void AddPassenger(FootClass* pPassenger)
		{ PUSH_VAR32(pPassenger); THISCALL(0x4733A0);}

	FootClass* GetFirstPassenger()
		{ return this->FirstPassenger; }

	FootClass* RemoveFirstPassenger()
		{ THISCALL(0x473430); }

	int GetTotalSize()
		{ THISCALL(0x473460); }

	signed int IndexOf(FootClass* candidate)
		{ PUSH_VAR32(candidate); THISCALL(0x473500); }
};

struct FlashData
{
	int DurationRemaining;
	bool FlashingNow;

	bool Update()
		{ THISCALL(0x4CC770); }
};

struct RecoilData
{
	int Travel;
	int CompressFrames;
	int HoldFrames;
	int RecoverFrames;
	int f_10;
	int f_14;
	int f_18;
	int f_1C;

	void Update()
		{ THISCALL(0x70ED10); }

	void Fire()
		{ THISCALL(0x70ECE0); }
};

class TechnoClass : public RadioClass
{
public:
	//IPersistStream
	virtual HRESULT _stdcall Load(IStream* pStm)
		{ PUSH_VAR32(pStm); PUSH_VAR32(this); CALL(0x70BF50); }
	virtual HRESULT _stdcall Save(IStream* pStm, BOOL fClearDirty)
		{ PUSH_VAR32(fClearDirty); PUSH_VAR32(pStm); PUSH_VAR32(this); CALL(0x70C250); }

	//Destructor
	virtual ~TechnoClass()
		{ THISCALL(0x6F4500); }

	//AbstractClass
	virtual void Init()
		{ THISCALL(0x6F3F40); }
	virtual void PointerExpired(void* p, bool bUnknown)
		{ PUSH_VAR8(bUnknown); PUSH_VAR32(p); THISCALL(0x7077C0); }
	virtual void CalculateChecksum(void* pChkSum)
		{ PUSH_VAR32(pChkSum); THISCALL(0x70C270); }
	virtual int GetOwningHouseIndex()
		{ return Owner->get_ArrayIndex(); }
	virtual HouseClass* GetOwningHouse()
		{ return Owner; }
	virtual void Update()
		{ THISCALL(0x6F9E50); }

	//ObjectClass
	virtual void AnimPointerExpired(AnimClass* pAnim)
		{ PUSH_VAR32(pAnim); THISCALL(0x710410); }
	virtual bool IsSelectable()
		{ THISCALL(0x6F32D0); }
	virtual eVisualType VisualCharacter(VARIANT_BOOL flag, DWORD dwUnk)
		{ PUSH_VAR32(dwUnk); PUSH_VAR8(flag); THISCALL(0x703860); }
	virtual eAction MouseOverCell(CellStruct uCell, bool bUnk1, bool bUnk2)
		{ PUSH_VAR8(bUnk2); PUSH_VAR8(bUnk1); PUSH_VAR32(uCell); THISCALL(0x700600); }
	virtual eAction MouseOverObject(ObjectClass* pObj, bool bUnk1)
		{ PUSH_VAR8(bUnk1); PUSH_VAR32(pObj); THISCALL(0x6FFEC0); }
	virtual ObjectTypeClass* GetType()
		{ return NULL; }
	virtual TechnoTypeClass* GetTechnoType()
		{ return NULL; }
	virtual DWORD GetTypeOwners()
/*
		{ ObjectTypeClass* ot=GetType();
								return ot->GetOwners(); }
*/
		{ return ((ObjectTypeClass *)GetType())->GetOwners(); }
	virtual bool CanBeRepaired()
		{ THISCALL(0x701140); }

	virtual bool IsActive()
		{ THISCALL(0x7010D0); } // !Deactivated , !UnderEMP, etc
	virtual bool vt_entry_A0()
		{ THISCALL(0x700C40); }
	virtual CoordStruct* GetFLH(CoordStruct *pDest, int idxWeapon, int nFLH_X, int nFLH_Y, int nFLH_Z)
		{ PUSH_VAR32(nFLH_Z); PUSH_VAR32(nFLH_Y); PUSH_VAR32(nFLH_X);
		  PUSH_VAR32(idxWeapon); PUSH_VAR32(pDest); THISCALL(0x6F3AD0); }
	virtual bool IsDisguised()
		{ return Disguised; }
	virtual bool IsDisguisedAs(HouseClass *Target)
		{ return Disguised; }
	virtual bool Exit()
		{ THISCALL(0x6F6AC0); }
	virtual bool Put(CoordStruct* pCrd, eDirection dFaceDir)
		{ PUSH_VAR32(dFaceDir); PUSH_VAR32(pCrd); THISCALL(0x6F6CA0); }
	virtual void RegisterDestruction(TechnoClass *pDestroyer) // Raises Map Events, grants veterancy, increments house kill counters
		{ PUSH_VAR32(pDestroyer); THISCALL(0x702D40); }
	virtual void RegisterDestruction(HouseClass *pDestroyerHouse) // ++destroyer's kill counters , etc
		{ PUSH_VAR32(pDestroyerHouse); THISCALL(0x703230); }
	virtual void Uncloak2()
		{ THISCALL(0x703850); } // just calls this->Uncloak(0)
	virtual int KickOutUnit(TechnoClass* pTechno, CellStruct uCell)
		{ return 0; }
	virtual void vt_entry_10C(DWORD dwUnk, DWORD dwUnk2)
		{ PUSH_VAR32(dwUnk2); PUSH_VAR32(dwUnk); THISCALL(0x6F60D0); }
	virtual void DrawExtras(DWORD dwUnk, DWORD dwUnk2) // draws ivan bomb, health bar, talk bubble, etc
		{ PUSH_VAR32(dwUnk2); PUSH_VAR32(dwUnk); THISCALL(0x6F5190); }
	virtual void vt_entry_11C()
		{ THISCALL(0x6F4A40); }
	virtual void See(DWORD dwUnk, DWORD dwUnk2)
		{ PUSH_VAR32(dwUnk2); PUSH_VAR32(dwUnk); THISCALL(0x70ADC0); }
	virtual bool SetLayer(eLayer value)
		{ PUSH_VAR32(value); THISCALL(0x6F4A70); }
	virtual bool vt_entry_13C()
		{ THISCALL(0x6FC030); }
	virtual void Flash(int nDuration)
		{ PUSH_VAR32(nDuration); THISCALL(0x6F9DD0); }
	virtual bool Select()
		{ THISCALL(0x6FBFA0); }
	virtual void IronCurtain(int nDuration, HouseClass *pSource, bool ForceShield)
		{ PUSH_VAR8(ForceShield); PUSH_VAR32(pSource); PUSH_VAR32(nDuration); THISCALL(0x70E2B0); }
	virtual void StopAirstrikeTimer()
		{ THISCALL(0x70E340); }
	virtual void StopAirstrikeTimer(int Duration)
		{ PUSH_VAR32(Duration); THISCALL(0x70E300); }
	virtual bool IsIronCurtained()
		{ THISCALL(0x41BF40); }
	virtual bool vt_entry_164(DWORD dwUnk, DWORD dwUnk2)
		{ PUSH_VAR32(dwUnk2); PUSH_VAR32(dwUnk); THISCALL(0x6F7970); }
	virtual int GetWeaponRange(int idxWeapon)
		{ PUSH_VAR32(idxWeapon); THISCALL(0x7012C0); }

	virtual eDamageState ReceiveDamage(int* pDamage, DWORD dwUnk1, WarheadTypeClass* pWH,
	  ObjectClass* pAttacker, DWORD dwUnk2, DWORD dwUnk3, HouseClass* pAttackingHouse)
		{ PUSH_VAR32(pAttackingHouse); PUSH_VAR32(dwUnk3); PUSH_VAR32(dwUnk2); PUSH_VAR32(pAttacker);
		  PUSH_VAR32(pWH); PUSH_VAR32(dwUnk1); PUSH_VAR32(pDamage); THISCALL(0x701900); }
	virtual void FreeCaptured()
		{ THISCALL(0x710460); }
	virtual void UpdatePosition(int dwUnk)
		{ PUSH_VAR32(dwUnk); THISCALL(0x6F5090); }
	virtual void ReceiveCommand(TechnoClass *From, eRadioCommands rcDoThis, DWORD dwUnk3)
		{ PUSH_VAR32(dwUnk3); PUSH_VAR32(From); PUSH_VAR32(rcDoThis); THISCALL(0x6F4AB0); }
	virtual bool vt_entry_198(DWORD dwUnk)
		{ PUSH_VAR32(dwUnk); THISCALL(0x6F4960); }

	virtual bool IsBeingWarpedOut()
		{ return BeingWarpedOut; }
	virtual bool IsWarpingIn()
		{ return WarpingOut; }
	virtual bool IsWarpingSomethingOut()
		{ THISCALL(0x70C5D0); }
	virtual bool IsNotWarping()
		{ THISCALL(0x70C5F0); }
	virtual LightConvertClass *GetRemapColour()
		{ THISCALL(0x705D70); }
	
	//MissionClass
	virtual void vt_entry_1F4(eMission mission)
		{ PUSH_VAR32(mission); THISCALL(0x7013A0); }
	virtual bool Mission_Revert()
		{ THISCALL(0x7013E0); }

	//TechnoClass
	virtual bool IsUnitFactory()
		{ return false; }
	virtual bool IsCloakable()
		{ return Cloakable; }
	virtual bool vt_entry_28C()
		{ THISCALL(0x6F3280); }
	virtual bool vt_entry_290()
		{ return false; }
	virtual bool SelfHealNow()
		{ THISCALL(0x70BE80); }
	virtual bool IsVoxel()
		{ THISCALL(0x6F9E10); }
	virtual bool vt_entry_29C()
		{ return true; }
	virtual bool ShouldBeCloaked()
		{ THISCALL(0x6FBDC0); }
	virtual bool ShouldNotBeCloaked()
		{ THISCALL(0x6FBC90); }
	virtual FacingStruct* vt_entry_2A8(FacingStruct* pFacing)
		{ PUSH_VAR32(pFacing); THISCALL(0x4E0150); }
	virtual bool IsArmed()
		{ THISCALL(0x701120); } // GetWeapon(primary) && GetWeapon(primary)->WeaponType
	virtual void vt_entry_2B0(DWORD dwUnk, DWORD dwUnk2, DWORD dwUnk3, DWORD dwUnk4, DWORD dwUnk5)
		{ PUSH_VAR32(dwUnk5); PUSH_VAR32(dwUnk4); PUSH_VAR32(dwUnk3); PUSH_VAR32(dwUnk2); PUSH_VAR32(dwUnk);
		  THISCALL(0x70C620); }
	virtual double GetStoragePercentage()
		{ THISCALL(0x708BC0); }
	virtual int GetPipFillLevel()
		{ THISCALL(0x708C30); }
	virtual int GetRefund()
		{ THISCALL(0x70ADA0); }
	virtual int GetThreatValue()
		{ THISCALL(0x708BB0); }
	virtual bool vt_entry_2C4(DWORD dwUnk)
		{ return true; }
	virtual DWORD vt_entry_2C8(DWORD dwUnk, DWORD dwUnk2)
		{ PUSH_VAR32(dwUnk2); PUSH_VAR32(dwUnk); THISCALL(0x6FDA00); }
	virtual bool vt_entry_2CC(DWORD dwUnk)
		{ PUSH_VAR32(dwUnk); THISCALL(0x707F60); }
	virtual int GetCrewCount()
		{ THISCALL(0x6F3950); }
	virtual int GetAntiAirValue()
		{ return 0; }
	virtual int GetAntiArmorValue()
		{ return 0; }
	virtual int GetAntiInfantryValue()
		{ return 0; }
	virtual void vt_entry_2E0()
		{ }
	virtual DWORD SelectWeapon(ObjectClass *Target)
		{ PUSH_VAR32(Target); THISCALL(0x6F3330); }
	virtual int SelectNavalTargeting(ObjectClass *Target)
		{ PUSH_VAR32(Target); THISCALL(0x6F3820); }
	virtual DWORD vt_entry_2EC(DWORD dwUnk)
		{ PUSH_VAR32(dwUnk); THISCALL(0x704350); }
	virtual int vt_entry_2F0()
		{ return 2; }
	virtual CellStruct* vt_entry_2F4()
		{ THISCALL(0x459DB0); }
	virtual void vt_entry_2F8(DWORD dwUnk)
		{ }
	virtual DWORD vt_entry_2FC(DWORD dwUnk, DWORD dwUnk2, DWORD dwUnk3)
		{ PUSH_VAR32(dwUnk3); PUSH_VAR32(dwUnk2); PUSH_VAR32(dwUnk);
								THISCALL(0x70AD50); }
	virtual DWORD vt_entry_300(DWORD dwUnk, DWORD dwUnk2)
		{ PUSH_VAR32(dwUnk2); PUSH_VAR32(dwUnk); THISCALL(0x6F3D60); }
	virtual DWORD vt_entry_304(DWORD dwUnk, DWORD dwUnk2)
		{ PUSH_VAR32(dwUnk2); PUSH_VAR32(dwUnk); THISCALL(0x708C10); }
	virtual DWORD vt_entry_308(DWORD dwUnk)
		{ PUSH_VAR32(dwUnk); THISCALL(0x708D70); }
	virtual InfantryTypeClass* GetCrew()
		{ THISCALL(0x707D20); }
	virtual bool vt_entry_310()
		{ THISCALL(0x700D10); }
	virtual bool CanDeploySlashUnload()
		{ THISCALL(0x700D50); }
	virtual int GetROF(int nWeapon)
		{ PUSH_VAR32(nWeapon); THISCALL(0x6FCFA0); }
	virtual int vt_entry_31C(int dwUnk)
		{ PUSH_VAR32(dwUnk); THISCALL(0x707E60); }
	virtual bool vt_entry_320()
		{ return false; }
	virtual bool vt_entry_324(DWORD dwUnk)
		{ PUSH_VAR32(dwUnk); THISCALL(0x70D1D0); }
	virtual bool vt_entry_328()
		{ THISCALL(0x70D420); }
	virtual bool vt_entry_32C(DWORD dwUnk)
		{ PUSH_VAR32(dwUnk); THISCALL(0x70D460); }
	virtual bool IsEngineer()
		{ return false; }
	virtual void vt_entry_334()
		{ }
	virtual DWORD vt_entry_338(DWORD dwUnk, DWORD dwUnk2, DWORD dwUnk3)
		{ PUSH_VAR32(dwUnk3); PUSH_VAR32(dwUnk2); PUSH_VAR32(dwUnk); THISCALL(0x70F8F0); }
	virtual bool OnEnterGrinder()
		{ return false; }
	virtual bool OnEnterBioReactor()
		{ return false; }
	virtual bool OnEnterTankBunker()
		{ return false; }
	virtual bool OnEnterBattleBunker()
		{ return false; }
	virtual bool OnGarrisonStructure()
		{ return false; }
	virtual bool IsPowerOnline()
		{ return false; } // only overwritten on BuildingClass
	virtual void QueueVoice(int idxVoc)
		{ PUSH_VAR32(idxVoc); THISCALL(0x708D90); }
	virtual int VoiceEnter()
		{ THISCALL(0x709020); }
	virtual int VoiceHarvest()
		{ THISCALL(0x709060); }
	virtual int VoiceSelect()
		{ THISCALL(0x708EB0); }
	virtual int VoiceCapture()
		{ THISCALL(0x708DC0); }
	virtual int VoiceMove()
		{ THISCALL(0x708FC0); }
	virtual int VoiceDeploy()
		{ THISCALL(0x708E00); }
	virtual int VoiceAttack(ObjectClass *Target)
		{ PUSH_VAR32(Target); THISCALL(0x7090A0); }
	virtual bool vt_entry_374(DWORD dwUnk)
		{ PUSH_VAR32(dwUnk); THISCALL(0x6FFE00); }
	virtual bool vt_entry_378(DWORD dwUnk, DWORD dwUnk2, DWORD dwUnk3, DWORD dwUnk4)
		{ PUSH_VAR32(dwUnk4); PUSH_VAR32(dwUnk3); PUSH_VAR32(dwUnk2); PUSH_VAR32(dwUnk); THISCALL(0x6FFBE0); }
	virtual bool IsUnderEMP()
		{ return (EMPLockRemaining > 0); }
	virtual bool vt_entry_380()
		{ return false; }
	virtual bool vt_entry_384()
		{ return false; }
	virtual void vt_entry_388(DWORD dwUnk)
		{ }
	virtual int GetDefaultSpeed()
		{ THISCALL(0x70EFE0); }
	virtual void DecreaseAmmo()
		{ if(Ammo > 0) --Ammo; }
	virtual void AddPassenger(FootClass* pPassenger)
		{ PUSH_VAR32(pPassenger); THISCALL(0x710670); }
	virtual bool CanDisguiseAs(ObjectClass *Target)
		{ PUSH_VAR32(Target); THISCALL(0x70EF00); }
	virtual bool TargetAndEstimateDamage(DWORD dwUnk, DWORD dwUnk2)
		{ PUSH_VAR32(dwUnk2); PUSH_VAR32(dwUnk); THISCALL(0x709820); }
	virtual DWORD vt_entry_3A0()
		{ THISCALL(0x6FCD40); }
	virtual bool TriggersCellInset(ObjectClass *Target)
		{ PUSH_VAR32(Target); THISCALL(0x6F7660); }
	virtual bool vt_entry_3A8(DWORD dwUnk)
		{ PUSH_VAR32(dwUnk); THISCALL(0x6F77B0); }
	virtual DWORD vt_entry_3AC(DWORD dwUnk)
		{ PUSH_VAR32(dwUnk); THISCALL(0x6F7780); }
	virtual DWORD vt_entry_3B0(DWORD dwUnk)
		{ PUSH_VAR32(dwUnk); THISCALL(0x6F7930); }
	virtual DWORD vt_entry_3B4(DWORD dwUnk)
		{ PUSH_VAR32(dwUnk); THISCALL(0x6F78D0); }
	virtual void vt_entry_3B8() = 0; //pure virtual
	virtual DWORD vt_entry_3BC(DWORD dwUnk, DWORD dwUnk2)
		{ PUSH_VAR32(dwUnk2); PUSH_VAR32(dwUnk); THISCALL(0x6FC090); }
	virtual int vt_entry_3C0(DWORD dwUnk, DWORD dwUnk2, DWORD dwUnk3, DWORD dwUnk4)
		{ PUSH_VAR32(dwUnk4); PUSH_VAR32(dwUnk3); PUSH_VAR32(dwUnk2); PUSH_VAR32(dwUnk); THISCALL(0x6FC0B0); }
	virtual CellClass* SelectAutoTarget(eTargetFlags TargetFlags, int CurrentThreat, bool OnlyTargetHouseEnemy)
		{ PUSH_VAR8(OnlyTargetHouseEnemy); PUSH_VAR32(CurrentThreat); PUSH_VAR32(TargetFlags); THISCALL(0x6F8DF0); }
	virtual void SetTarget(AbstractClass *Target)
		{ PUSH_VAR32(Target); THISCALL(0x6FCDB0); }
	virtual DWORD Fire(ObjectClass* pTarget, int nWeapon)
		{ PUSH_VAR32(nWeapon); PUSH_VAR32(pTarget); THISCALL(0x6FDD50); }
	virtual DWORD vt_entry_3D0()
		{ THISCALL(0x70F850); }
	virtual void SetOwningHouse(HouseClass* pHouse, DWORD dwUnk)
		{ PUSH_VAR32(dwUnk); PUSH_VAR32(pHouse); THISCALL(0x7014A0); }
	virtual void vt_entry_3D8(DWORD dwUnk, DWORD dwUnk2, DWORD dwUnk3)
		{ PUSH_VAR32(dwUnk3); PUSH_VAR32(dwUnk2); PUSH_VAR32(dwUnk);
		  THISCALL(0x70B280); }
	virtual bool Crash(DWORD dwUnk)
		{ return false; }
	virtual bool IsAreaFire()
		{ THISCALL(0x70DD50); }
	virtual int IsNotSprayAttack()
		{ THISCALL(0x70DD70); } // ![this]SprayAttack
	virtual int vt_entry_3E8()
		{ return 1; }
	virtual int IsNotSprayAttack2()
		{ THISCALL(0x70DDA0); } // ![this]SprayAttack
	virtual WeaponStruct* GetDeployWeapon()
		{ THISCALL(0x70E120); }
	virtual WeaponStruct* GetTurretWeapon()
		{ THISCALL(0x70E1A0); }
	virtual WeaponStruct* GetWeapon(int nWeaponNumber)
		{ PUSH_VAR32(nWeaponNumber); THISCALL(0x70E140); }
	virtual bool HasTurret()
		{ return ((TechnoTypeClass*)GetType())->get_Turret(); }
	virtual bool CanOccupyFire()
		{ return false; }
	virtual DWORD vt_entry_404()
		{ return 0; }
	virtual int GetOccupantCount()
		{ return 0; }
	virtual void OnFinishRepair()
		{ THISCALL(0x701410); }
	virtual void UpdateTimers()
		{ THISCALL(0x6FB740); }
	virtual void CreateGap()
		{ THISCALL(0x6FB170); } 
	virtual void DestroyGap()
		{ THISCALL(0x6FB470); }
	virtual void vt_entry_41C()
		{ THISCALL(0x70B570); }
	virtual void Sensed()
		{ THISCALL(0x6F4EB0); } // removes cloak from stealth units
	virtual void Reload()
		{ THISCALL(0x6FB010); }
	virtual void vt_entry_428()
		{ }
	virtual CoordStruct* GetTargetCoords(CoordStruct* pCrd)
		{ PUSH_VAR32(pCrd); THISCALL(0x705CA0); }
	virtual bool IsNotWarpingIn()
		{ THISCALL(0x705D50); } // not chronoshifting in
	virtual bool vt_entry_434(DWORD dwUnk)
		{ return false; }
	virtual void vt_entry_438(DWORD dwUnk, DWORD dwUnk2)
		{ }
	virtual DWORD vt_entry_43C(DWORD dwUnk)
		{ PUSH_VAR32(dwUnk); THISCALL(0x70ED80); }
	virtual bool vt_entry_440(DWORD dwUnk)
		{ PUSH_VAR32(dwUnk); THISCALL(0x70EE30); }

	//nooooooooooooooooooooooooooooooo (vader-style)
	//thought 666 is the number of the beast? in hex it's 444 D=
	virtual void vt_entry_444(DWORD dwUnk, DWORD dwUnk2, DWORD dwUnk3, DWORD dwUnk4, DWORD dwUnk5,
												DWORD dwUnk6, DWORD dwUnk7, DWORD dwUnk8, DWORD dwUnk9, DWORD dwUnk10)
		{ PUSH_VAR32(dwUnk10); PUSH_VAR32(dwUnk9); PUSH_VAR32(dwUnk8); PUSH_VAR32(dwUnk7); PUSH_VAR32(dwUnk6);
		  PUSH_VAR32(dwUnk5); PUSH_VAR32(dwUnk4); PUSH_VAR32(dwUnk3); PUSH_VAR32(dwUnk2); PUSH_VAR32(dwUnk);
		  THISCALL(0x706640); }
	virtual void vt_entry_448(DWORD dwUnk, DWORD dwUnk2)
		{ }
	virtual void DrawHealthBar(DWORD dwUnk, DWORD dwUnk2, DWORD dwUnk3)
		{ PUSH_VAR32(dwUnk3); PUSH_VAR32(dwUnk2); PUSH_VAR32(dwUnk); THISCALL(0x6F64A0); }
	virtual void DrawPipScalePips(DWORD dwUnk, DWORD dwUnk2, DWORD dwUnk3)
		{ PUSH_VAR32(dwUnk3); PUSH_VAR32(dwUnk2); PUSH_VAR32(dwUnk); THISCALL(0x709A90); }
	virtual void DrawVeterancyPips(DWORD dwUnk, DWORD dwUnk2, DWORD dwUnk3)
		{ PUSH_VAR32(dwUnk3); PUSH_VAR32(dwUnk2); PUSH_VAR32(dwUnk); THISCALL(0x70A990); }
	virtual void DrawExtraInfo(DWORD dwUnk, DWORD dwUnk2, DWORD dwUnk3)
		{ PUSH_VAR32(dwUnk3); PUSH_VAR32(dwUnk2); PUSH_VAR32(dwUnk); THISCALL(0x70AA60); }
	virtual void Uncloak(bool bPlaySound)
		{ PUSH_VAR8(bPlaySound); THISCALL(0x7036C0); }
	virtual void Cloak(bool bPlaySound)
		{ PUSH_VAR8(bPlaySound); THISCALL(0x703770); }
	virtual DWORD vt_entry_464(DWORD dwUnk)
		{ PUSH_VAR32(dwUnk); THISCALL(0x70D190); }
	virtual void UpdateRefinerySmokeSystems()
		{ } // BuildingClass only
	virtual DWORD DisguiseAs(ObjectClass *Target)
		{ PUSH_VAR32(Target); THISCALL(0x70E280); }
	virtual void ClearDisguise()
		{ Disguised = false; }
	virtual bool vt_entry_474()
		{ THISCALL(0x7099E0); }
	virtual bool vt_entry_478()
		{ return false; }
	virtual void vt_entry_47C(DWORD dwUnk)
		{ }
	virtual void SetDestination(CellClass* pCell, bool bUnk)
		{ }
	virtual bool vt_entry_484(DWORD dwUnk, DWORD dwUnk2)
		{ PUSH_VAR32(dwUnk2); PUSH_VAR32(dwUnk); THISCALL(0x709A40); }
	virtual void UpdateSight(DWORD dwUnk, DWORD dwUnk2, DWORD dwUnk3, DWORD dwUnk4, DWORD dwUnk5)
		{ PUSH_VAR32(dwUnk5); PUSH_VAR32(dwUnk4); PUSH_VAR32(dwUnk3); PUSH_VAR32(dwUnk2); PUSH_VAR32(dwUnk);
		  THISCALL(0x70AF50); }
	virtual void vt_entry_48C(DWORD dwUnk, DWORD dwUnk2, DWORD dwUnk3, DWORD dwUnk4)
		{ PUSH_VAR32(dwUnk4); PUSH_VAR32(dwUnk3); PUSH_VAR32(dwUnk2); PUSH_VAR32(dwUnk);
		  THISCALL(0x70B1D0); }
	virtual void vt_entry_490(DWORD dwUnk, DWORD dwUnk2) = 0; //pure virtual
	virtual void vt_entry_494()
		{ THISCALL(0x70CC90); }
	virtual void vt_entry_498()
		{ THISCALL(0x70CCC0); }
	virtual void vt_entry_49C()
		{ THISCALL(0x70CCF0); }
	virtual void vt_entry_4A0(DWORD dwUnk)
		{ PUSH_VAR32(dwUnk); THISCALL(0x70D990); }
	virtual void vt_entry_4A4(DWORD dwUnk)
		{ PUSH_VAR32(dwUnk); THISCALL(0x70F000); }
	virtual void vt_entry_4A8()
		{ }
	virtual bool vt_entry_4AC()
		{ return false; }
	virtual bool vt_entry_4B0()
		{ return false; }
	virtual int vt_entry_4B4()
		{ return -1; }
	virtual CoordStruct* vt_entry_4B8(CoordStruct* pCrd)
		{ pCrd->X=-1; pCrd->Y=-1; pCrd->Z=-1; return pCrd; }
	virtual DWORD vt_entry_4BC()
		{ THISCALL(0x70F070); }
	virtual bool CanAttack()
		{ THISCALL(0x70F090); }
	virtual bool vt_entry_4C4()
		{ return false; }
	virtual bool vt_entry_4C8()
		{ return false; }
	virtual void vt_entry_4CC()
		{ }
	virtual bool vt_entry_4D0()
		{ return false; }

	//Constructor
	TechnoClass(HouseClass* owner):RadioClass(false)
		{ PUSH_VAR32(owner); THISCALL(0x6F2B40); }

	//non-virtual
	void ReloadNow()
		{ THISCALL(0x6FB080); }

	bool ShouldSuppress(CellStruct *coords)
		{ PUSH_VAR32(coords); THISCALL(0x6F79A0); }

// CanTargetWhatAmI is a bitfield, if(!(CanTargetWhatAmI & (1 << tgt->WhatAmI())) { fail; }

// slave of the next one
	bool CanAutoTargetObject(eTargetFlags TargetFlags, int CanTargetWhatAmI, int WantedDistance,
	 TechnoClass *Target, signed int *ThreatPosed, DWORD dwUnk, CoordStruct *SourceCoords)
	{ PUSH_VAR32(SourceCoords); PUSH_VAR32(dwUnk); PUSH_VAR32(ThreatPosed); PUSH_VAR32(Target);
		PUSH_VAR32(WantedDistance); PUSH_VAR32(CanTargetWhatAmI); PUSH_VAR32(TargetFlags);
		THISCALL(0x6F7CA0);
	}

// called by AITeam Attack Target Type and autoscan
	bool TryAutoTargetObject(eTargetFlags TargetFlags, int CanTargetWhatAmI, CellStruct *Coords,
	 DWORD dwUnk1, DWORD *dwUnk2, signed int *ThreatPosed, DWORD dwUnk3)
	{ PUSH_VAR32(dwUnk3); PUSH_VAR32(ThreatPosed); PUSH_VAR32(dwUnk2); PUSH_VAR32(dwUnk1);
		PUSH_VAR32(Coords); PUSH_VAR32(CanTargetWhatAmI); PUSH_VAR32(TargetFlags);
		THISCALL(0x6F8960);
	}

protected:
	TechnoClass():RadioClass(false)
		{ }
	TechnoClass(bool X):RadioClass(X)
		{ }

	//===========================================================================
	//===== Properties ==========================================================
	//===========================================================================
public:
	PROPERTY_STRUCT(FlashData,   Flashing);
	PROPERTY(int,                DeployingAnimStartFrame);
	PROPERTY(bool,               unknown_bool_FC);
	PROPERTY_STRUCT(TimerStruct,        DeployingAnimTimer);
	PROPERTY(DWORD,              unknown_10C);
	PROPERTY(int,                unknown_int_110);
	PROPERTY_STRUCT(PassengersClass,		Passengers);
	PROPERTY(TechnoClass*,       Transporter); // unit carrying me
	PROPERTY(int,                unknown_int_120);
	PROPERTY(int,                CurrentWeaponNumber); // for gattling
	PROPERTY(int,                unknown_int_128);
	PROPERTY(AnimClass*,         BehindAnim);
	PROPERTY(AnimClass*,         DeployAnim);
	PROPERTY(bool,               InAir);
	PROPERTY(int,                CurrentTurret);
	PROPERTY(int,                CurrentRanking); //see RANK definitons, only used for promotion detection
	PROPERTY(DWORD,              CurrentGattlingStage);
	PROPERTY(DWORD,              unknown_144);
	PROPERTY(DWORD,              unknown_148);
	PROPERTY(HouseClass*,        OwningPlayer2); // only set in ctor
	PROPERTY_STRUCT(VeterancyStruct,    Veterancy);
	PROPERTY(double,             ArmorMultiplier);
	PROPERTY(double,             FirepowerMultiplier);
	PROPERTY_STRUCT(TimerStruct, unknown_timer_168);
	PROPERTY_STRUCT(TimerStruct, RadarFlashTimer);
	PROPERTY_STRUCT(TimerStruct, TargetingTimer); //Duration = 45 on init!
	PROPERTY_STRUCT(TimerStruct, IronCurtainTimer);
	PROPERTY_STRUCT(TimerStruct, IronTintTimer); // how often to alternate the effect color
	PROPERTY(int,                IronTintStage); // ^
	PROPERTY_STRUCT(TimerStruct, AirstrikeTimer);
	PROPERTY_STRUCT(TimerStruct, AirstrikeTintTimer); // tracks alternation of the effect color
	PROPERTY(DWORD,              AirstrikeTintStage); //  ^
	PROPERTY(int,                IronCurtainActive);	//0 or 1, NOT a bool
	PROPERTY(bool,               Deactivated); //Robot Tanks without power for instance
	PROPERTY(TechnoClass*,       DrainTarget); // eg Disk -> PowerPlant, this points to PowerPlant
	PROPERTY(TechnoClass*,       DrainingMe);  // eg Disk -> PowerPlant, this points to Disk
	PROPERTY(AnimClass*,         DrainAnim);
	PROPERTY(bool,               Disguised);
	PROPERTY(DWORD,              DisguiseCreationFrame);
	PROPERTY_STRUCT(TimerStruct, InfantryBlinkTimer); // Rules->InfantryBlinkDisguiseTime , detects mirage firing per description
	PROPERTY_STRUCT(TimerStruct, DisguiseBlinkTimer); // disguise disruption timer
	PROPERTY(bool,               unknown_bool_1F8);
	PROPERTY_STRUCT(TimerStruct, ReloadTimer);
	PROPERTY(DWORD,              unknown_208);
	PROPERTY(DWORD,              unknown_20C);
	PROPERTY(DWORD,              unknown_210);
	PROPERTY(int,                Group); //0-9, assigned by CTRL+Number, these kinds // also set by aimd TeamType->Group !
	PROPERTY(TechnoClass*,       FocusOnUnit); // when told to guard a unit or such
	PROPERTY(HouseClass*,        Owner);
	PROPERTY(eCloakStates,       CloakState);
	PROPERTY(int,                CloakingStage); // phase from [opaque] -> [fading] -> [transparent] , [General]CloakingStages= long
	PROPERTY(bool,               Cloaking);
	PROPERTY_STRUCT(TimerStruct, CloakTimer);
	PROPERTY(int,                CloakingSpeed);
	PROPERTY(int,                CloakingIncrementAmount); //defaults to 1
	PROPERTY_STRUCT(TimerStruct, CloakDelayTimer); // delay before cloaking again
	PROPERTY(DWORD,              unknown_24C);
	PROPERTY(bool,               unknown_bool_250);
	PROPERTY_STRUCT(CoordStruct, unknown_point3d_254);
	PROPERTY(DWORD,              unknown_260);
	PROPERTY(DWORD,              unknown_264);
	PROPERTY(bool,               GapSuperCharged); // GapGenerator, when SuperGapRadiusInCells != GapRadiusInCells, you can deploy the gap to boost radius
	PROPERTY(bool,               GeneratingGap); // is currently generating gap
	PROPERTY(int,                GapRadius);
	PROPERTY(bool,               BeingWarpedOut); // is being warped by CLEG
	PROPERTY(bool,               WarpingOut); // phasing in after chrono-jump
	PROPERTY(bool,               unknown_bool_272);
	PROPERTY(BYTE,               unused_273);
	PROPERTY(TemporalClass*,     TemporalImUsing); // CLEG attacking Power Plant : CLEG's this 
	PROPERTY(TemporalClass*,     TemporalTargetingMe); 	// CLEG attacking Power Plant : PowerPlant's this 
	PROPERTY(bool,               IsImmobilized); // by chrono aftereffects
	PROPERTY(DWORD,              unknown_280);
	PROPERTY(int,                ChronoLockRemaining); // countdown after chronosphere warps things around
	PROPERTY_STRUCT(CoordStruct, unknown_point3d_288);
	PROPERTY(AirstrikeClass*,    Airstrike); //Boris
	PROPERTY(bool,               Berzerk);
	PROPERTY(DWORD,              BerzerkDurationLeft);
	PROPERTY(DWORD,              unknown_2A0);
	PROPERTY(DWORD,              unknown_2A4);
	PROPERTY(DWORD,              unknown_2A8);
	PROPERTY(FootClass*,         LocomotorTarget); // mag->LocoTarget = victim
	PROPERTY(FootClass*,         LocomotorSource); // victim->LocoSource = mag
	PROPERTY(ObjectClass*,       Target); //if attacking
	PROPERTY(ObjectClass*,       LastTarget);
	PROPERTY(CaptureManagerClass*, CaptureManager); //for Yuris
	PROPERTY(TechnoClass*,       MindControlledBy);
	PROPERTY(bool,               MindControlledByAUnit);
	PROPERTY(AnimClass*,         MindControlRingAnim);
	PROPERTY(HouseClass*,        MindControlledByHouse); //used for a TAction
	PROPERTY(SpawnManagerClass*, SpawnManager);
	PROPERTY(TechnoClass*,       SpawnOwner); // on DMISL , points to DRED and such
	PROPERTY(SlaveManagerClass*, SlaveManager);
	PROPERTY(TechnoClass*,       SlaveOwner); // on SLAV, points to YAREFN
	PROPERTY(HouseClass*,        OriginallyOwnedByHouse); //used for mind control

		//units point to the Building bunkering them, building points to Foot contained within
	PROPERTY(TechnoClass*,       BunkerLinkedItem);

	PROPERTY(DWORD,              unknown_2E8);
	PROPERTY_STRUCT(TimerStruct, DiskLaserTimer);
	PROPERTY(DWORD,              unknown_2F8);
	PROPERTY(int,                Ammo);
	PROPERTY(int,                Value); // set to actual cost when this gets queued in factory, updated only in building's 42C

	
	PROPERTY(ParticleSystemClass*, FireParticleSystem);
	PROPERTY(ParticleSystemClass*, SparkParticleSystem);
	PROPERTY(ParticleSystemClass*, NaturalParticleSystem);
	PROPERTY(ParticleSystemClass*, DamageParticleSystem);
	PROPERTY(ParticleSystemClass*, RailgunParticleSystem);
	PROPERTY(ParticleSystemClass*, unk1ParticleSystem);
	PROPERTY(ParticleSystemClass*, unk2ParticleSystem);
	PROPERTY(ParticleSystemClass*, FiringParticleSystem);

	PROPERTY(WaveClass*,         Wave); //Beams
	PROPERTY(DWORD,              unknown_328);
	PROPERTY(DWORD,              unknown_32C);
	PROPERTY(DWORD,              unknown_330);
	PROPERTY(DWORD,              unknown_334);
	PROPERTY(InfantryTypeClass *, HijackedByInfantry); // mutant hijacker

	PROPERTY_STRUCT(OwnedTiberiumStruct, Tiberium);
	PROPERTY(DWORD,					unknown_34C);

	PROPERTY_STRUCT(TransitionTimer, UnloadTimer); // times the deploy, unload, etc. cycles

public:
	PROPERTY_STRUCT(FacingStruct,       BarrelFacing);
	PROPERTY_STRUCT(FacingStruct,       Facing);
	PROPERTY_STRUCT(FacingStruct,       TurretFacing);
	PROPERTY(DWORD,              unknown_3B8);
	PROPERTY_STRUCT(TimerStruct,        TargetLaserTimer);
	PROPERTY(short,              unknown_short_3C8);
	PROPERTY(WORD,               unknown_3CA);
	PROPERTY(bool,               unknown_bool_3CC);
	PROPERTY(bool,               unknown_bool_3CD);
	PROPERTY(bool,               unknown_bool_3CE);
	PROPERTY(bool,               unknown_bool_3CF);
	PROPERTY(bool,               unknown_bool_3D0);
	PROPERTY(bool,               HasBeenAttacked); // ReceiveDamage when not HouseClass_IsAlly
	PROPERTY(bool,               Cloakable);
	PROPERTY(bool,               IsPrimaryFactory); // doubleclicking a warfac/barracks sets it as primary
	PROPERTY(bool,               Spawned);
	PROPERTY(bool,               Produced);
	PROPERTY_STRUCT(RecoilData,  TurretRecoil);
	PROPERTY_STRUCT(RecoilData,  BarrelRecoil);
	PROPERTY(bool,               unknown_bool_418);
	PROPERTY(bool,               unknown_bool_419);
	PROPERTY(bool,               IsHumanControlled);
	PROPERTY(bool,               unknown_bool_41B);
	PROPERTY(bool,               unknown_bool_41C);
	PROPERTY(bool,               unknown_bool_41D);
	PROPERTY(bool,               unknown_bool_41E);
	PROPERTY(bool,               unknown_bool_41F);
	PROPERTY(bool,               unknown_bool_420);
	PROPERTY(bool,               RecruitableA); // these two are like Lenny and Carl, weird purpose and never seen separate
	PROPERTY(bool,               RecruitableB); // they're usually set on preplaced objects in maps
	PROPERTY(bool,               unknown_bool_423);
	PROPERTY(bool,               unknown_bool_424);
	PROPERTY(bool,               unknown_bool_425);
	PROPERTY(bool,               unknown_bool_426);
	PROPERTY(bool,               unknown_bool_427);
	PROPERTY(DWORD,              unknown_428);
	PROPERTY(HouseClass*,        ChronoWarpedByHouse);
	PROPERTY(bool,               unknown_bool_430);
	PROPERTY(bool,               unknown_bool_431);
	PROPERTY(bool,               unknown_bool_432);
	PROPERTY(DWORD,              OldTeam); // AITeamClass * really, TODO: fix when defined
	PROPERTY(bool,               unknown_bool_438);
	PROPERTY(bool,               Absorbed); // in UnitAbsorb/InfantryAbsorb or smth, lousy memory
	PROPERTY(bool,               unknown_bool_43A);
	PROPERTY(DWORD,              unknown_43C);
	PROPERTY_STRUCT(DynamicVectorClass<int>, CurrentTargetThreatValues);
	PROPERTY_STRUCT(DynamicVectorClass<AbstractClass*>, CurrentTargets);

 // if DistributedFire=yes, this is used to determine which possible targets should be ignored in the latest threat scan
	PROPERTY_STRUCT(DynamicVectorClass<AbstractClass*>, AttackedTargets);

	PROPERTY_STRUCT(Unsorted::AudioController, Audio3);

	PROPERTY(DWORD,              unknown_49C);
	PROPERTY(DWORD,              unknown_4A0);

	PROPERTY_STRUCT(Unsorted::AudioController, Audio4);

	PROPERTY(bool,               unknown_bool_4B8);
	PROPERTY(DWORD,              unknown_4BC);

	PROPERTY_STRUCT(Unsorted::AudioController, Audio5);

	PROPERTY(bool,               unknown_bool_4D4);
	PROPERTY(DWORD,              unknown_4D8);

	PROPERTY_STRUCT(Unsorted::AudioController, Audio6);

	PROPERTY(DWORD,              QueuedVoiceIndex);
	PROPERTY(DWORD,              unknown_4F4);
	PROPERTY(bool,               unknown_bool_4F8);
	PROPERTY(DWORD,				unknown_4FC);	//gets initialized with the current Frame, but this is NOT a TimerStruct!
	PROPERTY(DWORD,				unknown_500);
	PROPERTY(DWORD,              EMPLockRemaining);
	PROPERTY(DWORD,              ThreatPosed); // calculated to include cargo etc
	PROPERTY(DWORD,              ShouldLoseTargetNow);
	PROPERTY(DWORD,              FiringRadBeam); // RadBeamClass *, TODO: fix when declared
	PROPERTY(DWORD,              PlanningToken); // PlanningTokenClass *, user waypoint voodoo, TODO: fix when declared
	PROPERTY(ObjectTypeClass*,   Disguise);
	PROPERTY(HouseClass*,        DisguisedAsHouse);
};

#endif
