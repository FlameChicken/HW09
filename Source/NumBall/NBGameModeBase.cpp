#include "NBGameModeBase.h"
#include "NBPlayerState.h"
#include "NBPlayerController.h"
#include "GameFramework/GameStateBase.h"

ANBGameModeBase::ANBGameModeBase()
{
    PlayerStateClass = ANBPlayerState::StaticClass();
}

void ANBGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    GenerateAnswer();
    BroadcastMessage(TEXT("=== NumBall Start! Guess 3 digits (1~9, no duplicates) ==="));
}

void ANBGameModeBase::GenerateAnswer()
{
    Answer.Empty();

    TArray<int32> Pool;
    for (int32 i = 1; i <= 9; ++i) Pool.Add(i);

    for (int32 i = Pool.Num() - 1; i > 0; --i)
    {
        int32 j = FMath::RandRange(0, i);
        Pool.Swap(i, j);
    }

    for (int32 i = 0; i < 3; ++i)
        Answer.Add(Pool[i]);

    UE_LOG(LogTemp, Warning, TEXT("[DEBUG] Answer: %d%d%d"), Answer[0], Answer[1], Answer[2]);
}

FString ANBGameModeBase::ValidateInput(const FString& Input) const
{
    if (Input.Len() != 3)
        return TEXT("Please enter a 3-digit number. Try again.");

    for (TCHAR Ch : Input)
    {
        if (!FChar::IsDigit(Ch))
            return TEXT("Only digits allowed. Try again.");
    }

    for (TCHAR Ch : Input)
    {
        if (Ch == TEXT('0'))
            return TEXT("0 is not allowed. Try again.");
    }

    if (Input[0] == Input[1] || Input[1] == Input[2] || Input[0] == Input[2])
        return TEXT("Duplicate digits found. Try again.");

    return TEXT("");
}

FString ANBGameModeBase::EvaluateGuess(const FString& Guess) const
{
    int32 Strikes = 0;
    int32 Balls = 0;

    for (int32 i = 0; i < 3; ++i)
    {
        int32 GuessDigit = Guess[i] - TEXT('0');

        if (GuessDigit == Answer[i])
        {
            Strikes++;
        }
        else
        {
            for (int32 j = 0; j < 3; ++j)
            {
                if (j != i && GuessDigit == Answer[j])
                {
                    Balls++;
                    break;
                }
            }
        }
    }

    if (Strikes == 0 && Balls == 0)
        return TEXT("OUT");

    FString Result;
    if (Strikes > 0) Result += FString::Printf(TEXT("%dS"), Strikes);
    if (Balls > 0)   Result += FString::Printf(TEXT("%dB"), Balls);
    return Result;
}

void ANBGameModeBase::HandlePlayerGuess(APlayerController* PC, const FString& Input)
{
    if (!PC) return;

    ANBPlayerState* PS = PC->GetPlayerState<ANBPlayerState>();
    if (!PS) return;

    if (PS->bIsEliminated || PS->bHasWon)
    {
        BroadcastMessage(TEXT("You are already eliminated."), PC);
        return;
    }

    FString Error = ValidateInput(Input);
    if (!Error.IsEmpty())
    {
        BroadcastMessage(Error, PC);
        return;
    }

    PS->IncrementAttempts();

    FString Result = EvaluateGuess(Input);
    FString PlayerName = PS->GetPlayerName();
    FString AttemptStr = PS->GetAttemptsDisplayString();

    FString Message = FString::Printf(
        TEXT("[%s] %s Input: %s -> %s"),
        *PlayerName, *AttemptStr, *Input, *Result);

    BroadcastMessage(Message);

    if (Result.Equals(TEXT("3S")))
    {
        PS->bHasWon = true;
        CheckGameResult(PC);
        return;
    }

    if (GetEliminatedPlayers() >= GetTotalPlayers())
    {
        CheckGameResult(nullptr);
    }
}

void ANBGameModeBase::CheckGameResult(APlayerController* WinnerPC)
{
    FString Msg;

    if (WinnerPC)
    {
        ANBPlayerState* PS = WinnerPC->GetPlayerState<ANBPlayerState>();
        FString WinnerName = PS ? PS->GetPlayerName() : TEXT("Player");
        Msg = FString::Printf(TEXT("%s Wins! Answer was: %d%d%d"),
            *WinnerName, Answer[0], Answer[1], Answer[2]);
    }
    else
    {
        Msg = FString::Printf(TEXT("Draw! Answer was: %d%d%d"),
            Answer[0], Answer[1], Answer[2]);
    }

    BroadcastMessage(Msg);

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (ANBPlayerController* NPC = Cast<ANBPlayerController>(It->Get()))
        {
            NPC->ClientShowResultWidget(Msg);
        }
    }

    FTimerHandle ResetTimer;
    GetWorldTimerManager().SetTimer(ResetTimer, this, &ANBGameModeBase::ResetGame, 5.0f, false);
}

void ANBGameModeBase::ResetGame()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (ANBPlayerState* PS = It->Get()->GetPlayerState<ANBPlayerState>())
        {
            PS->ResetState();
        }

        if (ANBPlayerController* NPC = Cast<ANBPlayerController>(It->Get()))
        {
            NPC->ClientHideResultWidget();
        }
    }

    GenerateAnswer();
    BroadcastMessage(TEXT("New game started! Guess the 3-digit number."));
}

int32 ANBGameModeBase::GetTotalPlayers() const
{
    return GetWorld()->GetGameState()->PlayerArray.Num();
}

int32 ANBGameModeBase::GetEliminatedPlayers() const
{
    int32 Count = 0;
    for (APlayerState* PS : GetWorld()->GetGameState()->PlayerArray)
    {
        if (ANBPlayerState* NPS = Cast<ANBPlayerState>(PS))
        {
            if (NPS->bIsEliminated || NPS->bHasWon) Count++;
        }
    }
    return Count;
}

void ANBGameModeBase::BroadcastMessage(const FString& Message, APlayerController* TargetPC)
{
    if (TargetPC)
    {
        if (ANBPlayerController* NPC = Cast<ANBPlayerController>(TargetPC))
            NPC->ClientReceiveMessage(Message);
        return;
    }

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (ANBPlayerController* NPC = Cast<ANBPlayerController>(It->Get()))
        {
            NPC->ClientReceiveMessage(Message);
        }
    }
}