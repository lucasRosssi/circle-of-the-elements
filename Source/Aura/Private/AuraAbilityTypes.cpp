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
		if (StatusEffects.Num() > 0)
		{
			RepBits |= 1 << 10;
		}
		if (!ForwardVector.IsZero())
		{
			RepBits |= 1 << 11;
		}
		if (bApplyHitReact)
		{
			RepBits |= 1 << 12;
		}
		if (bIsAreaAbility)
		{
			RepBits |= 1 << 13;

			if (AreaInnerRadius > 0.f)
			{
				RepBits |= 1 << 14;
			}
			if (AreaOuterRadius > 0.f)
			{
				RepBits |= 1 << 15;
			}
			if (!AreaOrigin.IsZero())
			{
				RepBits |= 1 << 16;
			}
		}
		
	}

	Ar.SerializeBits(&RepBits, 19);

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
		SafeNetSerializeTArray_WithNetSerialize<3, FEffectParams>(Ar, StatusEffects, Map);
	}
	if (RepBits & (1 << 11))
	{
		ForwardVector.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 12))
	{
		Ar << bApplyHitReact;
	}
	if (RepBits & (1 << 13))
	{
		Ar << bIsAreaAbility;
		
		if (RepBits & (1 << 14))
		{
			Ar << AreaInnerRadius;
		}
		if (RepBits & (1 << 15))
		{
			Ar << AreaOuterRadius;	
		}
		if (RepBits & (1 << 16))
		{
			AreaOrigin.NetSerialize(Ar, Map, bOutSuccess);
		}
	}

	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}	
	
	bOutSuccess = true;
	return true;
}
