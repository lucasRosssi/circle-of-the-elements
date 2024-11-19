// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Level/Tiles/Tile.h"
#include "TileBasic.generated.h"

class UArrowComponent;
/**
 * 
 */
UCLASS()
class AURA_API ATileBasic : public ATile
{
	GENERATED_BODY()

public:
  ATileBasic();

  virtual void SetExitAvailable(EDirection WorldDirection, bool bAvailable) override;
  
protected:
  virtual void BeginPlay() override;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<UStaticMeshComponent> FloorMesh;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tile|Decorator")
  TObjectPtr<UChildActorComponent> ChildDecoratorComponent;
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<UStaticMeshComponent> UnderWallMesh1;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<UStaticMeshComponent> UnderWallMesh2;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<UStaticMeshComponent> UnderWallMesh3;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<UStaticMeshComponent> UnderWallMesh4;
  
#if WITH_EDITORONLY_DATA
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<UArrowComponent> NorthArrow;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<UArrowComponent> EastArrow;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<UArrowComponent> SouthArrow;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<UArrowComponent> WestArrow;
#endif
  
private:
  ADecorator* GetDecorator();

  UPROPERTY()
  ADecorator* Decorator = nullptr;
  
#if WITH_EDITOR
  TMap<EDirection, UArrowComponent*> Arrows;
#endif
  
};
