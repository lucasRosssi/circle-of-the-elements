// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/ActiveDamageAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Aura/AuraLogChannels.h"

FAbilityParams UActiveDamageAbility::MakeAbilityParamsFromDefaults(AActor* TargetActor) const
{
  float CurrentDamage = GetDamageAtLevel(GetAbilityLevel());
  FAbilityParams AbilityParams = Super::MakeAbilityParamsFromDefaults(TargetActor);
  AbilityParams.DamageParams.DamageEffectClass = DamageEffectClass;
  AbilityParams.DamageParams.DamageType = DamageType;
  AbilityParams.DamageParams.bApplyHitReact = bApplyHitReact;

  if (bIsComboSequence)
  {
    if (ComboIndex == ComboSequenceData.Num() - 1)
    {
      const float FinishEffectChange = ComboFinishMagnitude.GetValueAtLevel(GetAbilityLevel());
      CurrentDamage = CurrentDamage * (1.f + FinishEffectChange);
    }
    else
    {
      const float EffectChange = ComboMagnitude.GetValueAtLevel(GetAbilityLevel());
      CurrentDamage = CurrentDamage * (1.f + EffectChange * ComboIndex);
    }
  }

  AbilityParams.DamageParams.BaseDamage = CurrentDamage;


  return AbilityParams;
}

void UActiveDamageAbility::EndAbility(
  const FGameplayAbilitySpecHandle Handle,
  const FGameplayAbilityActorInfo* ActorInfo,
  const FGameplayAbilityActivationInfo ActivationInfo,
  bool bReplicateEndAbility,
  bool bWasCancelled
)
{
  Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

  ComboIndex = 0;
  bComboInputPressed = false;
  bInComboWindow = false;
}

bool UActiveDamageAbility::IsDamageAbility_Implementation() const
{
  return true;
}

FGameplayTag UActiveDamageAbility::GetDamageTypeTag_Implementation() const
{
  return DamageType;
}

FAbilityParams UActiveDamageAbility::ApplyEffectChangePerHitToAbilityParams(
  FAbilityParams& AbilityParams,
  int32 HitCount
)
{
  if (RangedHitMode != EAbilityHitMode::Default && GetMaxHitCount() > 1)
  {
    AbilityParams.DamageParams.BaseDamage *= (1.f + GetEffectChangePerHit() * HitCount);
    if (AbilityParams.EffectParams.Num() > 0)
    {
      for (auto& StatusEffect : AbilityParams.EffectParams)
      {
        StatusEffect.Value *= (1.f + GetEffectChangePerHit() * HitCount);
        StatusEffect.Duration *= (1.f + GetEffectChangePerHit() * HitCount);
      }
    }
  }

  return AbilityParams;
}

float UActiveDamageAbility::GetDamageAtLevel(int32 Level) const
{
  return Damage.GetValueAtLevel(Level) + AdditionalDamage;
}

int32 UActiveDamageAbility::GetRoundedDamageAtLevel_Implementation(int32 Level) const
{
  return Damage.AsInteger(Level);
}

void UActiveDamageAbility::HandleComboSequence()
{
  if (bIsPlayerAbility && (!bComboInputPressed || !bInComboWindow)) return;

  ComboIndex += 1;
  bComboInputPressed = false;
  bInComboWindow = false;

  if (!ComboSequenceData.IsValidIndex(ComboIndex)) return;

  OnNextComboSequence();
}

UAnimMontage* UActiveDamageAbility::GetAnimMontage() const
{
  return bIsComboSequence ? GetComboData().Montage : MontageToPlay;
}

FName UActiveDamageAbility::GetAbilitySocketName() const
{
  return bIsComboSequence ? GetComboData().SocketName : AbilitySocketName;
}

bool UActiveDamageAbility::IsUsingWeapon() const
{
  return bIsComboSequence ? GetComboData().bUseWeaponSocket : bUseWeaponSocket;
}

float UActiveDamageAbility::GetComboMagnitude() const
{
  return ComboMagnitude.GetValueAtLevel(GetAbilityLevel()) + AdditionalComboMagnitude;
}

float UActiveDamageAbility::GetComboFinishMagnitude() const
{
  return ComboFinishMagnitude.GetValueAtLevel(GetAbilityLevel()) + AdditionalComboFinishMagnitude;
}

FComboData UActiveDamageAbility::GetComboData() const
{
  if (!ComboSequenceData.IsValidIndex(ComboIndex))
  {
    UE_LOG(LogAura, Error, TEXT("Tried to access invalid index in ComboSequenceData!"));
    return FComboData();
  }

  FComboData ComboData = ComboSequenceData[ComboIndex];

  if (!ComboData.bUseCustomData)
  {
    ComboData.bUseWeaponSocket = bUseWeaponSocket;
    ComboData.SocketName = AbilitySocketName;
  }

  return ComboData;
}
