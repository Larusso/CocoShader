#!/usr/local/bin/python3
#coding: utf-8

"""splits tileset image in seperate images"""

__author___ = "Manfred Endres"
__versiom__ = "0.0.1"
__copyright__ = "Public Domain"
__date__ = "September 11, 2013"

import argparse
import math
import os.path
from PIL import Image

def split_into_grid(image_path, **kwargs):

	images = dict()

	tile_width = kwargs.get('tile_width',16)
	tile_height = kwargs.get('tile_height',16)

	start_offset_x = kwargs.get('start_offset_x',0)
	start_offset_y = kwargs.get('start_offset_y',0)

	max_width = kwargs.get('max_width',-1)
	max_height = kwargs.get('max_height',-1)

	gap_x = kwargs.get('gap_x',0)
	gap_y = kwargs.get('gap_y',0)

	stitch = kwargs.get('stitch',False)

	image_file = Image.open(image_path)
	image_width, image_height = image_file.size

	if max_width > 0 and image_width > max_width:
		image_width = max_width

	if max_height > 0 and image_height > max_height:
		image_height = max_height

	tiles_count_x = int(math.floor((image_width - start_offset_x) / (tile_width + gap_x)))
	tiles_count_y = int(math.floor((image_height - start_offset_y) / (tile_height + gap_y)))

	print(tiles_count_x,tiles_count_y)

	image_width, image_height = image_file.size

	for columns in range(tiles_count_x):
		for rows in range(tiles_count_y):
			image_copy = image_file.copy()

			left = (columns * (tile_width + gap_x)) + start_offset_x
			top = (rows * (tile_height + gap_y)) + start_offset_y
			box = (left,top,left+tile_width,top+tile_height)
			
			nImage = image_copy.crop(box)

			basename = os.path.basename(image_path)
			image_base = os.path.splitext(basename)[0]

			images['/{}_{}_{}.png'.format(image_base,columns,rows)] = nImage
			#nImage.save(output_path + '/{}_{}_{}.png'.format(image_base,columns,rows) )  

	return images, tiles_count_x, tiles_count_y

def save_images(images, output_path):
	for key, image in images.items():
		print('save image: {}'.format(key))
		image.save(output_path + key)

def stitch_images(images, columns, rows, tile_width, tile_height):
	stitch_image = Image.new('RGBA',(columns * tile_width,rows * tile_height),(255,0,0, 0))
	print(stitch_image)
	for key, image in images.items():
		name_parts = os.path.splitext(key)[0].split('_')
		print(name_parts)
		row = int(name_parts[-2])
		column = int(name_parts[-1])
		stitch_image.paste(image,(row * tile_width, column * tile_height))

	return stitch_image

def _tileset_split(args):
	images, tiles_count_x, tiles_count_y = split_into_grid(args.tileset, tile_width=args.tile_width, tile_height=args.tile_height, gap_x=args.gap_x, gap_y=args.gap_y, start_offset_x=args.start_offset_x,start_offset_y=args.start_offset_y,stitch=args.stitch)

	if(args.export_parts):
		save_images(images, args.out)

	if(args.stitch):
		stitch_image = stitch_images(images, tiles_count_x, tiles_count_y, args.tile_width, args.tile_height)
		save_images({"/{}_stitch.png".format(os.path.splitext(args.tileset)[0]): stitch_image}, args.out)

def add_arguments_to_parser(parser, set_default_func=False):
	parser.add_argument('tileset', type=str)
	parser.add_argument('out', type=str)
	parser.add_argument('-tile_width', type=int, default=16)
	parser.add_argument('-tile_height', type=int, default=16)

	parser.add_argument('-gap_x', type=int, default=0)
	parser.add_argument('-gap_y', type=int, default=0)

	parser.add_argument('-start_offset_x', type=int, default=0)
	parser.add_argument('-start_offset_y', type=int, default=0)

	parser.add_argument('-max_width', type=int, default=-1)
	parser.add_argument('-max_height', type=int, default=-1)

	parser.add_argument('--stitch', action='store_true')
	parser.add_argument('--export_parts', action='store_false')

	if set_default_func:
		parser.set_defaults(func=_tileset_split)

if __name__ == "__main__":
	parser = argparse.ArgumentParser(description=__doc__)
	add_arguments_to_parser(parser)
	args = parser.parse_args()
	tileset_split(args)




