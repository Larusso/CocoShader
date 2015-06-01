#ifndef __GPUTileMapLayer_H_
#define __GPUTileMapLayer_H_

#include "GPUTileMapLayerInfo.h"

class Texture2D;

USING_NS_CC;

class GPUTileMapLayer : public Node
{

public:
	static GPUTileMapLayer *create(GPUTileMapLayerInfo *layerInfo);

	Vec2 getMapInverseTextureSize();
	Vec2 getMapSize();
	virtual void draw(Renderer *renderer, Mat4 const &transform, uint32_t flags) override;
	virtual void setContentSize(cocos2d::Size const &contentSize) override;
	virtual void setPosition(const Vec2 &position) override;
	void updateQuad();

CC_CONSTRUCTOR_ACCESS:

	GPUTileMapLayer();
	virtual ~GPUTileMapLayer();

	virtual bool initWithLayerInfo(GPUTileMapLayerInfo *layerInfo);

protected:
	Vec2 _inverseTextureSize;
	Vec2 _inverseSpriteTextureSize;
	Vec2 _tileSize;
	Vec2 _offset;
	cocos2d::Texture2D *_map;
	cocos2d::Texture2D *_tileset;
	V3F_C4B_T2F_Quad _quad;
	QuadCommand _quadCommand;

	void updateShaderState();
};

#endif //__GPUTileMapLayer_H_
