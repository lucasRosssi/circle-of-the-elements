// Copyright Lucas Rossi


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AuraNamedArguments.h"
#include "AbilitySystem/Abilities/ActiveDamageAbility.h"
#include "AbilitySystem/Abilities/BaseAbility.h"
#include "AbilitySystem/Abilities/BeamAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/Aura.h"
#include "Character/AuraCharacterBase.h"
#include "Enums/TargetTeam.h"
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

UStatusEffectInfo* UAuraAbilitySystemLibrary::GetStatusEffectInfo(const UObject* WorldContextObject)
{
	const AAuraGameModeBase* AuraGameMode = CastChecked<AAuraGameModeBase>(
		UGameplayStatics::GetGameMode(WorldContextObject)
	);

	return AuraGameMode->StatusEffectInfo;
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

bool UAuraAbilitySystemLibrary::IsApplyingEffect(
	const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FAuraGameplayEffectContext* AuraEffectContext = 
		static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());

	if (AuraEffectContext)
	{
		return AuraEffectContext->IsApplyingEffect();
	}

	return false;
}

float UAuraAbilitySystemLibrary::GetEffectValue(
	const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FAuraGameplayEffectContext* AuraEffectContext = 
		static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());

	if (AuraEffectContext)
	{
		return AuraEffectContext->GetEffectValue();
	}

	return 0.f;
}

float UAuraAbilitySystemLibrary::GetEffectDuration(
	const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FAuraGameplayEffectContext* AuraEffectContext = 
		static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());

	if (AuraEffectContext)
	{
		return AuraEffectContext->GetEffectDuration();
	}

	return 0.f;
}

FGameplayTag UAuraAbilitySystemLibrary::GetEffectType(
	const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FAuraGameplayEffectContext* AuraEffectContext = 
		static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());

	if (AuraEffectContext)
	{
		if (AuraEffectContext->GetEffectType().IsValid())
		{
			return *AuraEffectContext->GetEffectType();
		}
	}

	return FGameplayTag();
}

FVector UAuraAbilitySystemLibrary::GetForwardVector(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FAuraGameplayEffectContext* AuraEffectContext = 
	static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());

	if (AuraEffectContext)
	{
		return AuraEffectContext->GetForwardVector();
	}

	return FVector::ZeroVector;
}

