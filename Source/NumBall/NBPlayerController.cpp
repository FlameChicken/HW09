#include "NBPlayerController.h"
#include "NBGameModeBase.h"
#include "NBResultWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"

void ANBPlayerController::ServerSendGuess_Implementation(const FString& Input)
{
    if (ANBGameModeBase* GM = GetWorld()->GetAuthGameMode<ANBGameModeBase>())
    {
        GM->HandlePlayerGuess(this, Input);
    }
}

void ANBPlayerController::ClientReceiveMessage_Implementation(const FString& Message)
{
    // 화면에 디버그 메시지로 표시 (나중에 채팅UI로 교체 가능)
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::White, Message);
    }
}

void ANBPlayerController::ClientShowResultWidget_Implementation(const FString& ResultMessage)
{
    if (!ResultWidgetClass) return;

    if (!ResultWidgetInstance)
    {
        ResultWidgetInstance = CreateWidget<UNBResultWidget>(this, ResultWidgetClass);
    }

    if (ResultWidgetInstance)
    {
        ResultWidgetInstance->SetResultText(ResultMessage);
        ResultWidgetInstance->AddToViewport();
    }
}

void ANBPlayerController::ClientHideResultWidget_Implementation()
{
    if (ResultWidgetInstance && ResultWidgetInstance->IsInViewport())
    {
        ResultWidgetInstance->RemoveFromViewport();
    }
}