// Copyright Lucas Rossi


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AuraNamedArguments.h"
#include "AbilitySystem/Abilities/ActiveDamageAbility.h"
#include "AbilitySystem/Abilities/BaseAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Character/AuraCharacterBase.h"
#include "Game/AuraGameModeBase.h"
#include "Game/TeamComponent.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "Player/MainPlayerController.h"
#include "UI/HUD/AuraHUD.h"

FWidgetControllerParams UAuraAbilitySystemLibrary::MakeWidgetControllerParams(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
		UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
		UAttributeSet* AS = PS->GetAttributeSet();
		
		return FWidgetControllerParams(PC, PS, ASC, AS);
	}

	return FWidgetControllerParams();
}

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(
	const UObject* WorldContextObject)
{
	const FWidgetControllerParams WidgetControllerParams = MakeWidgetControllerParams(WorldContextObject);
	if (!WidgetControllerParams.IsValid()) return nullptr;
	
	AAuraHUD* AuraHUD = WidgetControllerParams.PlayerController->GetHUD<AAuraHUD>();
	if (!AuraHUD) return nullptr;

	return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	const FWidgetControllerParams WidgetControllerParams = MakeWidgetControllerParams(WorldContextObject);
	if (!WidgetControllerParams.IsValid()) return nullptr;
	
	AAuraHUD* AuraHUD = WidgetControllerParams.PlayerController->GetHUD<AAuraHUD>();
	if (!AuraHUD) return nullptr;

	return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
}

USkillMenuWidgetController* UAuraAbilitySystemLibrary::GetSkillMenuWidgetController(
	const UObject* WorldContextObject)
{
	const FWidgetControllerParams WidgetControllerParams = MakeWidgetControllerParams(WorldContextObject);
	if (!WidgetControllerParams.IsValid()) return nullptr;
	
	AAuraHUD* AuraHUD = WidgetControllerParams.PlayerController->GetHUD<AAuraHUD>();
	if (!AuraHUD) return nullptr;

	return AuraHUD->GetSkillMenuWidgetController(WidgetControllerParams);
}

UCharacterInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(
	const UObject* WorldContextObject)
{
	const AAuraGameModeBase* AuraGameMode = CastChecked<AAuraGameModeBase>(
		UGameplayStatics::GetGameMode(WorldContextObject)
	);

	return AuraGameMode->CharacterClassInfo;
}

UAbilityInfo* UAuraAbilitySystemLibrary::GetAbilitiesInfo(const UObject* WorldContextObject)
{
	const AAuraGameModeBase* AuraGameMode = CastChecked<AAuraGameModeBase>(
		UGameplayStatics::GetGameMode(WorldContextObject)
	);

	return AuraGameMode->AbilityInfo;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(
		const UObject* WorldContextObject,
		ECharacterClass CharacterClass,
		float Level,
		UAbilitySystemComponent* ASC
	)
{
	UCharacterInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);

	const AActor* AvatarActor = ASC->GetAvatarActor();

	const auto ClassInfo = CharacterClassInfo
	 ->GetClassDefaultInfo(CharacterClass);
	
	FGameplayEffectContextHandle PrimaryContextHandle = ASC->MakeEffectContext();
	PrimaryContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimarySpecHandle = ASC->MakeOutgoingSpec(
		ClassInfo.PrimaryAttributes, 
		Level, 
		PrimaryContextHandle
	);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimarySpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryContextHandle = ASC->MakeEffectContext();
	SecondaryContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondarySpecHandle = ASC->MakeOutgoingSpec(
		CharacterClassInfo->SecondaryAttributes, 
		Level, 
		SecondaryContextHandle
	);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondarySpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalContextHandle = ASC->MakeEffectContext();
	VitalContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalSpecHandle = ASC->MakeOutgoingSpec(
		CharacterClassInfo->VitalAttributes, 
		Level, 
		VitalContextHandle
	);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalSpecHandle.Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(
		const UObject* WorldContextObject,
		UAbilitySystemComponent* ASC,
		ECharacterClass CharacterClass
	)
{
	UCharacterInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	for (const auto AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}

	AAuraCharacterBase* Character = Cast<AAuraCharacterBase>(ASC->GetAvatarActor());
	if (Character)
	{
		for (const auto AbilityClass : Character->CharacterAbilities)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
			ASC->GiveAbility(AbilitySpec);
		}
	}
	
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (auto AbilityClass : DefaultInfo.ClassAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(
				AbilityClass,
				ICombatInterface::Execute_GetCharacterLevel(ASC->GetAvatarActor())
			);
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

bool UAuraAbilitySystemLibrary::IsParried(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FAuraGameplayEffectContext* AuraEffectContext = 
		static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());

	if (AuraEffectContext)
	{
		return AuraEffectContext->IsParried();
	}

	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(
	const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FAuraGameplayEffectContext* AuraEffectContext = 
		static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());

	if (AuraEffectContext)
	{
		return AuraEffectContext->IsCriticalHit();
	}

	return false;
}

void UAuraAbilitySystemLibrary::SetIsParried(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInParried)
{
	FAuraGameplayEffectContext* AuraEffectContext =
		static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (AuraEffectContext)
	{
		AuraEffectContext->SetIsParried(bInParried);
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInCriticalHit)
{
	FAuraGameplayEffectContext* AuraEffectContext =
		static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (AuraEffectContext)
	{
		AuraEffectContext->SetIsCriticalHit(bInCriticalHit);
	}
}

void UAuraAbilitySystemLibrary::GetAliveCharactersWithinRadius(
	const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors,
	TArray<AActor*>& ActorsToIgnore,
	float Radius,
	const FVector& SphereOrigin
)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(
			Overlaps,
			SphereOrigin,
			FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius),
			SphereParams
		);
		for (auto& Overlap : Overlaps)
		{
			AActor* OverlappedActor = Overlap.GetActor();

			if (!OverlappedActor->Implements<UCombatInterface>()) return;
			
			const bool bIsAlive = !ICombatInterface::Execute_IsDead(OverlappedActor);
			if (bIsAlive)
			{
				OutOverlappingActors.AddUnique(OverlappedActor);
			}
		}
	}
}

