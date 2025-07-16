// Copyright Lucas Rossi


#include "Actor/SpiritActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/Abilities/BaseAbility.h"
#include "Aura/AuraLogChannels.h"
#include "Interfaces/AbilityInterface.h"
#include "Interfaces/CombatInterface.h"

ASpiritActor::ASpiritActor()
{
  PrimaryActorTick.bCanEverTick = true;
}

void ASpiritActor::SetAbilityTag(const FGameplayTag& InTag)
{
  if (!InTag.MatchesTag(FAuraGameplayTags::Get().Abilities))
  {
    UE_LOG(LogAura, Error, TEXT("[SpiritActor] Invalid Ability tag: %s"), *InTag.ToString())
  }
  
  AbilityTag = InTag;
}

void ASpiritActor::SetElementTag(const FGameplayTag& InTag)
{
  if (!InTag.MatchesTag(FAuraGameplayTags::Get().Abilities_Element))
  {
    UE_LOG(LogAura, Error, TEXT("[SpiritActor] Invalid Element tag: %s"), *InTag.ToString())
  }
  
  ElementTag = InTag;
}

void ASpiritActor::SetCooldownTag(const FGameplayTag& InTag)
{
  if (!InTag.MatchesTag(FAuraGameplayTags::Get().Cooldown))
  {
    UE_LOG(LogAura, Error, TEXT("[SpiritActor] Invalid Cooldown tag: %s"), *InTag.ToString())
  }
  
  CooldownTag = InTag;
}

void ASpiritActor::SetChargeTagAndCount(const FGameplayTag& InTag, int32 ChargesCount)
{
  if (!InTag.MatchesTag(FAuraGameplayTags::Get().Charges))
  {
    UE_LOG(LogAura, Error, TEXT("[SpiritActor] Invalid Charge tag: %s"), *InTag.ToString())
  }
  
  ChargeTag = InTag;
  MaxChargesCount = ChargesCount;
}

UNiagaraComponent* ASpiritActor::GetSpiritNiagara_Implementation()
{
  return nullptr;
}

FVector ASpiritActor::GetAbilityUseLocation()
{
  if (!GetOwnerASC() || !Ability) return FVector();

  const FName SocketName = IAbilityInterface::Execute_GetActiveAbilitySocketName(Ability);
  const bool bSocketInWeapon = IAbilityInterface::Execute_IsAbilityUsingWeapon(Ability);

  return ICombatInterface::Execute_GetAbilitySocketLocation(GetOwner(), SocketName, bSocketInWeapon);
}

FVector ASpiritActor::GetHijackLocation()
{
  if (!Hijacker || !Hijacker->Implements<UCombatInterface>()) return FVector();

  return ICombatInterface::Execute_GetAbilitySocketLocation(Hijacker, FName("SpiritHijackSocket"), true);
}

void ASpiritActor::BeginPlay()
{
  Super::BeginPlay();

  ListenForCooldownChange();
  ListenForAbilityActivation();
}

void ASpiritActor::ListenForCooldownChange()
{
  if (!GetOwnerASC()) return;

  if (ChargeTag.IsValid())
  {
    OwnerASC->RegisterGameplayTagEvent(
      ChargeTag,
      EGameplayTagEventType::AnyCountChange
    ).AddUObject(this, &ASpiritActor::ChargeTagChanged);
  }
  else if (CooldownTag.IsValid())
  {
    OwnerASC->RegisterGameplayTagEvent(
      CooldownTag,
      EGameplayTagEventType::NewOrRemoved
    ).AddUObject(this, &ASpiritActor::CooldownTagChanged);
  }
}

void ASpiritActor::ListenForAbilityActivation()
{
  if (!GetOwnerASC() || !AbilityTag.IsValid()) return;

  OnAnyAbilityActivated = OwnerASC->AbilityActivatedCallbacks.AddUObject(this, &ASpiritActor::AbilityActivated);
}

void ASpiritActor::AbilityActivated(UGameplayAbility* InAbility)
{
  if (InAbility->GetAssetTags().HasTagExact(AbilityTag))
  {
    bUsingAbility = true;
    Ability = Cast<UBaseAbility>(InAbility);
    if (Ability)
    {
      Ability->OnAbilityStateChanged.AddUObject(this, &ASpiritActor::AbilityStateChanged);
      OwnerASC->AbilityActivatedCallbacks.Remove(OnAnyAbilityActivated);
    }
  }
}

void ASpiritActor::AbilityStateChanged(bool bActivated)
{
  bUsingAbility = bActivated;
}

void ASpiritActor::CooldownTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
  if (NewCount > 0)
  {
    GetSpiritNiagara()->Deactivate();
    SetActorTickEnabled(false);
  }
  else
  {
    GetSpiritNiagara()->Activate();
    SetActorTickEnabled(false);
  }
}

void ASpiritActor::ChargeTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
  if (!GetOwnerASC()) return;

  const FGameplayEffectQuery Query = FGameplayEffectQuery
    ::MakeQuery_MatchAnyOwningTags(ChargeTag.GetSingleTagContainer());

  const TArray<FActiveGameplayEffectHandle> Handles = OwnerASC->GetActiveEffects(Query);

  const int32 CurrentCount = Handles.IsEmpty() ? 0 : OwnerASC->GetCurrentStackCount(Handles[0]);

  if (CurrentCount == 0)
  {
    GetSpiritNiagara()->Deactivate();
    SetActorTickEnabled(false);
  }
  else
  {
    const float NewScale = (CurrentCount + 0.f) / (MaxChargesCount + 0.f);
    for (const FName& ParameterName : NiagaraScaleParameters)
    {
      GetSpiritNiagara()->SetFloatParameter(ParameterName, NewScale);
    }
    
    if (!GetSpiritNiagara()->IsActive())
    {
      GetSpiritNiagara()->Activate();
      SetActorTickEnabled(true);
    }
  }

}

UAbilitySystemComponent* ASpiritActor::GetOwnerASC()
{
  if (GetOwner() == nullptr) return nullptr;

  if (OwnerASC == nullptr)
  {
    OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
  }

  return OwnerASC;
}
