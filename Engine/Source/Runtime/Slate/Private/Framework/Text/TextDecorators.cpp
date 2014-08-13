// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "SlatePrivatePCH.h"

#if WITH_FANCY_TEXT

#include "TextDecorators.h"

TSharedRef< FWidgetDecorator > FWidgetDecorator::Create( FString InRunName, const FCreateWidget& InCreateWidgetDelegate )
{
	return MakeShareable( new FWidgetDecorator( InRunName, InCreateWidgetDelegate ) );
}

FWidgetDecorator::FWidgetDecorator( FString InRunName, const FCreateWidget& InCreateWidgetDelegate )
	: RunName( InRunName )
	, CreateWidgetDelegate( InCreateWidgetDelegate )
{

}

bool FWidgetDecorator::Supports( const FTextRunParseResults& RunParseResult, const FString& Text ) const
{
	return ( RunParseResult.Name == RunName );
}

TSharedRef< ISlateRun > FWidgetDecorator::Create( const FTextRunParseResults& RunParseResult, const FString& OriginalText, const TSharedRef< FString >& InOutModelText, const ISlateStyle* Style )
{
	FTextRange ModelRange;
	ModelRange.BeginIndex = InOutModelText->Len();
	*InOutModelText += TEXT('\x200B'); // Zero-Width Breaking Space
	ModelRange.EndIndex = InOutModelText->Len();

	FTextRunInfo RunInfo( RunParseResult.Name, FText::FromString( OriginalText.Mid( RunParseResult.ContentRange.BeginIndex, RunParseResult.ContentRange.EndIndex - RunParseResult.ContentRange.BeginIndex ) ) );
	for(const TPair<FString, FTextRange>& Pair : RunParseResult.MetaData)
	{
		RunInfo.MetaData.Add(Pair.Key, OriginalText.Mid( Pair.Value.BeginIndex, Pair.Value.EndIndex - Pair.Value.BeginIndex));
	}

	return FSlateWidgetRun::Create( RunInfo, InOutModelText, CreateWidgetDelegate.Execute( RunInfo, Style ), ModelRange );
}

TSharedRef< FImageDecorator > FImageDecorator::Create( FString InRunName, const ISlateStyle* const InStyle )
{
	return MakeShareable( new FImageDecorator( InRunName, InStyle ) );
}

FImageDecorator::FImageDecorator( FString InRunName, const ISlateStyle* const InOverrideStyle )
	: RunName( InRunName )
	, OverrideStyle( InOverrideStyle )
{

}

bool FImageDecorator::Supports( const FTextRunParseResults& RunParseResult, const FString& Text ) const
{
	return ( RunParseResult.Name == RunName );
}

TSharedRef< ISlateRun > FImageDecorator::Create( const FTextRunParseResults& RunParseResult, const FString& OriginalText, const TSharedRef< FString >& InOutModelText, const ISlateStyle* Style )
{
	const FTextRange* const BrushNameRange = RunParseResult.MetaData.Find( TEXT("src") );

	FTextRange ModelRange;
	ModelRange.BeginIndex = InOutModelText->Len();
	*InOutModelText += TEXT('\x200B'); // Zero-Width Breaking Space
	ModelRange.EndIndex = InOutModelText->Len();

	FRunInfo RunInfo( RunParseResult.Name );
	for(const TPair<FString, FTextRange>& Pair : RunParseResult.MetaData)
	{
		RunInfo.MetaData.Add(Pair.Key, OriginalText.Mid( Pair.Value.BeginIndex, Pair.Value.EndIndex - Pair.Value.BeginIndex));
	}

	if ( BrushNameRange != NULL )
	{
		const FString BrushNameString = OriginalText.Mid(BrushNameRange->BeginIndex, BrushNameRange->EndIndex - BrushNameRange->BeginIndex);
		if ( OverrideStyle != NULL )
		{
			Style = OverrideStyle;
		}

		FName BrushName( *BrushNameString );
		if ( Style->HasWidgetStyle< FInlineTextImageStyle >( BrushName ) )
		{
			const FInlineTextImageStyle& ImageStyle = Style->GetWidgetStyle< FInlineTextImageStyle >( BrushName );
			return FSlateImageRun::Create( RunInfo, InOutModelText, &ImageStyle.Image, ImageStyle.Baseline, ModelRange );
		}

		return FSlateImageRun::Create( RunInfo, InOutModelText, Style->GetBrush( BrushName ), 0, ModelRange );
	}

	const FInlineTextImageStyle& ImageStyle = FInlineTextImageStyle::GetDefault();
	return FSlateImageRun::Create( RunInfo, InOutModelText, &ImageStyle.Image, ImageStyle.Baseline, ModelRange );
}

TSharedRef< FHyperlinkDecorator > FHyperlinkDecorator::Create( FString Id, const FSlateHyperlinkRun::FOnClick& NavigateDelegate )
{
	return MakeShareable( new FHyperlinkDecorator( Id, NavigateDelegate ) );
}

FHyperlinkDecorator::FHyperlinkDecorator( FString InId, const FSlateHyperlinkRun::FOnClick& InNavigateDelegate )
	: Id( InId )
	, NavigateDelegate( InNavigateDelegate )
{

}

bool FHyperlinkDecorator::Supports( const FTextRunParseResults& RunParseResult, const FString& Text ) const
{
	const FTextRange* const MetaDataIdRange = RunParseResult.MetaData.Find( TEXT("id") );
	FString MetaDataId;
	if(MetaDataIdRange)
	{
		MetaDataId = Text.Mid(MetaDataIdRange->BeginIndex, MetaDataIdRange->EndIndex - MetaDataIdRange->BeginIndex);
	}

	return (RunParseResult.Name == TEXT("a") && MetaDataId == Id);
}

TSharedRef< ISlateRun > FHyperlinkDecorator::Create( const FTextRunParseResults& RunParseResult, const FString& OriginalText, const TSharedRef< FString >& InOutModelText, const ISlateStyle* Style )
{
	FString StyleName = TEXT("Hyperlink");

	const FTextRange* const MetaDataStyleNameRange = RunParseResult.MetaData.Find( TEXT("style") );
	if ( MetaDataStyleNameRange != NULL )
	{
		const FString MetaDataStyleName = OriginalText.Mid(MetaDataStyleNameRange->BeginIndex, MetaDataStyleNameRange->EndIndex - MetaDataStyleNameRange->BeginIndex);
		StyleName = *MetaDataStyleName;
	}

	FTextRange ModelRange;
	ModelRange.BeginIndex = InOutModelText->Len();
	*InOutModelText += OriginalText.Mid(RunParseResult.ContentRange.BeginIndex, RunParseResult.ContentRange.EndIndex - RunParseResult.ContentRange.BeginIndex);
	ModelRange.EndIndex = InOutModelText->Len();

	if ( !Style->HasWidgetStyle<FHyperlinkStyle>( FName( *StyleName ) ) )
	{
		Style = &FCoreStyle::Get();
	}

	FRunInfo RunInfo( RunParseResult.Name );
	for(const TPair<FString, FTextRange>& Pair : RunParseResult.MetaData)
	{
		RunInfo.MetaData.Add(Pair.Key, OriginalText.Mid( Pair.Value.BeginIndex, Pair.Value.EndIndex - Pair.Value.BeginIndex));
	}

	return FSlateHyperlinkRun::Create( RunInfo, InOutModelText, Style->GetWidgetStyle<FHyperlinkStyle>( FName( *StyleName ) ), NavigateDelegate, ModelRange );
}
#endif //WITH_FANCY_TEXT