bool UAuraAbilitySystemLibrary::AreActorsFriends(const AActor* FirstActor, const AActor* 
                                                 SecondActor)
{
	if (!IsValid(FirstActor) || !IsValid(SecondActor)) return false;
	
	const UTeamComponent* FirstTeamComponent = FirstActor->GetComponentByClass<UTeamComponent>();
	const UTeamComponent* SecondTeamComponent = SecondActor->GetComponentByClass<UTeamComponent>();

	if (!FirstTeamComponent || !SecondTeamComponent)
	{
		return false;
	}

	return FirstTeamComponent->TeamID == SecondTeamComponent->TeamID;
}

bool UAuraAbilitySystemLibrary::AreActorsEnemies(const AActor* FirstActor, const AActor* 
SecondActor)
{
	if (!IsValid(FirstActor) || !IsValid(SecondActor)) return false;
	
	const UTeamComponent* FirstTeamComponent = FirstActor->GetComponentByClass<UTeamComponent>();
	const UTeamComponent* SecondTeamComponent = SecondActor->GetComponentByClass<UTeamComponent>();

	if (!FirstTeamComponent || !SecondTeamComponent)
	{
		return false;
	}

	return FirstTeamComponent->TeamID != SecondTeamComponent->TeamID;
}

bool UAuraAbilitySystemLibrary::IsPlayerUsingGamepad(const AActor* AvatarActor)
{
	AController* Controller = AvatarActor->GetInstigatorController();

	if (AMainPlayerController* PlayerController = Cast<AMainPlayerController>(Controller))
	{
		return PlayerController->GetUsingGamepad();
	}

	return false;
}

bool UAuraAbilitySystemLibrary::IsTargetInvulnerable(AActor* TargetActor)
{
	const UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent
	(TargetActor);

	if (!ASC) return false;

	return ASC->HasMatchingGameplayTag(FAuraGameplayTags::Get().Effects_Invulnerable);
}

FString UAuraAbilitySystemLibrary::GetAbilityDescription(
	const UObject* WorldContextObject,
	const FGameplayTag& AbilityTag,
	int32 Level
	)
{
	FAuraAbilityInfo Info = GetAbilitiesInfo(WorldContextObject)->FindAbilityInfoByTag(AbilityTag);
	UBaseAbility* Ability = Info.Ability.GetDefaultObject();
	FormatAbilityDescriptionAtLevel(Ability, Level, Info.Description);
	
	return FString::Printf(
		TEXT(
			"<Title>%s </>\n"
			"<Title18>Level </><Level>%d</>\n"
			"\n"
			"%s\n"
			"\n"
			"<Info>Mana - </><Mana>%d</>\n"
			"<Info>Cooldown - </>%ds"
			),
		*Info.Name.ToString(),
		Level,
		*Info.Description.ToString(),
		Ability->GetRoundedManaCost(Level),
		Ability->GetRoundedCooldown()
	);
}

