// Copyright Lucas Rossi


// ReSharper disable CppExpressionWithoutSideEffects
#include "AbilitySystem/Abilities/BaseAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/StatusEffectInfo.h"
#include "AbilitySystem/GameplayEffects/AuraStatusEffect.h"
#include "Aura/AuraLogChannels.h"
#include "Character/AuraCharacterBase.h"
#include "Character/AuraHero.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utils/AuraSystemsLibrary.h"

UBaseAbility::UBaseAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bServerRespectsRemoteAbilityCancellation = false;
}

void UBaseAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
  for (const FGameplayTag& UpgradeTag : UpgradeTags)
  {
    Execute_ApplyUpgrade(this, UpgradeTag);
  }
	
  if (IsChargesModeActive())
  {
    if (InstancingPolicy != EGameplayAbilityInstancingPolicy::InstancedPerActor)
    {
      UE_LOG(LogAura, Error, TEXT("Ability with Charges (%s) needs to be Instanced Per Actor!!"), *GetName());
      return;
    }
		
    ApplyGameplayEffectToOwner(
      Spec.Handle,
      ActorInfo,
      Spec.GetPrimaryInstance() ? Spec.GetPrimaryInstance()->GetCurrentActivationInfo() : FGameplayAbilityActivationInfo(),
      GetChargesEffect(),
      1.f,
      GetMaxCharges()
      );
		
    const FGameplayTagContainer& ChargesTags = GetChargesEffect()->GetGrantedTags();
		
    ActivationRequiredTags.AppendTags(ChargesTags);

    GetChargesEffect()->StackLimitCount = GetMaxCharges();
  }
}

void UBaseAbility::ActivateAbility(
  const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
  const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData
)
{
  Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
  
  OnAbilityStateChanged.Broadcast(true);
}

void UBaseAbility::EndAbility(
  const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
  const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled
)
{
  Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

  OnAbilityStateChanged.Broadcast(false);
}

bool UBaseAbility::CommitAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	FGameplayTagContainer* OptionalRelevantTags
	)
{
	if (!HasAuthority(&ActivationInfo)) return false;
	
	if (IsChargesModeActive())
	{
		const FGameplayTagContainer& ChargesTags = GetChargesEffect()->GetGrantedTags();
		
		const FGameplayEffectQuery Query = FGameplayEffectQuery
			::MakeQuery_MatchAnyOwningTags(ChargesTags);
		
		ActorInfo->AbilitySystemComponent->RemoveActiveEffects(Query, 1);

		HandleCooldownRecharge(Handle, ActorInfo, ActivationInfo);
		
		return CommitAbilityCost(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags);
	}
	
	return Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags);
}

bool UBaseAbility::CommitAbilityCooldown(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const bool ForceCooldown,
	FGameplayTagContainer* OptionalRelevantTags
	)
{
	if (!HasAuthority(&ActivationInfo)) return false;
	
	if (IsChargesModeActive())
	{
		const FGameplayTagContainer& ChargesTags = GetChargesEffect()->GetGrantedTags();
		
		const FGameplayEffectQuery Query = FGameplayEffectQuery
			::MakeQuery_MatchAnyOwningTags(ChargesTags);
		
		ActorInfo->AbilitySystemComponent->RemoveActiveEffects(Query, 1);

		HandleCooldownRecharge(Handle, ActorInfo, ActivationInfo);
		return true;
	}
	
	return Super::CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, ForceCooldown, OptionalRelevantTags);
}

bool UBaseAbility::CheckCooldown(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	FGameplayTagContainer* OptionalRelevantTags
	) const
{
	// With charges, cooldown will only affect the frequency a charge is recharged
	if (IsChargesModeActive())
	{
	  if (ChargesEffect)
	  {
	  	const FGameplayTagContainer& ChargesTags = ChargesEffect->GetGrantedTags();

	    const int32 ChargesCount = ActorInfo->AbilitySystemComponent->GetGameplayTagCount(ChargesTags.First());

	    return ChargesCount > 0;
	  }
	  return false;
	}
	
	return Super::CheckCooldown(Handle, ActorInfo, OptionalRelevantTags);
}

void UBaseAbility::OnAbilityLevelUp(
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilitySpec* Spec
		)
{
	if (const FGameplayTagContainer* CooldownTags = GetCooldownTags())
	{
		const FGameplayEffectQuery Query = FGameplayEffectQuery
			::MakeQuery_MatchAnyOwningTags(*CooldownTags);
		ActorInfo->AbilitySystemComponent->RemoveActiveEffects(Query);
	}
	
	if (bUseCharges)
	{
		GetChargesEffect()->StackLimitCount = GetMaxChargesAtLevel(Spec->Level);
		ApplyGameplayEffectToOwner(
			Spec->Handle,
			ActorInfo,
			Spec->Ability->GetCurrentActivationInfo(),
			GetChargesEffect(),
			1.f,
			GetMaxChargesAtLevel(Spec->Level)
			);
	}
}

