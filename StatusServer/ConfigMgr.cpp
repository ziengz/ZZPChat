#include "ConfigMgr.h"

ConfigMgr::ConfigMgr()
{
	//��ȡ��ǰ�ļ�Ŀ¼
	boost::filesystem::path current_path = boost::filesystem::current_path();
	//����config.ini�ļ�����·�� (��Ϊboost�ԡ�/�������������أ�ƴ������)
	boost::filesystem::path config_path = current_path / "config.ini";
	std::cout << "congif path is " << config_path << std::endl;
	
	//ʹ��boost::propertree����ȡini�ļ�
	boost::property_tree::ptree pt;
	boost::property_tree::read_ini(config_path.string(), pt);

	//����ini�ļ������е�section
	for (const auto& section_pair : pt) {
		//��Ϊconfig.ini�ļ��еĸ�ʽΪ��[GataServer]
									//  port = 10086  �൱�����Լ�ֵ��
		const std::string& section_name = section_pair.first;
		
		std::map<std::string, std::string> key_value;
		for (const auto& key_value_pair : section_pair.second) {
			const std::string& key = key_value_pair.first;
			//��Ϊvalue��Ҳ��Ϊptree����
			const std::string& value = key_value_pair.second.get_value<std::string>();
			
			key_value.emplace(std::make_pair(key, value));
		}
		SectionInfo section;
		section.section_datas = key_value;
		_config_map[section_name] = section;
	}

	//��ӡ����section��Ӧ��key,value
	for (const auto& section_entry : _config_map) {
		const std::string& section_name = section_entry.first;
		std::cout << "[" << section_name << "]" << std::endl;
		SectionInfo section_config = section_entry.second;
		for (const auto& key_value_pair : section_config.section_datas) {
			std::cout << key_value_pair.first << "=" << key_value_pair.second << std::endl;
		}
	}

}
