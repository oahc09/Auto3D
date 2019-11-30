#pragma once
#include "Core/GameManager.h"
#include "Batch2D.h"

namespace Auto3D {

class AScene2D;
class ACamera2D;
class ANode2D;

class AGraphics;
class FVertexBuffer;
class FIndexBuffer;
class FConstantBuffer;
class FShaderVariation;
struct FVertexElement;
class AShader;

/// ATexture coordinate index for the instance world matrix.
static const size_t U_INSTANCE_TEXCOORD = 4;

/// AShader constant buffers used by high-level rendering.
namespace EConstantBuffer2D
{
	enum Type
	{
		FRAME = 0,
		OBJECT,
		MATERIAL,
		LIGHTS
	};
};


/// APhysics sub system 
class AUTO_API ARenderer2D : public ABaseModule
{
	REGISTER_OBJECT_CLASS(ARenderer2D, ABaseModule)
public:
	/// Construct
	ARenderer2D();
	/// Destructor
	~ARenderer2D();
	/// Render the Renderer2D. If render command is false (default), is assumed to be the default Renderer2D render to backbuffer called by AEngine, and will be performed only once. Additional Renderer2D renders to a different rendertarget may be triggered from the renderpath.
	void Render(AScene2D* scene, ACamera2D* camera);
	/// Prepare view of objects and batch
	bool PrepareView(AScene2D* scend2d, ACamera2D* camera);
	/// Return initialized flag
	bool IsInitialized() { return _initialized; }
	/// Initialize rendering of a new view and collect visible objects from the camera's point of view. Return true on success (scene, camera and octree are non-null.)
	bool Collect2dObjects(AScene2D* scene, ACamera2D* camera);
	/// Collect and sort batches from the visible objects. To not go through the objects several times, all the passes should be specified at once instead of multiple calls to CollectBatches().
	void Collect2dBatches();
	/// Render of batchs
	void RenderBatches();
private:
	/// Initialize when screen mode initially set.
	void Initialize();
	/// Render batches from a specific queue and camera.
	void RenderBatches(const TVector<FBatch2D>& batches, ACamera2D* camera);
	/// AGraphics subsystem.
	TWeakPtr<AGraphics> _graphics;
	/// Renderer2D rendering batches.
	TVector<FBatch2D> _batches;
	/// Current scene.
	AScene2D* _scenes;
	/// Current 2d camera.
	ACamera2D* _camera;
	/// FGeometry nodes
	TVector<AGeometryNode2D*> _geometryNode;
	/// ARenderer does not have multiple queues
	FBatch2DQueue _batchQueue;
	/// Initialized flag.
	bool _initialized;
	/// Flag for Renderer2D already being rendered this frame.
	bool _rendered;
	/// Instance vertex buffer dirty flag.
	bool _instanceTransformsDirty;
	/// Instance transforms for uploading to the instance vertex buffer.
	TVector<TMatrix3x4F> _instanceTransforms;
	/// Per-frame vertex shader constant buffer.
	TSharedPtr<FConstantBuffer> _vsFrameConstantBuffer;
	/// Per-object vertex shader constant buffer.
	TSharedPtr<FConstantBuffer> _vsObjectConstantBuffer;
	/// Per-frame pixel shader constant buffer.
	TSharedPtr<FConstantBuffer> _psFrameConstantBuffer;
	/// Instance transform vertex buffer.
	TAutoPtr<FVertexBuffer> _instanceVertexBuffer;
	/// Vertex elements for the instance vertex buffer.
	TVector<FVertexElement> _instanceVertexElements;
	/// Render 2D shaderVariation vs.
	FShaderVariation* _vsv;
	/// Render 2D shaderVariation ps.
	FShaderVariation* _psv;
	/// Instance shaderVariation vs 
	FShaderVariation* _ivsv;
	/// Instance FShaderVariation ps
	FShaderVariation* _ipsv;
};

/// Register Renderer2D related object factories and attributes.
AUTO_API void RegisterRenderer2DLibrary();

}