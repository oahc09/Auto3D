#pragma once

#include "../../Base/AutoPtr.h"
#include "../GPUObject.h"
#include "../GraphicsDefs.h"

namespace Auto3D
{

/// GPU buffer for index data.
class AUTO_API IndexBuffer : public RefCounted, public GPUObject
{
public:
    /// Construct.
    IndexBuffer();
    /// Destruct.
    ~IndexBuffer();

    /// Release the index buffer and CPU shadow data.
    void Release() override;
    /// Recreate the GPU resource after data loss.
    void Recreate() override;

    /// Define buffer. Immutable buffers must specify initial data here.  Return true on success.
    bool Define(ResourceUsage usage, size_t numIndices, size_t indexSize, bool useShadowData, const void* data = nullptr);
    /// Redefine buffer data either completely or partially. Not supported for immutable buffers. Return true on success.
    bool SetData(size_t firstIndex, size_t numIndices, const void* data);

    /// Return CPU-side shadow data if exists.
    unsigned char* ShadowData() const { return _shadowData.Get(); }
    /// Return number of indices.
    size_t NumIndices() const { return _numIndices; }
    /// Return _size of index in bytes.
    size_t IndexSize() const { return _indexSize; }
    /// Return resource usage type.
    ResourceUsage Usage() const { return _usage; }
    /// Return whether is dynamic.
    bool IsDynamic() const { return _usage == USAGE_DYNAMIC; }
    /// Return whether is immutable.
    bool IsImmutable() const { return _usage == USAGE_IMMUTABLE; }

    /// Return the OpenGL buffer identifier. Used internally and should not be called by portable application code.
    unsigned GLBuffer() const { return _buffer; }

private:
    /// Create the GPU-side index buffer. Return true on success.
    bool Create(const void* data);

    /// OpenGL buffer object identifier.
    unsigned _buffer;
    /// CPU-side shadow data.
    AutoArrayPtr<unsigned char> _shadowData;
    /// Number of indices.
    size_t _numIndices;
    /// Size of index in bytes.
    size_t _indexSize;
    /// Resource usage type.
    ResourceUsage _usage;
};

}