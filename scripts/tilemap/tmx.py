#!/usr/local/bin/python3
#coding: utf-8

import json

class TMXLayer():
	"""Tiled .*tmx layer representation"""

	def __repr__(self):
		return "{} name: {} width: {} height: {}".format(self.__class__,self.name, self.width, self.height)


	def __init__(self):
		self.name = ''
		self.visible = False
		self.opacity = 1
		self.width = 0
		self.height = 0
		self.data = []
		self.type = 'tileLayer'


class TMXMap():
	"""Tiled *.tmx representation """
	
	def __repr__(self):
		return "{} width: {} height: {} layer: {}".format(self.__class__,self.map_width, self.map_heigth, len(self.layers))

	def __init__(self, file=None, file_type='json'):
		
		self.file = file
		self.file_type = file_type
		self.layers = []
		self.tilesheets = []
		self.gid_lookup = {}
		self.tile_width = 0
		self.tile_height = 0
		self.map_width = 0
		self.map_heigth = 0

		if self.file:
			self._parse_map_data()

	def _parse_map_data(self):
		if self.file_type == 'json':
			self._parse_map_json()

	def _parse_map_json(self):
		mapData = json.loads(self.file)

		self.map_width = mapData.get("width", 0)
		self.map_heigth = mapData.get("height", 0)
		self.tile_width = mapData.get("tilewidth", 0)
		self.tile_height = mapData.get("tileheight", 0)

		for layerData in mapData['layers']:
			if layerData['type'] == 'tilelayer':
				layer = TMXLayer()
				layer.width = layerData['width']
				layer.height = layerData['height']
				layer.type = layerData['type']
				layer.name = layerData['name']
				layer.visible = layerData['visible']
				
				layer.data = layerData['data']
				self.layers.append(layer)

	def _load_file(self, file, file_type):
		self.file = open(file,'r').read()
		self.file_type = file_type
		self._parse_map_data()

	def load_tmx_file(self, file_path):
		self._load_file(file_path, 'tmx')

	def load_json_file(self, file_path):
		self._load_file(file_path,'json')


