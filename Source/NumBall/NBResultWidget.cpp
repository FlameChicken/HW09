#include "NBResultWidget.h"
#include "Components/TextBlock.h"

void UNBResultWidget::SetResultText(const FString& Text)
{
    if (ResultText)
    {
        ResultText->SetText(FText::FromString(Text));
    }
}