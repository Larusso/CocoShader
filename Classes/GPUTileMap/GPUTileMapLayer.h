#ifndef __GPUTileMapLayer_H_
#define __GPUTileMapLayer_H_

class Texture2D;

USING_NS_CC;

class GPUTileMapLayer : public NS_CC::GLProgramState
{
public:

	static GPUTileMapLayer *create(GLProgram *glprogram);

	QuadCommand quadCommand;
};

#endif //__GPUTileMapLayer_H_
