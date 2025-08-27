// Copyright Lucas Rossi


#include "Managers/LocationManager.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/GameplayEffects/AuraStatusEffect.h"
#include "Actor/ElementalProp.h"
#include "Actor/Level/Gate.h"
#include "Algo/RandomShuffle.h"
#include "Aura/Aura.h"
#include "Aura/AuraMacros.h"
#include "Character/AuraEnemy.h"
#include "Character/AuraHero.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Data/RegionInfo.h"
#include "Enums/CardinalDirection.h"
#include "Managers/CombatManager.h"
#include "Managers/RewardManager.h"
#include "Managers/UIManager.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Utils/AuraSystemsLibrary.h"
#include "Utils/UtilityLibrary.h"

void ULocationManager::GenerateLocation()
{
  UE_LOG(LogAura, Display, TEXT("[LocationManager] Starting location generation..."))
  
  TotalAreas = FMath::RandRange(MinAreas, MaxAreas);
  UE_LOG(LogAura, Display, TEXT("[LocationManager] Location will have %d areas."), TotalAreas)
  
  const TMap<EAreaType, FAreaTypeData>& AreaTypesData = GetRegionInfo()->GetAreaTypesData();

  TMap<EAreaType, int32> GeneratedAreasCount;
  TArray<FAreaData> CachedAreas;
  
  FIntPoint NextCoordinate = FIntPoint(0, 0);
  PlaceAreaAtCoordinate(NextCoordinate, GetEntranceFromPool());
  UE_LOG(LogAura, Display, TEXT("[LocationManager] Generated Entrance at [%d,%d]"), NextCoordinate.X, NextCoordinate.Y)
  
  UE_LOG(LogAura, Display, TEXT("[LocationManager] Randomizing area types..."))
  for (int32 i = 2; i < TotalAreas - 1; i++)
  {
    EAreaType ChosenType = PickRandomAreaType(AreaTypesData, GeneratedAreasCount);
    CachedAreas.Add(GetAreaFromPoolByType(ChosenType));

    GeneratedAreasCount.FindOrAdd(ChosenType)++;
  }
  
  Algo::RandomShuffle(CachedAreas);

  UE_LOG(LogAura, Display, TEXT("[LocationManager] Generating areas..."))
  for (int32 i = 0; i < CachedAreas.Num(); i++)
  {
    NextCoordinate = GetNextCoordinate();
    PlaceAreaAtCoordinate(NextCoordinate, CachedAreas[i]);
    UE_LOG(
      LogAura,
      Display,
      TEXT("[LocationManager] Generated %s at [%d,%d]"),
      *UUtilityLibrary::EnumToString(CachedAreas[i].Type),
      CurrentCoordinate.X,
      CurrentCoordinate.Y
    )
  }

  NextCoordinate = GetNextCoordinate();
  PlaceAreaAtCoordinate(NextCoordinate, GetBossArenaFromPool());
  UE_LOG(LogAura, Display, TEXT("[LocationManager] Generated BossArena at [%d,%d]"), NextCoordinate.X, NextCoordinate.Y)
  
  NextCoordinate = GetNextCoordinate();
  PlaceAreaAtCoordinate(NextCoordinate, GetExitFromPool());
  UE_LOG(LogAura, Display, TEXT("[LocationManager] Generated Exit at [%d,%d]"), NextCoordinate.X, NextCoordinate.Y)

  UE_LOG(LogAura, Display, TEXT("[LocationManager] Location successfully generated!"));
}

