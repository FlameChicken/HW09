#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NBGameModeBase.generated.h"

UCLASS()
class NUMBALL_API ANBGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ANBGameModeBase();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "NumBall|Core")
    void GenerateAnswer();

    UFUNCTION(BlueprintCallable, Category = "NumBall|Core")
    FString ValidateInput(const FString& Input) const;

    UFUNCTION(BlueprintCallable, Category = "NumBall|Core")
    FString EvaluateGuess(const FString& Guess) const;

    UFUNCTION(BlueprintCallable, Category = "NumBall|Core")
    void HandlePlayerGuess(APlayerController* PC, const FString& Input);

    UFUNCTION(BlueprintCallable, Category = "NumBall|Control")
    void CheckGameResult(APlayerController* WinnerPC = nullptr);

    UFUNCTION(BlueprintCallable, Category = "NumBall|Control")
    void ResetGame();

private:
    TArray<int32> Answer;

    int32 GetTotalPlayers() const;
    int32 GetEliminatedPlayers() const;
    void BroadcastMessage(const FString& Message, APlayerController* TargetPC = nullptr);
};