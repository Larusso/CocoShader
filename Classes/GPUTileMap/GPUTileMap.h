#ifndef __TileMap_H_
#define __TileMap_H_
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
	//virtual void customDraw(const Mat4 &transform, uint32_t flags);
	NS_CC::Texture2D*			_tileset;
	NS_CC::Vector<GPUTileMapLayer *> _layers;
	QuadCommand _quadCommand;
	V3F_C4B_T2F_Quad _quad;
public:
	virtual void setContentSize(cocos2d::Size const &contentSize) override;

	void updateQuad();
	Vec2 _inverseSpriteTextureSize;
	Vec2 _tileSize;

	void updateGLProgramStateForLayer(GLProgramState *state, cocos2d::Texture2D *layer);

	void updateLayerContentSize();
};

#endif //__TileMap_H_