EAreaType ULocationManager::PickRandomAreaType(
  const TMap<EAreaType, FAreaTypeData>& AreaTypesData,
  const TMap<EAreaType, int32>& PlacedCount
) const
{
  struct FWeightedType { EAreaType Type; float Weight; };
  TArray<FWeightedType> WeightedList;
  float TotalWeight = 0.f;

  for (const auto& Pair : AreaTypesData)
  {
    const EAreaType Type = Pair.Key;
    const FAreaTypeData& Data = Pair.Value;

    // Skip fixed types
    if (Type == EAreaType::Entrance || Type == EAreaType::BossArena || Type == EAreaType::Exit)
      continue;

    if (Data.MinPerLocation > 0 && PlacedCount.FindRef(Type) < Data.MinPerLocation)
      return Type;

    // Skip if max already reached
    if (Data.MaxPerLocation > 0 && PlacedCount.FindRef(Type) >= Data.MaxPerLocation)
      continue;

    // Skip if no rarity
    if (Data.Rarity <= 0.f)
      continue;

    WeightedList.Add({Type, Data.Rarity});
    TotalWeight += Data.Rarity;
  }

  if (WeightedList.Num() == 0)
  {
    return EAreaType::DefaultArena;
  }

  float Roll = FMath::FRandRange(0.f, TotalWeight);
  for (const auto& Entry : WeightedList)
  {
    if (Roll < Entry.Weight)
      return Entry.Type;
    Roll -= Entry.Weight;
  }

  return WeightedList.Last().Type;
}

void ULocationManager::PlacePlayerInArea(const FAreaData& AreaData)
{
  TArray<AActor*> PlayerStarts;
  UGameplayStatics::GetAllActorsOfClass(
    this,
    APlayerStart::StaticClass(),
    PlayerStarts
  );

  if (PlayerStarts.IsEmpty()) return;

  const APlayerStart* Start = nullptr;
  const ECardinalDirection EnteringDirection = UUtilityLibrary::GetOppositeDirection(LastExit);
  const FString& DirectionString = UUtilityLibrary::GetDirectionString(EnteringDirection);
  for (AActor* StartActor : PlayerStarts)
  {
    Start = Cast<APlayerStart>(StartActor);

    if (!bStarted && AreaData.Type == EAreaType::Entrance)
    {
      if (Start->PlayerStartTag.ToString().Contains(FString("Start")))
      {
        bStarted = true;
        break;
      }
      
      continue;
    }

    if (
      // Start->GetLevel() &&
      // Start->GetLevel()->GetOuter()->GetName().Contains(AreaData.GetAreaName()) &&
      Start->PlayerStartTag.ToString().Contains(DirectionString)
      )
    {
      break;
    }
  }
  
  GetPlayerHero()->SetActorTransform(
    Start->GetActorTransform(),
    false,
    nullptr,
    ETeleportType::ResetPhysics
  );
}

void ULocationManager::ApplyAreaElementalFlowToCharacter(const AAuraCharacterBase* Character)
{
  UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
  
  ASC->RemoveActiveEffectsWithGrantedTags(
        FGameplayTagContainer({ FAuraGameplayTags::Get().StatusEffects_Buff_ElementalFlow })  
      );
  FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
  ContextHandle.AddSourceObject(Character);
      
  const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(
    GetCurrentAreaRef().ElementalFlowEffect,
    1,
    ContextHandle
  );
  UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
    SpecHandle,
    FAuraGameplayTags::Get().StatusEffects_Duration,
    0.f
  );
      
  ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void ULocationManager::OnEnemySpawned(AAuraEnemy* Enemy)
{
  if (!GetCurrentAreaRef().ElementalFlowEffect) return;
  
  ApplyAreaElementalFlowToCharacter(Enemy);
}

AAuraHero* ULocationManager::GetPlayerHero()
{
  if (!PlayerHero.IsValid())
  {
    APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
    PlayerHero = Cast<AAuraHero>(Player);
  }

  return PlayerHero.Get();
}

void ULocationManager::InitLocation()
{
  const FAreaData* AreaData = LocationLayout.Find(FIntPoint(0, 0));
  GUARD(AreaData,, TEXT("Entrance is invalid!"))
  
  LoadArea(*AreaData);
}

