#include "GPUTileMap.h"
#include "json/rapidjson.h"
#include "json/document.h"

USING_NS_CC;
using namespace rapidjson;

GPUTileMap *GPUTileMap::create(const std::string &filename)
{
	GPUTileMap *map = new(std::nothrow) GPUTileMap();
	if (map && map->initWithFile(filename))
	{
		map->autorelease();
		return map;
	}
	CC_SAFE_DELETE(map);
	return nullptr;
}

bool GPUTileMap::initWithFile(std::string const &filename)
{
	bool status = false;
	std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filename);
	std::string json = FileUtils::getInstance()->getStringFromFile(fullPath);

	Document document;
	document.Parse<0>(json.c_str());

	if (document.HasParseError())
	{
		CCLOG("GetParseError %s\n", document.GetParseError());
	}

	if (document.IsObject() && document.HasMember("name"))
	{
		CCLOG("%s\n", document["name"].GetString());
		_tileSize = Vec2(document["tileWidth"].GetInt(),document["tileHeight"].GetInt());

		const rapidjson::Value& tilesetValue = document["tileset"];
		const char *tilesetFilename = tilesetValue["source"].GetString();
		_tileset = Director::getInstance()->getTextureCache()->addImage(tilesetFilename);
		_tileset->setAliasTexParameters();
		_inverseSpriteTextureSize = Vec2(1 / _tileset->getContentSize().width, 1/_tileset->getContentSize().height);
		const rapidjson::Value& layers = document["layers"];
		for (SizeType i = 0; i < layers.Size(); i++)
		{
			const rapidjson::Value& layer = layers[i];
			const char *layerMap = layer["map"].GetString();

			auto layerInfo = GPUTileMapLayerInfo::create(layerMap, tilesetFilename, Vec2(0,0), _tileSize);
			GPUTileMapLayer *tileMapLayer = GPUTileMapLayer::create(layerInfo);
			this->addChild((Node *)tileMapLayer);
		}

		status = true;
	}

	return status;
}

void GPUTileMap::setContentSize(const NS_CC::Size &contentSize)
{
	Node::setContentSize(contentSize);
	for (auto layer : _children)
	{
		layer->setContentSize(contentSize);
	}
}

GPUTileMap::GPUTileMap(void)
		: _tileset(nullptr),
		_layers(0),
		_quad()
{
}

GPUTileMap::~GPUTileMap()
{
	CC_SAFE_RELEASE(_tileset);
}
