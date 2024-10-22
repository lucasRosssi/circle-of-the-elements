// Copyright Lucas Rossi


#include "Character/AuraHero.h"

#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "Components/AuraCamera.h"
#include "Components/TeamComponent.h"
#include "Components/WidgetComponent.h"
#include "Enums/CameraState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Level/RegionInfo.h"
#include "Managers/AbilityManager.h"
#include "Managers/UpgradeManager.h"
#include "Player/AuraPlayerState.h"
#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"
#include "Utils/AuraSystemsLibrary.h"

AAuraHero::AAuraHero()
{
  InteractWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("InteractMessage");
  InteractWidgetComponent->SetupAttachment(GetRootComponent());
  InteractWidgetComponent->SetVisibility(false);

  UCharacterMovementComponent* Movement = GetCharacterMovement();
  Movement->bOrientRotationToMovement = true;
  Movement->RotationRate = FRotator(0.f, 400.f, 0.f);
  Movement->bConstrainToPlane = true;
  Movement->bSnapToPlaneAtStart = true;

  bUseControllerRotationPitch = false;
  bUseControllerRotationRoll = false;
  bUseControllerRotationYaw = false;

  CharacterType = ECharacterType::Player;
  TeamComponent->TeamID = PLAYER_TEAM;
}

void AAuraHero::PossessedBy(AController* NewController)
{
  Super::PossessedBy(NewController);

  // Init ability actor info for the Server
  InitAbilityActorInfo();
  AddCharacterAbilities();

  if (ActiveCamera)
  {
    GetAuraPlayerController()->SetPlayerCamera(ActiveCamera->GetCameraComponent());
    GetAuraPlayerController()->SetViewTargetWithBlend(ActiveCamera);
  }
}

void AAuraHero::OnRep_PlayerState()
{
  Super::OnRep_PlayerState();

  // Init ability actor info for the Client
  InitAbilityActorInfo();
}

void AAuraHero::AddCharacterAbilities()
{
  Super::AddCharacterAbilities();

  // GetAuraASC()->AddCharacterAbilities(
  //   EligibleAbilities,
  //   FAuraGameplayTags::Get().Abilities_Status_Eligible
  // );

  if (UAbilityManager* AbilityManager = UAuraSystemsLibrary::GetAbilityManager(this))
  {
    AbilityManager->GiveAcquiredAbilities(this);
  }
  if (UUpgradeManager* UpgradeManager = UAuraSystemsLibrary::GetUpgradeManager(this))
  {
    UpgradeManager->GiveAcquiredUpgrades();
  }
}

void AAuraHero::Die(const FVector& DeathImpulse)
{
  bDying = true;

  StartDeath();
}

void AAuraHero::SetCustomDepth_Implementation(int32 Value)
{
  Super::SetCustomDepth_Implementation(Value);
  Weapon->SetRenderCustomDepth(Value > 0);
  Weapon->SetCustomDepthStencilValue(Value);
}

void AAuraHero::DeathMontageEndRagdoll()
{
  RagdollMesh();
}

int32 AAuraHero::GetCharacterLevel_Implementation() const
{
  return GetAuraPlayerState()->GetCharacterLevel();
}

void AAuraHero::AddToXP_Implementation(int32 InXP)
{
  GetAuraPlayerState()->AddXP(InXP);
}

int32 AAuraHero::GetXP_Implementation() const
{
  return GetAuraPlayerState()->GetXP();;
}

int32 AAuraHero::GetAttributePoints_Implementation() const
{
  return GetAuraPlayerState()->GetAttributePoints();
}

int32 AAuraHero::GetSkillPoints_Implementation() const
{
  return GetAuraPlayerState()->GetSkillPoints();
}

void AAuraHero::AddAttributePoints_Implementation(int32 Amount)
{
  GetAuraPlayerState()->AddAttributePoints(Amount);
}

void AAuraHero::AddSkillPoints_Implementation(int32 Amount)
{
  GetAuraPlayerState()->AddSkillPoints(Amount);
}

void AAuraHero::ShowTargetingActor_Implementation(
  TSubclassOf<ATargetingActor> TargetingActorClass,
  ETargetTeam TargetTeam,
  float Radius
)
{
  AuraPlayerController->ShowTargetingActor(TargetingActorClass, TargetTeam, Radius);
}

void AAuraHero::HideTargetingActor_Implementation()
{
  AuraPlayerController->HideTargetingActor();
}

void AAuraHero::SetInteractMessageVisible_Implementation(const FText& InteractText)
{
  const bool bVisible = !InteractText.IsEmptyOrWhitespace();
  SetInteractionWidgetText(InteractText);
  InteractWidgetComponent->SetVisibility(bVisible);
}

