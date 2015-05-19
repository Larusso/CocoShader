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
	//_customCommand.init(_globalZOrder + 10);
	//_customCommand.func = CC_CALLBACK_0(GPUTileMap::customDraw, this, transform, flags);


	float x1 = 0.0f;
	float y1 = 0.0f;
	float x2 = x1 + this->getContentSize().width;
	float y2 = y1 + this->getContentSize().height;

	// Don't update Z.
	_quad.bl.vertices.set(x1, y1, 0.0f);
	_quad.br.vertices.set(x2, y1, 0.0f);
	_quad.tl.vertices.set(x1, y2, 0.0f);
	_quad.tr.vertices.set(x2, y2, 0.0f);

	_quad.bl.texCoords.u = 1;
	_quad.bl.texCoords.v = 1;

	_quad.br.texCoords.u = 0;
	_quad.br.texCoords.v = 1;

	_quad.tl.texCoords.u = 1;
	_quad.tl.texCoords.v = 0;

	_quad.tr.texCoords.u = 0;
	_quad.tr.texCoords.v = 0;

	Color4B color4( 255,255,255,255 );
	_quad.bl.colors = color4;
	_quad.br.colors = color4;
	_quad.tl.colors = color4;
	_quad.tr.colors = color4;

	setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));

	_quadCommand.init(_globalZOrder, _tileset->getName(), getGLProgramState(), BlendFunc::ALPHA_PREMULTIPLIED, &_quad, 1, transform, flags);
	renderer->addCommand(&_quadCommand);

	//renderer->addCommand(&_customCommand);
	//Node::draw(renderer, transform, flags);
}

void GPUTileMap::customDraw(const Mat4 &transform, uint32_t flags)
{
	float selfHeight = this->getContentSize().height;
	float selfWidth = this->getContentSize().width;

	cocos2d::Point vertices[4] = {cocos2d::Point(300.f, 300.f), cocos2d::Point(10.f, selfHeight - 10), cocos2d::Point(selfWidth - 10, selfHeight - 10), cocos2d::Point(selfWidth - 10, 10.f)};

	cocos2d::DrawPrimitives::drawSolidPoly(vertices, 4, Color4F(0, 1, 0, 1));
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

		const rapidjson::Value& tilesetValue = document["tileset"];
		const char *tilesetFilename = tilesetValue["source"].GetString();
		_tileset = Director::getInstance()->getTextureCache()->addImage(tilesetFilename);

		auto drawSprite = Sprite::create(tilesetFilename);

		//drawSprite->setTextureRect(Rect(0,0,this->getContentSize().width, this->getContentSize().height));
		//this->addChild(drawSprite);



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
	}

	return status;
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
