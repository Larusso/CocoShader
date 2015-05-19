#ifndef __GPUTileMapLayer_H_
#define __GPUTileMapLayer_H_

class Texture2D;

NS_CC_BEGIN

class GPUTileMapLayer : NS_CC::Sprite
{
public:

	static GPUTileMapLayer* create(const std::string& filename);
	/*static GPUTileMapLayer* create();
	static GPUTileMapLayer* createWithTexture(NS_CC::Texture2D *texture);

	virtual void setTexture(const std::string &filename );
	virtual void setTexture(NS_CC::Texture2D *texture);*/
};

NS_CC_END
#endif //__GPUTileMapLayer_H_
