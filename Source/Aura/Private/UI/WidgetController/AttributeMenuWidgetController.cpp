// Copyright Lucas Rossi


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AbilitySystem/Data/LevelInfo.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);

	AuraPlayerState->OnXPChangedDelegate.AddUObject(
		this,
		&UAttributeMenuWidgetController::OnXPChanged
	);
	AuraPlayerState->OnAttributePointsChangedDelegate.AddUObject(
		this,
		&UAttributeMenuWidgetController::OnAttributePointsChanged
	);
	
	check(AttributeInfo);

	for (auto& Tag : AttributeInfo.Get()->AttributeInformation)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Tag.AttributeGetter)
			.AddLambda(
				[this, Tag](const FOnAttributeChangeData& Data)
				{
					BroadcastAttributeInfo(Tag.AttributeTag);
				}
			);	
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	const AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	check(AttributeInfo);

	OnXPChanged(AuraPlayerState->GetXP());
	OnAttributePointsChanged((AuraPlayerState->GetAttributePoints()));

	for (auto& Tag : AttributeInfo.Get()->AttributeInformation)
	{
		BroadcastAttributeInfo(Tag.AttributeTag);
	}
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::OnXPChanged(int32 NewXP) const
{
	const AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	const ULevelInfo* LevelInfo = AuraPlayerState->LevelInfo;
	checkf(LevelInfo, TEXT("Unable to find LevelInfo. Please fill out AuraPlayerState Blueprint."));
	
	const int32 Level = LevelInfo->FindLevelByXP(NewXP);
	const int32 MaxLevel = LevelInfo->LevelInformation.Num();

	if (Level < MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelInfo->LevelInformation[Level].XPRequirement;
		const int32 PrevRequirement = LevelInfo->LevelInformation[Level - 1].XPRequirement;
		const int32 DeltaRequirement = LevelUpRequirement - PrevRequirement;
		const int32 XPForThisLevel = NewXP - PrevRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaRequirement);

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

void UAttributeMenuWidgetController::OnAttributePointsChanged(int32 NewAttributePoints)
{
	OnAttributePointsChangedDelegate.Broadcast(NewAttributePoints);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Tag);
	
	Info.AttributeValue = Info.AttributeGetter.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

