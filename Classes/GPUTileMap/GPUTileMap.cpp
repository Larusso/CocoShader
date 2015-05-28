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
	GLProgramState *state = getGLProgramState();

	state->setUniformVec2("u_inverseSpriteTextureSize", _inverseSpriteTextureSize);
	state->setUniformVec2("u_tileSize", _tileSize);
	state->setUniformVec2("u_inverseTileSize", Vec2(1 / _tileSize.x,1 / _tileSize.y));
	state->setUniformVec2("u_viewportSize", this->getContentSize());
	state->setUniformVec2("u_viewOffset", Vec2(0,0));
	state->setUniformTexture("u_sprites", _tileset);
	state->setUniformVec2("u_spriteTextureSize", _tileset->getContentSize());

	for (auto item : _layers)
	{
		GPUTileMapLayer *layer = (GPUTileMapLayer *)item;
		state->setUniformTexture("u_tiles", item->getTexture());
		state->setUniformVec2("u_inverseTileTextureSize", layer->getInverseTextureSize());

		_quadCommand.init(_globalZOrder, _tileset->getName(), getGLProgramState(), BlendFunc::ALPHA_PREMULTIPLIED, &_quad, 1, transform, flags);
		renderer->addCommand(&_quadCommand);
	}
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
			GPUTileMapLayer *tileMapLayer = GPUTileMapLayer::create(layerMap);
			//this->addChild((Node *)tileMapLayer);

			_layers.insert(_layers.size(), (Sprite *)tileMapLayer);

		}

		status = true;

		auto program = GLProgramCache::getInstance()->getGLProgram("GPUTileMapShader");
		if (!program)
		{
			program = GLProgram::createWithFilenames("map.vs", "map.fs");
			program->link();
			program->updateUniforms();
			GLProgramCache::getInstance()->addGLProgram(program, "GPUTileMapShader");
		}

		//this->setGLProgram(program);
		setGLProgramState(GLProgramState::getOrCreateWithGLProgram(program));

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

	auto state = this->getGLProgramState();
	printf(state->getGLProgram()->getProgramLog().data());
}


void GPUTileMap::setContentSize(const NS_CC::Size &contentSize)
{
	Node::setContentSize(contentSize);
	updateQuad();
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