void ULocationManager::HandleDirectionalProps(const FAreaData& CurrentArea, const FName& PropTag)
{
  TArray<AActor*> DirectionClosedProps;
  UGameplayStatics::GetAllActorsWithTag(GetOwner(), PropTag, DirectionClosedProps);
  for (AActor* Actor : DirectionClosedProps)
  {
    bool bShouldDestroy = false;
    for (const ECardinalDirection Direction : CurrentArea.OpenDirections)
    {
      FString DirectionString = UUtilityLibrary::GetDirectionString(Direction);
      if (PropTag == DIRECTION_CLOSED_PROP_TAG)
      {
        bShouldDestroy = false;
        if (Actor->Tags.Contains(DirectionString))
        {
          bShouldDestroy = true;
          break;
        }
      }
      else if (PropTag == DIRECTION_OPEN_PROP_TAG)
      {
        bShouldDestroy = true;
        if (Actor->Tags.Contains(DirectionString))
        {
          bShouldDestroy = false;
          break;
        }
      }
    }

    if (bShouldDestroy)
    {
      Actor->Destroy();
    }
  }
}

void ULocationManager::HandleGates(const FAreaData& CurrentArea)
{
  TArray<AActor*> GateActors;
  UGameplayStatics::GetAllActorsOfClass(GetOwner(), AGate::StaticClass(), GateActors);
  for (AActor* GateActor : GateActors)
  {
    AGate* Gate = Cast<AGate>(GateActor);

    if (CurrentArea.OpenDirections.Contains(Gate->GetDirection()))
    {
      Gate->SetIsActive(true);
      if (CurrentArea.IsArena())
      {
        if (CurrentArea.bCombatFinished) Gate->Enable();
      }
      else
      {
        Gate->Enable();
      }
    }
    else
    {
      Gate->SetIsActive(false);
    }
  }
}

void ULocationManager::InitArea()
{
  FAreaData& CurrentArea = GetCurrentAreaRef();
  
  HandleGates(CurrentArea);
  HandleDirectionalProps(CurrentArea, DIRECTION_CLOSED_PROP_TAG);
  HandleDirectionalProps(CurrentArea, DIRECTION_OPEN_PROP_TAG);
  if (CurrentArea.ElementTag.IsValid())
  {
    HandleElementalProps(CurrentArea);
  }
  if (CurrentArea.ElementalFlowEffect)
  {
    ApplyAreaElementalFlowToCharacter(GetPlayerHero());
    if (GetCombatManager())
    {
      CombatManager->OnEnemySpawnedDelegate.AddUniqueDynamic(this, &ULocationManager::OnEnemySpawned);
    }
    
  }
  PlacePlayerInArea(CurrentArea);
  CurrentArea.bVisited = true;
  
  OnInitAreaDelegate.Broadcast();
}

void ULocationManager::ExitArea(ECardinalDirection ExitDirection)
{
  OnExitAreaDelegate.Broadcast();
  
  LastExit = ExitDirection;
  CameraBoundaryActors.Empty();
  UAuraSystemsLibrary::GetUIManager(GetOwner(), 0)
    ->GetOverlayWidgetController()
    ->StartTransition();

  const FIntPoint& NextAreaCoordinate = UUtilityLibrary::GetCoordinateOffsetFromDirection(ExitDirection) + PlayerCoordinate;
  const FAreaData* NextAreaDataPtr = LocationLayout.Find(NextAreaCoordinate);

  GUARD(NextAreaDataPtr,, TEXT("Could not find next area at [%d,%d]!"), NextAreaCoordinate.X, NextAreaCoordinate.Y)

  NextAreaData = *NextAreaDataPtr;

  UnloadArea(GetCurrentAreaRef());
  // LoadArea(NextAreaData);
}

int32 ULocationManager::GetCurrentRegionRecommendedLevel()
{
  if (Region == ERegion::Undefined) return 1;
  
  if (!GetRegionInfo()) return 1;

  const FRegionData* RegionData = RegionInfo->GetRegionData(Region);
  if (!RegionData) return 1;

  return RegionInfo->GetRegionData(Region)->RecommendedLevel;
}

TArray<FIntPoint> ULocationManager::GetConnectedCoordinates(const FIntPoint& Coordinate) const
{
  const FAreaData* Data = LocationLayout.Find(Coordinate);

  if (!Data) return TArray<FIntPoint>();

  TArray<FIntPoint> ConnectedCoordinates;
  for (const ECardinalDirection Direction : Data->OpenDirections)
  {
    const FIntPoint& ConnectedCoordinate = Coordinate + UUtilityLibrary::GetCoordinateOffsetFromDirection(Direction);
    if (LocationLayout.Contains(ConnectedCoordinate))
    {
      ConnectedCoordinates.Add(ConnectedCoordinate);
    }
  }

  return ConnectedCoordinates;
}

