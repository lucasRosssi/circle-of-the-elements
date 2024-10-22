// Copyright Lucas Rossi


#include "Managers/CameraManager.h"

#include "Components/AuraCamera.h"
#include "Kismet/GameplayStatics.h"

ACameraManager::ACameraManager()
{
  
}

AAuraCamera* ACameraManager::GetActiveCamera()
{
  if (ActiveCamera == nullptr)
  {
    ActiveCamera = Cast<AAuraCamera>(UGameplayStatics::GetActorOfClass(this, AAuraCamera::StaticClass()));
  }
  
  return ActiveCamera;
}
