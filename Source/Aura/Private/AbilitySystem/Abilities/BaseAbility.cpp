// Copyright Lucas Rossi


// ReSharper disable CppExpressionWithoutSideEffects
#include "AbilitySystem/Abilities/BaseAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/StatusEffectInfo.h"
#include "AbilitySystem/GameplayEffects/StatusEffect.h"
#include "Aura/AuraLogChannels.h"
#include "Kismet/KismetMathLibrary.h"

UBaseAbility::UBaseAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bServerRespectsRemoteAbilityCancellation = false;
}

void UBaseAbility::OnGiveAbility(
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec
	)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
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
			Spec.ActivationInfo,
			GetChargesEffect(),
			1.f,
			GetMaxCharges()
			);
		
		FGameplayTagContainer ChargesTags;
		GetChargesEffect()->GetOwnedGameplayTags(ChargesTags);
		
		ActivationRequiredTags.AppendTags(ChargesTags);

		GetChargesEffect()->StackLimitCount = GetMaxCharges();
	}
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
		FGameplayTagContainer ChargesTags;
		GetChargesEffect()->GetOwnedGameplayTags(ChargesTags);
		
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
		FGameplayTagContainer ChargesTags;
		GetChargesEffect()->GetOwnedGameplayTags(ChargesTags);
		
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
	if (IsChargesModeActive()) return true;
	
	return Super::CheckCooldown(Handle, ActorInfo, OptionalRelevantTags);
}

void UBaseAbility::OnAbilityLevelUp(
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilitySpec* Spec
		)
{
	const FGameplayEffectQuery Query = FGameplayEffectQuery
		::MakeQuery_MatchAnyOwningTags(*GetCooldownTags());
	ActorInfo->AbilitySystemComponent->RemoveActiveEffects(Query);
	
	if (bUseCharges)
	{
		GetChargesEffect()->StackLimitCount = GetMaxChargesAtLevel(Spec->Level);
		ApplyGameplayEffectToOwner(
			Spec->Handle,
			ActorInfo,
			Spec->ActivationInfo,
			GetChargesEffect(),
			1.f,
			GetMaxChargesAtLevel(Spec->Level)
			);
	}
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
	return FMath::RoundToInt32(MaxCharges.GetValueAtLevel(Level));
}

float UBaseAbility::GetAreaInnerRadius() const
{
	return AreaInnerRadius.GetValueAtLevel(GetAbilityLevel());
}

float UBaseAbility::GetAreaOuterRadius() const
{
	return AreaOuterRadius.GetValueAtLevel(GetAbilityLevel());
}

int32 UBaseAbility::GetMaxCharges() const
{
	return FMath::RoundToInt32(MaxCharges.GetValueAtLevel(GetAbilityLevel()));
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

	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return AbilityParams;
	
	AbilityParams.WorldContextObject = GetAvatarActorFromActorInfo();
	AbilityParams.SourceASC = GetAbilitySystemComponentFromActorInfo();
	AbilityParams.TargetASC =
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	AbilityParams.AbilityLevel = GetAbilityLevel();

	if (bIsAreaAbility)
	{
		AbilityParams.bIsAreaAbility = true;
		AbilityParams.AreaInnerRadius = AreaInnerRadius.GetValueAtLevel(GetAbilityLevel());
		AbilityParams.AreaOuterRadius = AreaOuterRadius.GetValueAtLevel(GetAbilityLevel());
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
	
	if (StatusEffectData.IsValid())
	{
		UStatusEffectInfo* StatusEffectInfo = UAuraAbilitySystemLibrary
			::GetStatusEffectInfo(AbilityParams.WorldContextObject);
		AbilityParams.EffectParams.GameplayEffectClass = StatusEffectInfo
			->StatusEffects.Find(StatusEffectData.StatusEffectTag)->StatusEffectClass;
		AbilityParams.EffectParams.GameplayTag = StatusEffectData.StatusEffectTag;
		AbilityParams.EffectParams.Value = StatusEffectData.Value.GetValueAtLevel(GetAbilityLevel());
		AbilityParams.EffectParams.Duration = StatusEffectData.Duration.GetValueAtLevel(GetAbilityLevel());
	}
	
	return AbilityParams;
}

FGameplayTag UBaseAbility::GetAbilityTag()
{
	if (AbilityTag.IsValid()) return AbilityTag;
	
	TArray<FGameplayTag> GameplayTags;
	AbilityTags.GetGameplayTagArray(GameplayTags);
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
	return bUseCharges && MaxCharges.GetValueAtLevel(GetAbilityLevel()) > 1 && IsValid(ChargesEffectClass);
}