void ULocationManager::GetXCoordinateRange(int32& OutMin, int32& OutMax) const
{
  TArray<FIntPoint> Coordinates;
  LocationLayout.GenerateKeyArray(Coordinates);

  for (const FIntPoint& Coordinate : Coordinates)
  {
    OutMin = FMath::Min(OutMin, Coordinate.X);
    OutMax = FMath::Max(OutMax, Coordinate.X);
  }
}

void ULocationManager::GetYCoordinateRange(int32& OutMin, int32& OutMax) const
{
  TArray<FIntPoint> Coordinates;
  LocationLayout.GenerateKeyArray(Coordinates);
  
  for (const FIntPoint& Coordinate : Coordinates)
  {
    OutMin = FMath::Min(OutMin, Coordinate.Y);
    OutMax = FMath::Max(OutMax, Coordinate.Y);
  }
}

void ULocationManager::BeginPlay()
{
  Super::BeginPlay();

  if (bPCGActive)
  {
    GenerateLocation();
  }

  InitLocation();
}

FAreaData ULocationManager::GetAreaFromPool(TArray<FAreaData>& Pool, const TArray<FAreaData>& Source)
{
  if (Pool.IsEmpty())
  {
    Pool = Source;
    Algo::RandomShuffle(Pool);
  }

  GUARD(!Pool.IsEmpty(), FAreaData(), TEXT("Pool is empty!"))

  return Pool.Pop();
}

FAreaData ULocationManager::GetAreaFromPoolByType(EAreaType AreaType)
{
  switch (AreaType)
  {
  case EAreaType::Entrance:
    return GetEntranceFromPool();

  case EAreaType::DefaultArena:
    return GetDefaultArenaFromPool();

  case EAreaType::SpiritArena:
    return GetSpiritArenaFromPool();

  case EAreaType::BossArena:
    return GetBossArenaFromPool();

  case EAreaType::RewardArea:
    return GetRewardAreaFromPool();

  case EAreaType::SpecialArea:
    return GetSpecialAreaFromPool();

  case EAreaType::Exit:
    return GetExitFromPool();

  default:
    return GetDefaultArenaFromPool();
  }
}

FAreaData ULocationManager::GetEntranceFromPool()
{
  FAreaData Data = GetAreaFromPool(EntrancesPool, GetRegionInfo()->GetEntrances(Region));
  Data.Type = EAreaType::Entrance;
  return Data;
}

FAreaData ULocationManager::GetDefaultArenaFromPool()
{
  FAreaData Data = GetAreaFromPool(DefaultArenasPool, GetRegionInfo()->GetDefaultArenas(Region));
  Data.Type = EAreaType::DefaultArena;
  return Data;
}

FAreaData ULocationManager::GetSpiritArenaFromPool()
{
  FAreaData Data = GetAreaFromPool(SpiritArenasPool, GetRegionInfo()->GetSpiritArenas(Region));
  Data.Type = EAreaType::SpiritArena;
  return Data;
}

FAreaData ULocationManager::GetBossArenaFromPool()
{
  FAreaData Data = GetAreaFromPool(BossArenasPool, GetRegionInfo()->GetBossArenas(Region));
  Data.Type = EAreaType::BossArena;
  return Data;
}

FAreaData ULocationManager::GetRewardAreaFromPool()
{
  FAreaData Data = GetAreaFromPool(RewardAreasPool, GetRegionInfo()->GetRewardAreas(Region));
  Data.Type = EAreaType::RewardArea;
  return Data;
}

FAreaData ULocationManager::GetSpecialAreaFromPool()
{
  FAreaData Data = GetAreaFromPool(SpecialAreasPool, GetRegionInfo()->GetSpecialAreas(Region));
  Data.Type = EAreaType::SpecialArea;
  return Data;
}

FAreaData ULocationManager::GetExitFromPool()
{
  FAreaData Data = GetAreaFromPool(ExitsPool, GetRegionInfo()->GetExits(Region));
  Data.Type = EAreaType::Exit;
  return Data;
}

