// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/ActiveAbility.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/AuraLogChannels.h"
#include "Interfaces/AttributeSetInterface.h"

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
	return MaxHitCount.AsInteger(Level) + AdditionalMaxHitCount;
}

float UActiveAbility::GetEffectChangePerHitAtLevel_Implementation(int32 Level) const
{
	return EffectChangePerHit.GetValueAtLevel(Level) + AdditionalEffectChangePerHit;
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

AActor* UActiveAbility::GetNextRicochetTarget(AActor* HitTarget)
{
  if (!IsValid(HitTarget)) return nullptr;
  
  TArray ActorsToIgnore({GetAvatarActorFromActorInfo()});
  ActorsToIgnore.Append(RicochetHitActors);
  AActor* NextTarget = UAuraAbilitySystemLibrary::GetClosestActorToTarget(
    HitTarget,
    RicochetRadius.GetValueAtLevel(GetAbilityLevel()),
    AbilityTargetTeam == ETargetTeam::Both ? AbilityTargetTeam : ETargetTeam::Friends,
    ActorsToIgnore
    );

  RicochetHitActors.Add(HitTarget);
  return NextTarget;
}

void UActiveAbility::ApplyUpgrade_Implementation(const FGameplayTag& UpgradeTag)
{
  UE_LOG(
    LogAura,
    Error,
    TEXT("ApplyUpgrade is not overriden in BP, but ability has UpgradeTags!")
  );
}

void UActiveAbility::RemoveUpgrade_Implementation(const FGameplayTag& UpgradeTag)
{
  UE_LOG(
    LogAura,
    Error,
    TEXT("RemoveUpgrade is not overriden in BP, but ability has UpgradeTags!")
  );
}

UAnimMontage* UActiveAbility::GetAnimMontage() const
{
  return MontageToPlay;
}

float UActiveAbility::GetMontagePlayRate() const
{
  float AbilitySpeed = 0.f;

  const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
  if (SpeedAttributeTag.MatchesTagExact(AuraTags.Attributes_Secondary_AttackSpeed))
  {
    AbilitySpeed = IAttributeSetInterface::Execute_GetAttackSpeed(GetOwningActorFromActorInfo());
  }
  else if (SpeedAttributeTag.MatchesTagExact(AuraTags.Attributes_Secondary_CastSpeed))
  {
    AbilitySpeed = IAttributeSetInterface::Execute_GetCastSpeed(GetOwningActorFromActorInfo());
  }
  else if (SpeedAttributeTag.MatchesTagExact(AuraTags.Attributes_Secondary_MovementSpeed))
  {
    AbilitySpeed = IAttributeSetInterface::Execute_GetMovementSpeed(GetOwningActorFromActorInfo());
  }
    
	return (MontagePlayRate.GetValueAtLevel(GetAbilityLevel()) + AdditionalMontagePlayRate) * AbilitySpeed;
}

float UActiveAbility::GetAnimRootMotionTranslateScale() const
{
	return AnimRootMotionTranslateScale.GetValueAtLevel(GetAbilityLevel()) + AdditionalAnimRootMotionTranslateScale;
}

int32 UActiveAbility::GetMaxHitCount() const
{
	return MaxHitCount.AsInteger(GetAbilityLevel()) + AdditionalMaxHitCount; 
}

float UActiveAbility::GetEffectChangePerHit() const
{
	return EffectChangePerHit.GetValueAtLevel(GetAbilityLevel()) + AdditionalEffectChangePerHit;
}

int32 UActiveAbility::GetActorCharges() const
{
  return ActorCharges.AsInteger(GetAbilityLevel()) + AdditionalActorCharges;
}

void UActiveAbility::ClearRicochetHitTargets()
{
	RicochetHitActors.Empty();
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
