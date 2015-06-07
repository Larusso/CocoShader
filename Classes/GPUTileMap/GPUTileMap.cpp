#include "GPUTileMap.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "gpuTileMapShaders.h"

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
	_insideBounds = (flags & FLAGS_TRANSFORM_DIRTY) ? renderer->checkVisibility(transform, _contentSize) : _insideBounds;

	if(_insideBounds)
	{
		for (auto item : _layers)
		{
			item->quadCommand.init(_globalZOrder, _tileset->getName(), item, BlendFunc::ALPHA_PREMULTIPLIED, &_quad, 1, transform, flags);
			renderer->addCommand(&item->quadCommand);
		}
	}

#if	GPU_TILEMAP_DEBUG_DRAW
	_debugDrawNode->clear();
	Vec2 vertices[4] = {
			Vec2( _quad.bl.vertices.x, _quad.bl.vertices.y ),
			Vec2( _quad.br.vertices.x, _quad.br.vertices.y ),
			Vec2( _quad.tr.vertices.x, _quad.tr.vertices.y ),
			Vec2( _quad.tl.vertices.x, _quad.tl.vertices.y ),
	};
	_debugDrawNode->drawPoly(vertices, 4, true, Color4F(1.0, 1.0, 1.0, 1.0));

	Vec2 anchorPoint = this->_anchorPointInPoints;

	_debugDrawNode->drawDot(anchorPoint, 1.0, Color4F(1.0, 1.0, 1.0, 1.0));
#endif //GPU_TILEMAP_DEBUG_DRAW
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
		if( Node::init())
		{
			// default transform anchor: center
			setAnchorPoint(Vec2(0.5f, 0.5f));

			auto program = GLProgramCache::getInstance()->getGLProgram("GPUTileMapShader");
			if (!program)
			{
				program = GLProgram::createWithByteArrays(gpuTileMap_vert, gpuTileMap_frag);
				program->link();
				//program->updateUniforms();
				GLProgramCache::getInstance()->addGLProgram(program, "GPUTileMapShader");
			}

			CCLOG("%s\n", document["name"].GetString());
			_tileSize = Vec2(document["tileWidth"].GetInt(), document["tileHeight"].GetInt());

			const rapidjson::Value &tilesetValue = document["tileset"];
			const char *tilesetFilename = tilesetValue["source"].GetString();
			_tileset = Director::getInstance()->getTextureCache()->addImage(tilesetFilename);
			_tileset->setAliasTexParameters();
			_inverseSpriteTextureSize = Vec2(1 / _tileset->getContentSize().width, 1 / _tileset->getContentSize().height);
			const rapidjson::Value &layers = document["layers"];

			for (SizeType i = 0; i < layers.Size(); i++)
			{
				const rapidjson::Value &layer = layers[i];
				const char *layerMap = layer["map"].GetString();
				auto map = Director::getInstance()->getTextureCache()->addImage(layerMap);
				map->setAliasTexParameters();
				auto state = GPUTileMapLayer::create(program);
				updateGLProgramStateForLayer((GLProgramState *) state, map);

				_layers.insert(_layers.size(), state);
			}

			status = true;

			updateQuad();
		}
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


void GPUTileMap::setContentSize(const NS_CC::Size & contentSize)
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
#if GPU_TILEMAP_DEBUG_DRAW
    _debugDrawNode = DrawNode::create();
    addChild(_debugDrawNode);
#endif //GPU_TILEMAP_DEBUG_DRAW
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

void GPUTileMap::setMapOffset(const cocos2d::Point & mapPosition)
{
	for (auto item : _layers)
	{
		item->setUniformVec2("u_viewOffset", mapPosition);
	}
}
