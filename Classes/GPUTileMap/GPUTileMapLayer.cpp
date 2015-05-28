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


bool GPUTileMapLayer::initWithFile(const std::string &filename)
{
	bool status = Sprite::initWithFile(filename);
	if(status)
	{
		cocos2d::Texture2D *tex = this->getTexture();
		//tex->setAntiAliasTexParameters();
		tex->setAliasTexParameters();
		_inverseTextureSize = Vec2( 1 / tex->getContentSize().width, 1 / tex->getContentSize().height);
	}
	return status;
}

Vec2 GPUTileMapLayer::getInverseTextureSize()
{
	return _inverseTextureSize;
}

Vec2 GPUTileMapLayer::getTextureSize()
{
	return this->getTexture()->getContentSize();
}
