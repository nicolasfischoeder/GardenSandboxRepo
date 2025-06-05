#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "DamageTypes.generated.h"

/** DamageType representing fire damage */
UCLASS()
class GARDENSANDBOX_API UFireDamageType : public UDamageType
{
    GENERATED_BODY()
};

/** DamageType representing frost damage */
UCLASS()
class GARDENSANDBOX_API UFrostDamageType : public UDamageType
{
    GENERATED_BODY()
};

/** DamageType representing poison damage */
UCLASS()
class GARDENSANDBOX_API UPoisonDamageType : public UDamageType
{
    GENERATED_BODY()
};

