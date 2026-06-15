#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NBResultWidget.generated.h"

UCLASS()
class NUMBALL_API UNBResultWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "NumBall|UI")
    void SetResultText(const FString& Text);

protected:
    // 블루프린트 위젯에서 TextBlock 이름을 "ResultText"로 만들면 자동 연결
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ResultText;
};