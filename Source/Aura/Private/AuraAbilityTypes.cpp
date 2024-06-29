#include "AuraAbilityTypes.h"


bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	uint32 RepBits = 0;
	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid() )
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
		if (bParried)
		{
			RepBits |= 1 << 7;
		}
		if (bCriticalHit)
		{
			RepBits |= 1 << 8;
		}
		if (bApplyingEffect)
		{
			RepBits |= 1 << 9;
		}
		if (EffectValue > 0.f)
		{
			RepBits |= 1 << 10;
		}
		if (EffectDuration > 0.f)
		{
			RepBits |= 1 << 11;
		}
		if (EffectType.IsValid())
		{
			RepBits |= 1 << 12;
		}
		if (!ForwardVector.IsZero())
		{
			RepBits |= 1 << 13;
		}
		if (bApplyHitReact)
		{
			RepBits |= 1 << 14;
		}
		
	}

	Ar.SerializeBits(&RepBits, 15);

	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = MakeShared<FHitResult>();
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}
	if (RepBits & (1 << 7))
	{
		Ar << bParried;
	}
	if (RepBits & (1 << 8))
	{
		Ar << bCriticalHit;
	}
	if (RepBits & (1 << 9))
	{
		Ar << bApplyingEffect;
	}
	if (RepBits & (1 << 10))
	{
		Ar << EffectValue;
	}
	if (RepBits & (1 << 11))
	{
		Ar << EffectDuration;
	}
	if (RepBits & (1 << 12))
	{
		if (Ar.IsLoading())
		{
			if (!EffectType.IsValid())
			{
				EffectType = MakeShared<FGameplayTag>();
			}
		}
		EffectType->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 14))
	{
		ForwardVector.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 15))
	{
		Ar << bApplyHitReact;
	}

	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}	
	
	bOutSuccess = true;
	return true;
}
