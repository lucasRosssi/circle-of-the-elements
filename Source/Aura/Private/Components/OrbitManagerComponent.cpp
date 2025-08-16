#include "Components/OrbitManagerComponent.h"
#include "Actor/SpiritActor.h"
#include "Managers/LocationManager.h"
#include "Utils/AuraSystemsLibrary.h"

UOrbitManagerComponent::UOrbitManagerComponent()
{
  PrimaryComponentTick.bCanEverTick = true;
}

void UOrbitManagerComponent::BeginPlay()
{
  Super::BeginPlay();

  ULocationManager* LocationManager = UAuraSystemsLibrary::GetLocationManager(GetOwner());
  if (!LocationManager) return;

  LocationManager->OnInitAreaDelegate.AddDynamic(this, &UOrbitManagerComponent::TeleportSpiritsToOwner);
}

void UOrbitManagerComponent::TeleportSpiritsToOwner()
{
  const AActor* Owner = GetOwner();
  if (!Owner || ActiveSpirits.Num() == 0) return;

  for (ASpiritActor* Spirit : ActiveSpirits)
  {
    Spirit->SetActorTransform(
      Owner->GetActorTransform(),
      false,
      nullptr,
      ETeleportType::ResetPhysics
    );
  }
}

void UOrbitManagerComponent::RegisterSpirit(ASpiritActor* Spirit)
{
  if (Spirit && !ActiveSpirits.Contains(Spirit))
  {
    ActiveSpirits.Add(Spirit);
    OrbitSpeed.Add(FMath::RandRange(MinOrbitSpeed, MaxOrbitSpeed));
    OrbitRadius.Add(FMath::RandRange(MinOrbitRadius, MaxOrbitRadius));
    OrbitHeightOffset.Add(FMath::RandRange(MinOrbitHeightOffset, MaxOrbitHeightOffset));
    InterpSpeed.Add(FMath::RandRange(MinInterpSpeed, MaxInterpSpeed));
    CurrentOrbitAngle.Add(0.f);
  }
}

void UOrbitManagerComponent::UnregisterSpirit(ASpiritActor* Spirit)
{
  if (Spirit)
  {
    const int32 Index = ActiveSpirits.Find(Spirit);
    if (Index != INDEX_NONE)
    {
      ActiveSpirits.RemoveAt(Index);
      OrbitSpeed.RemoveAt(Index);
      OrbitRadius.RemoveAt(Index);
      OrbitHeightOffset.RemoveAt(Index);
      InterpSpeed.RemoveAt(Index);
      CurrentOrbitAngle.RemoveAt(Index);
    }

    Spirit->Destroy();
  }
}

void UOrbitManagerComponent::TickComponent(
  float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction
)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  const AActor* Owner = GetOwner();
  if (!Owner || ActiveSpirits.Num() == 0) return;

  const FVector OwnerLocation = Owner->GetActorLocation();

  const float AngleStep = 360.f / ActiveSpirits.Num();

  for (int32 i = 0; i < ActiveSpirits.Num(); ++i)
  {
    ASpiritActor* Spirit = ActiveSpirits[i];
    if (!IsValid(Spirit)) continue;

    if (Spirit->IsUsingAbility())
    {
      FVector TargetLocation = Spirit->GetAbilityUseLocation();
      FVector SmoothedLocation = FMath::VInterpConstantTo(
        Spirit->GetActorLocation(),
        TargetLocation,
        DeltaTime,
        AbilityActivationInterpSpeed
      );
      Spirit->SetActorLocation(SmoothedLocation);
    }
    else if (Spirit->GetHijacker())
    {
      FVector TargetLocation = Spirit->GetHijackLocation();
      FVector SmoothedLocation = FMath::VInterpConstantTo(
        Spirit->GetActorLocation(),
        TargetLocation,
        DeltaTime,
        HijackInterpSpeed
      );
      Spirit->SetActorLocation(SmoothedLocation);
    }
    else
    {
      CurrentOrbitAngle[i] += OrbitSpeed[i] * DeltaTime;
      float AngleDegrees = CurrentOrbitAngle[i] + (i * AngleStep);
      const float AngleRadians = FMath::DegreesToRadians(AngleDegrees);

      FVector Offset;
      Offset.X = FMath::Cos(AngleRadians) * OrbitRadius[i];
      Offset.Y = FMath::Sin(AngleRadians) * OrbitRadius[i];
      Offset.Z = OrbitHeightOffset[i];

      FVector TargetLocation = OwnerLocation + Offset;
      FVector SmoothedLocation = FMath::VInterpTo(
        Spirit->GetActorLocation(),
        TargetLocation,
        DeltaTime,
        InterpSpeed[i]
      );
      Spirit->SetActorLocation(SmoothedLocation);
    }
  }
}
