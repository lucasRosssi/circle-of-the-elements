// Copyright Lucas Rossi


#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"

#include "AbilitySystemComponent.h"

UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange(
	UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayTag& InCooldownTag
)
{
	UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();
	WaitCooldownChange->ASC = AbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTag;

	if (!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}

	// When a cooldown effect has ended (tag removed)
	AbilitySystemComponent->RegisterGameplayTagEvent(
		InCooldownTag,
		EGameplayTagEventType::NewOrRemoved
	).AddUObject(
			WaitCooldownChange,
			&UWaitCooldownChange::OnCooldownTagChanged
		);

	// When a cooldown effect has been applied (tag added)
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf
		.AddUObject(
			WaitCooldownChange,
			&UWaitCooldownChange::OnActiveCooldownAdded
	);
	
	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	if (!IsValid(ASC)) return;
	
	ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved)
		.RemoveAll(this);

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitCooldownChange::OnCooldownTagChanged(
	const FGameplayTag InCooldownTag,
	int32 NewCount
)
{
	if (NewCount == 0)
	{
		CooldownEnd.Broadcast(0.f);
	}
}

void UWaitCooldownChange::OnActiveCooldownAdded(
	UAbilitySystemComponent* TargetASC,
	const FGameplayEffectSpec& SpecApplied,
	FActiveGameplayEffectHandle ActiveCooldownHandle
)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		const FGameplayEffectQuery GameplayEffectQuery =
			FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		
		const TArray<float> TimesRemaining = ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		
		const float TimeRemaining = FMath::Max(TimesRemaining);
		
		CooldownStart.Broadcast(TimeRemaining);
	}
}
