Tilemap utils
============

Introduction
------------

The tile map util is a package of python modules dedicated for tilemaps

## Features
- split a level image into a tileset image
- split a tile set image into tiles
- convert a level image into a [Tiled][tiled_link] (*.tmx) tile map

## Usage

    usage: tileUtil.py [-h] {list2map,tileset_split,map_writer} ...

    positional arguments:
      {list2map,tileset_split,map_writer}
                            sub-command help
        list2map            creates tileset(s) from given image(s)
        tileset_split       splits tileset image in seperate images
        map_writer          creates a 'Tiled' tile map from a given level image
                            and a tileset You can create a tileset from a level
                            image with 'Image2Map'
    
    optional arguments:
      -h, --help            show this help message and exit


## Requirements

- python 2.x or 3.x 
- networkx
- PIP (python 2.x)
- Pillow (python 3.x)

## Credits

Credits goes to Georg Muntingh, Bjorn Lindeijer for the original python 2.x 'Image to Map' and 'MapWriter' [scripts][script_gist].

## License

Public Domain [see][license_link]


[tiled_link]: http://www.mapeditor.org/ "Tiled"
[script_gist]: https://gist.github.com/bjorn/5498157 "Original Python Scripts"
[license_link]: LICENSE.md "License Public Domain"





