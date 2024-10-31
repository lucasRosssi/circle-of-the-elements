// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/ActiveAbility.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Interfaces/AttributeSetInterface.h"

AActor* UActiveAbility::GetNextBounceTarget(AActor* HitTarget)
{
  if (!IsValid(HitTarget)) return nullptr;
  
	TArray ActorsToIgnore({GetAvatarActorFromActorInfo()});
	ActorsToIgnore.Append(BounceHitActors);
	AActor* NextTarget = UAuraAbilitySystemLibrary::GetClosestActorToTarget(
		HitTarget,
		BounceRadius.GetValueAtLevel(GetAbilityLevel()),
		AbilityTargetTeam == ETargetTeam::Both ? AbilityTargetTeam : ETargetTeam::Friends,
		ActorsToIgnore
		);

	BounceHitActors.Add(HitTarget);
	return NextTarget;
}

bool UActiveAbility::CommitAbility(
  const FGameplayAbilitySpecHandle Handle,
  const FGameplayAbilityActorInfo* ActorInfo,
  const FGameplayAbilityActivationInfo ActivationInfo,
  FGameplayTagContainer* OptionalRelevantTags
)
{
  if (CheckForClarityEffect(ActorInfo))
  {
    return Super::CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false, OptionalRelevantTags);
  }

  return Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags);
}

bool UActiveAbility::CommitAbilityCost(
  const FGameplayAbilitySpecHandle Handle,
  const FGameplayAbilityActorInfo* ActorInfo,
  const FGameplayAbilityActivationInfo ActivationInfo,
  FGameplayTagContainer* OptionalRelevantTags
)
{
  if (CheckForClarityEffect(ActorInfo)) return true;
  
  return Super::CommitAbilityCost(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags);
}

int32 UActiveAbility::GetMaxHitCountAtLevel_Implementation(int32 Level) const
{
	return MaxHitCount.AsInteger(Level);
}

float UActiveAbility::GetEffectChangePerHitAtLevel_Implementation(int32 Level) const
{
	return EffectChangePerHit.GetValueAtLevel(Level);
}

bool UActiveAbility::IsActiveAbility_Implementation() const
{
	return true;
}

void UActiveAbility::GetMontageParams(
	UAnimMontage*& Montage,
	float& PlayRate,
	float& RootMotionScale
	) const
{
	Montage = GetAnimMontage();
	PlayRate = GetMontagePlayRate();
	RootMotionScale = GetAnimRootMotionTranslateScale();
}

UAnimMontage* UActiveAbility::GetAnimMontage() const
{
  return MontageToPlay;
}

float UActiveAbility::GetMontagePlayRate() const
{
  const float ActionSpeed = IAttributeSetInterface::Execute_GetActionSpeed(GetOwningActorFromActorInfo());
	return MontagePlayRate.GetValueAtLevel(GetAbilityLevel()) * ActionSpeed;
}

float UActiveAbility::GetAnimRootMotionTranslateScale() const
{
	return AnimRootMotionTranslateScale.GetValueAtLevel(GetAbilityLevel());
}

int32 UActiveAbility::GetMaxHitCount() const
{
	return MaxHitCount.AsInteger(GetAbilityLevel()); 
}

float UActiveAbility::GetEffectChangePerHit() const
{
	return EffectChangePerHit.GetValueAtLevel(GetAbilityLevel());
}

void UActiveAbility::ClearBounceHitTargets()
{
	BounceHitActors.Empty();
}

bool UActiveAbility::CheckForClarityEffect(const FGameplayAbilityActorInfo* ActorInfo)
{
  if (!CostGameplayEffectClass) return true;
  
  const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
  
  if (ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(AuraTags.StatusEffects_Buff_Clarity))
  {
    const FGameplayTagContainer Container = FGameplayTagContainer({ AuraTags.StatusEffects_Buff_Clarity });
    const FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAllOwningTags(Container);
    ActorInfo->AbilitySystemComponent->RemoveActiveEffects(Query, 1);

    return true;
  }
  return false;
}
