#include "GPUTileMapLayer.h"

USING_NS_CC;

GPUTileMapLayer *GPUTileMapLayer::create(GPUTileMapLayerInfo *layerInfo)
{
	GPUTileMapLayer *layer = new(std::nothrow) GPUTileMapLayer();
	if (layer && layer->initWithLayerInfo(layerInfo))
	{
		layer->autorelease();
		return layer;
	}

	CC_SAFE_DELETE(layer);
	return nullptr;
}

bool GPUTileMapLayer::initWithLayerInfo(GPUTileMapLayerInfo *layerInfo)
{
	if(layerInfo)
	{
		_map = layerInfo->getMap();
		_map->setAliasTexParameters();

		_tileset = layerInfo->getTileset();
		_tileset->setAliasTexParameters();

		_inverseTextureSize = Vec2( 1 / _map->getContentSize().width, 1 / _map->getContentSize().height);
		_inverseSpriteTextureSize = Vec2( 1 / _tileset->getContentSize().width, 1 / _tileset->getContentSize().height);

		_tileSize = layerInfo->tileSize;

		_offset = layerInfo->offset;

		auto program = GLProgramCache::getInstance()->getGLProgram("GPUTileMapShader");
		if (!program)
		{
			program = GLProgram::createWithFilenames("map.vs", "map.fs");
			program->link();
			program->updateUniforms();
			GLProgramCache::getInstance()->addGLProgram(program, "GPUTileMapShader");
		}

		setGLProgramState(GLProgramState::create(program));
		updateShaderState();

		return true;
	}

	return false;
}

void GPUTileMapLayer::updateShaderState()
{
	auto state = getGLProgramState();
	state->setUniformVec2("u_inverseSpriteTextureSize", _inverseSpriteTextureSize);
	state->setUniformVec2("u_tileSize", _tileSize);
	state->setUniformVec2("u_inverseTileSize", Vec2(1 / _tileSize.x,1 / _tileSize.y));
	state->setUniformVec2("u_viewportSize", getContentSize());
	state->setUniformVec2("u_viewOffset",  Vec2(0,0));

	state->setUniformTexture("u_sprites", _tileset);
	state->setUniformVec2("u_spriteTextureSize", _tileset->getContentSize());

	state->setUniformTexture("u_tiles", _map);
	state->setUniformVec2("u_inverseTileTextureSize", _inverseTextureSize);
}

void GPUTileMapLayer::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	_quadCommand.init(_globalZOrder, _tileset->getName(), getGLProgramState(), BlendFunc::ALPHA_NON_PREMULTIPLIED, &_quad, 1, transform, flags);
	renderer->addCommand(&_quadCommand);
}

Vec2 GPUTileMapLayer::getMapInverseTextureSize()
{
	return _inverseTextureSize;
}

Vec2 GPUTileMapLayer::getMapSize()
{
	return _map->getContentSize();
}

GPUTileMapLayer::GPUTileMapLayer(void)
		: _map(nullptr),
		_tileset(nullptr),
		_inverseTextureSize(Size::ZERO),
		_inverseSpriteTextureSize(Size::ZERO),
		_offset(Point::ZERO),
		_tileSize(Size::ZERO),
		_quad()
{
}

GPUTileMapLayer::~GPUTileMapLayer()
{

}

void GPUTileMapLayer::setPosition(const Vec2 &position)
{
	Node::setPosition(position);
}

void GPUTileMapLayer::updateQuad()
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

void GPUTileMapLayer::setContentSize(const NS_CC::Size &contentSize)
{
	Node::setContentSize(contentSize);
	updateQuad();
	updateShaderState();
}
