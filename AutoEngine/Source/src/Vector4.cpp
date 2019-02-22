#include "Vector4.h"

namespace Auto3D {

const Vector4F Vector4F::ZERO;
const Vector4F Vector4F::ONE(1.0f, 1.0f, 1.0f, 1.0f);

const Vector4I Vector4I::ZERO;
const Vector4I Vector4I::ONE(1, 1, 1, 1);

const Vector4C Vector4C::ZERO;
const Vector4C Vector4C::ONE(1, 1, 1, 1);

const Vector4D Vector4D::ZERO;
const Vector4D Vector4D::ONE(1.0, 1.0, 1.0, 1.0);

template<typename _Ty> STRING Vector4<_Ty>::ToString() const
{
	char tempBuffer[KhSTL::CONVERSION_BUFFER_LENGTH];
	sprintf(tempBuffer, "%g %g %g %g", _x, _y, _z, _w);
	return STRING(tempBuffer);
}

}
