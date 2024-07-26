// Copyright Lucas Rossi


#include "AbilitySystem/AbilityTasks/TargetData.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/ActiveAbility.h"
#include "Actor/TargetingActor.h"
#include "Aura/Aura.h"
#include "Player/AuraPlayerController.h"

UTargetData* UTargetData::
	CreateTargetData(UGameplayAbility* OwningAbility)
{
	UTargetData* MyObj = NewAbilityTask<UTargetData>(OwningAbility);
	
	return MyObj;
}

void UTargetData::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();

	if (bIsLocallyControlled)
	{
		SendMouseOrGamepadData();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(
			SpecHandle,
			ActivationPredictionKey
		).AddUObject(this, &UTargetData::OnTargetDataReplicatedCallback);

		const bool bCalledDelegate = AbilitySystemComponent.Get()
			->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);

		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetData::SendMouseOrGamepadData()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	AAuraPlayerController* MainPC =
		CastChecked<AAuraPlayerController>(Ability->GetCurrentActorInfo()->PlayerController.Get());
	UActiveAbility* AuraAbility = CastChecked<UActiveAbility>(Ability);
	
	FHitResult HitResult;
	if (AuraAbility->bUsesMovementInputDirection)
	{
		FVector AvatarLocation = Ability->GetAvatarActorFromActorInfo()->GetActorLocation();
		HitResult.Location = AvatarLocation + MainPC->GetInputDirection() * 10000;
		HitResult.ImpactPoint = HitResult.Location;
	}
	else
	{
		if (MainPC->IsUsingGamepad())
		{
			if (MainPC->IsTargeting())
			{
				HitResult.Location = MainPC->GetTargetingActor()->GetActorLocation();
			}
			else
			{
				FVector AvatarLocation = Ability->GetAvatarActorFromActorInfo()->GetActorLocation();
				HitResult.Location = AvatarLocation + MainPC->GetInputDirection() * 10000;
			}
			HitResult.ImpactPoint = HitResult.Location;
		}
		else
		{
			MainPC->GetHitResultUnderCursor(
				ECC_Target,
				false,
				HitResult
			);
		}
	}

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();

	Data->HitResult = HitResult;
	DataHandle.Add(Data);
	
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey
	);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetData::OnTargetDataReplicatedCallback(
	const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey()
	);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
