#include "ConfigMgr.h"

ConfigMgr::ConfigMgr()
{
	//获取当前文件目录
	boost::filesystem::path current_path = boost::filesystem::current_path();
	//构建config.ini文件完整路径 (因为boost对“/”符号做过重载，拼接作用)
	boost::filesystem::path config_path = current_path / "config.ini";
	std::cout << "congif path is " << config_path << std::endl;
	
	//使用boost::propertree来读取ini文件
	boost::property_tree::ptree pt;
	boost::property_tree::read_ini(config_path.string(), pt);

	//遍历ini文件中所有的section
	for (const auto& section_pair : pt) {
		//因为config.ini文件中的格式为：[GataServer]
									//  port = 10086  相当于两对键值对
		const std::string& section_name = section_pair.first;
		
		std::map<std::string, std::string> key_value;
		for (const auto& key_value_pair : section_pair.second) {
			const std::string& key = key_value_pair.first;
			//因为value中也可为ptree类型
			const std::string& value = key_value_pair.second.get_value<std::string>();
			
			key_value.emplace(std::make_pair(key, value));
		}
		SectionInfo section;
		section.section_datas = key_value;
		_config_map[section_name] = section;
	}

	//打印所有section对应的key,value
	for (const auto& section_entry : _config_map) {
		const std::string& section_name = section_entry.first;
		std::cout << "[" << section_name << "]" << std::endl;
		SectionInfo section_config = section_entry.second;
		for (const auto& key_value_pair : section_config.section_datas) {
			std::cout << key_value_pair.first << "=" << key_value_pair.second << std::endl;
		}
	}

}
