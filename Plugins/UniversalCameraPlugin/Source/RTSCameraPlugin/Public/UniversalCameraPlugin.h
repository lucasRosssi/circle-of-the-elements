// Universal Camera Plugin - Mathieu Jacq 2021

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FUniversalCameraPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
