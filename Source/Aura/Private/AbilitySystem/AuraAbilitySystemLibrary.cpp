// Copyright Lucas Rossi


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AuraNamedArguments.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Abilities/BaseAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/UpgradeInfo.h"
#include "Aura/Aura.h"
#include "Character/AuraCharacterBase.h"
#include "Components/TeamComponent.h"
#include "Engine/DamageEvents.h"
#include "Enums/TargetTeam.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/HUD/MainMenuHUD.h"
#include "UI/WidgetController/MainMenuWidgetController.h"

UAuraAbilitySystemComponent* UAuraAbilitySystemLibrary::GetAuraAbilitySystemComponent(
  AActor* Actor
)
{
  return Cast<UAuraAbilitySystemComponent>(
    UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor)
  );
}

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

UUpgradeMenuWidgetController* UAuraAbilitySystemLibrary::GetUpgradeMenuWidgetController(
  const UObject* WorldContextObject
)
{
  const FWidgetControllerParams WidgetControllerParams = MakeWidgetControllerParams(WorldContextObject);
  if (!WidgetControllerParams.IsValid()) return nullptr;

  AAuraHUD* AuraHUD = WidgetControllerParams.PlayerController->GetHUD<AAuraHUD>();
  if (!AuraHUD) return nullptr;

  return AuraHUD->GetUpgradeMenuWidgetController(WidgetControllerParams);
}

ULoadoutWidgetController* UAuraAbilitySystemLibrary::GetLoadoutWidgetController(const UObject* WorldContextObject)
{
  const FWidgetControllerParams WidgetControllerParams = MakeWidgetControllerParams(WorldContextObject);
  if (!WidgetControllerParams.IsValid()) return nullptr;

  AAuraHUD* AuraHUD = WidgetControllerParams.PlayerController->GetHUD<AAuraHUD>();
  if (!AuraHUD) return nullptr;

  return AuraHUD->GetLoadoutWidgetController(WidgetControllerParams);
}

UMainMenuWidgetController* UAuraAbilitySystemLibrary::GetMainMenuWidgetController(const UObject* WorldContextObject)
{
  AMainMenuHUD* MainMenuHUD = WorldContextObject->GetWorld()->GetFirstPlayerController()->GetHUD<AMainMenuHUD>();
  if (!MainMenuHUD) return nullptr;

  return MainMenuHUD->GetMainMenuWidgetController();
}

// void UAuraAbilitySystemLibrary::GiveStartupAbilities(
//   const UObject* WorldContextObject,
//   UAbilitySystemComponent* ASC
// )
// {
//   UCharacterInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
//   for (const auto AbilityClass : CharacterClassInfo->CommonAbilities)
//   {
//     FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
//     ASC->GiveAbility(AbilitySpec);
//   }
//
//   AAuraCharacterBase* Character = Cast<AAuraCharacterBase>(ASC->GetAvatarActor());
//   if (Character)
//   {
//     for (const auto AbilityClass : Character->NativeCharacterAbilities)
//     {
//       FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
//       ASC->GiveAbility(AbilitySpec);
//     }
//   }
// }

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

TArray<FEffectParams> UAuraAbilitySystemLibrary::GetStatusEffects(
  const FGameplayEffectContextHandle& EffectContextHandle)
{
  const FAuraGameplayEffectContext* AuraEffectContext =
    static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());

  if (AuraEffectContext)
  {
    if (AuraEffectContext->GetStatusEffects().Num() > 0)
    {
      return AuraEffectContext->GetStatusEffects();
    }
  }

  return TArray<FEffectParams>();
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

bool UAuraAbilitySystemLibrary::IsRadialDamage(
  const FGameplayEffectContextHandle& EffectContextHandle)
{
  const FAuraGameplayEffectContext* AuraEffectContext =
    static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());

  if (AuraEffectContext)
  {
    return AuraEffectContext->IsAreaAbility();
  }

  return false;
}

float UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(
  const FGameplayEffectContextHandle& EffectContextHandle)
{
  const FAuraGameplayEffectContext* AuraEffectContext =
    static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());

  if (AuraEffectContext)
  {
    return AuraEffectContext->GetAreaInnerRadius();
  }

  return 0.f;
}

float UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(
  const FGameplayEffectContextHandle& EffectContextHandle)
{
  const FAuraGameplayEffectContext* AuraEffectContext =
    static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());

  if (AuraEffectContext)
  {
    return AuraEffectContext->GetAreaOuterRadius();
  }

  return 0.f;
}

