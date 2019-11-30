#include "Camera.h"
#include "Math/Matrix3x4.h"
#include "Engine/Components/Image.h"
#include "Graphics/Texture.h"
#include "Debug/Log.h"
#include "Scene/Scene.h"
#include "Debug/DebugNew.h"

namespace Auto3D
{

static const float DEFAULT_NEARCLIP = 0.1f;
static const float DEFAULT_FARCLIP = 1000.0f;
static const float DEFAULT_FOV = 45.0f;
static const float DEFAULT_ORTHOSIZE = 20.0f;
static const FColor DEFAULT_AMBIENT_COLOR(0.25f, 0.25f, 0.25f, 1.0f);

static const TMatrix4x4F flipMatrix(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, -1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
    );

ACamera::ACamera() :
    _viewMatrix(TMatrix3x4F::IDENTITY),
    _viewMatrixDirty(false),
    _orthographic(false),
    _flipVertical(false),
    _nearClip(DEFAULT_NEARCLIP),
    _farClip(DEFAULT_FARCLIP),
    _fov(DEFAULT_FOV),
    _orthoSize(DEFAULT_ORTHOSIZE),
    _aspectRatio(1.0f),
    _zoom(1.0f),
    _lodBias(1.0f),
    _viewLayoutMask(M_MAX_UNSIGNED),
    _ambientColor(DEFAULT_AMBIENT_COLOR),
    _projectionOffset(TVector2F::ZERO),
    _reflectionPlane(FPlane::UP),
    _clipPlane(FPlane::UP),
    _useReflection(false),
    _useClipping(false)
{
    _reflectionMatrix = _reflectionPlane.ReflectionMatrix();
}

void ACamera::RegisterObject()
{
    RegisterFactory<ACamera>();
    CopyBaseAttributes<ACamera, ASpatialNode>();

    RegisterAttribute("nearClip", &ACamera::GetNearClip, &ACamera::SetNearClip, DEFAULT_NEARCLIP);
    RegisterAttribute("farClip", &ACamera::GetFarClip, &ACamera::SetFarClip, DEFAULT_FARCLIP);
    RegisterAttribute("fov", &ACamera::GetFov, &ACamera::SetFov, DEFAULT_FOV);
    RegisterAttribute("aspectRatio", &ACamera::GetAspectRatio, &ACamera::SetAspectRatio, 1.0f);
    RegisterAttribute("orthographic", &ACamera::IsOrthographic, &ACamera::SetOrthographic, false);
    RegisterAttribute("orthoSize", &ACamera::GetOrthoSize, &ACamera::SetOrthoSize, DEFAULT_ORTHOSIZE);
    RegisterAttribute("zoom", &ACamera::GetZoom, &ACamera::SetZoom, 1.0f);
    RegisterAttribute("lodBias", &ACamera::GetLodBias, &ACamera::SetLodBias, 1.0f);
    RegisterAttribute("viewMask", &ACamera::GetViewMask, &ACamera::SetLayoutMask, M_MAX_UNSIGNED);
    RegisterRefAttribute("ambientColor", &ACamera::GetAmbientColor, &ACamera::SetAmbientColor, DEFAULT_AMBIENT_COLOR);
    RegisterRefAttribute("projectionOffset", &ACamera::GetProjectionOffset, &ACamera::SetProjectionOffset, TVector2F::ZERO);
    RegisterMixedRefAttribute("reflectionPlane", &ACamera::ReflectionPlaneAttr, &ACamera::SetReflectionPlaneAttr, TVector4F(0.0f, 1.0f, 0.0f, 0.0f));
    RegisterMixedRefAttribute("clipPlane", &ACamera::ClipPlaneAttr, &ACamera::SetClipPlaneAttr, TVector4F(0.0f, 1.0f, 0.0f, 0.0f));
    RegisterAttribute("useReflection", &ACamera::GetUseReflection, &ACamera::SetUseReflection, false);
    RegisterAttribute("useClipping", &ACamera::GetUseClipping, &ACamera::SetUseClipping, false);
}
void ACamera::SetNearClip(float nearClip)
{
    _nearClip = Max(nearClip, M_EPSILON);
}

void ACamera::SetFarClip(float farClip)
{
    _farClip = Max(farClip, M_EPSILON);
}

void ACamera::SetFov(float fov)
{
    _fov = Clamp(fov, 0.0f, 180.0f);
}

void ACamera::SetOrthoSize(float orthoSize)
{
    _orthoSize = orthoSize;
    _aspectRatio = 1.0f;
}

void ACamera::SetOrthoSize(const TVector2F& orthoSize)
{
    _orthoSize = orthoSize._y;
    _aspectRatio = orthoSize._x / orthoSize._y;
}

void ACamera::SetAspectRatio(float aspectRatio)
{
    _aspectRatio = Max(aspectRatio, M_EPSILON);
}

void ACamera::SetZoom(float zoom)
{
    _zoom = Max(zoom, M_EPSILON);
}

void ACamera::SetLodBias(float bias)
{
    _lodBias = Max(bias, M_EPSILON);
}

void ACamera::SetLayoutMask(unsigned mask)
{
	_viewLayoutMask = mask;
}

void ACamera::SetLayoutMaskIndex(unsigned maskIndex)
{
	_viewLayoutMask &= ~(1 << maskIndex);
}

void ACamera::SetLayoutMaskName(const FString& name)
{
	AScene* scene = ParentScene();
	if (!scene)
		return;

	const THashMap<FString, unsigned char>& layous = scene->Layers();

	auto it = layous.Find(name);
	if (it != layous.End())
		_viewLayoutMask &= ~(1 << it->_second);
	else
		ErrorString("Layer" + name + " not defined in the scene");
}

void ACamera::SetLayoutMaskOutIndex(unsigned maskIndex)
{
	_viewLayoutMask |= 1 << maskIndex;
}

void ACamera::SetLayoutMaskOutName(const FString& name)
{
	AScene* scene = ParentScene();
	if (!scene)
		return;

	const THashMap<FString, unsigned char>& layous = scene->Layers();

	auto it = layous.Find(name);
	if (it != layous.End())
		_viewLayoutMask |= 1 << it->_second;
	else
		ErrorString("Layer" + name + " not defined in the scene");
}

void ACamera::SetLayoutMaskAll()
{
	_viewLayoutMask = 0;
}

void ACamera::SetOrthographic(bool enable)
{
    _orthographic = enable;
}

void ACamera::SetAmbientColor(const FColor& color)
{
    _ambientColor = color;
}

void ACamera::SetProjectionOffset(const TVector2F& offset)
{
    _projectionOffset = offset;
}

void ACamera::SetUseReflection(bool enable)
{
    _useReflection = enable;
    _viewMatrixDirty = true;
}

void ACamera::SetReflectionPlane(const FPlane& plane)
{
    _reflectionPlane = plane;
    _reflectionMatrix = plane.ReflectionMatrix();
    _viewMatrixDirty = true;
}

void ACamera::SetUseClipping(bool enable)
{
    _useClipping = enable;
}

void ACamera::SetClipPlane(const FPlane& plane)
{
    _clipPlane = plane;
}


void ACamera::SetFlipVertical(bool enable)
{
    _flipVertical = enable;
}

float ACamera::GetNearClip() const
{
    // Orthographic camera has always near clip at 0 to avoid trouble with shader depth parameters,
    // and unlike in perspective mode there should be no depth buffer precision issue
    return _orthographic ? 0.0f : _nearClip;
}

FFrustum ACamera::GetWorldFrustum() const
{
    FFrustum ret;
    TMatrix3x4F worldTransform = EffectiveWorldTransform();

    if (!_orthographic)
        ret.Define(_fov, _aspectRatio, _zoom, GetNearClip(), _farClip, worldTransform);
    else
        ret.DefineOrtho(_orthoSize, _aspectRatio, _zoom, GetNearClip(), _farClip, worldTransform);

    return ret;
}

FFrustum ACamera::WorldSplitFrustum(float nearClip, float farClip) const
{
    FFrustum ret;
    TMatrix3x4F worldTransform = EffectiveWorldTransform();

    nearClip = Max(nearClip, GetNearClip());
    farClip = Min(farClip, _farClip);
    if (farClip < nearClip)
        farClip = nearClip;

    if (!_orthographic)
        ret.Define(_fov, _aspectRatio, _zoom, nearClip, farClip, worldTransform);
    else
        ret.DefineOrtho(_orthoSize, _aspectRatio, _zoom, nearClip, farClip, worldTransform);

    return ret;
}

FFrustum ACamera::GetViewSpaceFrustum() const
{
    FFrustum ret;

    if (!_orthographic)
        ret.Define(_fov, _aspectRatio, _zoom, GetNearClip(), _farClip);
    else
        ret.DefineOrtho(_orthoSize, _aspectRatio, _zoom, GetNearClip(), _farClip);

    return ret;
}

FFrustum ACamera::ViewSpaceSplitFrustum(float nearClip, float farClip) const
{
    FFrustum ret;

    nearClip = Max(nearClip, GetNearClip());
    farClip = Min(farClip, _farClip);
    if (farClip < nearClip)
        farClip = nearClip;

    if (!_orthographic)
        ret.Define(_fov, _aspectRatio, _zoom, nearClip, farClip);
    else
        ret.DefineOrtho(_orthoSize, _aspectRatio, _zoom, nearClip, farClip);

    return ret;
}

const TMatrix3x4F& ACamera::GetViewMatrix() const
{
    if (_viewMatrixDirty)
    {
        _viewMatrix = EffectiveWorldTransform().Inverse();
        _viewMatrixDirty = false;
    }

    return _viewMatrix;
}

TMatrix4x4F ACamera::GetProjectionMatrix(bool apiSpecific) const
{
    TMatrix4x4F ret(TMatrix4x4F::ZERO);

    bool openGLFormat = apiSpecific;

    // Whether to construct matrix using OpenGL or Direct3D clip space convention
    #ifndef AUTO_OPENGL
    openGLFormat = false;
    #endif

    if (!_orthographic)
    {
        float h = (1.0f / tanf(_fov * M_DEGTORAD * 0.5f)) * _zoom;
        float w = h / _aspectRatio;
        float q, r;

        if (openGLFormat)
        {
            q = (_farClip + _nearClip) / (_farClip - _nearClip);
            r = -2.0f * _farClip * _nearClip / (_farClip - _nearClip);
        }
        else
        {
            q = _farClip / (_farClip - _nearClip);
            r = -q * _nearClip;
        }

        ret._m00 = w;
        ret._m02 = _projectionOffset._x * 2.0f;
        ret._m11 = h;
        ret._m12 = _projectionOffset._y * 2.0f;
        ret._m22 = q;
        ret._m23 = r;
        ret._m32 = 1.0f;
    }
    else
    {
        // Disregard near clip, because it does not affect depth precision as with perspective projection
        float h = (1.0f / (_orthoSize * 0.5f)) * _zoom;
        float w = h / _aspectRatio;
        float q, r;

        if (openGLFormat)
        {
            q = 2.0f / _farClip;
            r = -1.0f;
        }
        else
        {
            q = 1.0f / _farClip;
            r = 0.0f;
        }

        ret._m00 = w;
        ret._m03 = _projectionOffset._x * 2.0f;
        ret._m11 = h;
        ret._m13 = _projectionOffset._y * 2.0f;
        ret._m22 = q;
        ret._m23 = r;
        ret._m33 = 1.0f;
    }

    if (_flipVertical)
        ret = flipMatrix * ret;

    return ret;
}

void ACamera::FrustumSize(TVector3F& near, TVector3F& far) const
{
    near._z = GetNearClip();
    far._z = _farClip;

    if (!_orthographic)
    {
        float halfViewSize = tanf(_fov * M_DEGTORAD * 0.5f) / _zoom;
        near._y = near._z * halfViewSize;
        near._x = near._y * _aspectRatio;
        far._y = far._z * halfViewSize;
        far._x = far._y * _aspectRatio;
    }
    else
    {
        float halfViewSize = _orthoSize * 0.5f / _zoom;
        near._y = far._y = halfViewSize;
        near._x = far._x = near._y * _aspectRatio;
    }

    if (_flipVertical)
    {
        near._y = -near._y;
        far._y = -far._y;
    }
}

float ACamera::GetHalfViewSize() const
{
    if (!_orthographic)
        return tanf(_fov * M_DEGTORAD * 0.5f) / _zoom;
    else
        return _orthoSize * 0.5f / _zoom;
}

FRay ACamera::ScreenRay(float x, float y) const
{
    FRay ret;

    // If projection is invalid, just return a ray pointing forward
    if (!IsProjectionValid())
    {
        ret._origin = GetWorldPosition();
        ret._direction = GetWorldDirection();
        return ret;
    }

    TMatrix4x4F viewProjInverse = (GetProjectionMatrix(false) * GetViewMatrix()).Inverse();

    // The parameters range from 0.0 to 1.0. Expand to normalized device coordinates (-1.0 to 1.0) & flip Y axis
    x = 2.0f * x - 1.0f;
    y = 1.0f - 2.0f * y;
    TVector3F near(x, y, 0.0f);
    TVector3F far(x, y, 1.0f);

    ret._origin = viewProjInverse * near;
    ret._direction = ((viewProjInverse * far) - ret._origin).Normalized();
    return ret;
}

TVector2F ACamera::WorldToScreenPoint(const TVector3F& worldPos) const
{
    TVector3F eyeSpacePos = GetViewMatrix() * worldPos;
    TVector2F ret;

    if (eyeSpacePos._z > 0.0f)
    {
        TVector3F screenSpacePos = GetProjectionMatrix(false) * eyeSpacePos;
        ret._x = screenSpacePos._x;
        ret._y = screenSpacePos._y;
    }
    else
    {
        ret._x = (-eyeSpacePos._x > 0.0f) ? -1.0f : 1.0f;
        ret._y = (-eyeSpacePos._y > 0.0f) ? -1.0f : 1.0f;
    }

    ret._x = (ret._x * 0.5f) + 0.5f;
    ret._y = 1.0f - ((ret._y * 0.5f) + 0.5f);
    return ret;
}

TVector3F ACamera::ScreenToWorldPoint(const TVector3F& screenPos) const
{
    FRay ray = ScreenRay(screenPos._x, screenPos._y);
    return ray._origin + ray._direction * screenPos._z;
}

float ACamera::Distance(const TVector3F& worldPos) const
{
    if (!_orthographic)
        return (worldPos - GetWorldPosition()).Length();
    else
        return Abs((GetViewMatrix() * worldPos)._z);
}

float ACamera::LodDistance(float distance, float scale, float bias) const
{
    float d = Max(_lodBias * bias * scale * _zoom, M_EPSILON);
    if (!_orthographic)
        return distance / d;
    else
        return _orthoSize / d;
}

FQuaternion ACamera::FaceCameraRotation(const TVector3F& position, const FQuaternion& rotation, EFaceCameraMode::Type mode)
{
    switch (mode)
    {
    default:
        return rotation;

    case EFaceCameraMode::ROTATE_XYZ:
        return GetWorldRotation();

    case EFaceCameraMode::ROTATE_Y:
        {
            TVector3F euler = rotation.EulerAngles();
            euler._y = GetWorldRotation().EulerAngles()._y;
            return FQuaternion(euler._x, euler._y, euler._z);
        }

    case EFaceCameraMode::LOOKAT_XYZ:
        {
            FQuaternion lookAt;
            lookAt.FromLookRotation(position - GetWorldPosition());
            return lookAt;
        }

    case EFaceCameraMode::LOOKAT_Y:
        {
            // Make the Y-only lookat happen on an XZ plane to make sure there are no unwanted transitions
            // or singularities
            TVector3F lookAtVec(position - GetWorldPosition());
            lookAtVec._y = 0.0f;

            FQuaternion lookAt;
            lookAt.FromLookRotation(lookAtVec);

            TVector3F euler = rotation.EulerAngles();
            euler._y = lookAt.EulerAngles()._y;
            return FQuaternion(euler._x, euler._y, euler._z);
        }
    }
}

TMatrix3x4F ACamera::EffectiveWorldTransform() const
{
    TMatrix3x4F worldTransform(GetWorldPosition(), GetWorldRotation(), 1.0f);
    return _useReflection ? _reflectionMatrix * worldTransform : worldTransform;
}

bool ACamera::IsProjectionValid() const
{
    return _farClip > GetNearClip();
}

void ACamera::OnTransformChanged()
{
    ASpatialNode::OnTransformChanged();

    _viewMatrixDirty = true;
}

void ACamera::SetReflectionPlaneAttr(const TVector4F& value)
{
    SetReflectionPlane(FPlane(value));
}

void ACamera::SetClipPlaneAttr(const TVector4F& value)
{
    SetClipPlane(FPlane(value));
}

TVector4F ACamera::ReflectionPlaneAttr() const
{
    return _reflectionPlane.ToVector4();
}

TVector4F ACamera::ClipPlaneAttr() const
{
    return _clipPlane.ToVector4();
}

}