FString UAuraAbilitySystemLibrary::GetAbilityNextLevelDescription(
	const UObject* WorldContextObject,
	const FGameplayTag& AbilityTag,
	int32 Level
	)
{
	FAuraAbilityInfo Info = GetAbilitiesInfo(WorldContextObject)->FindAbilityInfoByTag(AbilityTag);
	UBaseAbility* Ability = Info.Ability.GetDefaultObject();
	FormatAbilityDescriptionAtLevel(Ability, Level, Info.NextLevelDescription);

	const int32 ManaCost = Ability->GetRoundedManaCost(Level);
	const int32 NextManaCost = Ability->GetRoundedManaCost(Level + 1);
	FString ManaCostText;
	if (NextManaCost - ManaCost == 0)
	{
		ManaCostText = FString::Printf(
			TEXT("<Info>Mana - </><Mana>%d</>"),
			ManaCost
		);
		
	}
	else
	{
		ManaCostText = FString::Printf(
			TEXT("<Info>Mana - </><Old>%d</> > <Mana>%d</>"),
			ManaCost,
			NextManaCost
		);
	}

	return FString::Printf(
		TEXT(
			"<Title>%s </>\n"
			"<Title18>Level </><Old>%d</> > <Level>%d</>\n"
			"\n"
			"%s\n"
			"\n"
			"%s\n"
			"<Info>Cooldown - </>%ds"
			),
		*Info.Name.ToString(),
		Level,
		Level + 1,
		*Info.NextLevelDescription.ToString(),
		*ManaCostText,
		Ability->GetRoundedCooldown()
	);
}

void UAuraAbilitySystemLibrary::FormatAbilityDescriptionAtLevel(
	UBaseAbility* Ability,
	int32 Level,
	FText& OutDescription
	)
{
	const FAuraGameplayTags Tags = FAuraGameplayTags::Get();
	const FAuraNamedArguments Args = FAuraNamedArguments::Get();
	
	if (UActiveDamageAbility* DamageAbility = Cast<UActiveDamageAbility>(Ability))
	{
		OutDescription = FText::FormatNamed(
			OutDescription,
			Args.FDmg_0,
			DamageAbility->GetRoundedDamageAtLevel(Level, Tags.Damage_Fire),
			Args.FDmg_1,
			DamageAbility->GetRoundedDamageAtLevel(Level + 1, Tags.Damage_Fire)
			);
	}
}

FString UAuraAbilitySystemLibrary::GetAbilityLockedDescription(
	int32 Level,
	const FGameplayTagContainer& AbilitiesRequirement
	)
{
	FString RequirementText = FString::Printf(
		TEXT("<Default>Skill locked until level </><Level>%d</>"),
		Level
	);
	if (AbilitiesRequirement.IsEmpty())
	{
		return RequirementText;
	}
	
	if (AbilitiesRequirement.Num() == 1)
	{
		RequirementText.Appendf(
			TEXT("<Default>. Required skill: </><Skill>%s</>"),
			*AbilitiesRequirement.First().GetTagName().ToString()
		);
	}

	RequirementText.Append(
		TEXT("<Default>. Required skills: </>")
	);

	int32 Index = 0;
	for (const auto& AbilityTag : AbilitiesRequirement)
	{
		RequirementText.Appendf(
			TEXT("<Skill>%s</>"),
			*AbilityTag.GetTagName().ToString()
		);

		if (AbilitiesRequirement.IsValidIndex(Index + 1))
		{
			RequirementText.Append(TEXT(", "));
		}

		Index++;
	}
	
	return RequirementText;
}

int32 UAuraAbilitySystemLibrary::GetXPRewardForTypeAndLevel(
	const UObject* WorldContextObject,
	ECharacterType CharacterType,
	int32 Level
)
{
	const UCharacterInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (!CharacterClassInfo) return 0;

	const FScalableFloat* XPCurve = CharacterClassInfo->XPReward.Find(CharacterType);
	checkf(
		XPCurve,
		TEXT("XP Curve not found for this CharacterType, fill in its XP Curve in the DA_CharacterClassInfo Blueprint"),
	);
	const float XPReward = XPCurve->GetValueAtLevel(Level);

	return static_cast<int32>(XPReward);
}
