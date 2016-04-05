// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreUObject.h"
#include "MaterialMerging.h"
#include "MeshMerging.generated.h"

/** The importance of a mesh feature when automatically generating mesh LODs. */
UENUM()
namespace EMeshFeatureImportance
{
	enum Type
	{
		Off,
		Lowest,
		Low,
		Normal,
		High,
		Highest
	};
}


/** Settings used to reduce a mesh. */
USTRUCT()
struct FMeshReductionSettings
{
	GENERATED_USTRUCT_BODY()

		/** Percentage of triangles to keep. 1.0 = no reduction, 0.0 = no triangles. */
		UPROPERTY(EditAnywhere, Category = ReductionSettings)
		float PercentTriangles;

	/** The maximum distance in object space by which the reduced mesh may deviate from the original mesh. */
	UPROPERTY(EditAnywhere, Category = ReductionSettings)
		float MaxDeviation;

	/** Threshold in object space at which vertices are welded together. */
	UPROPERTY(EditAnywhere, Category = ReductionSettings)
		float WeldingThreshold;

	/** Angle at which a hard edge is introduced between faces. */
	UPROPERTY(EditAnywhere, Category = ReductionSettings)
		float HardAngleThreshold;

	/** Higher values minimize change to border edges. */
	UPROPERTY(EditAnywhere, Category = ReductionSettings)
		TEnumAsByte<EMeshFeatureImportance::Type> SilhouetteImportance;

	/** Higher values reduce texture stretching. */
	UPROPERTY(EditAnywhere, Category = ReductionSettings)
		TEnumAsByte<EMeshFeatureImportance::Type> TextureImportance;

	/** Higher values try to preserve normals better. */
	UPROPERTY(EditAnywhere, Category = ReductionSettings)
		TEnumAsByte<EMeshFeatureImportance::Type> ShadingImportance;

	/*UPROPERTY(EditAnywhere, Category = ReductionSettings)
	bool bActive;*/

	UPROPERTY(EditAnywhere, Category = ReductionSettings)
		bool bRecalculateNormals;

	UPROPERTY(EditAnywhere, Category = ReductionSettings)
		int32 BaseLODModel;

	UPROPERTY(EditAnywhere, Category = ReductionSettings)
		bool bGenerateUniqueLightmapUVs;

	UPROPERTY(EditAnywhere, Category = ReductionSettings)
		bool bKeepSymmetry;

	UPROPERTY(EditAnywhere, Category = ReductionSettings)
		bool bVisibilityAided;

	UPROPERTY(EditAnywhere, Category = ReductionSettings)
		bool bCullOccluded;

	/** Higher values generates fewer samples*/
	UPROPERTY(EditAnywhere, Category = ReductionSettings)
		TEnumAsByte<EMeshFeatureImportance::Type> VisibilityAggressiveness;

	/** Higher values minimize change to vertex color data. */
	UPROPERTY(EditAnywhere, Category = ReductionSettings)
		TEnumAsByte<EMeshFeatureImportance::Type> VertexColorImportance;

	/** Default settings. */
	FMeshReductionSettings()
		: PercentTriangles(1.0f)
		, MaxDeviation(0.0f)
		, WeldingThreshold(0.0f)
		, HardAngleThreshold(80.0f)
		, SilhouetteImportance(EMeshFeatureImportance::Normal)
		, TextureImportance(EMeshFeatureImportance::Normal)
		, ShadingImportance(EMeshFeatureImportance::Normal)
		, bRecalculateNormals(false)
		, BaseLODModel(0)
		, bGenerateUniqueLightmapUVs(false)
		, bKeepSymmetry(false)
		, bVisibilityAided(false)
		, bCullOccluded(false)
		, VisibilityAggressiveness(EMeshFeatureImportance::Lowest)
		, VertexColorImportance(EMeshFeatureImportance::Off)
	{
	}

