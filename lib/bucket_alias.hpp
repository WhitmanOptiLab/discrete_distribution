#pragma once

#include "utility.h"
#include <queue>
#include <random>
#include <iostream>
#include <algorithm>
#include <set>
#include <chrono>
#include <stdexcept>
//#include "XoshiroCpp.hpp"
#include "robin_hood.h"
#include "basic_alias.hpp"
#include "string"
using namespace std;

const int small_event = -100000;
inline double fastPow(double a, double b) {
    union {
        double d;
        int x[2];
    } u = { a };
    u.x[1] = (int)(b * (u.x[1] - 1072632447) + 1072632447);
    u.x[0] = 0;
    return u.d;
}

class BucketMethod {
public:

    //XoshiroCpp::Xoroshiro128Plus rng;
    chrono::time_point<chrono::system_clock> start, end;
    chrono::duration<double> random_generate_seconds;
    //create the prob_bias and alias table
    string method_name() {
        return "bucket";
    }
    /*alias() {}*/
    struct Bucket {
        Bucket() {
            id = 0;
            max_weight = 0;
            bucket_weight = 0;
        }
        Bucket(int _id, double _max_weight) {
            id = _id, max_weight = _max_weight;
            //ele_size = 0;
            bucket_weight = 0;
            //max_weight = 0;
        }

        int id;
        double max_weight;
        double bucket_weight;
        //double max_weight;
        vector<Element> elements;
    };


    int log2_evil(double d) {
        return ((reinterpret_cast<unsigned long long&>(d) >> 52) & 0x7ff) - 1023;
    }
    //set<Element> weight_bst;
    int find_bucket(const double& _weight) {
        return log2(_weight);
    }

    int log2_floor(const double& num) {
        return log2(num);
    }

    inline int log2_ceil(const double& num) {
        return ceil(log2(num));
    }

    BucketMethod(int num, vector<Element> all_ele) : gen(rd()) {
        constexpr std::uint64_t seed = 777;
        //rng = XoshiroCpp::Xoroshiro128Plus(seed);
        tot_weight = 0;
        ele_size = 0;
        rest_weight = 0;
        position_map.reserve(all_ele.size());
        //sort(all_ele.begin(), all_ele.end(), cmp_element_weight);
        for (int i = 0; i < num; i++) {
            int bucket_idx = find_bucket(all_ele[i].weight);
            BucketInsert(bucket_idx, all_ele[i]);
            //cerr << all_ele[i].weight << " " << bucket_map[bucket_idx].max_weight << "\n";
        }
        prepare_alias = use_alias = largest_bucket_id = clean_largest_bucket_id = 0;
        clean_bucket_list = 0;

        //get_random_bucket()
    }

    template <class InputIt>
    BucketMethod(InputIt first, InputIt last): BucketMethod(last-first,weightsToElements(first,last)){
    }

    void BucketInsert(int& ins_B_id, Element& ins_ele) {
        ele_size++;
        if (bucket_map.find(ins_B_id) == bucket_map.end())
            bucket_map[ins_B_id] = Bucket(ins_B_id, pow(2, ins_B_id + 1));
        //max_weight = max(max_weight, ins_ele.weight);
        //bucket_map[ins_B_id].ele_size++;
        Bucket& ins_B = bucket_map[ins_B_id];
        ins_B.bucket_weight += ins_ele.weight;
        ins_B.elements.emplace_back(ins_ele);
        position_map[ins_ele.key]=make_pair(ins_B_id, ins_B.elements.size() - 1);
        //position_map[ins_ele.key] = make_pair(ins_B_id, ins_B.elements.size() - 1);
        tot_weight += ins_ele.weight;
    }


    void BucketDelete(int del_B_id, int del_key) {

        //max_weight = max(max_weight, ins_ele.weight);
        Bucket& del_B = bucket_map[del_B_id];
        int del_pos = position_map[del_key].second;
        int tail_key = del_B.elements[del_B.elements.size() - 1].key;
        del_B.bucket_weight -= del_B.elements[del_pos].weight;
        tot_weight -= del_B.elements[del_pos].weight;

        del_B.elements[del_pos] = del_B.elements[del_B.elements.size() - 1];
        position_map[tail_key] = make_pair(del_B_id, del_pos);
        position_map.erase(del_key);
        del_B.elements.pop_back();
        //        <<" "<<del_pos<<" "<<del_B.elements.size()<<"\n";
        if (del_B.elements.size() == 0) {
            bucket_map.erase(del_B_id);
        }
        ele_size--;
        //ele_size--;
    }

