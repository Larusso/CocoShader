#ifndef __GPUTileMapLayerInfo_H_
#define __GPUTileMapLayerInfo_H_

class Texture2D;

USING_NS_CC;

class GPUTileMapLayerInfo : public Ref
{
public:
	static GPUTileMapLayerInfo* create(const std::string& map,const std::string& tileset, Vec2 offset, Vec2 tileSize);

CC_CONSTRUCTOR_ACCESS:

	GPUTileMapLayerInfo();
	virtual ~GPUTileMapLayerInfo();

	virtual bool initWithLayerValues(const std::string &mapSource, const std::string &tilesetSource, Vec2 offset, Vec2 tileSize);

	virtual NS_CC::Texture2D* getTileset();
	virtual NS_CC::Texture2D* getMap();

	Vec2 tileSize;
	Vec2 offset;

protected:
	NS_CC::Texture2D* _tileset;
	NS_CC::Texture2D* _map;


};


#endif //__GPUTileMapLayerInfo_H_
