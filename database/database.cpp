#include "database.h"
#include "../config/config.h"
#include <functional>

namespace database{
    Database::Database(){
        _connection_string+="host=";
        _connection_string+=Config::get().get_host();
        _connection_string+=";user=";
        _connection_string+=Config::get().get_login();
        _connection_string+=";db=";
        _connection_string+=Config::get().get_database();
        _connection_string+=";port=";
        _connection_string+=Config::get().get_port();
        _connection_string+=";password=";
        _connection_string+=Config::get().get_password();

        std::cout << "Connection string:" << _connection_string << std::endl;
         Poco::Data::MySQL::Connector::registerConnector();
        _pool = std::make_unique<Poco::Data::SessionPool>(Poco::Data::MySQL::Connector::KEY, _connection_string);
    }
//    #pragma region Sharding
    
    size_t Database::get_max_shard()
    {
        return 3;
    }

    std::vector<std::string> Database::get_all_hints(){
        std::vector<std::string> result;
        for(size_t i=0;i<=get_max_shard();++i){
            std::string shard_name = "-- sharding:";
            shard_name += std::to_string(i);
            result.push_back(shard_name);
        }
        return result;
    }

    std::string Database::sharding_hint_single(long id)
    {
        std::string key;

        key += std::to_string(id);

        size_t shard_number = std::hash<std::string>{}(key) % (Database::get_max_shard());

        std::string result = "-- sharding:";
        result += std::to_string(shard_number);
        return result;
    }
    std::string Database::sharding_hint(long from, long to){

        std::string key;

        key += std::to_string(from);
        key += ";";
        key += std::to_string(to);

        size_t shard_number = std::hash<std::string>{}(key) % (Database::get_max_shard());

        std::string result = "-- sharding:";
        result += std::to_string(shard_number);
        return result;
    }
    
    std::string Database::sharding_hint_2(std::string first_name, std::string last_name)
    {
        std::string key;

        key += first_name;
        key += ";";
        key += last_name;

        size_t shard_number = std::hash<std::string>{}(key) % (Database::get_max_shard());

        std::string result = "-- sharding:";
        result += std::to_string(shard_number);
        return result;

    }
//    #pragma endregion Sharding - End


    Database& Database::get(){
        static Database _instance;
        return _instance;
    }

    Poco::Data::Session Database::create_session(){
        return Poco::Data::Session(_pool->get());
    }

}