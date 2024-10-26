// Copyright Lucas Rossi


#include "UI/Widget/BaseWidget.h"

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