bool UAuraAbilitySystemLibrary::GetApplyHitReact(
	const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FAuraGameplayEffectContext* AuraEffectContext = 
		static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());

	if (AuraEffectContext)
	{
		return AuraEffectContext->GetApplyHitReact();
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

void UAuraAbilitySystemLibrary::SetIsApplyingEffect(
	FGameplayEffectContextHandle& EffectContextHandle,
	bool bInApplyingEffect
	)
{
	FAuraGameplayEffectContext* AuraEffectContext =
		static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (AuraEffectContext)
	{
		AuraEffectContext->SetIsApplyingEffect(bInApplyingEffect);
	}
}

void UAuraAbilitySystemLibrary::SetEffectValue(
	FGameplayEffectContextHandle& EffectContextHandle,
	float InEffectValue
	)
{
	FAuraGameplayEffectContext* AuraEffectContext =
		static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (AuraEffectContext)
	{
		AuraEffectContext->SetEffectValue(InEffectValue);
	}
}

void UAuraAbilitySystemLibrary::SetEffectDuration(
	FGameplayEffectContextHandle& EffectContextHandle,
	float InEffectDuration
	)
{
	FAuraGameplayEffectContext* AuraEffectContext =
		static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (AuraEffectContext)
	{
		AuraEffectContext->SetEffectDuration(InEffectDuration);
	}
}

void UAuraAbilitySystemLibrary::SetEffectType(
	FGameplayEffectContextHandle& EffectContextHandle,
	const FGameplayTag& InEffectType
	)
{
	FAuraGameplayEffectContext* AuraEffectContext =
		static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (AuraEffectContext)
	{
		const TSharedPtr<FGameplayTag> EffectType = MakeShared<FGameplayTag>(InEffectType);
		AuraEffectContext->SetEffectType(EffectType);
	}
}

void UAuraAbilitySystemLibrary::SetForwardVector(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InForce)
{
	FAuraGameplayEffectContext* AuraEffectContext =
		static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (AuraEffectContext)
	{
		AuraEffectContext->SetForwardVector(InForce);
	}
}

void UAuraAbilitySystemLibrary::SetApplyHitReact(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInApplyHitReact)
{
	FAuraGameplayEffectContext* AuraEffectContext =
		static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (AuraEffectContext)
	{
		AuraEffectContext->SetApplyHitReact(bInApplyHitReact);
	}
}

void UAuraAbilitySystemLibrary::GetAliveCharactersWithinRadius(
	const AActor* ContextActor,
	TArray<AActor*>& OutOverlappingActors,
	TArray<AActor*>& ActorsToIgnore,
	float Radius,
	const FVector& SphereOrigin,
	ETargetTeam TargetTeam
)
{
	if (TargetTeam == ETargetTeam::ETT_Self) return;
	
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(ContextActor, EGetWorldErrorMode::LogAndReturnNull))
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

			if (!OverlappedActor->Implements<UCombatInterface>()) continue;
			if (ICombatInterface::Execute_IsDead(OverlappedActor)) continue;

			if (TargetTeam == ETargetTeam::ETT_Both)
			{
				OutOverlappingActors.AddUnique(OverlappedActor);
				continue;
			}

			if (TargetTeam == ETargetTeam::ETT_Enemies &&
				AreActorsEnemies(ContextActor, OverlappedActor)
				)
			{
				OutOverlappingActors.AddUnique(OverlappedActor);
				continue;
			}

			if (TargetTeam == ETargetTeam::ETT_Friends &&
				AreActorsFriends(ContextActor, OverlappedActor)
				)
			{
				OutOverlappingActors.AddUnique(OverlappedActor);
			}
		}
	}
}

