// Copyright Epic Games, Inc. All Rights Reserved.

#include "Gam415_AserranoGameMode.h"
#include "Gam415_AserranoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGam415_AserranoGameMode::AGam415_AserranoGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
