#pragma once
#include "const.h"
#include <boost/filesystem.hpp>
#include <map>
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <iostream>

struct SectionInfo
{
	SectionInfo() {}
	//
	SectionInfo(const SectionInfo& section) {
		this->section_datas = section.section_datas;
	}
	SectionInfo& operator=(const SectionInfo& section) {
		if (&section == this) {
			return *this;
		}
		this->section_datas = section.section_datas;
	}

	std::map<std::string, std::string> section_datas;
	std::string operator[](const std::string& key) {
		if (section_datas.find(key) == section_datas.end()) {
			return "";
		}
		return section_datas[key];
	}
};

class ConfigMgr
{
public:
	~ConfigMgr() {
		_config_map.clear();
	}
	ConfigMgr(const ConfigMgr& config) {
		this->_config_map = config._config_map;
	}
	ConfigMgr& operator=(const ConfigMgr& config) {
		if (&config == this) {
			return *this;
		}
		this->_config_map = config._config_map;
		return *this;
	}
	SectionInfo operator[](const std::string& key) {
		if (_config_map.find(key) == _config_map.end()) {
			return SectionInfo();
		}
		return _config_map[key];
	}
	static ConfigMgr& Instance() {
		static ConfigMgr configmgr;
		return configmgr;
	}

private:
	ConfigMgr();
	std::map<std::string, SectionInfo> _config_map;
};

