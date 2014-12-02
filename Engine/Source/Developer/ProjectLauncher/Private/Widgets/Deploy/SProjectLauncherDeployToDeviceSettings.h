// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once


/**
 * Implements the deploy-to-device settings panel.
 */
class SProjectLauncherDeployToDeviceSettings
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SProjectLauncherDeployToDeviceSettings) { }
	SLATE_END_ARGS()

public:

	/**
	 * Constructs the widget.
	 *
	 * @param InArgs - The Slate argument list.
	 * @param InModel - The data model.
	 */
	void Construct(	const FArguments& InArgs, const FProjectLauncherModelRef& InModel, EVisibility InShowAdvanced = EVisibility::Visible );

private:

private:

	// Holds a pointer to the data model.
	FProjectLauncherModelPtr Model;
};