AActor* UAuraAbilitySystemLibrary::GetClosestActorToTarget(
	AActor* TargetActor,
	float Radius,
	ETargetTeam TargetTeam,
	TArray<AActor*>& ActorsToIgnore
	)
{
	if (TargetTeam == ETargetTeam::ETT_Self) return nullptr;
	
	const UWorld* World = GEngine->GetWorldFromContextObject(TargetActor, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return nullptr;
	
	ActorsToIgnore.AddUnique(TargetActor);
	
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	World->OverlapMultiByObjectType(
		Overlaps,
		TargetActor->GetActorLocation(),
		FQuat::Identity,
		FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
		FCollisionShape::MakeSphere(Radius),
		SphereParams
	);

	if (Overlaps.IsEmpty()) return nullptr;
	
	float ClosestDistance = Radius;
	AActor* ClosestActor = nullptr;
	for (auto& Overlap : Overlaps)
	{
		AActor* OverlappedActor = Overlap.GetActor();

		if (!OverlappedActor->Implements<UCombatInterface>()) continue;
		if (ICombatInterface::Execute_IsDead(OverlappedActor)) continue;
		const float Distance = TargetActor->GetDistanceTo(OverlappedActor);
		if (ClosestDistance < Distance) continue;

		if (TargetTeam == ETargetTeam::ETT_Enemies &&
			AreActorsEnemies(TargetActor, OverlappedActor)
			)
		{
			ClosestDistance = Distance;
			ClosestActor = OverlappedActor;
			continue;
		}

		if (
			TargetTeam == ETargetTeam::ETT_Friends &&
			AreActorsFriends(TargetActor, OverlappedActor)
			)
		{
			ClosestDistance = Distance;
			ClosestActor = OverlappedActor;
			continue;
		}

		if (TargetTeam == ETargetTeam::ETT_Both)
		{
			ClosestDistance = Distance;
			ClosestActor = OverlappedActor;
		}
	}

	return ClosestActor;
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

void UAuraAbilitySystemLibrary::GetFriendsWithinRadius(
	AActor* TargetActor,
	TArray<AActor*>& OutFriends,
	float Radius,
	const FVector& SphereOrigin)
{
	TArray ActorsToIgnore({
		TargetActor
	});
	GetAliveCharactersWithinRadius(
	 TargetActor,
	 OutFriends,
	 ActorsToIgnore,
	 Radius,
	 SphereOrigin,
	 ETargetTeam::ETT_Friends
	);
}

void UAuraAbilitySystemLibrary::GetFriendsWithinTrace(
	AActor* TargetActor,
	TArray<AActor*>& OutFriends,
	const FVector& Start,
	const FVector& End,
	float Radius)
{
	const TArray ActorsToIgnore({ TargetActor });
	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::SphereTraceMulti(
		TargetActor,
		Start,
		End,
		Radius,
		ETT_MultiHitTrace,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResults,
		true
	);

	for (const auto& HitResult : HitResults)
	{
		if (
			HitResult.GetActor() &&
			AreActorsFriends(TargetActor, HitResult.GetActor())
			)
		{
			OutFriends.AddUnique(HitResult.GetActor());
		}
	}
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

void UAuraAbilitySystemLibrary::GetEnemiesWithinRadius(AActor* TargetActor,
	TArray<AActor*>& OutEnemies, float Radius, const FVector& SphereOrigin)
{
	TArray ActorsToIgnore({
		TargetActor
	});
	GetAliveCharactersWithinRadius(
	 TargetActor,
	 OutEnemies,
	 ActorsToIgnore,
	 Radius,
	 SphereOrigin,
	 ETargetTeam::ETT_Enemies
	);
}

TArray<AActor*> UAuraAbilitySystemLibrary::GetAllTeamMembersFromActor(AActor* Actor)
{
	return TArray({ Actor });
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

	return ASC->HasMatchingGameplayTag(FAuraGameplayTags::Get().StatusEffects_Invulnerable);
}

FString UAuraAbilitySystemLibrary::GetAbilityDescription(
	const UAbilityInfo* AbilityInfo,
	const FGameplayTag& AbilityTag,
	int32 Level
	)
{
	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
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
	const UAbilityInfo* AbilityInfo,
	const FGameplayTag& AbilityTag,
	int32 Level
	)
{
	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
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
	const FAuraNamedArguments& Args = FAuraNamedArguments::Get();

	if (const UActiveAbility* ActiveAbility = Cast<UActiveAbility>(Ability))
		if (ActiveAbility->IsBounceModeActive())
		{
			OutDescription = FText::FormatNamed(
				OutDescription,
				Args.BounceCount_0,
				ActiveAbility->GetMaxBounceCountAtLevel(Level),
				Args.BounceCount_1,
				ActiveAbility->GetMaxBounceCountAtLevel(Level + 1)
			);
		}
	
	if (const UActiveDamageAbility* DamageAbility = Cast<UActiveDamageAbility>(Ability))
	{
		OutDescription = FText::FormatNamed(
			OutDescription,
			Args.Dmg_0,
			DamageAbility->GetRoundedDamageAtLevel(Level),
			Args.Dmg_1,
			DamageAbility->GetRoundedDamageAtLevel(Level + 1)
			);

		if (const UBeamAbility* BeamAbility = Cast<UBeamAbility>(DamageAbility))
		{
			OutDescription = FText::FormatNamed(
				OutDescription,
				Args.Period,
				BeamAbility->GetBeamTickPeriod()
			);
		}
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

FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyAbilityEffect(
	const FAbilityParams& AbilityParams,
	bool& bSuccess
	)
{
	bSuccess = false;
	
	const AActor* SourceAvatarActor = AbilityParams.SourceASC->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContextHandle = AbilityParams
		.SourceASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);

	if (HasAnyHarmfulEffectBlockTag(AbilityParams.TargetASC))
	{
		if (HasAnyParryTag(AbilityParams.TargetASC))
		{
			const FGameplayTagContainer TagContainer({
				FAuraGameplayTags::Get().Abilities_Reaction_ShieldStackRemove
			});
			AbilityParams.TargetASC->TryActivateAbilitiesByTag(TagContainer);
		}
		return EffectContextHandle;
	}
	
	const FDamageParams& DamageParams = AbilityParams.DamageParams;
	const FEffectParams& EffectParams = AbilityParams.EffectParams;
	
	SetForwardVector(EffectContextHandle, AbilityParams.ForwardVector);
	
	if (DamageParams.IsValid())
	{
		SetApplyHitReact(EffectContextHandle, DamageParams.bApplyHitReact);

		const FGameplayEffectSpecHandle DamageSpecHandle = AbilityParams.SourceASC->MakeOutgoingSpec(
				DamageParams.DamageEffectClass,
				AbilityParams.AbilityLevel,
				EffectContextHandle
				);

		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
			DamageSpecHandle,
			DamageParams.DamageType,
			DamageParams.BaseDamage
			);
		
		AbilityParams.TargetASC
			->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data);
		bSuccess = true;
	}

	if (EffectParams.IsValid())
	{
		const FGameplayEffectSpecHandle EffectSpecHandle = AbilityParams.SourceASC->MakeOutgoingSpec(
				EffectParams.GameplayEffectClass,
				AbilityParams.AbilityLevel,
				EffectContextHandle
				);

		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
			EffectSpecHandle,
			EffectParams.GameplayTag,
			EffectParams.Value
			);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
			EffectSpecHandle,
			FAuraGameplayTags::Get().StatusEffects_Duration,
			EffectParams.Duration
			);

		AbilityParams.TargetASC
			->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
		
		bSuccess = true;
	}

	return EffectContextHandle;
}

