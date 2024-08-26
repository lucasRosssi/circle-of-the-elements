// Copyright Lucas Rossi


#include "AbilitySystem/AbilityTasks/TargetData.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/ActiveAbility.h"
#include "Actor/TargetingActor.h"
#include "Aura/Aura.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
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
	
	AAuraPlayerController* AuraPC =
		CastChecked<AAuraPlayerController>(Ability->GetCurrentActorInfo()->PlayerController.Get());
	UActiveAbility* AuraAbility = CastChecked<UActiveAbility>(Ability);
	
	FVector AvatarLocation = GetAvatarActor()->GetActorLocation();
	FHitResult HitResult;
	if (AuraAbility->bUsesMovementInputDirection)
	{
		HitResult.Location = AvatarLocation + AuraPC->GetInputDirection() * 10000;
		HitResult.ImpactPoint = HitResult.Location;
	}
	else
	{
		if (AuraPC->IsUsingGamepad())
		{
			if (AuraPC->IsTargeting())
			{
				HitResult.Location = AuraPC->GetTargetingActor()->GetActorLocation();
			}
			else
			{
				HitResult.Location = AvatarLocation + AuraPC->GetInputDirection() * 2000;
				AuraPC->AimAbilityGamepad(GetAvatarActor(), HitResult);
				
			}
			HitResult.ImpactPoint = HitResult.Location;
		}
		else
		{
			AuraPC->GetHitResultUnderCursor(
				ECC_Target,
				false,
				HitResult
			);

			if (HitResult.bBlockingHit)
			{
				AuraPC->AimAbilityMouse(GetAvatarActor(), HitResult);
			}
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