bool UBaseAbility::IsActiveAbility_Implementation() const
{
	return false;
}

bool UBaseAbility::IsDamageAbility_Implementation() const
{
	return false;
}

bool UBaseAbility::IsAreaEffectActorAbility_Implementation() const
{
	return false;
}

float UBaseAbility::GetManaCostAtLevel(int32 Level) const
{
	float ManaCost = 0.f;
	if(const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (auto Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(
					Level,
					ManaCost
					);
				break;
			}
		}
	}

	return FMath::Abs(ManaCost);
}

int32 UBaseAbility::GetRoundedManaCostAtLevel(int32 Level) const
{
	return FMath::RoundToInt32(GetManaCostAtLevel(Level));
}

float UBaseAbility::GetCooldownAtLevel(int32 Level) const
{
	return Cooldown.GetValueAtLevel(Level);
}

int32 UBaseAbility::GetRoundedCooldownAtLevel(int32 Level) const
{
	return FMath::RoundToInt32(GetCooldownAtLevel(Level));
}

int32 UBaseAbility::GetMaxChargesAtLevel(int32 Level) const
{
	return FMath::RoundToInt32(MaxCharges.GetValueAtLevel(Level)) + AdditionalCharges;
}

float UBaseAbility::GetAreaRadius() const
{
	return AreaRadius.GetValueAtLevel(GetAbilityLevel()) + AdditionalAreaRadius;
}

int32 UBaseAbility::GetMaxCharges() const
{
	return FMath::RoundToInt32(MaxCharges.GetValueAtLevel(GetAbilityLevel())) + AdditionalCharges;
}

void UBaseAbility::DisablePlayerInput()
{
  if (APlayerController* PlayerController = Cast<APlayerController>(GetAvatarCharacter()->GetController()))
  {
    PlayerController->DisableInput(PlayerController);
  }
}

void UBaseAbility::EnablePlayerInput()
{
  if (APlayerController* PlayerController = Cast<APlayerController>(GetAvatarCharacter()->GetController()))
  {
    PlayerController->EnableInput(PlayerController);
  }
}

FGameplayEffectSpec UBaseAbility::GetEffectSpecByTag(const FGameplayTag& Tag)
{
  if (!IsInstantiated()) return FGameplayEffectSpec();
  
  const UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
  if (!ASC) return FGameplayEffectSpec();

  const FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAllOwningTags(FGameplayTagContainer({ Tag }));
  const TArray<FActiveGameplayEffectHandle> Effects = ASC->GetActiveEffects(Query);

  if (Effects.IsEmpty()) return FGameplayEffectSpec();

  const FActiveGameplayEffect* ActiveEffect = ASC->GetActiveGameplayEffect(Effects[0]);
  if (!ActiveEffect) return FGameplayEffectSpec();

  return ActiveEffect->Spec;
}

int32 UBaseAbility::GetEffectLevelByTag(const FGameplayTag& Tag)
{
  return GetEffectSpecByTag(Tag).GetLevel();
}

UGameplayEffect* UBaseAbility::GetChargesEffect()
{
	if (ChargesEffect == nullptr)
	{
		ChargesEffect = ChargesEffectClass.GetDefaultObject();
	}

	return ChargesEffect;
}

FAbilityParams UBaseAbility::MakeAbilityParamsFromDefaults(AActor* TargetActor) const
{
	FAbilityParams AbilityParams;
	AbilityParams.WorldContextObject = GetAvatarActorFromActorInfo();
	AbilityParams.SourceASC = GetAbilitySystemComponentFromActorInfo();
	AbilityParams.TargetASC =
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	AbilityParams.AbilityLevel = GetAbilityLevel();

	if (bIsAreaAbility)
	{
		AbilityParams.bIsAreaAbility = true;
		AbilityParams.AreaInnerRadius = AreaRadius.GetValueAtLevel(GetAbilityLevel());
		AbilityParams.AreaOuterRadius = AbilityParams.AreaInnerRadius;
		AbilityParams.AreaOrigin = AreaOrigin;
		if (IsValid(TargetActor))
		{
			const FVector ActorLocation = TargetActor->GetActorLocation();
			AbilityParams.ForwardVector = UKismetMathLibrary::GetDirectionUnitVector(
				AreaOrigin, 
				ActorLocation
				);
		}
	}
	
	if (StatusEffectData.Num() > 0)
	{
	  for (const auto& Effect : StatusEffectData)
	  {
		  UStatusEffectInfo* StatusEffectInfo = UAuraSystemsLibrary
			  ::GetStatusEffectInfo(AbilityParams.WorldContextObject);
	    FEffectParams EffectParams;
		  EffectParams.GameplayEffectClass = StatusEffectInfo
			  ->StatusEffects.Find(Effect.StatusEffectTag)->StatusEffectClass;
		  EffectParams.GameplayTag = Effect.StatusEffectTag;
		  EffectParams.Value = Effect.Value.GetValueAtLevel(GetAbilityLevel());
		  EffectParams.Duration = Effect.Duration.GetValueAtLevel(GetAbilityLevel());
	    EffectParams.Stacks = Effect.Stacks.GetValueAtLevel(GetAbilityLevel());

	    AbilityParams.EffectParams.Add(EffectParams);
	  }
	}

  if (HealEffectClass != nullptr && GetHealAtLevel(GetAbilityLevel()) > 0.f)
  {
    AbilityParams.HealParams.HealEffectClass = HealEffectClass;
    AbilityParams.HealParams.BaseHeal = GetHealAtLevel(GetAbilityLevel());
  }
	
	return AbilityParams;
}

