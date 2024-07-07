// Copyright Lucas Rossi


#include "AbilitySystem/AsyncTasks/WaitChargesChange.h"

#include "AbilitySystemComponent.h"

UWaitChargesChange* UWaitChargesChange::WaitForChargesChange(
	UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayTag& InChargeTag
	)
{
	UWaitChargesChange* WaitChargesChange = NewObject<UWaitChargesChange>();
	WaitChargesChange->ASC = AbilitySystemComponent;
	WaitChargesChange->ChargeTag = InChargeTag;

	if (!IsValid(AbilitySystemComponent) || !InChargeTag.IsValid())
	{
		WaitChargesChange->EndTask();
		return nullptr;
	}

	// When a charge effect has added or removed
	AbilitySystemComponent->RegisterGameplayTagEvent(
		InChargeTag,
		EGameplayTagEventType::NewOrRemoved
	).AddUObject(
			WaitChargesChange,
			&UWaitChargesChange::OnChargeTagChanged
		);

	// When a charge effect has been applied (tag added)
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf
		.AddUObject(
			WaitChargesChange,
			&UWaitChargesChange::OnChargesAdded
	);

	// Charge effect may already be active, because we grant them when giving the ability, so we have to check here
	const FGameplayEffectQuery Query = FGameplayEffectQuery
			::MakeQuery_MatchAnyOwningTags(InChargeTag.GetSingleTagContainer());
		
	const TArray<FActiveGameplayEffectHandle> Handles = AbilitySystemComponent->GetActiveEffects(Query);
	
	if (Handles.IsEmpty()) return WaitChargesChange;
	
	const FActiveGameplayEffectHandle ChargeHandle = Handles[0];

	const bool bAlreadyBound = AbilitySystemComponent
		->OnGameplayEffectStackChangeDelegate(ChargeHandle)
		->IsBoundToObject(WaitChargesChange);

	if (bAlreadyBound) return WaitChargesChange;
	
	AbilitySystemComponent
		->OnGameplayEffectStackChangeDelegate(ChargeHandle)
		->AddWeakLambda(WaitChargesChange,[WaitChargesChange](
				FActiveGameplayEffectHandle EffectHandle,
				int32 NewStack,
				int32 PrevStack
				)
				{
					WaitChargesChange->ChargesChange.Broadcast(NewStack);
				});

	return WaitChargesChange;
}

void UWaitChargesChange::EndTask()
{
	if (!IsValid(ASC)) return;
	
	ASC->RegisterGameplayTagEvent(ChargeTag, EGameplayTagEventType::NewOrRemoved)
		.RemoveAll(this);

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitChargesChange::OnChargeTagChanged(
	const FGameplayTag InChargeTag,
	int32 NewCount
)
{
	if (NewCount == 0)
	{
		ChargesChange.Broadcast(0);
	}
}

void UWaitChargesChange::OnChargesAdded(
	UAbilitySystemComponent* TargetASC,
	const FGameplayEffectSpec& SpecApplied,
	FActiveGameplayEffectHandle ActiveChargeHandle
	)
{
	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	if (GrantedTags.HasTagExact(ChargeTag))
	{
		const FGameplayEffectQuery GameplayEffectQuery =
			FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(ChargeTag.GetSingleTagContainer());
		
		ChargesChange.Broadcast(ASC->GetAggregatedStackCount(GameplayEffectQuery));

		const bool bAlreadyBound = ASC
		->OnGameplayEffectStackChangeDelegate(ActiveChargeHandle)
		->IsBoundToObject(this);

		if (bAlreadyBound) return;
	
		ASC
			->OnGameplayEffectStackChangeDelegate(ActiveChargeHandle)
			->AddWeakLambda(this,[this](
					FActiveGameplayEffectHandle EffectHandle,
					int32 NewStack,
					int32 PrevStack
					)
					{
						ChargesChange.Broadcast(NewStack);
					});
	}
}
