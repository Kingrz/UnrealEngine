// Copyright 2015 Kite & Lightning.  All rights reserved.

#include "StereoPanoramaPrivatePCH.h"
#include "StereoStaticMeshComponent.h"
#include "StaticMeshResources.h"

class FStereoStaticMeshSceneProxy : public FStaticMeshSceneProxy
{
    ESPStereoCameraLayer::Type EyeToRender;

public:

    FStereoStaticMeshSceneProxy(UStereoStaticMeshComponent* Component) :
        FStaticMeshSceneProxy(Component)
    {
        EyeToRender = Component->EyeToRender;
    }

    virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) override
    {
        FPrimitiveViewRelevance viewRelevance = FStaticMeshSceneProxy::GetViewRelevance(View);
        bool bVisible = true;

        switch (View->StereoPass)
        {
        case eSSP_RIGHT_EYE:
            if (EyeToRender != ESPStereoCameraLayer::RightEye && EyeToRender != ESPStereoCameraLayer::BothEyes)
            {
                bVisible = false;
            }
            break;

        case eSSP_LEFT_EYE:
            if (EyeToRender != ESPStereoCameraLayer::LeftEye && EyeToRender != ESPStereoCameraLayer::BothEyes)
            {
                bVisible = false;
            }
            break;

        case eSSP_FULL:
        default:
            //Draw both planes when in mono mode
            break;
        }

        viewRelevance.bDrawRelevance &= bVisible;

        return viewRelevance;

    }


};

FPrimitiveSceneProxy* UStereoStaticMeshComponent::CreateSceneProxy()
{
    if (StaticMesh == NULL
        || StaticMesh->RenderData == NULL
        || StaticMesh->RenderData->LODResources.Num() == 0
        || StaticMesh->RenderData->LODResources[0].VertexBuffer.GetNumVertices() == 0)
    {
        return NULL;
    }

    FPrimitiveSceneProxy* Proxy = ::new FStereoStaticMeshSceneProxy(this);
    return Proxy;
}