AAuraCharacterBase* UBaseAbility::GetAvatarCharacter()
{
	if (AvatarCharacter == nullptr)
	{
		AvatarCharacter = Cast<AAuraCharacterBase>(GetAvatarActorFromActorInfo());
	}

	return AvatarCharacter;
}

AAuraHero* UBaseAbility::GetAvatarHero()
{
  if (AvatarHero == nullptr)
  {
    AvatarHero = Cast<AAuraHero>(GetAvatarActorFromActorInfo());
  }

  return AvatarHero;
}

AAuraCamera* UBaseAbility::GetPlayerCamera()
{
  if (GetAvatarHero() == nullptr)
  {
    UE_LOG(LogAura, Error, TEXT("Avatar is not of the AuraHero class, can't get player camera!"));
    return nullptr;
  }

  return AvatarHero->GetActiveCamera();
}

FGameplayTag UBaseAbility::GetAbilityTag()
{
	if (AbilityTag.IsValid()) return AbilityTag;
	
	TArray<FGameplayTag> GameplayTags;
	GetAssetTags().GetGameplayTagArray(GameplayTags);
	for (auto Tag : GameplayTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
		{
			AbilityTag = Tag;
			break;
		}
	}

	return AbilityTag;
}

void UBaseAbility::HandleCooldownRecharge(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo& ActivationInfo)
{
	if (
		RechargeMode == EAbilityRechargeMode::AllCharges &&
		ChargesEffectClass.GetDefaultObject()->StackDurationRefreshPolicy == EGameplayEffectStackingDurationPolicy::NeverRefresh &&
		ActorInfo->AbilitySystemComponent->GetGameplayEffectCount(
			CooldownGameplayEffectClass,
			nullptr
			) > 0
		) return;
	
	const FActiveGameplayEffectHandle CooldownHandle = ApplyGameplayEffectToOwner(
		Handle,
		ActorInfo,
		ActivationInfo,
		GetCooldownGameplayEffect(),
		1.f,
		1
	);

	const bool bRemoveAlreadyBound = ActorInfo->AbilitySystemComponent
		->OnGameplayEffectRemoved_InfoDelegate(CooldownHandle)
		->IsBoundToObject(this);

	if (!bRemoveAlreadyBound)
	{
		ActorInfo->AbilitySystemComponent
			->OnGameplayEffectRemoved_InfoDelegate(CooldownHandle)
			->AddWeakLambda(this, [this, Handle, ActorInfo, 
			ActivationInfo](
				const FGameplayEffectRemovalInfo& RemovalInfo
				)
			{
				int32 Stacks;
				if (RechargeMode == EAbilityRechargeMode::OneCharge)
				{
					Stacks = 1;
				}
				else
				{
					Stacks = GetMaxCharges();
				}

				ApplyGameplayEffectToOwner(
						Handle,
						ActorInfo,
						ActivationInfo,
						ChargesEffect,
						1.f,
						Stacks
						);
			});
	}

	if (RechargeMode == EAbilityRechargeMode::AllCharges) return;

	const bool bStackChangeAlreadyBound = ActorInfo->AbilitySystemComponent
		->OnGameplayEffectStackChangeDelegate(CooldownHandle)
		->IsBoundToObject(this);

	if (!bStackChangeAlreadyBound)
	{
		ActorInfo->AbilitySystemComponent
			->OnGameplayEffectStackChangeDelegate(CooldownHandle)
			->AddWeakLambda(this,[this, Handle, ActorInfo, ActivationInfo](
					FActiveGameplayEffectHandle EffectHandle,
					int32 NewStack,
					int32 PrevStack
					)
					{
						if (NewStack > PrevStack) return;
		
						const int32 CooldownStacksRemoved = PrevStack - NewStack;
						ApplyGameplayEffectToOwner(
							Handle,
							ActorInfo,
							ActivationInfo,
							GetChargesEffect(),
							1.f,
							CooldownStacksRemoved
							);
					});
	}
}

bool UBaseAbility::IsChargesModeActive() const
{
	return bUseCharges && IsValid(ChargesEffectClass);
}

float UBaseAbility::GetHealAtLevel(int32 Level) const
{
  return Heal.GetValueAtLevel(Level);
}

int32 UBaseAbility::GetRoundedHealAtLevel_Implementation(int32 Level) const
{
  return FMath::RoundToInt32(GetHealAtLevel(Level));
}