void ULocationManager::HandleArenaGeneration(const FAreaData& AreaData)
{
  GUARD(GetCombatManager(), , TEXT("CombatManager is invalid!"))

  int32& ArenaLevel = AreaData.IsSpiritArena() ? GeneratedSpiritArenaLevel : GeneratedArenaLevel;
  
  CombatManager->GenerateArenaCombat(AreaData, ArenaLevel);
  ArenaLevel++;
}

void ULocationManager::HandleElementalProps(const FAreaData& AreaData)
{
  TArray<AActor*> ElementalPropActors;
  UGameplayStatics::GetAllActorsOfClass(GetOwner(), AElementalProp::StaticClass(), ElementalPropActors);

  for (AActor* Actor : ElementalPropActors)
  {
    AElementalProp* Prop = Cast<AElementalProp>(Actor);
    Prop->SetElementTag(AreaData.ElementTag);
  }
}

void ULocationManager::LoadArea(const FAreaData& AreaData)
{
  GUARD(!AreaData.World.IsNull(),, TEXT("World level asset path is invalid!"))

  PrevPlayerCoordinate = PlayerCoordinate;
  PlayerCoordinate = AreaData.Coordinate;
  
  FLatentActionInfo LatentInfo;
  LatentInfo.CallbackTarget = this;
  LatentInfo.ExecutionFunction = FName("OnAreaLoaded");
  LatentInfo.Linkage = 0;
  LatentInfo.UUID = __LINE__;

  UGameplayStatics::LoadStreamLevelBySoftObjectPtr(
    this,
    AreaData.World,
    true,
    false,
    LatentInfo
  );
}

void ULocationManager::OnAreaLoaded()
{
  InitArea();

  UAuraSystemsLibrary::GetUIManager(GetOwner(), 0)
    ->GetOverlayWidgetController()
    ->EndTransition();
  
  // if (PrevPlayerCoordinate == PlayerCoordinate) return;
  //
  // const FAreaData* AreaData = LocationLayout.Find(PrevPlayerCoordinate);
  // if (AreaData && AreaData->World != GetCurrentAreaRef().World) UnloadArea(*AreaData);

}

void ULocationManager::UnloadArea(const FAreaData& AreaData)
{
  GUARD(!AreaData.World.IsNull(),, TEXT("World level asset path is invalid!"))
  
  FLatentActionInfo LatentInfo;
  LatentInfo.CallbackTarget = this;
  LatentInfo.ExecutionFunction = FName("OnAreaUnloaded");
  LatentInfo.Linkage = 0;
  LatentInfo.UUID = __LINE__;

  UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(
      this,
      AreaData.World,
      LatentInfo,
      false
  );
}

void ULocationManager::OnAreaUnloaded()
{
  LoadArea(NextAreaData);
}

bool ULocationManager::IsCoordinateFree(const FIntPoint& Coordinate) const
{
  return !LocationLayout.Contains(Coordinate);
}

FIntPoint ULocationManager::Backtrack()
{
  UE_LOG(LogAura, Display, TEXT("[LocationManager] Backtracking..."))
  
  TArray<FIntPoint> PrevCoordinates;
  LocationLayout.GenerateKeyArray(PrevCoordinates);
  PrevCoordinates.Remove(CurrentCoordinate);

  Algo::RandomShuffle(PrevCoordinates);

  FIntPoint FreeCoordinate;
  for (const FIntPoint& BacktrackCoordinate : PrevCoordinates)
  {
    FreeCoordinate = GetFreeAdjacentCoordinate(BacktrackCoordinate);
    if (FreeCoordinate != BacktrackCoordinate)
    {
      CurrentCoordinate = BacktrackCoordinate;
      break;
    }
  }

  return FreeCoordinate;
}