    inline void find_largest_bucket_id() {
        if (clean_largest_bucket_id) return;
        int r_ = log2_floor(tot_weight);
        //cerr << r_ << " " << tot_weight << "\n";
        for (;; r_--){ //getting caught in infinite for loop - meaning that bucket_map.find(r_)==bucket_map.end()
            if (bucket_map.find(r_) != bucket_map.end()) {
                largest_bucket_id = r_;
                clean_largest_bucket_id = 1;
                return;
            }
            
        }
    }

    void create_alias() {

        vector<Element> sampleElements;
        find_largest_bucket_id();
        int left_range = largest_bucket_id - 2 * log2_ceil(ele_size);
        double major_weight = 0;
        for (int i = largest_bucket_id; i >= left_range; i--) {
            if (bucket_map.find(i) != bucket_map.end()) {
                sampleElements.push_back(Element(-1, i, bucket_map[i].bucket_weight));
                //cerr << i << " " << bucket_map[i].bucket_weight << '\n';
                major_weight += bucket_map[i].bucket_weight;
            }
        }
        sampleElements.push_back(Element(-1, small_event, tot_weight - major_weight));
        rest_weight = tot_weight - major_weight;
        //cerr << rest_weight <<" "<<bucket_map[26].bucket_weight<<" "<< bucket_map[25].bucket_weight
        //    << " " << bucket_map[24].bucket_weight << " " << "\n";
        bucket_alias.reconstruct(sampleElements.size(), sampleElements);
        //bucket_alias = tmp_alias;
        prepare_alias = 1;
        return;
    }

    void print_bucket() {
        for (auto i : bucket_map) {
            cerr << i.first << " " << i.second.bucket_weight << "\n";
            
        }

    }
    void print_hit_bucket() {
        for (int i = 30; i >= 10; i--) {
            cerr << hit[i] << " ";
        }
    }
    //randomly generate
    void collect_buckets() {
        tmp_bucket_list.clear();
        bool major_flag = 0;
        int left_range = largest_bucket_id - 2 * log2_ceil(ele_size);
        for (int i = largest_bucket_id; i >= left_range; i--) {
            if (bucket_map.find(i) != bucket_map.end()) {
                tmp_bucket_list.push_back(make_pair(bucket_map[i].bucket_weight, &bucket_map[i]));
            }
        }

        clean_bucket_list = true;
    }

    
    // Bucket* get_random_bucket() {
    //     int cur_block_id = 0;
    //     //if (!clean_bucket_list) {
    //     //    collect_buckets();
    //     //}

    //     uniform_real_distribution<double> dr(0, tot_weight);
    //     double random_bucket_weight = dr(rng);
    //     for (auto i : tmp_bucket_list) {
    //         if (random_bucket_weight <= i.first) {
    //             return i.second;
    //         }
    //         random_bucket_weight -= i.first;
    //     }
    //     int left_range = largest_bucket_id - 2 * log2_ceil(ele_size);

    //     for (auto i : bucket_map) {
    //         if (i.first < left_range) {
    //             if (random_bucket_weight <= i.second.bucket_weight) {
    //                 return &(i.second);
    //             }
    //             else random_bucket_weight -= i.second.bucket_weight;
    //         }
    //     }

