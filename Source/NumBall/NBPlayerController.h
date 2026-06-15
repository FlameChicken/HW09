#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NBPlayerController.generated.h"

UCLASS()
class NUMBALL_API ANBPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    // 플레이어가 숫자 입력 → 서버로 전송
    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "NumBall")
    void ServerSendGuess(const FString& Input);

    // 서버 → 이 클라이언트로 채팅 메시지 전송
    UFUNCTION(Client, Reliable, Category = "NumBall")
    void ClientReceiveMessage(const FString& Message);

    // 결과 위젯 표시
    UFUNCTION(Client, Reliable, BlueprintCallable, Category = "NumBall")
    void ClientShowResultWidget(const FString& ResultMessage);

    // 결과 위젯 숨기기
    UFUNCTION(Client, Reliable, BlueprintCallable, Category = "NumBall")
    void ClientHideResultWidget();

protected:
    // 블루프린트에서 위젯 클래스 지정용
    UPROPERTY(EditDefaultsOnly, Category = "NumBall|UI")
    TSubclassOf<class UNBResultWidget> ResultWidgetClass;

    UPROPERTY()
    class UNBResultWidget* ResultWidgetInstance;
};