FVector UAuraAbilitySystemLibrary::GetOrigin(
  const FGameplayEffectContextHandle& EffectContextHandle)
{
  const FAuraGameplayEffectContext* AuraEffectContext =
    static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());

  if (AuraEffectContext)
  {
    return AuraEffectContext->GetAreaOrigin();
  }

  return FVector::ZeroVector;
}

FRadialProps UAuraAbilitySystemLibrary::GetRadialProps(
  const FGameplayEffectContextHandle& EffectContextHandle
)
{
  FRadialProps RadialProps;
  RadialProps.Origin = GetOrigin(EffectContextHandle);
  RadialProps.InnerRadius = GetRadialDamageInnerRadius(EffectContextHandle);
  RadialProps.OuterRadius = GetRadialDamageOuterRadius(EffectContextHandle);

  return RadialProps;
}

void UAuraAbilitySystemLibrary::SetIsParried(
  FGameplayEffectContextHandle& EffectContextHandle,
  bool bInParried
)
{
  FAuraGameplayEffectContext* AuraEffectContext =
    static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
  if (AuraEffectContext)
  {
    AuraEffectContext->SetIsParried(bInParried);
  }
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(
  FGameplayEffectContextHandle& EffectContextHandle,
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

void UAuraAbilitySystemLibrary::SetStatusEffects(
  FGameplayEffectContextHandle& EffectContextHandle,
  const TArray<FEffectParams>& InEffectsArray
)
{
  FAuraGameplayEffectContext* AuraEffectContext =
    static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
  if (AuraEffectContext)
  {
    AuraEffectContext->SetStatusEffects(InEffectsArray);
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

void UAuraAbilitySystemLibrary::SetIsAreaAbility(
  FGameplayEffectContextHandle& EffectContextHandle,
  bool bInIsRadialDamage
)
{
  FAuraGameplayEffectContext* AuraEffectContext =
    static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
  if (AuraEffectContext)
  {
    AuraEffectContext->SetIsAreaAbility(bInIsRadialDamage);
  }
}

void UAuraAbilitySystemLibrary::SetAreaInnerRadius(
  FGameplayEffectContextHandle& EffectContextHandle,
  float InInnerRadius
)
{
  FAuraGameplayEffectContext* AuraEffectContext =
    static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
  if (AuraEffectContext)
  {
    AuraEffectContext->SetAreaInnerRadius(InInnerRadius);
  }
}

void UAuraAbilitySystemLibrary::SetAreaOuterRadius(
  FGameplayEffectContextHandle& EffectContextHandle,
  float InOuterRadius
)
{
  FAuraGameplayEffectContext* AuraEffectContext =
    static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
  if (AuraEffectContext)
  {
    AuraEffectContext->SetAreaOuterRadius(InOuterRadius);
  }
}

void UAuraAbilitySystemLibrary::SetAreaOrigin(
  FGameplayEffectContextHandle& EffectContextHandle,
  const FVector& InOrigin
)
{
  FAuraGameplayEffectContext* AuraEffectContext =
    static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
  if (AuraEffectContext)
  {
    AuraEffectContext->SetAreaOrigin(InOrigin);
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
  if (TargetTeam == ETargetTeam::Self) return;

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

    AddOverlappedCharactersByTeam(ContextActor, OutOverlappingActors, Overlaps, TargetTeam);
  }
}

void UAuraAbilitySystemLibrary::GetAliveCharactersWithinBox(
  const AActor* ContextActor,
  TArray<AActor*>& OutOverlappingActors,
  TArray<AActor*>& ActorsToIgnore,
  const FVector& Dimensions,
  const FVector& Center,
  const FQuat& Rotation,
  ETargetTeam TargetTeam
)
{
  if (TargetTeam == ETargetTeam::Self) return;

  FCollisionQueryParams BoxParams;
  BoxParams.AddIgnoredActors(ActorsToIgnore);

  TArray<FOverlapResult> Overlaps;
  if (const UWorld* World = GEngine->GetWorldFromContextObject(ContextActor, EGetWorldErrorMode::LogAndReturnNull))
  {
    World->OverlapMultiByObjectType(
      Overlaps,
      Center,
      Rotation,
      FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
      FCollisionShape::MakeBox(Dimensions),
      BoxParams
    );

    AddOverlappedCharactersByTeam(ContextActor, OutOverlappingActors, Overlaps, TargetTeam);
  }
}

void UAuraAbilitySystemLibrary::GetAliveCharactersWithinCone(
  const AActor* ContextActor,
  TArray<AActor*>& OutOverlappingActors,
  TArray<AActor*>& ActorsToIgnore,
  const FVector& Origin,
  const FVector& Direction,
  float Length,
  float AngleWidth,
  float AngleHeight,
  ETargetTeam TargetTeam
)
{
  if (TargetTeam == ETargetTeam::Self) return;

  FCollisionQueryParams ConeParams;
  ConeParams.AddIgnoredActors(ActorsToIgnore);

  const TArray<FOverlapResult> Overlaps;
  if (const UWorld* World = GEngine->GetWorldFromContextObject(ContextActor, EGetWorldErrorMode::LogAndReturnNull))
  {
    //TODO: Cone logic

    AddOverlappedCharactersByTeam(ContextActor, OutOverlappingActors, Overlaps, TargetTeam);
  }
}

void UAuraAbilitySystemLibrary::GetAliveCharactersWithinLine(
  const AActor* ContextActor,
  TArray<AActor*>& OutOverlappingActors,
  TArray<AActor*>& ActorsToIgnore,
  const FVector& LineStart,
  const FVector& LineEnd,
  ETargetTeam TargetTeam
)
{
  if (TargetTeam == ETargetTeam::Self) return;

  FCollisionQueryParams LineParams;
  LineParams.AddIgnoredActors(ActorsToIgnore);

  TArray<FHitResult> HitResults;
  if (const UWorld* World = GEngine->GetWorldFromContextObject(ContextActor, EGetWorldErrorMode::LogAndReturnNull))
  {
    World->LineTraceMultiByObjectType(
      HitResults,
      LineStart,
      LineEnd,
      FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
      LineParams
    );

    AddHitCharactersByTeam(ContextActor, OutOverlappingActors, HitResults, TargetTeam);
  }
}

AActor* UAuraAbilitySystemLibrary::GetClosestActorToTarget(
  AActor* TargetActor,
  float Radius,
  ETargetTeam TargetTeam,
  TArray<AActor*>& ActorsToIgnore
)
{
  if (TargetTeam == ETargetTeam::Self) return nullptr;

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

    if (TargetTeam == ETargetTeam::Enemies &&
      AreActorsEnemies(TargetActor, OverlappedActor)
    )
    {
      ClosestDistance = Distance;
      ClosestActor = OverlappedActor;
      continue;
    }

    if (
      TargetTeam == ETargetTeam::Friends &&
      AreActorsFriends(TargetActor, OverlappedActor)
    )
    {
      ClosestDistance = Distance;
      ClosestActor = OverlappedActor;
      continue;
    }

    if (TargetTeam == ETargetTeam::Both)
    {
      ClosestDistance = Distance;
      ClosestActor = OverlappedActor;
    }
  }

  return ClosestActor;
}

AActor* UAuraAbilitySystemLibrary::GetClosestTargetToPoint(
  AActor* Instigator,
  FVector Point,
  float Radius,
  ETargetTeam TargetTeam,
  TArray<AActor*>& ActorsToIgnore
)
{
  const UWorld* World = GEngine->GetWorldFromContextObject(Instigator, EGetWorldErrorMode::LogAndReturnNull);
  if (!World) return nullptr;

  if (TargetTeam == ETargetTeam::Enemies) ActorsToIgnore.AddUnique(Instigator);

  FCollisionQueryParams SphereParams;
  SphereParams.AddIgnoredActors(ActorsToIgnore);

  TArray<FOverlapResult> Overlaps;
  World->OverlapMultiByObjectType(
    Overlaps,
    Point,
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
    const float Distance = FVector::Distance(Point, OverlappedActor->GetActorLocation());
    if (ClosestDistance < Distance) continue;

    if (TargetTeam == ETargetTeam::Enemies &&
      AreActorsEnemies(Instigator, OverlappedActor)
    )
    {
      ClosestDistance = Distance;
      ClosestActor = OverlappedActor;
      continue;
    }

    if (
      TargetTeam == ETargetTeam::Friends &&
      AreActorsFriends(Instigator, OverlappedActor)
    )
    {
      ClosestDistance = Distance;
      ClosestActor = OverlappedActor;
      continue;
    }

    if (TargetTeam == ETargetTeam::Both)
    {
      ClosestDistance = Distance;
      ClosestActor = OverlappedActor;
    }
  }

  return ClosestActor;
}

bool UAuraAbilitySystemLibrary::IsEnvironment(const AActor* Actor)
{
  if (!IsValid(Actor)) return false;

  const UTeamComponent* TeamComponent = Actor->GetComponentByClass<UTeamComponent>();

  return !IsValid(TeamComponent);
}

bool UAuraAbilitySystemLibrary::AreActorsFriends(const AActor* FirstActor, const AActor*
                                                 SecondActor)
{
  if (!IsValid(FirstActor) || !IsValid(SecondActor)) return false;

  const UTeamComponent* FirstTeamComponent = FirstActor->GetComponentByClass<UTeamComponent>();
  const UTeamComponent* SecondTeamComponent = SecondActor->GetComponentByClass<UTeamComponent>();

  if (
    !FirstTeamComponent ||
    !SecondTeamComponent ||
    FirstTeamComponent->TeamID == HOSTILE_TO_ALL ||
    SecondTeamComponent->TeamID == HOSTILE_TO_ALL
  )
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
    ETargetTeam::Friends
  );
}

void UAuraAbilitySystemLibrary::GetFriendsWithinTrace(
  AActor* TargetActor,
  TArray<AActor*>& OutFriends,
  const FVector& Start,
  const FVector& End,
  float Radius)
{
  const TArray ActorsToIgnore({TargetActor});
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

bool UAuraAbilitySystemLibrary::AreActorsEnemies(
  const AActor* FirstActor,
  const AActor* SecondActor
)
{
  if (!IsValid(FirstActor) || !IsValid(SecondActor)) return false;

  const UTeamComponent* FirstTeamComponent = FirstActor->GetComponentByClass<UTeamComponent>();
  const UTeamComponent* SecondTeamComponent = SecondActor->GetComponentByClass<UTeamComponent>();

  if (!FirstTeamComponent || !SecondTeamComponent)
  {
    return false;
  }

  return FirstTeamComponent->TeamID != SecondTeamComponent->TeamID ||
    FirstTeamComponent->TeamID == HOSTILE_TO_ALL ||
    SecondTeamComponent->TeamID == HOSTILE_TO_ALL;
}

void UAuraAbilitySystemLibrary::GetEnemiesWithinRadius(AActor* TargetActor,
                                                       TArray<AActor*>& OutEnemies, float Radius,
                                                       const FVector& SphereOrigin)
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
    ETargetTeam::Enemies
  );
}

TArray<AActor*> UAuraAbilitySystemLibrary::GetAllTeamMembersFromActor(AActor* Actor)
{
  return TArray({Actor});
}

bool UAuraAbilitySystemLibrary::IsPlayerUsingGamepad(const AActor* AvatarActor)
{
  AController* Controller = AvatarActor->GetInstigatorController();

  if (AAuraPlayerController* PlayerController = Cast<AAuraPlayerController>(Controller))
  {
    return PlayerController->IsUsingGamepad();
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
  const UBaseAbility* Ability = Info.Ability.GetDefaultObject();
  FormatAbilityDescriptionAtLevel(Info, Level, Info.Description);

  FString ManaCostText;
  FString CooldownText;
  FString ChargesText;
  MakeAbilityDetailsText(Ability, Level, ManaCostText, CooldownText, ChargesText);

  return FString::Printf(
    TEXT(
      "<Title>%s </>\n"
      "<Title18>Level </><Level>%d</>\n"
      "\n"
      "%s\n"
      "%s"
      "%s"
      "%s"
    ),
    *Info.Name.ToString(),
    Level,
    *Info.Description.ToString(),
    *ManaCostText,
    *ChargesText,
    *CooldownText
  );
}

FString UAuraAbilitySystemLibrary::GetAbilityNextLevelDescription(
  const UAbilityInfo* AbilityInfo,
  const FGameplayTag& AbilityTag,
  int32 Level
)
{
  FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
  const UBaseAbility* Ability = Info.Ability.GetDefaultObject();
  FormatAbilityDescriptionAtLevel(Info, Level, Info.NextLevelDescription);

  FString ManaCostText;
  FString CooldownText;
  MakeManaAndCooldownTextNextLevel(Ability, Level, ManaCostText, CooldownText);

  return FString::Printf(
    TEXT(
      "<Title>%s </>\n"
      "<Title18>Level </><Old>%d</> > <Level>%d</>\n"
      "\n"
      "%s\n"
      "\n"
      "%s\n"
      "%s"
    ),
    *Info.Name.ToString(),
    Level,
    Level + 1,
    *Info.NextLevelDescription.ToString(),
    *ManaCostText,
    *CooldownText
  );
}

void UAuraAbilitySystemLibrary::FormatAbilityDescriptionAtLevel(
  const FAuraAbilityInfo& AbilityInfo,
  int32 Level,
  FText& OutDescription
)
{
  const FAuraNamedArguments& Args = FAuraNamedArguments::Get();
  const UBaseAbility* Ability = AbilityInfo.Ability.GetDefaultObject();

  if (IAbilityInterface::Execute_IsDamageAbility(Ability))
  {
    const FGameplayTag& DamageTypeTag = IAbilityInterface::Execute_GetDamageTypeTag(Ability);
    if (const auto Tuple = Args.DamageTypeTexts.Find(DamageTypeTag))
    {
      OutDescription = FText::FormatNamed(
        OutDescription,
        Tuple->Key,
        Tuple->Value
      );
    }
    if (const auto NextTuple = Args.NextDamageTypeTexts.Find(DamageTypeTag))
    {
      OutDescription = FText::FormatNamed(
        OutDescription,
        NextTuple->Key,
        NextTuple->Value
      );
    }

    OutDescription = FText::FormatNamed(
      OutDescription,
      Args.Dmg,
      IAbilityInterface::Execute_GetRoundedDamageAtLevel(Ability, Level),
      Args.Dmg_,
      IAbilityInterface::Execute_GetRoundedDamageAtLevel(Ability, Level + 1)
    );
  }

  const TArray<FScalableFloat>& Percents = AbilityInfo.DescriptionPercents;
  if (Percents.Num() > 0)
  {
    for (int32 i = 0; i < Percents.Num(); i++)
    {
      OutDescription = FText::FormatNamed(
        OutDescription,
        Args.AbilityGenericArgs[i].Percent,
        Percents[i].GetValueAtLevel(Level) * 100,
        Args.AbilityGenericArgs[i].Percent_,
        Percents[i].GetValueAtLevel(Level + 1) * 100
      );
    }
  }

  const TArray<FScalableFloat>& Values = AbilityInfo.DescriptionValues;
  if (Values.Num() > 0)
  {
    for (int32 i = 0; i < Values.Num(); i++)
    {
      OutDescription = FText::FormatNamed(
        OutDescription,
        Args.AbilityGenericArgs[i].Value,
        Values[i].GetValueAtLevel(Level),
        Args.AbilityGenericArgs[i].Value_,
        Values[i].GetValueAtLevel(Level + 1)
      );
    }
  }
}

void UAuraAbilitySystemLibrary::FormatUpgradeDescriptionAtLevel(
  const FAuraUpgradeInfo& UpgradeInfo, int32 Level, FText& OutDescription
)
{
  const FAuraNamedArguments& Args = FAuraNamedArguments::Get();

  const TArray<FScalableFloat>& Percents = UpgradeInfo.DescriptionPercents;
  if (Percents.Num() > 0)
  {
    for (int32 i = 0; i < Percents.Num(); i++)
    {
      OutDescription = FText::FormatNamed(
        OutDescription,
        Args.AbilityGenericArgs[i].Percent,
        Percents[i].GetValueAtLevel(Level) * 100,
        Args.AbilityGenericArgs[i].Percent_,
        Percents[i].GetValueAtLevel(Level + 1) * 100
      );
    }
  }

  const TArray<FScalableFloat>& Values = UpgradeInfo.DescriptionValues;
  if (Values.Num() > 0)
  {
    for (int32 i = 0; i < Values.Num(); i++)
    {
      OutDescription = FText::FormatNamed(
        OutDescription,
        Args.AbilityGenericArgs[i].Value,
        Values[i].GetValueAtLevel(Level),
        Args.AbilityGenericArgs[i].Value_,
        Values[i].GetValueAtLevel(Level + 1)
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

void UAuraAbilitySystemLibrary::MakeAbilityDetailsText(
  const UBaseAbility* Ability,
  int32 Level,
  FString& OutManaText,
  FString& OutCooldownText,
  FString& OutChargesText
)
{
  const int32 ManaCost = Ability->GetRoundedManaCostAtLevel(Level);
  const int32 Cooldown = Ability->GetRoundedCooldownAtLevel(Level);
  const int32 Charges = Ability->GetMaxChargesAtLevel(Level);

  if (ManaCost == 0)
  {
    OutManaText = FString::Printf(TEXT(""));
  }
  else
  {
    OutManaText = FString::Printf(
      TEXT("\n<Info>Mana - </><Mana>%d</>"),
      ManaCost
    );
  }

  if (Cooldown == 0)
  {
    OutCooldownText = FString::Printf(TEXT(""));
  }
  else
  {
    OutCooldownText = FString::Printf(
      TEXT("\n<Info>Cooldown - </>%ds"),
      Cooldown
    );
  }

  if (Charges <= 1)
  {
    OutChargesText = FString::Printf(TEXT(""));
  }
  else
  {
    OutChargesText = FString::Printf(
      TEXT("\n<Info>Charges - </>%d"),
      Charges
    );
  }
}

void UAuraAbilitySystemLibrary::MakeManaAndCooldownTextNextLevel(
  const UBaseAbility* Ability,
  int32 Level,
  FString& OutManaText,
  FString& OutCooldownText
)
{
  const int32 ManaCost = Ability->GetRoundedManaCostAtLevel(Level);
  const int32 NextManaCost = Ability->GetRoundedManaCostAtLevel(Level + 1);
  const int32 Cooldown = Ability->GetRoundedCooldownAtLevel(Level);
  const int32 NextCooldown = Ability->GetRoundedCooldownAtLevel(Level + 1);
  if (NextManaCost - ManaCost == 0)
  {
    if (ManaCost == 0)
    {
      OutManaText = FString::Printf(TEXT(""));
    }
    else
    {
      OutManaText = FString::Printf(
        TEXT("<Info>Mana - </><Mana>%d</>"),
        ManaCost
      );
    }
  }
  else
  {
    OutManaText = FString::Printf(
      TEXT("<Info>Mana - </><Old>%d</> > <Mana>%d</>"),
      ManaCost,
      NextManaCost
    );
  }

  if (NextCooldown - Cooldown == 0)
  {
    if (Cooldown == 0)
    {
      OutCooldownText = FString::Printf(TEXT(""));
    }
    else
    {
      OutCooldownText = FString::Printf(
        TEXT("<Info>Cooldown - </>%ds"),
        Cooldown
      );
    }
  }
  else
  {
    OutCooldownText = FString::Printf(
      TEXT("<Info>Cooldown - </><Old>%d</> > %ds"),
      Cooldown,
      NextCooldown
    );
  }
}

bool UAuraAbilitySystemLibrary::IsAbilityGranted(const UAbilitySystemComponent* ASC, const FGameplayTag& AbilityTag)
{
  TArray<FGameplayAbilitySpec*> SpecArray;
  ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(
    FGameplayTagContainer({ AbilityTag }),
    SpecArray
    );

  return !SpecArray.IsEmpty();
}

bool UAuraAbilitySystemLibrary::IsAbilityActive(const UAbilitySystemComponent* ASC, const FGameplayTag& AbilityTag)
{
  TArray<FGameplayAbilitySpec*> SpecArray;
  ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(
    FGameplayTagContainer({ AbilityTag }),
    SpecArray
    );

  if (SpecArray.IsEmpty()) return false;

  const FGameplayAbilitySpec* AbilitySpec = SpecArray[0];

  return AbilitySpec->IsActive();
}

void UAuraAbilitySystemLibrary::AddOverlappedCharactersByTeam(
  const AActor* ContextActor,
  TArray<AActor*>& OutOverlappingActors,
  const TArray<FOverlapResult>& Overlaps,
  ETargetTeam TargetTeam
)
{
  for (const auto& Overlap : Overlaps)
  {
    AActor* OverlappedActor = Overlap.GetActor();

    if (!OverlappedActor->Implements<UCombatInterface>()) continue;
    if (ICombatInterface::Execute_IsDead(OverlappedActor)) continue;

    switch (TargetTeam)
    {
    case ETargetTeam::Enemies:
      {
        if (AreActorsEnemies(ContextActor, OverlappedActor))
        {
          OutOverlappingActors.AddUnique(OverlappedActor);
        }
        break;
      }
    case ETargetTeam::Friends:
      {
        if (AreActorsFriends(ContextActor, OverlappedActor))
        {
          OutOverlappingActors.AddUnique(OverlappedActor);
        }
        break;
      }
    case ETargetTeam::Both:
      {
        OutOverlappingActors.AddUnique(OverlappedActor);
        break;
      }
    default:
      {
      }
    }
  }
}

void UAuraAbilitySystemLibrary::AddHitCharactersByTeam(
  const AActor* ContextActor,
  TArray<AActor*>& OutOverlappingActors,
  const TArray<FHitResult>& HitResults,
  ETargetTeam TargetTeam
)
{
  for (const auto& HitResult : HitResults)
  {
    AActor* HitActor = HitResult.GetActor();

    if (!HitActor->Implements<UCombatInterface>()) continue;
    if (ICombatInterface::Execute_IsDead(HitActor)) continue;

    switch (TargetTeam)
    {
    case ETargetTeam::Enemies:
      {
        if (AreActorsEnemies(ContextActor, HitActor))
        {
          OutOverlappingActors.AddUnique(HitActor);
        }
        break;
      }
    case ETargetTeam::Friends:
      {
        if (AreActorsFriends(ContextActor, HitActor))
        {
          OutOverlappingActors.AddUnique(HitActor);
        }
      }
    case ETargetTeam::Both:
      {
        OutOverlappingActors.AddUnique(HitActor);
      }
    default:
      {
      }
    }
  }
}

// TArray<FGameplayTag> UAuraAbilitySystemLibrary::GetAllAbilitiesFromLevel(
// 	const UObject* WorldContextObject,
// 	ECharacterName CharacterName,
// 	int32 Level
// 	)
// {
// 	TArray<FGameplayTag> AbilitiesTags;
// 	const UAbilityInfo* AbilityInfo = GetAbilitiesInfo(WorldContextObject);
//
// 	if (AbilityInfo == nullptr) return AbilitiesTags;
//
// 	for (auto Info : AbilityInfo->FindAbilitiesFromCharacter(CharacterName))
// 	{
// 		if (Info.LevelRequirement == Level) AbilitiesTags.Add(Info.AbilityTag);
// 	}
//
// 	return AbilitiesTags;
// }
//
// TArray<FGameplayTag> UAuraAbilitySystemLibrary::GetRandomAbilitiesFromLevel(
// 	const UObject* WorldContextObject,
// 	ECharacterName CharacterName,
// 	int32 Level,
// 	int32 Amount
// 	)
// {
// 	TArray<FGameplayTag> RandomAbilitiesTags;
// 	TArray<FGameplayTag> AllAbilitiesTags = GetAllAbilitiesFromLevel(
// 		WorldContextObject, 
// 		CharacterName,
// 		Level
// 		);
//
// 	if (AllAbilitiesTags.IsEmpty()) return RandomAbilitiesTags;
// 	
// 	for (int32 i = 1; i <= Amount; i++)
// 	{
// 		const int32 index = FMath::RandRange(0, AllAbilitiesTags.Num() - 1);
//
// 		if (AllAbilitiesTags.IsValidIndex(index))
// 		{
// 			RandomAbilitiesTags.Add(AllAbilitiesTags[index]);
// 			AllAbilitiesTags.RemoveAt(index);
// 		}
// 	}
//
// 	return RandomAbilitiesTags;
// }

FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyAbilityEffect(
  const FAbilityParams& AbilityParams,
  bool& bSuccess
)
{
  bSuccess = false;
  
  const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();

  const AActor* SourceAvatarActor = AbilityParams.SourceASC->GetAvatarActor();

  FGameplayEffectContextHandle EffectContextHandle = AbilityParams
                                                     .SourceASC->MakeEffectContext();
  EffectContextHandle.AddSourceObject(SourceAvatarActor);

  const FHealParams& HealParams = AbilityParams.HealParams;
  
  if (HealParams.IsValid())
  {
    const FGameplayEffectSpecHandle HealSpecHandle = AbilityParams.SourceASC->MakeOutgoingSpec(
      HealParams.HealEffectClass,
      AbilityParams.AbilityLevel,
      EffectContextHandle
    );

    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
      HealSpecHandle,
      AuraTags.Heal,
      HealParams.BaseHeal
    );

    AbilityParams.SourceASC
      ->ApplyGameplayEffectSpecToTarget(*HealSpecHandle.Data, AbilityParams.TargetASC);
    bSuccess = true;
  }

  if (HasAnyHarmfulEffectBlockTag(AbilityParams.TargetASC))
  {
    if (HasAnyParryTag(AbilityParams.TargetASC))
    {
      const FGameplayTagContainer TagContainer({
        AuraTags.Abilities_Reaction_ShieldStackRemove
      });
      AbilityParams.TargetASC->TryActivateAbilitiesByTag(TagContainer);
    }
    return EffectContextHandle;
  }

  const FDamageParams& DamageParams = AbilityParams.DamageParams;
  const TArray<FEffectParams>& EffectParams = AbilityParams.EffectParams;

  SetForwardVector(EffectContextHandle, AbilityParams.ForwardVector);
  SetIsAreaAbility(EffectContextHandle, AbilityParams.bIsAreaAbility);
  if (AbilityParams.bIsAreaAbility)
  {
    SetAreaOrigin(EffectContextHandle, AbilityParams.AreaOrigin);
    SetAreaInnerRadius(EffectContextHandle, AbilityParams.AreaInnerRadius);
    SetAreaOuterRadius(EffectContextHandle, AbilityParams.AreaOuterRadius);
  }

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

    AbilityParams.SourceASC
      ->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data, AbilityParams.TargetASC);
    bSuccess = true;
  }

  if (EffectParams.Num() > 0)
  {
    for (const auto& Effect : EffectParams)
    {
      if (!Effect.IsValid()) continue;
      const FGameplayEffectSpecHandle EffectSpecHandle = AbilityParams.SourceASC->MakeOutgoingSpec(
        Effect.GameplayEffectClass,
        AbilityParams.AbilityLevel,
        EffectContextHandle
      );

      UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
        EffectSpecHandle,
        Effect.GameplayTag,
        Effect.Value
      );
      UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
        EffectSpecHandle,
        FAuraGameplayTags::Get().StatusEffects_Duration,
        Effect.Duration
      );

      for (int32 i = 0; i < Effect.Stacks; i++)
      {
        AbilityParams.SourceASC
                     ->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data, AbilityParams.TargetASC);
      }
    }

    bSuccess = true;
  }

  return EffectContextHandle;
}

