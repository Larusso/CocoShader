#include "GPUTileMapLayer.h"

USING_NS_CC;

GPUTileMapLayer *GPUTileMapLayer::create(GLProgram *glprogram)
{
	GPUTileMapLayer* ret = nullptr;
	ret = new (std::nothrow) GPUTileMapLayer();
	if(ret && ret->init(glprogram))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}