	FMeshReductionSettings(const FMeshReductionSettings& Other)
		: PercentTriangles(Other.PercentTriangles)
		, MaxDeviation(Other.MaxDeviation)
		, WeldingThreshold(Other.WeldingThreshold)
		, HardAngleThreshold(Other.HardAngleThreshold)
		, SilhouetteImportance(Other.ShadingImportance)
		, TextureImportance(Other.TextureImportance)
		, ShadingImportance(Other.ShadingImportance)
		, bRecalculateNormals(Other.bRecalculateNormals)
		, BaseLODModel(Other.BaseLODModel)
		, bGenerateUniqueLightmapUVs(Other.bGenerateUniqueLightmapUVs)
		, bKeepSymmetry(Other.bKeepSymmetry)
		, bVisibilityAided(Other.bVisibilityAided)
		, bCullOccluded(Other.bCullOccluded)
		, VisibilityAggressiveness(Other.VisibilityAggressiveness)
		, VertexColorImportance(Other.VertexColorImportance)
	{
	}

	/** Equality operator. */
	bool operator==(const FMeshReductionSettings& Other) const
	{
		return PercentTriangles == Other.PercentTriangles
			&& MaxDeviation == Other.MaxDeviation
			&& WeldingThreshold == Other.WeldingThreshold
			&& HardAngleThreshold == Other.HardAngleThreshold
			&& SilhouetteImportance == Other.SilhouetteImportance
			&& TextureImportance == Other.TextureImportance
			&& ShadingImportance == Other.ShadingImportance
			&& bRecalculateNormals == Other.bRecalculateNormals
			&& BaseLODModel == Other.BaseLODModel
			&& bGenerateUniqueLightmapUVs == Other.bGenerateUniqueLightmapUVs
			&& bKeepSymmetry == Other.bKeepSymmetry
			&& bVisibilityAided == Other.bVisibilityAided
			&& bCullOccluded == Other.bCullOccluded
			&& VisibilityAggressiveness == Other.VisibilityAggressiveness
			&& VertexColorImportance == Other.VertexColorImportance;
	}

	/** Inequality. */
	bool operator!=(const FMeshReductionSettings& Other) const
	{
		return !(*this == Other);
	}
};


USTRUCT()
struct FMeshProxySettings
{
	GENERATED_USTRUCT_BODY()
		/** Screen size of the resulting proxy mesh in pixel size*/
		UPROPERTY(EditAnywhere, Category = ProxySettings)
		int32 ScreenSize;

	/** Material simplification */
	UPROPERTY(EditAnywhere, Category = ProxySettings)
		FMaterialProxySettings MaterialSettings;

	UPROPERTY()
		int32 TextureWidth_DEPRECATED;
	UPROPERTY()
		int32 TextureHeight_DEPRECATED;

	UPROPERTY()
		bool bExportNormalMap_DEPRECATED;

	UPROPERTY()
		bool bExportMetallicMap_DEPRECATED;

	UPROPERTY()
		bool bExportRoughnessMap_DEPRECATED;

	UPROPERTY()
		bool bExportSpecularMap_DEPRECATED;

	/** Material simplification */
	UPROPERTY()
		FMaterialSimplificationSettings Material_DEPRECATED;


	/** Determines whether or not the correct LOD models should be calculated given the source meshes and transition size */
	UPROPERTY(EditAnywhere, Category = ProxySettings)
		bool bCalculateLODSourceModels;

	/** Distance at which meshes should be merged together */
	UPROPERTY(EditAnywhere, Category = ProxySettings)
		float MergeDistance;

	/** Angle at which a hard edge is introduced between faces */
	UPROPERTY(EditAnywhere, Category = ProxySettings, meta = (DisplayName = "Hard Edge Angle"))
		float HardAngleThreshold;

	/** Lightmap resolution */
	UPROPERTY(EditAnywhere, Category = ProxySettings)
		int32 LightMapResolution;

	/** Whether Simplygon should recalculate normals, otherwise the normals channel will be sampled from the original mesh */
	UPROPERTY(EditAnywhere, Category = ProxySettings)
		bool bRecalculateNormals;

	UPROPERTY()
		bool bBakeVertexData_DEPRECATED;

