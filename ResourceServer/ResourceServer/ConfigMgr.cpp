#include "ConfigMgr.h"

ConfigMgr::ConfigMgr() {
	// 获取当前目录
	auto current_path = boost::filesystem::current_path();
	// 构建ini文件完整目录
	auto config_path = current_path / "config.ini";
	std::cout << "config path : " << config_path << std::endl;

	// 使用boost.propertyTree 来读取ini文件
	boost::property_tree::ptree pt;
	boost::property_tree::read_ini(config_path.string(), pt);
	for (const auto& section_pair : pt) {
		const std::string& section_name = section_pair.first;
		const boost::property_tree::ptree& section_tree = section_pair.second;

		// 对于每个section，遍历器所有的key-value对
		std::map<std::string, std::string> section_config;
		for (const auto& key_value_pair : section_tree) {
			const std::string& key = key_value_pair.first;
			const std::string& value = key_value_pair.second.get_value<std::string>();
			section_config[key] = value;
		}
		SectionInfo sectionInfo;
		sectionInfo._section_datas = section_config;
		_config_map[section_name] = sectionInfo;
	}

	// 输出所有的section和key-value对
	for (const auto& section_entry : _config_map) {
		const std::string& section_name = section_entry.first;
		SectionInfo sectionInfo = section_entry.second;
		std::cout << "[" << section_name << "]" << std::endl;
		for (const auto& key_value_pair : sectionInfo._section_datas) {
			std::cout << key_value_pair.first << "=" << key_value_pair.second << std::endl;
		}
	}

	InitPath();
}

std::string ConfigMgr::GetValue(const std::string& section, const std::string& key) {
	if (_config_map.find(section) == _config_map.end()) {
		return "";
	}
	return _config_map[section].GetValue(key);
}
boost::filesystem::path ConfigMgr::GetFileOutPath() {
	return _static_path;
}
void ConfigMgr::InitPath() {
	auto current_path = boost::filesystem::current_path();
	std::string bindir = _config_map["Output"].GetValue("Path");
	std::string staticdir = _config_map["Static"].GetValue("Path");
	_static_path = current_path / bindir / staticdir;
	_bin_path = current_path / bindir;

	// 检查路径是否存在
	if (!boost::filesystem::exists(_static_path)) {
		// 如果不存在创建路径
		if (boost::filesystem::create_directories(_static_path)) {
			std::cout << "路径已成功创建：" << _static_path.string() << std::endl;
		}
		else {
			std::cout << "路径创建失败: " << _static_path.string() << std::endl;
		}
	}
	else {
		std::cout << "路径已经创建：" << _static_path.string() << std::endl;
	}

}