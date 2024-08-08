// Copyright Lucas Rossi


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetAuraAttributeSet()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAttributeSet()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAuraAttributeSet()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAttributeSet()->GetMaxMana());

	BroadcastAbilityInfo();
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetAuraPlayerState()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
		}
	);
	
	AbilitySystemComponent->
		GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetHealthAttribute())
		.AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
	AbilitySystemComponent->
		GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetMaxHealthAttribute())
	.AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->
		GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetManaAttribute())
	.AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->
		GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetMaxManaAttribute())
	.AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);
	
	if (GetAuraAbilitySystemComponent())
	{
		if (GetAuraAbilitySystemComponent()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetAuraAbilitySystemComponent()->AbilitiesGivenDelegate.AddUObject(
				this,
				&UOverlayWidgetController::BroadcastAbilityInfo
			);
		}
		
		GetAuraAbilitySystemComponent()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag& Tag : AssetTags)
				{
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(MessageTag))
					{
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);

		GetAuraAbilitySystemComponent()->AbilityEquipped.AddUObject(
			this,
			&UOverlayWidgetController::OnAbilityEquipped
			);
		
		GetAuraAbilitySystemComponent()->AbilityStateChanged.AddLambda(
		[this](
				const FGameplayTag& AbilityTag,
				const FGameplayTag& StatusTag,
				const FGameplayTag& InputTag,
				int32 NewLevel
				)
			{
				if (AbilityInfo)
				{
					FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
					Info.StatusTag = StatusTag;
					Info.InputTag = InputTag;
					AbilityInfoDelegate.Broadcast(Info);
				}
			}
		);
	}
}

void UOverlayWidgetController::StartTransition()
{
	StartTransitionDelegate.Broadcast(true);
}

void UOverlayWidgetController::EndTransition()
{
	StartTransitionDelegate.Broadcast(false);
}

void UOverlayWidgetController::OnAbilityEquipped(
	const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag,
	const FGameplayTag& InputTag,
	const FGameplayTag& PreviousInputTag
	)
{
	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
	FAuraAbilityInfo LastAbilitySlotInfo;
	LastAbilitySlotInfo.StatusTag = Tags.Abilities_Status_Unlocked;
	LastAbilitySlotInfo.InputTag = PreviousInputTag;
	LastAbilitySlotInfo.AbilityTag = Tags.Abilities_NONE;

	// Broadcast empty info if PreviousSlot is a valid slot. Only if equipping an already equipped skill
	AbilityInfoDelegate.Broadcast(LastAbilitySlotInfo);

	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
	Info.StatusTag = StatusTag;
	Info.InputTag = InputTag;
	AbilityInfoDelegate.Broadcast(Info);
}

int32 UOverlayWidgetController::GetCurrentAbilityCharges(const FGameplayTag& ChargeTag)
{
	if (!IsValid(AbilitySystemComponent)) return 0;

	const FGameplayEffectQuery Query = FGameplayEffectQuery
			::MakeQuery_MatchAnyOwningTags(ChargeTag.GetSingleTagContainer());
		
	const TArray<FActiveGameplayEffectHandle> Handles = AbilitySystemComponent->GetActiveEffects(Query);

	if (Handles.IsEmpty()) return 0;

	return AbilitySystemComponent->GetCurrentStackCount(Handles[0]);
}
