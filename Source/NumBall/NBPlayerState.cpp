#include "NBPlayerState.h"
#include "Net/UnrealNetwork.h"

ANBPlayerState::ANBPlayerState()
{
    CurrentAttempts = 0;
    MaxAttempts = 3;
    bHasWon = false;
    bIsEliminated = false;
}

FString ANBPlayerState::GetAttemptsDisplayString() const
{
    return FString::Printf(TEXT("[%d / %d]"), CurrentAttempts, MaxAttempts);
}

void ANBPlayerState::IncrementAttempts()
{
    CurrentAttempts++;
    if (CurrentAttempts >= MaxAttempts)
    {
        bIsEliminated = true;
    }
}

void ANBPlayerState::ResetState()
{
    CurrentAttempts = 0;
    bHasWon = false;
    bIsEliminated = false;
}

void ANBPlayerState::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ANBPlayerState, CurrentAttempts);
    DOREPLIFETIME(ANBPlayerState, MaxAttempts);
    DOREPLIFETIME(ANBPlayerState, bHasWon);
    DOREPLIFETIME(ANBPlayerState, bIsEliminated);
}