#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <memory>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/SessionPool.h>

namespace database{
    class Database{
        private:
            std::string _connection_string;
            std::unique_ptr<Poco::Data::SessionPool> _pool;
            Database();
        public:
            static Database& get();
            Poco::Data::Session create_session();

            static size_t get_max_shard();
            static std::string sharding_hint_single(long id);
            static std::string sharding_hint(long from,long to);
            static std::string sharding_hint_2(std::string first_name, std::string last_name);
            
            static std::vector<std::string> get_all_hints();
    };
}
#endif