    // }
    template<class URNG>
    int operator()(URNG& rng) {
        int cur_block_id;
        //int setAt=0;
        Bucket* cur_bucket = NULL;
        if (use_alias) { //this literally never happens bc use_alias is never set to true - idk why it is here
            //throw std::runtime_error("use alias is true?????");
            //std::cout<<"use alias is true??"<<std::endl;
            if (!prepare_alias) {
                create_alias();
            }
            cur_block_id = bucket_alias.operator()(rng);
            if (cur_block_id == small_event) {
                find_largest_bucket_id();
                uniform_real_distribution<double> dr(0, rest_weight);
                double random_bucket_weight = dr(rng);
                int left_range = largest_bucket_id - 2 * log2_ceil(ele_size);
                for (auto i : bucket_map) {
                    if (i.first < left_range) {
                        if (random_bucket_weight <= i.second.bucket_weight) {
                            cur_block_id = i.first;
                            break;
                        }
                        else random_bucket_weight -= i.second.bucket_weight;
                    }
                }
            }
            cur_bucket = &(bucket_map[cur_block_id]);
        }
        else {
            if (!clean_largest_bucket_id) { 
                find_largest_bucket_id();
            }
            if (!clean_bucket_list) {
                collect_buckets(); //this makes tmp_bucket_list containt a list from largest bucket id to smallest bucket id in the upper portion of 
                // the range (discounts the smaller buckets bc they won't contain as many elements and are less likely to be chosen) 
                // entries in the format <bucket weight,&bucket>
            }
            bool major_flag = 0;
            cur_bucket = tmp_bucket_list[tmp_bucket_list.size() - 1].second; //cur_bucket is bucket with smallest bucket id in the range 
            //setAt=1;
            //checkForBucketMismatch(*cur_bucket,"cur_bucket just set (checkpoint 1)");
            uniform_real_distribution<double> dr(0, tot_weight);
            double random_bucket_weight = dr(rng); //chooses a weight between 0 and the total weight of all elements
            std::cout<<"target is "<<random_bucket_weight<<std::endl;
            for (auto i : tmp_bucket_list) { //go down through the buckets (bucket Ids are decreasing and see if random weight is more than the bucket weight. if yes, select. if no, subtract the bucket weight from the random weight)
                if (random_bucket_weight <= i.first) {
                    major_flag = 1;
                    cur_bucket = i.second;
                    //setAt=2;
                    //checkForBucketMismatch(*cur_bucket,"checkpoint 2");
                    break;
                }
                random_bucket_weight -= i.first;
            }
            if (!major_flag) { //if it didn't select a bucket, then search the rest of the range that got left out above 
                int left_range = largest_bucket_id - 2 * log2_ceil(ele_size);
                for (auto&i : bucket_map) {
                    if (i.first < left_range) {
                        if (random_bucket_weight <= i.second.bucket_weight) {
                            cur_bucket = &i.second;
                            // checkForBucketMismatch(*cur_bucket,"checkpoint 3");
                            // setAt=3;
                            //d::cout<<"checkpoint 3 current bucket ";
                            //intABucketNoException(*cur_bucket);

                            break;
                        }
                        else random_bucket_weight -= i.second.bucket_weight;
                    }
                }
            }

        }
        uniform_int_distribution<int> pos_rand(0, cur_bucket->elements.size() - 1);
        uniform_real_distribution<double> dr2(0, cur_bucket->max_weight);
        int tmp_pos;
        double rej_weight;
        int counter = 0;

        //std::cout<<"current bucket set at "<<setAt<<std::endl;
        //d::cout<<"checkpoint 4 current bucket ";
        //printABucketNoException(*cur_bucket);
        //checkAllBuckets();
        //checkForBucketMismatch(*cur_bucket,"right before infinite loop  cur_bucket set at checkpoint "+std::to_string(setAt));

        while (1) {
            tmp_pos = pos_rand(rng);
            rej_weight = dr2(rng);
            if (rej_weight > cur_bucket->elements[tmp_pos].weight){
                counter ++;
                // if ((counter>10000000) && (counter%10000==0)){
                //     std::cout<<"INFINITE LOOP bucket size is "<<cur_bucket->elements.size()<<std::endl;
                //     //printABucket(*cur_bucket);
                //     //throw std::runtime_error("infinite loop in selector");

                // }
            }
                //continue;
            else {
                //std::cout<<"checkpoint 10"<<std::endl;
                //std::cout<<"while loop exited "<<counter<<"         bucket size is "<<cur_bucket->elements.size()<<std::endl;
                //rintABucket(*cur_bucket);
                return cur_bucket->elements[tmp_pos].value;
            }
            
        }
    }

    void checkAllBuckets(){
        for(auto it=bucket_map.begin();it!=bucket_map.end();it++){
            for(int i=0;i<it->second.elements.size();i++){
                if (it->second.elements[i].weight<=0){
                    std::cout<<"problem in bucket "<<it->second.id<<std::endl;
                    printABucket(it->second);
                }
                if(it->second.elements.size()==0){
                    std::cout<<"checkAllBuckets found that the bucket "<<it->second.id<<" is empty"<<std::endl;
                }
            }
        }
    }

    void checkForBucketMismatch(Bucket cur_bucket,std::string message){
        auto bucketCheck = bucket_map.find(cur_bucket.id);
        if(bucketCheck==bucket_map.end()){
            std::cout<<"cur_bucket not in bucket map"<<std::endl;
        }
        else if(bucketCheck->second.elements !=cur_bucket.elements){
            std::cout<<"there's a mismatch in bucket "<<cur_bucket.id<<" between the current bucket and the one in the bucket map     message is: "<<message<<std::endl;
            std::cout<<"current bucket: ";
            printABucketNoException(cur_bucket);
            std::cout<<"bucket map bucket: ";
            printABucketNoException(bucketCheck->second);
            throw std::runtime_error("bucket map mismatch");
        }

    }

    void printABucket(Bucket toPrint){
        bool except=false;
        std::cout<<"printing bucket "<<toPrint.id<<": ";
        for(int i=0;i<toPrint.elements.size();i++){
            std::cout<<toPrint.elements[i].weight<<", ";
            if (toPrint.elements[i].weight<=0){
                except=true;
            }
        }
        std::cout<<std::endl;
        if (except==true){
            throw std::runtime_error("element in bucket <=0");
        }
    }