void AAuraHero::AddInteractableToList_Implementation(const UInteractComponent* InteractableComponent)
{
  if (GetAuraPlayerController())
  {
    AuraPlayerController->AddInteractableInRange(InteractableComponent);
  }
}

void AAuraHero::RemoveInteractableFromList_Implementation(const UInteractComponent* InteractableComponent)
{
  if (GetAuraPlayerController())
  {
    AuraPlayerController->RemoveInteractableInRange(InteractableComponent);
  }
}

ECharacterName AAuraHero::GetHeroName_Implementation()
{
  return GetCharacterName();
}

void AAuraHero::StartDeath()
{
  bDead = true;
  OnDeath.Broadcast(this);

  ActiveCamera->SetCameraState(ECameraState::PlayerDeath);

  GetMovementComponent()->Deactivate();
  GetAuraPlayerController()->DisableController();
  
  UGameplayStatics::SetGlobalTimeDilation(this, 0.25f);
  PlayAnimMontage(HitReactMontage, 0.66f);

  FTimerHandle DeathTimer;
  GetWorld()->GetTimerManager().SetTimer(
    DeathTimer,
    this,
    &AAuraHero::EndDeath,
    0.3f,
    false
  );
}

void AAuraHero::EndDeath()
{
  GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_DEFAULT_COLOR);
  Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_DEFAULT_COLOR);

  UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
    this,
    DeathBloodEffect,
    GetActorLocation()
  );
  NiagaraComponent->SetRenderCustomDepth(true);
  NiagaraComponent->SetCustomDepthStencilValue(CUSTOM_DEPTH_DEFAULT_COLOR);
  NiagaraComponent->MarkRenderStateDirty();

  UGameplayStatics::PlaySound2D(
    this,
    DeathSound1,
    1.f,
    0.7f
  );

  UGameplayStatics::PlaySound2D(
    this,
    DeathSound2,
    1.f,
    0.7f
  );

  UGameplayStatics::SetGlobalTimeDilation(this, 1.f);

  if (DeathMontage != nullptr)
  {
    UGameplayStatics::PlaySoundAtLocation(
      this,
      DeathSound,
      GetActorLocation(),
      GetActorRotation()
    );

    ReleaseWeapon();
    const float MontageLength = PlayAnimMontage(DeathMontage);

    FTimerHandle MontageTimer;
    GetWorld()->GetTimerManager().SetTimer(
      MontageTimer,
      this,
      &AAuraHero::DeathMontageEndRagdoll,
      MontageLength,
      false
    );
  }
  else
  {
    Super::Die();
  }

  FTimerHandle BackToHomeTimer;
  GetWorld()->GetTimerManager().SetTimer(
      BackToHomeTimer,
      this,
      &AAuraHero::BackToHome,
      BackToHomeDelay,
      false
      );
}

void AAuraHero::BackToHome()
{
  const TSoftObjectPtr<UWorld> HomeLevel = UAuraSystemsLibrary::GetRegionInfo(this)->GetHomeLevel();
  UGameplayStatics::OpenLevelBySoftObjectPtr(this, HomeLevel);
}

AAuraPlayerState* AAuraHero::GetAuraPlayerState() const
{
  AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
  check(AuraPlayerState);

  return AuraPlayerState;
}

AAuraPlayerController* AAuraHero::GetAuraPlayerController()
{
  if (AuraPlayerController == nullptr)
  {
    AuraPlayerController = Cast<AAuraPlayerController>(GetController());
  }

  return AuraPlayerController.Get();
}

void AAuraHero::BeginPlay()
{
  Super::BeginPlay();

  FActorSpawnParameters SpawnParameters;
  SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
  ActiveCamera = GetWorld()->SpawnActor<AAuraCamera>(
    CameraClass,
    GetActorLocation(),
    GetActorRotation(),
    SpawnParameters
  );

  if (ActiveCamera)
  {
    ActiveCamera->SetPlayerActor(this);
    ActiveCamera->SetCameraState(ECameraState::Default);
  }
}

void AAuraHero::InitAbilityActorInfo()
{
  AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
  check(AuraPlayerState);

  AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
  OnASCRegistered.Broadcast(AbilitySystemComponent);
  AttributeSet = AuraPlayerState->GetAttributeSet();
  AttributeSet->AssignPrimeAttribute(PrimeAttributeTag);

  AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);

  GetAuraASC()->AbilityActorInfoSet();

  if (AAuraHUD* AuraHUD = GetAuraPlayerController()->GetHUD<AAuraHUD>())
  {
    AuraHUD->InitOverlay(AuraPlayerController.Get(),
                         AuraPlayerState,
                         AbilitySystemComponent,
                         AttributeSet);
  }

  InitializeDefaultAttributes();
}
