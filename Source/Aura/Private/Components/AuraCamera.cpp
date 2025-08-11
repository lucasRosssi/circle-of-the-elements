// Copyright Lucas Rossi


#include "Components/AuraCamera.h"

#include "Aura/Aura.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/OverlapResult.h"
#include "Enums/CameraState.h"
#include "Interfaces/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/CombatManager.h"
#include "Managers/LocationManager.h"
#include "Utils/AuraSystemsLibrary.h"

AAuraCamera::AAuraCamera()
{
  MovementSpeed = 600.f;
  OffsetSpeed = 0.f;
  RotateSpeed = 0.f;
  ZoomSpeed = 0.f;
  MovementLagSpeed = 12.f;
  IgnoreTimeDilation = true;
  ConstrainLocationToShape = true;
  bFollowTargetIgnoreRestrictions = false;

  SpringArmComponent->SetUsingAbsoluteRotation(true);
  SpringArmComponent->bDoCollisionTest = false;
  SpringArmComponent->TargetArmLength = 4500.f;
  Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
  Camera->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
  Camera->bUsePawnControlRotation = false;
  Camera->SetFieldOfView(DefaultFOV);

  SpotLight = CreateDefaultSubobject<USpotLightComponent>("SpotLight");
  SpotLight->SetupAttachment(GetRootComponent());
  SpotLight->Intensity = 50000.f;
  SpotLight->AttenuationRadius = 3000.f;
  SpotLight->SetUseTemperature(true);
  SpotLight->Temperature = 5000.f;
  SpotLight->CastShadows = false;
  SpotLight->SetVisibility(false);
}

void AAuraCamera::SetCameraBoundaries(const TArray<AActor*>& BoundaryActors)
{
  SetConstrainDots(GetActorsLocation2D(BoundaryActors));
}

void AAuraCamera::SetCameraBoundaries()
{
  const TArray<AActor*> CameraBoundaries = UAuraSystemsLibrary::GetLocationManager(this)
    ->GetCameraBoundaryActors();
  SetConstrainDots(GetActorsLocation2D(CameraBoundaries));
}

void AAuraCamera::RemoveCameraBoundaries()
{
  SetConstrainDots(TArray<FVector2d>());
}

APostProcessVolume* AAuraCamera::GetPostProcessVolume()
{
  if (PostProcessVolume == nullptr)
  {
    PostProcessVolume = Cast<APostProcessVolume>(
      UGameplayStatics::GetActorOfClass(this, APostProcessVolume::StaticClass())
    );
  }

  return PostProcessVolume;
}

void AAuraCamera::SetCameraState(ECameraState State)
{
  CameraState = State;

  switch (CameraState)
  {
  case ECameraState::Default:
    {
      HandleDefaultState();
      break;
    }
  case ECameraState::PlayerDeath:
    {
      HandlePlayerDeathState();
      break;
    }
  case ECameraState::BossDeath:
    {
      HandleBossDeathState();
      break;
    }
  case ECameraState::SecondWind:
    {
      HandleSecondWindState();
      break;
    }
  default:
    {
    }
  }
}

void AAuraCamera::BeginPlay()
{
  Super::BeginPlay();

  GetPostProcessVolume();

  ULocationManager* LocationManager = UAuraSystemsLibrary::GetLocationManager(this);
  if (LocationManager)
  {
    LocationManager->OnInitAreaDelegate.AddDynamic(this, &AAuraCamera::OnLocationUpdate);
    LocationManager->OnExitAreaDelegate.AddDynamic(this, &AAuraCamera::OnExitLocation);
  }
}

void AAuraCamera::Tick(float DeltaSeconds)
{
  Super::Tick(DeltaSeconds);

  switch (CameraState)
  {
  case ECameraState::Default:
    {
      InterpToDefaultFOV(DeltaSeconds);
      break;
    }
  case ECameraState::PlayerDeath:
  case ECameraState::BossDeath:
    {
      InterpToDeathFOV(DeltaSeconds);
      break;
    }
  case ECameraState::SecondWind:
    {
      InterpToSecondWindFOV(DeltaSeconds);
      break;
    }
  default:
    {
    }
  }
}

void AAuraCamera::OnLocationUpdate()
{
  SetCameraBoundaries();
  MovementLagSpeed = 12.f;
}

