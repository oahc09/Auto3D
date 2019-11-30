#pragma once

#include "Container/Ptr.h"
#include "AutoConfig.h"

namespace Auto3D
{

class AGraphics;

/// Base class for objects that allocate GPU resources.
class AUTO_API FGPUObject
{
public:
    /// Construct. Acquire the %AGraphics subsystem if available and register self.
    FGPUObject();
    /// Destruct. Unregister from the %AGraphics subsystem.
    virtual ~FGPUObject();
    
    /// Release the GPU resource.
    virtual void Release();
    /// Recreate the GPU resource after data loss. Not called on all rendering API's.
    virtual void Recreate();
    /// Return whether the contents have been lost due to graphics context having been destroyed.
    virtual bool IsDataLost() const { return _dataLost; }

	/// Clear the data lost flag.
	void ClearDataLost();
	/// Return the graphics subsystem associated with this GPU object.
	AGraphics* GetGraphics() const;
    /// Set data lost state. Not needed on all rendering API's.
    void SetDataLost(bool enable) { _dataLost = enable; }

protected:
    /// %AGraphics subsystem pointer.
    TWeakPtr<AGraphics> _graphics;

private:
    /// Data lost flag.
    bool _dataLost;
};

}