FIntPoint ULocationManager::GetNextCoordinate()
{
  FIntPoint NextCoordinate = GetFreeAdjacentCoordinate(CurrentCoordinate);

  const int32 AreaCount = LocationLayout.Num();
  const float BacktrackRoll = FMath::FRandRange(0.f, 1.f);
  const bool bCanBacktrack =
    bBacktrackEnabled &&
    AreaCount > MinAreaCountToBacktrack &&
    AreaCount < MaxAreaCountToBacktrack &&
    AreaCount < TotalAreas - 1; // The last 2 areas should be connected (boss and exit)
  
  if (NextCoordinate == CurrentCoordinate || (bCanBacktrack && BacktrackRoll <= BacktrackChance))
  {
    NextCoordinate = Backtrack();
  }

  return NextCoordinate;
}

FIntPoint ULocationManager::GetFreeAdjacentCoordinate(const FIntPoint& Coordinate) const
{
  TArray<FIntPoint> FreeCoordinates = UUtilityLibrary::GetAdjacentCoordinates(Coordinate)
      .FilterByPredicate([this](const FIntPoint& AdjCoordinate)
      {
        return IsCoordinateFree(AdjCoordinate);
      });

  if (FreeCoordinates.IsEmpty())
  {
    return Coordinate;
  }

  return FreeCoordinates[FMath::RandRange(0, FreeCoordinates.Num() - 1)];
}

void ULocationManager::ConnectAreas(FAreaData& FromArea, FAreaData& ToArea, ECardinalDirection Direction)
{
  FromArea.OpenDirections.Add(Direction);

  const ECardinalDirection OppositeDir = UUtilityLibrary::GetOppositeDirection(Direction);

  ToArea.OpenDirections.Add(OppositeDir);
}

void ULocationManager::AssignSpiritArenaElement(FAreaData& NewAreaData)
{
  URewardManager* RewardManager = UAuraSystemsLibrary::GetRewardManager(GetOwner());
  if (RewardManager)
  {
    const FGameplayTag& EssenceTag = RewardManager->GetNextRewardInBag();
    NewAreaData.ElementTag = FAuraGameplayTags::Get().EssenceToAbility[EssenceTag];
    
    const UAbilityInfo* AbilitiesInfo = UAuraSystemsLibrary::GetAbilitiesInfo(GetOwner());
    const FAbilityElementInfo& ElementInfo = AbilitiesInfo->FindElementInfo(NewAreaData.ElementTag);
    NewAreaData.ElementalFlowEffect = ElementInfo.ElementalFlowEffect;
  }
}

void ULocationManager::PlaceAreaAtCoordinate(const FIntPoint& Coordinate, const FAreaData& AreaData)
{
  FAreaData NewAreaData = AreaData;
  NewAreaData.Coordinate = Coordinate;
  if (AreaData.IsArena()) HandleArenaGeneration(NewAreaData);
  if (AreaData.IsSpiritArena()) AssignSpiritArenaElement(NewAreaData);
  
  PrevCoordinate = CurrentCoordinate;
  LocationLayout.Add(Coordinate, NewAreaData);
  CurrentCoordinate = Coordinate;

  if (CurrentCoordinate == FIntPoint(0, 0)) return;


  FAreaData* PrevArea = LocationLayout.Find(PrevCoordinate);
  FAreaData* CurrentArea = LocationLayout.Find(CurrentCoordinate);
  GUARD(
    PrevArea && CurrentArea,,
    TEXT("Something went wrong with location generation, PrevArea or CurrentArea are invalid!")
  )

  const ECardinalDirection ConnectDirection = UUtilityLibrary::GetDirectionFromCoordinateOffset(
    CurrentCoordinate - PrevCoordinate
  );

  ConnectAreas(*PrevArea, *CurrentArea, ConnectDirection);
}


URegionInfo* ULocationManager::GetRegionInfo()
{
  if (!RegionInfo.IsValid())
  {
    RegionInfo = UAuraSystemsLibrary::GetRegionInfo(GetOwner());
  }

  GUARD(RegionInfo.IsValid(), nullptr, TEXT("RegionInfo is invalid!"))
  return RegionInfo.Get();
}

UCombatManager* ULocationManager::GetCombatManager()
{
  if (!CombatManager.IsValid())
  {
    CombatManager = UAuraSystemsLibrary::GetCombatManager(GetOwner());
  }

  return CombatManager.Get();
}
