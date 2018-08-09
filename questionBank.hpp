//
// Created by huberyzhang on 2018/7/27.
//

#include <boost/core/typeinfo.hpp>
#include <boost/interprocess/exceptions.hpp>
#include "../include/eosio/chain/types.hpp"
#include <boost/multiprecision/cpp_int.hpp>
#include "../../fc/include/fc/crypto/sha256.hpp"
#include <map>

namespace celesos{
    namespace chain{
        namespace forest{
            typedef struct{
                block_id_type seed;
                block_id_type forest;
                uint32_t block_number;
				uint32_t next_block_num;
				
                boost::multiprecision::uint256_t target;
            }forest_struct;



            class forest_bank{
            public:
                
                virtual ~forest_bank();
				
				forest_bank& get_instance();
				
                bool get_forest(forest_struct& forest, const account_name& account);
                bool verify_wood(uint32_t block_number, const account_name& account, const uint64_t wood);
                
            private:
			
				forest_bank( );
				
                boost::multiprecision::uint512_t hex_to_dec(char *source);
                int get_index_of_signs(char ch);
                //forest_struct _forest_struct;
                std::map<uint32_t, std::vector<celesos::ethash::node>> cache_map;	//key: block_num
				void update_cache(uint32_t block_number);
            };
        }
    }
}