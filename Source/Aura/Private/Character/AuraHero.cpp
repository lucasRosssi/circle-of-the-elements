// Copyright Lucas Rossi


#include "Character/AuraHero.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "Components/AuraCamera.h"
#include "Components/TeamComponent.h"
#include "Components/WidgetComponent.h"
#include "Enums/CameraState.h"
#include "Game/AuraSaveGame.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/AbilityManager.h"
#include "Managers/CombatManager.h"
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
  InitializeAttributes();
  InitializeUpgrades();
  InitializeAbilities();

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

void AAuraHero::InitializeAbilities()
{
  Super::InitializeAbilities();
  
  if (UAbilityManager* AbilityManager = UAuraSystemsLibrary::GetAbilityManager(this))
  {
    AbilityManager->GiveAcquiredAbilities(this);
  }
}

void AAuraHero::InitializeUpgrades()
{
  if (UUpgradeManager* UpgradeManager = UAuraSystemsLibrary::GetUpgradeManager(this))
  {
    UpgradeManager->GiveAcquiredUpgrades();
  }
}

void AAuraHero::Die(const FVector& DeathImpulse)
{
  bDying = true;

  if (GetSaveGame())
  {
    SaveGame->OnPlayerDeath();
    UAuraSystemsLibrary::SaveCurrentGame(this);
  }

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

void AAuraHero::AddToXP_Implementation(int32 InXP)
{
  GetAuraPlayerState()->AddXP(InXP);
}

int32 AAuraHero::GetAttributePoints_Implementation()
{
  return GetAuraPlayerState()->GetAttributePoints();
}

int32 AAuraHero::GetSkillPoints_Implementation()
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

void AAuraHero::AddResource_Implementation(const FGameplayTag& ResourceTag, int32 Amount)
{
  GetAuraPlayerState()->AddPlayerResource(ResourceTag, Amount);
}

void AAuraHero::SpendAttributePointsRandomly_Implementation()
{
  if (GetAuraASC())
  {
    const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
    const TArray AttributeTags = TArray({
      AuraTags.Attributes_Primary_Strength,
      AuraTags.Attributes_Primary_Dexterity,
      AuraTags.Attributes_Primary_Constitution,
      AuraTags.Attributes_Primary_Intelligence,
      AuraTags.Attributes_Primary_Wisdom,
      AuraTags.Attributes_Primary_Charisma
    });

    const int32 AttributePoints = GetAuraPlayerState()->GetAttributePoints();
    for (int32 i = 0; i < AttributePoints; i++)
    {
      const int32 RandomAttributeIndex = FMath::RandRange(0, 5);

      GetAuraASC()->UpgradeAttribute(AttributeTags[RandomAttributeIndex]);
    }
  }
}

void AAuraHero::AddAttribute_Implementation(const FGameplayTag& AttributeTag, int32 Amount)
{
  GetAuraASC()->AddAttribute(AttributeTag, Amount);
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
  UAuraSystemsLibrary::BackToHome(this);
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

  if (UCombatManager* CombatManager = UAuraSystemsLibrary::GetCombatManager(this))
  {
    CombatManager->OnCombatFinishedDelegate.AddUniqueDynamic(this, &AAuraHero::SaveCurrentHealth);
  }
}

void AAuraHero::InitializeAttributes()
{
  const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();

  FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
  EffectContextHandle.AddSourceObject(this);

  const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
    DefaultPrimaryAttributes,
    1.f,
    EffectContextHandle
    );

  float InitialStrength = 10.f;
  float InitialDexterity = 10.f;
  float InitialConstitution = 10.f;
  float InitialIntelligence = 10.f;
  float InitialWisdom = 10.f;
  float InitialCharisma = 10.f;

  if (GetSaveGame())
  {
    InitialStrength = SaveGame->AttributeSet.Attributes[AuraTags.Attributes_Primary_Strength];
    InitialDexterity = SaveGame->AttributeSet.Attributes[AuraTags.Attributes_Primary_Dexterity];
    InitialConstitution = SaveGame->AttributeSet.Attributes[AuraTags.Attributes_Primary_Constitution];
    InitialIntelligence = SaveGame->AttributeSet.Attributes[AuraTags.Attributes_Primary_Intelligence];
    InitialWisdom = SaveGame->AttributeSet.Attributes[AuraTags.Attributes_Primary_Wisdom];
    InitialCharisma = SaveGame->AttributeSet.Attributes[AuraTags.Attributes_Primary_Charisma];
  }
  
  UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
    SpecHandle,
    AuraTags.Attributes_Primary_Strength,
    InitialStrength
    );
  UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
    SpecHandle,
    AuraTags.Attributes_Primary_Dexterity,
    InitialDexterity
    );
  UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
    SpecHandle,
    AuraTags.Attributes_Primary_Constitution,
    InitialConstitution
    );
  UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
    SpecHandle,
    AuraTags.Attributes_Primary_Intelligence,
    InitialIntelligence
    );
  UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
    SpecHandle,
    AuraTags.Attributes_Primary_Wisdom,
    InitialWisdom
    );
  UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
    SpecHandle,
    AuraTags.Attributes_Primary_Charisma,
    InitialCharisma
    );
  
  AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
  
  Super::InitializeAttributes();

  // if (GetSaveGame())
  // {
  //   GetAttributeSet()->SetHealth(SaveGame->HeroHealth);
  // }
}

void AAuraHero::InitAbilityActorInfo()
{
  AbilitySystemComponent = GetAuraPlayerState()->GetAbilitySystemComponent();
  OnASCRegistered.Broadcast(AbilitySystemComponent);
  AttributeSet = AuraPlayerState->GetAttributeSet();
  AttributeSet->AssignPrimeAttribute(PrimeAttributeTag);

  AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);

  GetAuraASC()->AbilityActorInfoSet();
  AuraPlayerState->InitializeState();
  
  if (AAuraHUD* AuraHUD = GetAuraPlayerController()->GetHUD<AAuraHUD>())
  {
    AuraHUD->InitOverlay(
      AuraPlayerController.Get(),
      AuraPlayerState,
      AbilitySystemComponent,
      AttributeSet);
  }
}

void AAuraHero::SaveCurrentHealth()
{
  if (GetSaveGame())
  {
    SaveGame->HeroHealth = AttributeSet->GetHealth();
  }
}

AAuraPlayerState* AAuraHero::GetAuraPlayerState()
{
  if (!IsValid(AuraPlayerState))
  {
    AuraPlayerState = GetPlayerState<AAuraPlayerState>();
  }

  return AuraPlayerState;
}

UAuraSaveGame* AAuraHero::GetSaveGame()
{
  if (SaveGame == nullptr)
  {
    SaveGame = UAuraSystemsLibrary::GetCurrentSaveGameObject(this);
  }

  return SaveGame;
}
