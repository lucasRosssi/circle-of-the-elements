// Copyright Lucas Rossi


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AbilitySystem/Data/LevelInfo.h"
#include "Aura/Aura.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraPlayerState()->OnXPChangedDelegate.AddUObject(
		this,
		&UAttributeMenuWidgetController::OnXPChanged
	);
	GetAuraPlayerState()->OnAttributePointsChangedDelegate.AddUObject(
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
	check(AttributeInfo);

	OnXPChanged(GetAuraPlayerState()->GetXP());
	OnAttributePointsChanged(GetAuraPlayerState()->GetAttributePoints());

	for (auto& Tag : AttributeInfo.Get()->AttributeInformation)
	{
		BroadcastAttributeInfo(Tag.AttributeTag);
	}
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	GetAuraAbilitySystemComponent()->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::OnXPChanged(int32 NewXP)
{
	const ULevelInfo* LevelInfo = GetAuraPlayerState()->LevelInfo;
	checkf(LevelInfo, TEXT("Unable to find LevelInfo. Please fill out AuraPlayerState Blueprint."));
	
	const int32 Level = LevelInfo->FindLevelByXP(NewXP);

	if (Level < MAX_LEVEL && Level > 0)
	{
		const int32 LevelUpRequirement = LevelInfo->XPRequirement.AsInteger(Level);
		const int32 PrevRequirement = LevelInfo->XPRequirement.AsInteger(Level - 1);
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