float UAuraAbilitySystemLibrary::GetRadialDamageWithFalloff(
  const AActor* TargetActor,
  float BaseDamage,
  float MinimumDamage,
  const FVector& Origin,
  float DamageInnerRadius,
  float DamageOuterRadius,
  float DamageFalloff
)
{
  if (!IsValid(TargetActor)) return BaseDamage;

  FRadialDamageParams RadialDamageParams;
  RadialDamageParams.BaseDamage = BaseDamage;
  RadialDamageParams.MinimumDamage = MinimumDamage;
  RadialDamageParams.DamageFalloff = DamageFalloff;
  RadialDamageParams.InnerRadius = DamageInnerRadius;
  RadialDamageParams.OuterRadius = DamageOuterRadius;
  const float DamageScale = RadialDamageParams.GetDamageScale(
    (Origin - TargetActor->GetActorLocation()).Length()
  );
  return BaseDamage * DamageScale;
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

    float DeltaSpread;
    if (Spread > 359.f)
    {
      DeltaSpread = Spread / Count;
    }
    else
    {
      DeltaSpread = Spread / (Count - 1);
    }

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

void UAuraAbilitySystemLibrary::ChangeActorTeam(
  int32 InTeamID,
  AActor* TargetActor,
  bool bPermanent
)
{
  if (!IsValid(TargetActor)) return;

  UTeamComponent* TeamComponent = TargetActor->GetComponentByClass<UTeamComponent>();

  if (!TeamComponent) return;

  TeamComponent->ChangeTeam(InTeamID, bPermanent);
}

void UAuraAbilitySystemLibrary::JoinToActorTeam(
  AActor* ActorToJoinTo,
  AActor* JoiningActor,
  bool bPermanent
)
{
  if (!IsValid(ActorToJoinTo) || !IsValid(JoiningActor)) return;

  if (UTeamComponent* JoiningActorTC = JoiningActor->GetComponentByClass<UTeamComponent>())
  {
    JoiningActorTC->JoinActorTeam(ActorToJoinTo, bPermanent);
  }
}

void UAuraAbilitySystemLibrary::GoBackToOriginalTeam(AActor* TargetActor)
{
  if (!IsValid(TargetActor)) return;

  UTeamComponent* TeamComponent = TargetActor->GetComponentByClass<UTeamComponent>();

  if (!TeamComponent) return;

  TeamComponent->GoBackToOriginalTeam();
}

AActor* UAuraAbilitySystemLibrary::GetActiveEffectCauser(
  UAbilitySystemComponent* AbilitySystemComponent,
  const FGameplayTag& EffectTag
)
{
  FGameplayEffectQuery Query;
  Query.OwningTagQuery = FGameplayTagQuery::MakeQuery_MatchTag(EffectTag);
  const TArray<FActiveGameplayEffectHandle> ActiveEffects = AbilitySystemComponent
    ->GetActiveEffects(Query);

  if (ActiveEffects.IsEmpty()) return nullptr;

  return AbilitySystemComponent
         ->GetEffectContextFromActiveGEHandle(ActiveEffects[0]).GetEffectCauser();
}
