#!/usr/local/bin/python3
#coding: utf-8

""" creates a 'Tiled' tile map from a given level image and a tileset
    You can create a tileset from a level image with 'Image2Map'
"""

__author___ = "Bjorn Lindeijer, Georg Muntingh, Manfred Endres"
__versiom__ = "2.0"
__copyright__ = "Public Domain"
__date__ = "September 11, 2013"

import os
import sys
import math
import struct
import base64
import zlib
import argparse

from PIL import Image
from xml.dom.minidom import Document

class Tileset:
    """ This class represents a set of tiles.
    """

    def __init__(self, tileImageFile, tileWidth, tileHeight):
        self.TileWidth = tileWidth
        self.TileHeight = tileHeight
        self.Filename = tileImageFile
        self.Name = os.path.splitext(os.path.basename(tileImageFile))[0]
        self.List = []
        self.TileDict = {}
        self.readTiles()

    def readTiles(self):
        """ This method reads the tiles from the tileset and also fills up the tile dictionary.
        """
        TileImage = Image.open(self.Filename).convert("RGB")
        TileIW, TileIH = TileImage.size
        TilesetW, TilesetH = TileIW // self.TileWidth, TileIH // self.TileHeight

        for y in range(TilesetH):
            for x in range(TilesetW):
                box = self.TileWidth * x, self.TileHeight * y, self.TileWidth * (x+1), self.TileHeight * (y+1)
                tile = TileImage.crop(box)
                self.List.append(tile)

                str = tile.tostring()
                if not str in self.TileDict:
                    #print("add tile: ", str)
                    self.TileDict[str] = len(self.List) - 1
        print("tile count: {}, unique count: {}".format(len(self.List),len(self.TileDict.values())))

    def findTile(self, tileImage):
        """ This method returns the tile index for the given tile image if it is part of this tileset,
            and returns 0 if the tile could not be found. Constant complexity due to dictionary lookup.
        """
        str = tileImage.tostring()
        if str in self.TileDict:
            return self.TileDict[str] + 1
        else:
            return 0

class TileMap:
    """ This class represents a tile map.
    """

    def __init__(self, mapImageFile, tileSet, tileWidth, tileHeight):
        self.TileWidth = tileWidth
        self.TileHeight = tileHeight
        self.TileSet = tileSet
        self.List = []
        self.MapImageFile = mapImageFile
        self.readMap()

    def readMap(self):
        """ This function takes the map image, and obtains a list self.List, where
            an entry equals i if self.TileSet.List[i-1] is the corresponding picture on the map
            image. If a matching tile is not found, i is set to 0.
        """
        MapImage = Image.open(self.MapImageFile).convert("RGB")
        MapImageWidth, MapImageHeight = MapImage.size
        self.Width, self.Height = MapImageWidth // self.TileWidth, MapImageHeight // self.TileHeight
        progress = -1

        for y in range(self.Height):
            for x in range(self.Width):
                box = self.TileWidth * x, self.TileHeight * y, self.TileWidth * (x+1), self.TileHeight * (y+1)
                tile = MapImage.crop(box)
                self.List.append(self.TileSet.findTile(tile))

                # Calculate the progress, and print it to the screen.
                p = ((x + y * self.Width) * 100) / (self.Width * self.Height)
                if progress != p:
                    progress = p
                    self.printProgress(progress)

        self.printProgress(100)

    def printProgress(self, percentage):
        """ This function prints the percentage on the current row after erasing what is already there.
        """
        #print '%s\r' % ' '*20,       # clean up row
        #print '%3d%% ' % percentage, # ending with comma prevents newline from being appended
        sys.stdout.flush()

    def write(self, fileName):
        doc = Document()
        map = doc.createElement("map")
        map.setAttribute("version", "0.99b")
        map.setAttribute("orientation", "orthogonal")
        map.setAttribute("width", str(self.Width))
        map.setAttribute("height", str(self.Height))
        map.setAttribute("tilewidth", str(self.TileWidth))
        map.setAttribute("tileheight", str(self.TileHeight))
        tileset = doc.createElement("tileset")
        tileset.setAttribute("name", self.TileSet.Name)
        tileset.setAttribute("firstgid", str(1))
        tileset.setAttribute("tilewidth", str(self.TileSet.TileWidth))
        tileset.setAttribute("tileheight", str(self.TileSet.TileHeight))
        image = doc.createElement("image")
        image.setAttribute("source", os.path.basename(self.TileSet.Filename))
        tileset.appendChild(image)
        map.appendChild(tileset)
        layer = doc.createElement("layer")
        layer.setAttribute("name", "Ground")
        layer.setAttribute("width", str(self.Width))
        layer.setAttribute("height", str(self.Height))
        data = doc.createElement("data")

        data.setAttribute("encoding", "base64")
        TileData = bytearray()
        for tileId in self.List:
            TileData = TileData + struct.pack("<l", tileId)  # pack the tileId into a long
        b64data = doc.createTextNode(base64.b64encode(TileData).decode())
        data.appendChild(b64data)

        layer.appendChild(data)
        map.appendChild(layer)
        doc.appendChild(map)
        file = open(fileName, "w")
        file.write(doc.toprettyxml(indent = " "))
        file.close()

def _mapWriter(args):
    map = TileMap(args.image, Tileset(args.tile_set, args.tile_width, args.tile_height), args.tile_width, args.tile_height)
    map.write(os.path.splitext(args.image)[0] + ".tmx")

def add_arguments_to_parser(mapWriter_parser, set_default_func=False):
    mapWriter_parser.add_argument('-tile_width', type=int, default=16)
    mapWriter_parser.add_argument('-tile_height', type=int, default=16)
    mapWriter_parser.add_argument('image', type=str, help='image to convert')
    mapWriter_parser.add_argument('tile_set', type=str, help='tileset for conversion')

    if set_default_func:
        mapWriter_parser.set_defaults(func=_mapWriter)

if __name__ == "__main__":
    
    parser = argparse.ArgumentParser(description=__doc__)
    add_arguments_to_parser(parser)
    args = parser.parse_args()

    _mapWriter(args)
    