    void printABucketNoException(Bucket toPrint){
        std::cout<<"printing bucket"<<toPrint.id<< " with size "<<toPrint.elements.size()<<": ";
        for(int i=0;i<toPrint.elements.size();i++){
            std::cout<<toPrint.elements[i].weight<<", ";
        }
        std::cout<<std::endl;
    }




    void delete_element(int del_key) {
        if (position_map.find(del_key) == position_map.end()) {

            cerr << "Element not exists\n";
        }
        BucketDelete(position_map[del_key].first, del_key);
        clean_largest_bucket_id = 0;
        prepare_alias = 0;
        clean_bucket_list = 0;
    }

    void insert_element(Element& ins_ele) {
        if (position_map.find(ins_ele.key) != position_map.end()) {
            cerr << "Element exists\n";
        }
        int ins_bucket_id = find_bucket(ins_ele.weight);
        BucketInsert(ins_bucket_id, ins_ele);
        clean_largest_bucket_id = 0;
        prepare_alias = 0;
        clean_bucket_list = 0;
        return;
    }

    void update_weight(int upd_key, double new_weight) {
        int new_bucket_id = find_bucket(new_weight);
        int old_bucket_id = position_map[upd_key].first;
        if (new_bucket_id == old_bucket_id) {
            int ele_pos = position_map[upd_key].second; //what's ele_pos?
            Bucket& ins_B = bucket_map[new_bucket_id];
            double diff_weight = new_weight - ins_B.elements[ele_pos].weight;
            ins_B.bucket_weight+=diff_weight;
            ins_B.elements[ele_pos].weight = new_weight;
            tot_weight += diff_weight;
            //std::cout<<"buckets the same. new bucket is ";
            //printABucket(ins_B);
        }
        else {
            Element new_ele = bucket_map[old_bucket_id].elements[position_map[upd_key].second];
            new_ele.weight = new_weight;
            BucketDelete(old_bucket_id, upd_key);
            BucketInsert(new_bucket_id, new_ele);
            // //std::cout<<"buckets different. old bucket: ";
            // auto it = bucket_map.find(old_bucket_id);
            // if (it != bucket_map.end()){
            //     printABucket(bucket_map[old_bucket_id]);
            // } 
            // else{
            //     //std::cout<<"empty"<<std::endl;
            // }
            // //std::cout<<"new bucket is ";
            // printABucket(bucket_map[new_bucket_id]);
        }
        clean_largest_bucket_id = 0;
        prepare_alias = 0;
        clean_bucket_list = 0;
        checkAllBuckets();
        return;
    }

    void update_weight(int upd_key, Element new_ele) {
        throw std::runtime_error("wrong update weight called");
        double new_weight = new_ele.weight;
        int new_bucket_id = find_bucket(new_weight);
        int old_bucket_id = position_map[upd_key].first;
        if (new_bucket_id == old_bucket_id) {
            int ele_pos = position_map[upd_key].second;
            Bucket& ins_B = bucket_map[new_bucket_id];
            double diff_weight = new_weight - ins_B.elements[ele_pos].weight;
            ins_B.bucket_weight += diff_weight;
            ins_B.elements[ele_pos] = new_ele;
            tot_weight += diff_weight;
        }
        else {
            //Element new_ele = bucket_map[old_bucket_id].elements[position_map[upd_key].second];
            //new_ele.weight = new_weight;
            BucketDelete(old_bucket_id, upd_key);
            BucketInsert(new_bucket_id, new_ele);
        }
        clean_largest_bucket_id = 0;
        prepare_alias = 0;
        clean_bucket_list = 0;
        return;
    }

    double tot_weight;
    robin_hood::unordered_map<int, pair<int, int> > position_map;
public:
    //int ele_size, reconstruct_flag;
    //int block_capcity[30];
    random_device rd;
    mt19937 gen;

    double rest_weight;
    int prepare_alias;
    int use_alias;
    int largest_bucket_id;
    int clean_largest_bucket_id;
    int ele_size;
    int clean_bucket_list;
    int hit[30] = { 0 };
    // key -> <bucket_id, element position in bucket>
    // maintain non-empty bucket
    robin_hood::unordered_map<int, Bucket> bucket_map;
    uniform_int_distribution<int> pos_rand;
    aliasMethod bucket_alias;
    vector<pair<double, Bucket*> > tmp_bucket_list;
    vector<Bucket> tmp_ins_bucket_list;

    private:
        template<class InputIt>
        vector<Element> weightsToElements(InputIt start, InputIt end){
            vector<Element> elements;
            int i=0;
            for(auto it=start;it<end;it++){
                elements.emplace_back(i,i,*it);
                i++;
            }
            return elements;
        }
};