	/** Default settings. */
	FMeshProxySettings()
		: ScreenSize(300)
		, TextureWidth_DEPRECATED(512)
		, TextureHeight_DEPRECATED(512)
		, bExportNormalMap_DEPRECATED(true)
		, bExportMetallicMap_DEPRECATED(false)
		, bExportRoughnessMap_DEPRECATED(false)
		, bExportSpecularMap_DEPRECATED(false)
		, MergeDistance(4)
		, HardAngleThreshold(80.0f)
		, LightMapResolution(256)
		, bRecalculateNormals(true)
	{ }

	/** Equality operator. */
	bool operator==(const FMeshProxySettings& Other) const
	{
		return ScreenSize == Other.ScreenSize
			&& MaterialSettings == Other.MaterialSettings
			&& bRecalculateNormals == Other.bRecalculateNormals
			&& HardAngleThreshold == Other.HardAngleThreshold
			&& MergeDistance == Other.MergeDistance;
	}

	/** Inequality. */
	bool operator!=(const FMeshProxySettings& Other) const
	{
		return !(*this == Other);
	}

	/** Handles deprecated properties */
	void PostLoadDeprecated();
};

/**
* Mesh merging settings
*/
USTRUCT()
struct FMeshMergingSettings
{
	GENERATED_USTRUCT_BODY()

		/** Whether to generate lightmap UVs for a merged mesh*/
		UPROPERTY(EditAnywhere, Category = FMeshMergingSettings)
		bool bGenerateLightMapUV;

	/** Target UV channel in a merged mesh for a lightmap */
	UPROPERTY(EditAnywhere, Category = FMeshMergingSettings)
		int32 TargetLightMapUVChannel;

	/** Target lightmap resolution */
	UPROPERTY(EditAnywhere, Category = FMeshMergingSettings)
		int32 TargetLightMapResolution;

	/** Whether we should import vertex colors into merged mesh */
	UPROPERTY()
		bool bImportVertexColors_DEPRECATED;

	/** Whether merged mesh should have pivot at world origin, or at first merged component otherwise */
	UPROPERTY(EditAnywhere, Category = FMeshMergingSettings)
		bool bPivotPointAtZero;

	/** Whether to merge physics data (collision primitives)*/
	UPROPERTY(EditAnywhere, Category = FMeshMergingSettings)
		bool bMergePhysicsData;

	/** Whether to merge source materials into one flat material */
	UPROPERTY(EditAnywhere, Category = MeshMerge)
		bool bMergeMaterials;

	/** Material simplification */
	UPROPERTY(EditAnywhere, Category = MeshMerge, meta = (editcondition = "bMergeMaterials"))
		FMaterialProxySettings MaterialSettings;

	UPROPERTY(EditAnywhere, Category = MeshMerge)
		bool bBakeVertexData;

	/** Whether to export normal maps for material merging */
	UPROPERTY()
		bool bExportNormalMap_DEPRECATED;
	/** Whether to export metallic maps for material merging */
	UPROPERTY()
		bool bExportMetallicMap_DEPRECATED;
	/** Whether to export roughness maps for material merging */
	UPROPERTY()
		bool bExportRoughnessMap_DEPRECATED;
	/** Whether to export specular maps for material merging */
	UPROPERTY()
		bool bExportSpecularMap_DEPRECATED;
	/** Merged material texture atlas resolution */
	UPROPERTY()
		int32 MergedMaterialAtlasResolution_DEPRECATED;

	/** Default settings. */
	FMeshMergingSettings()
		: bGenerateLightMapUV(false)
		, TargetLightMapUVChannel(1)
		, TargetLightMapResolution(256)
		, bImportVertexColors_DEPRECATED(false)
		, bPivotPointAtZero(false)
		, bMergePhysicsData(false)
		, bMergeMaterials(false)
		, bExportNormalMap_DEPRECATED(true)
		, bExportMetallicMap_DEPRECATED(false)
		, bExportRoughnessMap_DEPRECATED(false)
		, bExportSpecularMap_DEPRECATED(false)
		, MergedMaterialAtlasResolution_DEPRECATED(1024)
	{
	}

	/** Handles deprecated properties */
	void PostLoadDeprecated();
};