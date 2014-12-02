// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

/*==============================================================================
NiagaraEffectRenderer.h: Base class for Niagara render modules
==============================================================================*/
#pragma once

#include "SceneUtils.h"
#include "ParticleHelper.h"
#include "ParticleVertexFactory.h"
#include "ParticleBeamTrailVertexFactory.h"
#include "Components/NiagaraComponent.h"

DECLARE_CYCLE_STAT(TEXT("Generate Sprite Vertex Data"), STAT_NiagaraGenSpriteVertexData, STATGROUP_Niagara);
DECLARE_CYCLE_STAT(TEXT("Generate Ribbon Vertex Data"), STAT_NiagaraGenRibbonVertexData, STATGROUP_Niagara);

/** Struct used to pass dynamic data from game thread to render thread */
struct FNiagaraDynamicDataBase
{
};






/**
* Base class for Niagara effect renderers. Effect renderers handle generating vertex data for and
* drawing of simulation data coming out of FNiagaraSimulation instances.
*/
class NiagaraEffectRenderer
{
public:
	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector, const FNiagaraSceneProxy *SceneProxy) const = 0;

	virtual void SetDynamicData_RenderThread(FNiagaraDynamicDataBase* NewDynamicData) = 0;
	virtual void CreateRenderThreadResources() = 0;
	virtual void ReleaseRenderThreadResources() = 0;
	virtual FNiagaraDynamicDataBase *GenerateVertexData(const FNiagaraEmitterParticleData &Data) = 0;
	virtual int GetDynamicDataSize() = 0;

	virtual bool HasDynamicData() = 0;

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View, FNiagaraSceneProxy *SceneProxy)
	{
		FPrimitiveViewRelevance Result;
		bool bHasDynamicData = HasDynamicData();

		Result.bDrawRelevance = bHasDynamicData && SceneProxy->IsShown(View) && View->Family->EngineShowFlags.Particles;
		Result.bShadowRelevance = bHasDynamicData && SceneProxy->IsShadowCast(View);
		Result.bDynamicRelevance = bHasDynamicData;
		if (bHasDynamicData && View->Family->EngineShowFlags.Bounds)
		{
			Result.bOpaqueRelevance = true;
		}
		MaterialRelevance.SetPrimitiveViewRelevance(Result);

		return Result;
	}


	UMaterial *GetMaterial()	{ return Material; }
	void SetMaterial(UMaterial *InMaterial, ERHIFeatureLevel::Type FeatureLevel)
	{
		Material = InMaterial;
		if (!Material)
		{
			Material = UMaterial::GetDefaultMaterial(MD_Surface);
		}
		check(Material);
		MaterialRelevance = Material->GetRelevance(FeatureLevel);
	}

	virtual ~NiagaraEffectRenderer() {}

protected:
	NiagaraEffectRenderer()	
		: Material(nullptr)
	{
	}

	UMaterial* Material;

private:
	FMaterialRelevance MaterialRelevance;
};



/**
* NiagaraEffectRendererSprites renders an FNiagaraSimulation as sprite particles
*/
class NiagaraEffectRendererSprites : public NiagaraEffectRenderer
{
public:	
	NiagaraEffectRendererSprites(ERHIFeatureLevel::Type FeatureLevel);
	~NiagaraEffectRendererSprites()
	{
		ReleaseRenderThreadResources();
	}


	virtual void ReleaseRenderThreadResources() override;

	// FPrimitiveSceneProxy interface.
	virtual void CreateRenderThreadResources() override;

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector, const FNiagaraSceneProxy *SceneProxy) const override;



	/** Update render data buffer from attributes */
	FNiagaraDynamicDataBase *GenerateVertexData(const FNiagaraEmitterParticleData &Data) override;




	virtual void SetDynamicData_RenderThread(FNiagaraDynamicDataBase* NewDynamicData) override;
	int GetDynamicDataSize();
	bool HasDynamicData();

private:
	struct FNiagaraDynamicDataSprites* DynamicDataRender;
	mutable TUniformBuffer<FPrimitiveUniformShaderParameters> WorldSpacePrimitiveUniformBuffer;
	class FParticleSpriteVertexFactory* VertexFactory;
};










/**
* NiagaraEffectRendererRibbon renders an FNiagaraSimulation as a ribbon connecting all particles
* in order by particle age.
*/
class NiagaraEffectRendererRibbon : public NiagaraEffectRenderer
{
public:
	NiagaraEffectRendererRibbon(ERHIFeatureLevel::Type FeatureLevel);
	~NiagaraEffectRendererRibbon()
	{
		ReleaseRenderThreadResources();
	}

	virtual void ReleaseRenderThreadResources() override;
	virtual void CreateRenderThreadResources() override;


	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector, const FNiagaraSceneProxy *SceneProxy) const override;

	/** Update render data buffer from attributes */
	FNiagaraDynamicDataBase *GenerateVertexData(const FNiagaraEmitterParticleData &Data);

	void AddRibbonVert(TArray<FParticleBeamTrailVertex>& RenderData, FVector ParticlePos, const FNiagaraEmitterParticleData &Data, FVector2D UV1,
		const FVector4 &Color, const FVector4 &Age, const FVector4 &Rotation)
	{
		FParticleBeamTrailVertex& NewVertex = *new(RenderData)FParticleBeamTrailVertex;
		NewVertex.Position = ParticlePos;
		NewVertex.OldPosition = NewVertex.Position;
		NewVertex.Color = FLinearColor(Color);
		NewVertex.ParticleId = 0;
		NewVertex.RelativeTime = Age.X;
		NewVertex.Size = FVector2D(1.0f, 1.0f);
		NewVertex.Rotation = Rotation.X;
		NewVertex.SubImageIndex = 0.f;
		NewVertex.Tex_U = UV1.X;
		NewVertex.Tex_V = UV1.Y;
		NewVertex.Tex_U2 = UV1.X;
		NewVertex.Tex_V2 = UV1.Y;
	}



	virtual void SetDynamicData_RenderThread(FNiagaraDynamicDataBase* NewDynamicData) override;
	int GetDynamicDataSize();
	bool HasDynamicData();


private:
	struct FNiagaraDynamicDataRibbon* DynamicDataRender;
	mutable TUniformBuffer<FPrimitiveUniformShaderParameters> WorldSpacePrimitiveUniformBuffer;
	class FParticleBeamTrailVertexFactory* VertexFactory;
};


