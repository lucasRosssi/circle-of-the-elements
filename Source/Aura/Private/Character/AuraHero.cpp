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
#include "Components/OrbitManagerComponent.h"
#include "Components/TeamComponent.h"
#include "Components/WidgetComponent.h"
#include "Enums/CameraState.h"
#include "Game/AuraSaveGame.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/AbilityManager.h"
#include "Managers/UpgradeManager.h"
#include "Player/AuraPlayerState.h"
#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"
#include "Utils/AuraSystemsLibrary.h"

AAuraHero::AAuraHero()
{
  OrbitManager = CreateDefaultSubobject<UOrbitManagerComponent>(TEXT("OrbitManager"));
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
  RegisterElementalFlowEvents();

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

int32 AAuraHero::GetCharacterLevel_Implementation()
{
  if (GetAuraPlayerState())
  {
    return AuraPlayerState->GetCharacterLevel();
  }

  return 1;
}

TArray<USpirit*> AAuraHero::GetEquippedSpirits_Implementation()
{
  if (!GetAuraPlayerState()) return TArray<USpirit*>();  

  return AuraPlayerState->GetEquippedSpirits();
}

TArray<USpirit*> AAuraHero::GetAvailableEquippedSpirits_Implementation()
{
  if (!GetAuraPlayerState()) return TArray<USpirit*>();  

  const UAbilityInfo* AbilityDataAsset = UAuraSystemsLibrary::GetAbilitiesInfo(this);

  if (!AbilityDataAsset) return AuraPlayerState->GetEquippedSpirits();
  
  return AuraPlayerState->GetEquippedSpirits().FilterByPredicate([AbilityDataAsset, this](USpirit* Spirit)
  {
    const FAuraAbilityInfo& Info = AbilityDataAsset->FindAbilityInfoByTag(Spirit->GetAbilityTag());

    if (Info.ChargesTag.IsValid()) return GetAuraASC()->HasMatchingGameplayTag(Info.ChargesTag);
    if (Info.CooldownTag.IsValid()) return !GetAuraASC()->HasMatchingGameplayTag(Info.CooldownTag);

    return true;
  });
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

void AAuraHero::AddInteractableToList_Implementation(UInteractComponent* InteractableComponent)
{
  if (GetAuraPlayerController())
  {
    AuraPlayerController->AddInteractableInRange(InteractableComponent);
  }
}

void AAuraHero::RemoveInteractableFromList_Implementation(UInteractComponent* InteractableComponent)
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
      AuraTags.Attributes_Primary_Ferocity,
      AuraTags.Attributes_Primary_Agility,
      AuraTags.Attributes_Primary_Toughness,
      AuraTags.Attributes_Primary_Attunement,
      AuraTags.Attributes_Primary_Willpower,
      AuraTags.Attributes_Primary_Faith
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

  // if (UCombatManager* CombatManager = UAuraSystemsLibrary::GetCombatManager(this))
  // {
  //   CombatManager->OnCombatFinishedDelegate.AddUniqueDynamic(this, &AAuraHero::SaveCurrentHealth);
  // }
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

  float InitialStrength = 1.f;
  float InitialDexterity = 1.f;
  float InitialConstitution = 1.f;
  float InitialIntelligence = 1.f;
  float InitialWisdom = 1.f;
  float InitialCharisma = 1.f;

  if (GetSaveGame())
  {
    InitialStrength = SaveGame->AttributeSet.Attributes[AuraTags.Attributes_Primary_Ferocity];
    InitialDexterity = SaveGame->AttributeSet.Attributes[AuraTags.Attributes_Primary_Agility];
    InitialConstitution = SaveGame->AttributeSet.Attributes[AuraTags.Attributes_Primary_Toughness];
    InitialIntelligence = SaveGame->AttributeSet.Attributes[AuraTags.Attributes_Primary_Attunement];
    InitialWisdom = SaveGame->AttributeSet.Attributes[AuraTags.Attributes_Primary_Willpower];
    InitialCharisma = SaveGame->AttributeSet.Attributes[AuraTags.Attributes_Primary_Faith];
  }
  
  UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
    SpecHandle,
    AuraTags.Attributes_Primary_Ferocity,
    InitialStrength
    );
  UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
    SpecHandle,
    AuraTags.Attributes_Primary_Agility,
    InitialDexterity
    );
  UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
    SpecHandle,
    AuraTags.Attributes_Primary_Toughness,
    InitialConstitution
    );
  UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
    SpecHandle,
    AuraTags.Attributes_Primary_Attunement,
    InitialIntelligence
    );
  UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
    SpecHandle,
    AuraTags.Attributes_Primary_Willpower,
    InitialWisdom
    );
  UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
    SpecHandle,
    AuraTags.Attributes_Primary_Faith,
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

  AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);

  GetAuraASC()->AbilityActorInfoSet();
  AuraPlayerState->InitializeState();

  if (GetSaveGame())
  {
    const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
    AttributeSet->SetFerocity(SaveGame->AttributeSet.Attributes[AuraTags.Attributes_Primary_Ferocity]);
    AttributeSet->SetAgility(SaveGame->AttributeSet.Attributes[AuraTags.Attributes_Primary_Agility]);
    AttributeSet->SetToughness(SaveGame->AttributeSet.Attributes[AuraTags.Attributes_Primary_Toughness]);
    AttributeSet->SetAttunement(SaveGame->AttributeSet.Attributes[AuraTags.Attributes_Primary_Attunement]);
    AttributeSet->SetWillpower(SaveGame->AttributeSet.Attributes[AuraTags.Attributes_Primary_Willpower]);
    AttributeSet->SetFaith(SaveGame->AttributeSet.Attributes[AuraTags.Attributes_Primary_Faith]);
  }
  
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
