#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NBPlayerState.generated.h"

UCLASS()
class NUMBALL_API ANBPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    ANBPlayerState();

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "NumBall")
    int32 CurrentAttempts;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "NumBall")
    int32 MaxAttempts;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "NumBall")
    bool bHasWon;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "NumBall")
    bool bIsEliminated;

    UFUNCTION(BlueprintPure, Category = "NumBall")
    int32 GetCurrentAttempts() const { return CurrentAttempts; }

    UFUNCTION(BlueprintPure, Category = "NumBall")
    int32 GetMaxAttempts() const { return MaxAttempts; }

    UFUNCTION(BlueprintPure, Category = "NumBall")
    FString GetAttemptsDisplayString() const;

    void IncrementAttempts();
    void ResetState();

    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};