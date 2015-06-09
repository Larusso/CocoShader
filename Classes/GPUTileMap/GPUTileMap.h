#ifndef __TileMap_H_
#define __TileMap_H_

#ifndef GPU_TILEMAP_DEBUG_DRAW
#define GPU_TILEMAP_DEBUG_DRAW 1
#endif //GPU_TILEMAP_DEBUG_DRAW

#include <vector>
#include "GPUTileMapLayer.h"

class Texture2D;

USING_NS_CC;

class GPUTileMap : public Node
{

public:
	static GPUTileMap* create(const std::string& filename);

	virtual void draw(Renderer *renderer, Mat4 const &transform, uint32_t flags) override;

CC_CONSTRUCTOR_ACCESS:

	GPUTileMap();
	virtual ~GPUTileMap();

	virtual bool initWithFile(const std::string& filename);

protected:
	NS_CC::Texture2D*			_tileset;
	NS_CC::Vector<GPUTileMapLayer *> _layers;
	V3F_C4B_T2F_Quad _quad;
public:
	virtual void setContentSize(cocos2d::Size const &contentSize) override;
	virtual void setMapOffset(cocos2d::Point const &mapPosition);

	void updateQuad();
	Vec2 _inverseSpriteTextureSize;
	Vec2 _tileSize;
	int _insideBounds;

	void updateGLProgramStateForLayer(GLProgramState *state, cocos2d::Texture2D *layer);

	void updateLayerContentSize();
#if GPU_TILEMAP_DEBUG_DRAW
	DrawNode *_debugDrawNode;
#endif //GPU_TILEMAP_DEBUG_DRAW

	virtual void updateTransform() override;
};

#endif //__TileMap_H_