void AAuraCamera::OnExitLocation()
{
  RemoveCameraBoundaries();
  MovementLagSpeed = 0.f;
}

void AAuraCamera::HandleDefaultState()
{
  bFollowTargetIgnoreRestrictions = false;

  FollowTarget(
    AUniversalCamera::UseActor(PlayerActor.Get()),
    FConstrainVector2(),
    FBoolRotation());

  if (GetPostProcessVolume()->Settings.WeightedBlendables.Array.Num() >= 3)
  {
    GetPostProcessVolume()->Settings.WeightedBlendables.Array[0].Weight = 1.f;
    GetPostProcessVolume()->Settings.WeightedBlendables.Array[1].Weight = 0.f;
    GetPostProcessVolume()->Settings.WeightedBlendables.Array[2].Weight = 0.f;
  }

  ICombatInterface::Execute_SetCustomDepth(PlayerActor.Get(), 0);

  SpotLight->SetVisibility(false);
}

void AAuraCamera::HandlePlayerDeathState()
{
  bFollowTargetIgnoreRestrictions = true;

  FCollisionQueryParams SphereParams;
  SphereParams.AddIgnoredActor(this);
  SphereParams.AddIgnoredActor(FollowTargetSettings.TargetSettings.Actor);

  TArray<FOverlapResult> Overlaps;
  if (const UWorld* World = GetWorld())
  {
    World->OverlapMultiByObjectType(
      Overlaps,
      GetActorLocation(),
      FQuat::Identity,
      FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllObjects),
      FCollisionShape::MakeSphere(500.f),
      SphereParams
    );
    for (auto& Overlap : Overlaps)
    {
      AActor* OverlappedActor = Overlap.GetActor();

      if (!IsValid(OverlappedActor)) continue;

      OverlappedActor->SetActorHiddenInGame(true);
    }
  }

  GetPostProcessVolume()->Settings.WeightedBlendables.Array[0].Weight = 0.f;
  GetPostProcessVolume()->Settings.WeightedBlendables.Array[1].Weight = 1.f;
  
  ICombatInterface::Execute_SetCustomDepth(PlayerActor.Get(), CUSTOM_DEPTH_FULL_RED);

  SpotLight->SetVisibility(true);
}

void AAuraCamera::HandleBossDeathState()
{
  AActor* Boss = UAuraSystemsLibrary::GetCombatManager(this)->GetCurrentBoss();
  if (!Boss) return;

  bFollowTargetIgnoreRestrictions = true;
  FollowTarget(
    AUniversalCamera::UseActor(Boss),
    FConstrainVector2(),
    FBoolRotation());

  GetPostProcessVolume()->Settings.WeightedBlendables.Array[0].Weight = 0.f;
  GetPostProcessVolume()->Settings.WeightedBlendables.Array[1].Weight = 1.f;

  ICombatInterface::Execute_SetCustomDepth(Boss, CUSTOM_DEPTH_FULL_RED);

  SpotLight->SetVisibility(true);
}

void AAuraCamera::HandleSecondWindState()
{
  bFollowTargetIgnoreRestrictions = true;

  GetPostProcessVolume()->Settings.WeightedBlendables.Array[2].Weight = 1.f;

  ICombatInterface::Execute_SetCustomDepth(PlayerActor.Get(), CUSTOM_DEPTH_DEFAULT_COLOR);

  SpotLight->SetVisibility(true);
}

void AAuraCamera::InterpToDefaultFOV(float DeltaSeconds)
{
  Camera->SetFieldOfView(FMath::FInterpTo(
    Camera->FieldOfView,
    DefaultFOV,
    DeltaSeconds,
    DefaultInterpSpeed
  ));
}

void AAuraCamera::InterpToDeathFOV(float DeltaSeconds)
{
  Camera->SetFieldOfView(FMath::FInterpTo(
    Camera->FieldOfView,
    DeathFOV,
    DeltaSeconds,
    DeathInterpSpeed
  ));
}

void AAuraCamera::InterpToSecondWindFOV(float DeltaSeconds)
{
  Camera->SetFieldOfView(FMath::FInterpTo(
    Camera->FieldOfView,
    SecondWindFOV,
    DeltaSeconds,
    SecondWindInterpSpeed
  ));
}
