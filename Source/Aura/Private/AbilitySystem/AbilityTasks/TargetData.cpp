// Copyright Lucas Rossi


#include "AbilitySystem/AbilityTasks/TargetData.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Player/MainPlayerController.h"

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
	
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	AMainPlayerController* MainPC = CastChecked<AMainPlayerController>(PC);
	UAuraGameplayAbility* AuraAbility = CastChecked<UAuraGameplayAbility>(Ability);
	
	FHitResult HitResult;
	if (MainPC->GetUsingGamepad() || AuraAbility->bUsesInputDirection)
	{
		FVector AvatarLocation = Ability->GetAvatarActorFromActorInfo()->GetActorLocation();
		HitResult.Location = AvatarLocation + MainPC->GetInputDirection() * 10000;
	}
	else
	{
		PC->GetHitResultUnderCursor(
			ECC_Visibility,
			false,
			HitResult
		);
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
