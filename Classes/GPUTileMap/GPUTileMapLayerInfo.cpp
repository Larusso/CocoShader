#include "GPUTileMapLayerInfo.h"

GPUTileMapLayerInfo *GPUTileMapLayerInfo::create(const std::string &map, const std::string &tileset, Vec2 offset, Vec2 tileSize)
{
	GPUTileMapLayerInfo *layerInfo = new(std::nothrow) GPUTileMapLayerInfo();
	if (layerInfo && layerInfo->initWithLayerValues(map,tileset,offset,tileSize))
	{
		layerInfo->autorelease();
		return layerInfo;
	}
	CC_SAFE_DELETE(layerInfo);
	return nullptr;
}

bool GPUTileMapLayerInfo::initWithLayerValues(const std::string &mapSource, const std::string &tilesetSource, Vec2 offset, Vec2 tileSize)
{
	cocos2d::Texture2D *map = nullptr;
	cocos2d::Texture2D *tileSet = nullptr;

	map = Director::getInstance()->getTextureCache()->addImage(mapSource);
	tileSet = Director::getInstance()->getTextureCache()->addImage(tilesetSource);

	if(map && tileSet)
	{
		_tileset = tileSet;
		_map = map;
		this->offset = offset;
		this->tileSize = tileSize;

		return true;
	}
	return false;
}

GPUTileMapLayerInfo::GPUTileMapLayerInfo():
		_tileset(nullptr),
		_map(nullptr),
		offset(Point::ZERO),
		tileSize(Size::ZERO)
{

}

GPUTileMapLayerInfo::~GPUTileMapLayerInfo()
{

}

NS_CC::Texture2D *GPUTileMapLayerInfo::getTileset()
{
	return _tileset;
}

NS_CC::Texture2D *GPUTileMapLayerInfo::getMap()
{
	return _map;
}
