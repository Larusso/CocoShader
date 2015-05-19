#!/usr/local/bin/python3
#coding: utf-8

""" converts a Tiled *.tmx map into a bitmap representation
	each layer will be saved as a seperate image"""

__author___ = "Manfred Endres"
__versiom__ = "0.0.1"
__copyright__ = "Public Domain"
__date__ = "September 13, 2013"

import argparse
import os
from tilemap.tmx import TMXMap
from PIL import Image

def generate_image(layer):
	layer_image = Image.new('RGBA', (layer.width, layer.height),(0,0,0,255))
	pix = layer_image.load()
	for index, value in enumerate(layer.data):
		px = index % layer.width
		py = index // layer.width
		pix[px,py] = value | (0xFF << 24)
	return layer_image

def map2bitmap(tilemap_path, file_type):
	map = TMXMap(open(tilemap_path,'r').read(),file_type)

	for layer in map.layers:
		image = generate_image(layer)
		image.save("{}/layer_{}.png".format(os.path.dirname(tilemap_path), layer.name))

def _map2bitmap(args):
	map2bitmap(args.tilemap, args.map_type)

def add_arguments_to_parser(parser, set_default_func=False):

	parser.add_argument('tilemap',type=str, help='path to the *tmx or json tilemap to convert')
	parser.add_argument('-map_type', type=str, choices=['json','tmx'], default='json', help='map type, default *.json')

	if set_default_func:
		parser.set_defaults(func=_map2bitmap)

if __name__ == "__main__":
	parser = argparse.ArgumentParser(description=__doc__)
	add_arguments_to_parser(parser)
	args = parser.parse_args()
	_map2bitmap(args)