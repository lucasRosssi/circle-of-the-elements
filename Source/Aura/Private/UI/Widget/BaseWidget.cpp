// Copyright Lucas Rossi


#include "UI/Widget/BaseWidget.h"

#include "Aura/AuraLogChannels.h"
#include "Blueprint/WidgetTree.h"

void UBaseWidget::AssignWidgetController_Implementation(UObject* InWidgetController)
{
  SetWidgetController(InWidgetController);
}

void UBaseWidget::SetWidgetController(UObject* InWidgetController)
{
  WidgetController = InWidgetController;
  WidgetControllerSet();

  // Propagate widget controller to children
  WidgetTree->ForEachWidget(
    [&](UWidget* Widget)
    {
      if (Widget->Implements<UWidgetControllerInterface>())
      {
          Execute_AssignWidgetController(Widget, InWidgetController);
      }
    }
  );
}

UUINavWidget* UBaseWidget::GoToControlledWidget(
  TSubclassOf<UUINavWidget> NewWidgetClass,
  UObject* InWidgetController,
  const bool bRemoveParent,
  const bool bDestroyParent,
  const int ZOrder
)
{
  UUINavWidget* Widget = CreateControlledWidget(NewWidgetClass, InWidgetController);

  if (!Widget)
  {
    UE_LOG(
      LogAura,
      Error,
      TEXT("Failed to create widget from %s. New widget class: %s"),
      *GetName(),
      NewWidgetClass ? *NewWidgetClass->GetName() : TEXT("Undefined")
    );
    return nullptr;
  }

  return GoToBuiltWidget(Widget, bRemoveParent, bDestroyParent, ZOrder);
}

UUINavWidget* UBaseWidget::CreateControlledWidget(TSubclassOf<UUINavWidget> NewWidgetClass, UObject* InWidgetController)
{
  UUINavWidget* Widget = CreateWidget<UUINavWidget>(GetOwningPlayer(), NewWidgetClass);

  if (Widget->Implements<UWidgetControllerInterface>() && IsValid(InWidgetController))
  {
    Execute_AssignWidgetController(Widget, InWidgetController);
  }

  return Widget;
}
