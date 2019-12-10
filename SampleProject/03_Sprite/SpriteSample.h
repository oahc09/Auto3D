#pragma once

#include "../Sample.h"

using namespace Auto3D;

class SpriteSample : public Sample
{
	DECLARE_CLASS(SpriteSample, Sample)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;

	const int flowerNum = 200;
	struct FlowerMSG
	{
		FlowerMSG() = default;
		FlowerMSG(float _speed, float _rotateOffset):
			speed(_speed),
			rotateOffset(_rotateOffset)
		{}
		//Rang[0,10]
		float speed;
		//Rang[-1.0,1.0]
		float rotateOffset;
	};
	//First speed ,Second sprite
	TVector<TPair<FlowerMSG, ASprite2D*> > sprites;
	ACamera2D* camera2d;
};