bool UAuraAbilitySystemLibrary::HasAnyHarmfulEffectBlockTag(const UAbilitySystemComponent* TargetASC)
{
	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
	const TArray EffectBlockTagsArray({
		Tags.StatusEffects_Invulnerable,
		Tags.StatusEffects_Buff_Shield
	});
	
	const FGameplayTagContainer EffectBlockTagsContainer = FGameplayTagContainer::CreateFromArray(EffectBlockTagsArray);
	return TargetASC->HasAnyMatchingGameplayTags(EffectBlockTagsContainer);
}

bool UAuraAbilitySystemLibrary::HasAnyParryTag(const UAbilitySystemComponent* TargetASC)
{
	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
	const TArray ParryTagsArray({
		Tags.StatusEffects_Buff_Shield
	});
	
	const FGameplayTagContainer ParryTagsContainer = FGameplayTagContainer::CreateFromArray
	(ParryTagsArray);
	return TargetASC->HasAnyMatchingGameplayTags(ParryTagsContainer);
}

TArray<FRotator> UAuraAbilitySystemLibrary::EvenlySpacedRotators(
	const FVector& Forward,
	const FVector& Axis,
	float Spread,
	int32 Count
	)
{
	TArray<FRotator> Rotators;
	
	if (Count > 1)
	{
		const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
		const float DeltaSpread = Spread / (Count - 1);

		for (int32 i = 0; i < Count; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, Axis);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}

	return Rotators;
}

TArray<FVector> UAuraAbilitySystemLibrary::EvenlyRotatedVectors(
	const FVector& Forward,
	const FVector& Axis,
	float Spread,
	int32 Count
	)
{
	TArray<FVector> Vectors;
	
	if (Count > 1)
	{
		const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
		const float DeltaSpread = Spread / (Count - 1);

		for (int32 i = 0; i < Count; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, Axis);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(Forward);
	}

	return Vectors;
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
