//
// Created by huberyzhang on 2018/7/25.
//
#include "questionBank.hpp"
#include "../include/eosio/chain/fork_database.hpp"
#include "../include/eosio/chain/controller.hpp"
#include "../include/eosio/chain/block.hpp"
#include "../include/eosio/chain/block_header.hpp"



namespace celesos{
    namespace chain{
        namespace forest {
            static uint16_t question_space_number = 600;//问题间隔块数
            static uint16_t question_period = 21600;//问题有效期
            forestBank::forestBank() {}

            forestBank::~forestBank() {}

            FOREST_STRUCT forestBank::get_wood(account_name name) {
                const controller control = controller::controller(controller::config());
                uint32_t current_block_number = control.head_block_num();
                //试图更新cache
                forestBank::update_cache(current_block_number);

				// udpate
                uint32_t current_forest_number = current_block_number/question_space_number * question_space_number;


                if((forest_struct.target == 0)
					|| temp_forest_number != forest_struct.block_number){
                    block_id_type result_value = control.get_block_id_for_num(temp_forest_number);

                    forest_struct.seed = fc::sha256::hash(result_value.str());
                    forest_struct.forest = fc::sha256::hash(result_value.str()+name.to_string());
                    forest_struct.block_number = temp_forest_number;

                    //计算难度
                    boost::multiprecision::uint512_t value = 1<<58;

                    forest_struct.target = value;
                }
                return forest_struct;
            }


            int forestBank::getIndexOfSigns(char ch) {
                if(ch >= '0' && ch <= '9')
                {
                    return ch - '0';
                }
                if(ch >= 'A' && ch <='F')
                {
                    return ch - 'A' + 10;
                }
                if(ch >= 'a' && ch <= 'f')
                {
                    return ch - 'a' + 10;
                }
                return -1;
            }
            boost::multiprecision::uint512_t forestBank::hexToDec(char *source) {
                boost::multiprecision::uint512_t sum = 0;
                boost::multiprecision::uint512_t t = 1;
                size_t i;

                size_t len = strlen(source);
                for(i=len-1; i>=0; i--)
                {
                    sum += t * forestBank::getIndexOfSigns(*(source + i));
                    t *= 16;
                }

                return sum;
            }
            bool forestBank::verify_wood(uint32_t block_number,account_name account,boost::multiprecision::uint512_t wood){
                const controller control = controller::controller(controller::config());
                uint32_t current_block_number = control.head_block_num();
                if(block_number <= current_block_number - question_period){
                    //过期了
                    return false;
                } else if(block_number%question_space_number != 0){
                    //不是问题
                    return false;
                }else{
                    //这里验证答案是否正确
                    signed_block_ptr block_ptr = control.fetch_block_by_number(block_number);
                    //这里获取目标难度
                    boost::multiprecision::uint512_t target = 1<<58;
                    //准备需要验证的参数
                    uint32_t cache_number = block_number/question_period;
                    std::map<uint32_t, string>::iterator cache_iter = cache_map.find(cache_number);
                    string cache = (*cache_iter).second;

                    block_id_type block_id = control.get_block_id_for_num(block_number);
                    block_id_type wood_forest = fc::sha256::hash(block_id.str()+account.to_string());
                    //调用验证函数
                    fc::sha256 result_hash = fc::sha256::hash("wood");

                    boost::multiprecision::uint512_t result_value = forestBank::hexToDec((result_hash.str()).c_str());

					//update from meet
                    if(result_value <= target){
                        //难度符合目标难度
                        return true;

                    }else{
                        //难度不符合目标难度
                        return false;
                    }
                }
            }

            void forestBank::update_cache(uint32_t block_number){
                //保留最近的两个cache
                uint32_t current_cache_number = block_number/question_period;
                if(cache_map.find(current_cache_number) == cache_map.end())
                {
                    std::map<uint32_t, string>::iterator iter;
                    if(current_cache_number-1 >= 0)
                    {
                        iter = cache_map.find(current_cache_number-1);
                    }
                    cache_map.clear();

                    if(iter != cache_map.end())
                    {
                        cache_map.insert(iter);
                    }

                    cache_map.insert(std::make_pair(current_cache_number, "cache feed"));
                }
            }


        }
    }
}