#ifndef __GPUTileMapLayer_H_
#define __GPUTileMapLayer_H_

class Texture2D;

USING_NS_CC;

class GPUTileMapLayer : NS_CC::Sprite
{
public:

	static GPUTileMapLayer* create(const std::string& filename);
	/*static GPUTileMapLayer* create();
	static GPUTileMapLayer* createWithTexture(NS_CC::Texture2D *texture);

	virtual void setTexture(const std::string &filename );
	virtual void setTexture(NS_CC::Texture2D *texture);*/

	Vec2 getInverseTextureSize();

	Vec2 getTextureSize();

private:
	virtual bool initWithFile(const std::string &filename) override;
	Vec2 _inverseTextureSize;
};

#endif //__GPUTileMapLayer_H_
