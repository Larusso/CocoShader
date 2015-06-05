#include <MacTypes.h>
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

void GPUTileMap::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	int count = 0;
	for (auto item : _layers)
	{
		item->quadCommand.init(_globalZOrder + count, _tileset->getName(), item, BlendFunc::ALPHA_PREMULTIPLIED, &_quad, 1, transform, flags);
		renderer->addCommand(&item->quadCommand);
		count++;
	}
}

void GPUTileMap::updateGLProgramStateForLayer(GLProgramState *state, cocos2d::Texture2D *layer)
{
	state->setUniformVec2("u_inverseSpriteTextureSize", _inverseSpriteTextureSize);
	state->setUniformVec2("u_tileSize", _tileSize);
	state->setUniformVec2("u_inverseTileSize", Vec2(1 / _tileSize.x,1 / _tileSize.y));
	state->setUniformVec2("u_viewportSize", getContentSize());
	state->setUniformVec2("u_viewOffset", Vec2(0,0));

	state->setUniformTexture("u_sprites", _tileset);
	state->setUniformVec2("u_spriteTextureSize", _tileset->getContentSize());

	state->setUniformTexture("u_tiles", layer);
	state->setUniformVec2("u_inverseTileTextureSize", Vec2(1 / layer->getContentSize().width,1 / layer->getContentSize().height));

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
		auto program = GLProgramCache::getInstance()->getGLProgram("GPUTileMapShader");
		if (!program)
		{
			program = GLProgram::createWithFilenames("map.vs", "map.fs");
			program->link();
			//program->updateUniforms();
			GLProgramCache::getInstance()->addGLProgram(program, "GPUTileMapShader");
		}

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
			auto map = Director::getInstance()->getTextureCache()->addImage(layerMap);
			map->setAliasTexParameters();
			auto state = GPUTileMapLayer::create(program);
			updateGLProgramStateForLayer((GLProgramState *)state, map);

			_layers.insert(_layers.size(), state);

		}

		status = true;

		updateQuad();
	}

	return status;
}

void GPUTileMap::updateQuad()
{
	float x1 = 0.0f;
	float y1 = 0.0f;
	float x2 = x1 + getContentSize().width;
	float y2 = y1 + getContentSize().height;

	// Don't update Z.
	_quad.bl.vertices.set(x1, y1, 0.0f);
	_quad.br.vertices.set(x2, y1, 0.0f);
	_quad.tl.vertices.set(x1, y2, 0.0f);
	_quad.tr.vertices.set(x2, y2, 0.0f);

	_quad.bl.texCoords.u = 0;
	_quad.bl.texCoords.v = 1;

	_quad.br.texCoords.u = 1;
	_quad.br.texCoords.v = 1;

	_quad.tl.texCoords.u = 0;
	_quad.tl.texCoords.v = 0;

	_quad.tr.texCoords.u = 1;
	_quad.tr.texCoords.v = 0;

	Color4B color4( 255,255,255,255 );
	_quad.bl.colors = color4;
	_quad.br.colors = color4;
	_quad.tl.colors = color4;
	_quad.tr.colors = color4;
}


void GPUTileMap::setContentSize(const NS_CC::Size &contentSize)
{
	Node::setContentSize(contentSize);
	updateQuad();
	updateLayerContentSize();
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

void GPUTileMap::updateLayerContentSize()
{
	for (auto item : _layers)
	{
		item->setUniformVec2("u_viewportSize", getContentSize());
	}
}
