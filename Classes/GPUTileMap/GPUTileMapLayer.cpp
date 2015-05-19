#include "GPUTileMapLayer.h"

USING_NS_CC;

GPUTileMapLayer *GPUTileMapLayer::create(const std::string &filename)
{
	GPUTileMapLayer *layer = new(std::nothrow) GPUTileMapLayer();
	if (layer && layer->initWithFile(filename))
	{
		layer->autorelease();
		return layer;
	}

	CC_SAFE_DELETE(layer);
	return nullptr;
}
