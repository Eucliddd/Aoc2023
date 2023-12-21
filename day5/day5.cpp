#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <tuple>
#include <memory>
#include <array>
#include <sstream>
#include <algorithm>
#include <cassert>

using namespace std;

const int seed_num = 20;

using type = uint64_t;
const type MAX = UINT64_MAX;
using Tup = tuple<type, type, type>;
using myIt = vector<Tup>::iterator;

auto LowerBound(myIt begin, myIt end, type value){
    auto first = begin;
    auto last = end - 1;
    while(first <= last){
        auto mid = first + (last - first) / 2;
        type mid_value = get<0>(*mid);
        if(mid_value > value){
            if(mid == begin)
                return end;
            else
                last = mid - 1;
            last = mid - 1;
        }else{
            first = mid + 1;
        }
    }
    return last;
}


int puzzle1(vector<Tup>& seed_to_soil, 
            vector<Tup>& soil_to_fertilizer, 
            vector<Tup>& fertilizer_to_water, 
            vector<Tup>& water_to_light, 
            vector<Tup>& light_to_temperature, 
            vector<Tup>& temperature_to_humidity, 
            vector<Tup>& humidity_to_location, 
            array<type, seed_num>& seeds){
    //calculate
    type min = MAX;
    for(int i=0; i<seed_num; i++){
        type seed = seeds[i];
        auto it = LowerBound(seed_to_soil.begin(), seed_to_soil.end(), seed);
        type soil = seed;
        if(it != seed_to_soil.end()){
            type seed_begin = get<0>(*it);
            type soil_begin = get<1>(*it);
            type length = get<2>(*it);
            if(seed <= seed_begin + length){
                soil = soil_begin + seed - seed_begin;
            }
        }
        it = LowerBound(soil_to_fertilizer.begin(), soil_to_fertilizer.end(), soil);
        type fertilizer = soil;
        if(it != soil_to_fertilizer.end()){
            type soil_begin = get<0>(*it);
            type fertilizer_begin = get<1>(*it);
            type length = get<2>(*it);
            if(soil <= soil_begin + length){
                fertilizer = fertilizer_begin + soil - soil_begin;
            }
        }
        it = LowerBound(fertilizer_to_water.begin(), fertilizer_to_water.end(), fertilizer);
        type water = fertilizer;
        if(it != fertilizer_to_water.end()){
            type fertilizer_begin = get<0>(*it);
            type water_begin = get<1>(*it);
            type length = get<2>(*it);
            if(fertilizer <= fertilizer_begin + length){
                water = water_begin + fertilizer - fertilizer_begin;
            }
        }
        it = LowerBound(water_to_light.begin(), water_to_light.end(), water);
        type light = water;
        if(it != water_to_light.end()){
            type water_begin = get<0>(*it);
            type light_begin = get<1>(*it);
            type length = get<2>(*it);
            if(water <= water_begin + length){
                light = light_begin + water - water_begin;
            }
        }
        it = LowerBound(light_to_temperature.begin(), light_to_temperature.end(), light);
        type temperature = light;
        if(it != light_to_temperature.end()){
            type light_begin = get<0>(*it);
            type temperature_begin = get<1>(*it);
            type length = get<2>(*it);
            if(light <= light_begin + length){
                temperature = temperature_begin + light - light_begin;
            }
        }
        it = LowerBound(temperature_to_humidity.begin(), temperature_to_humidity.end(), temperature);
        type humidity = temperature;
        if(it != temperature_to_humidity.end()){
            type temperature_begin = get<0>(*it);
            type humidity_begin = get<1>(*it);
            type length = get<2>(*it);
            if(temperature <= temperature_begin + length){
                humidity = humidity_begin + temperature - temperature_begin;
            }
        }
        it = LowerBound(humidity_to_location.begin(), humidity_to_location.end(), humidity);
        type location = humidity;
        if(it != humidity_to_location.end()){
            type humidity_begin = get<0>(*it);
            type location_begin = get<1>(*it);
            type length = get<2>(*it);
            if(humidity <= humidity_begin + length){
                location = location_begin + humidity - humidity_begin;
            }
        }
        if(location < min){
            min = location;
        }
    }
    return min;
}

void constructMap(vector<Tup>& seed_to_location, int level, vector<vector<Tup>>& maps, type low, type high, type& index){
    if(level == 0){
        if(low >= high) 
            return;
        assert(low < high);
        type len = high - low;
        seed_to_location.emplace_back(make_tuple(index, low, len));
        index += len;
    }
    else{
        auto it1 = LowerBound(maps[level-1].begin(), maps[level-1].end(), low);
        auto it2 = LowerBound(maps[level-1].begin(), maps[level-1].end(), high);
        if(it1 == it2){
            type src = get<0>(*it1);
            type dest = get<1>(*it1);
            type len = get<2>(*it1);
            if(high <= src + len){
                type l = dest + low - src;
                type r = dest + high - src;
                constructMap(seed_to_location, level-1, maps, l, r, index);
            }
            else if(low >= src + len){
                constructMap(seed_to_location, level-1, maps, low, high, index);
            }
            else{
                type l = dest + low - src;
                type r = dest + len;
                constructMap(seed_to_location, level-1, maps, l, r, index);
                l = src + len;
                r = high;
                constructMap(seed_to_location, level-1, maps, l, r, index);
            }
        }
        else{
            // low to it1+1
            type src = get<0>(*it1);
            type dest = get<1>(*it1);
            type len = get<2>(*it1);
            if(low < src + len){
                type l = dest + low - src;
                type r = dest + len;
                constructMap(seed_to_location, level-1, maps, l, r, index);
                l = src + len;
                r = get<0>(*(it1+1));
                if(l < r) constructMap(seed_to_location, level-1, maps, l, r, index); 
            }
            else{
                type r = get<0>(*(it1+1));
                constructMap(seed_to_location, level-1, maps, low, r, index);
            }
            // it1+1 to it2
            for(auto it = it1 + 1; it < it2; it++){
                src = get<0>(*it);
                dest = get<1>(*it);
                len = get<2>(*it);
                type l = dest;
                type r = dest + len;
                constructMap(seed_to_location, level-1, maps, l, r, index);
                l = src + len;
                r = get<0>(*(it+1));
                if(r > l) constructMap(seed_to_location, level-1, maps, l, r, index);
            }
            // it2 to high
            src = get<0>(*it2);
            dest = get<1>(*it2);
            len = get<2>(*it2);
            if(high <= src + len){
                type l = dest;
                type r = dest + high - src;
                constructMap(seed_to_location, level-1, maps, l, r, index);
            }
            else{
                type l = dest;
                type r = dest + len;
                constructMap(seed_to_location, level-1, maps, l, r, index);
                l = src + len;
                r = high;
                constructMap(seed_to_location, level-1, maps, l, r, index);
            }
        }
    }
}

void printMap(vector<Tup>& seed_to_location){
    for(auto it = seed_to_location.begin(); it != seed_to_location.end(); it++){
        cout << get<0>(*it) << " " << get<1>(*it) << " " << get<2>(*it) << endl;
    }
}

int puzzle2(vector<Tup>& seed_to_soil, 
            vector<Tup>& soil_to_fertilizer, 
            vector<Tup>& fertilizer_to_water, 
            vector<Tup>& water_to_light, 
            vector<Tup>& light_to_temperature, 
            vector<Tup>& temperature_to_humidity, 
            vector<Tup>& humidity_to_location, 
            array<type, seed_num>& seeds){
    //calculate
    type min = MAX;
    vector<Tup> seed_to_loaction;
    vector<vector<Tup>> maps;
    maps.emplace_back(humidity_to_location);
    maps.emplace_back(temperature_to_humidity);
    maps.emplace_back(light_to_temperature);
    maps.emplace_back(water_to_light);
    maps.emplace_back(fertilizer_to_water);
    maps.emplace_back(soil_to_fertilizer);
    maps.emplace_back(seed_to_soil);
    type index = 0;
    constructMap(seed_to_loaction, 7, maps, 0, MAX, index);
    printMap(seed_to_loaction);
    for(int i=0; i<seed_num; i+=2){
        type seed_begin = seeds[i];
        type seed_end = seeds[i] + seeds[i+1];
        auto it1 = LowerBound(seed_to_loaction.begin(), seed_to_loaction.end(), seed_begin);
        auto it2 = LowerBound(seed_to_loaction.begin(), seed_to_loaction.end(), seed_end);
        type src = get<0>(*it1);
        type dest = get<1>(*it1);
        type len = get<2>(*it1);
        min = min < dest + seed_begin - src ? min : dest + seed_begin - src;
        for(auto it = it1 + 1; it <= it2; it++){
            src = get<0>(*it);
            dest = get<1>(*it);
            len = get<2>(*it);
            min = min < dest ? min : dest;
        }
    }
    return min;
}

int main(){
    vector<Tup> seed_to_soil;
    vector<Tup> soil_to_fertilizer;
    vector<Tup> fertilizer_to_water;
    vector<Tup> water_to_light;
    vector<Tup> light_to_temperature;
    vector<Tup> temperature_to_humidity;
    vector<Tup> humidity_to_location;
    array<type, seed_num> seeds;
    ifstream file("input.txt");
    string line;
    //read seeds
    getline(file, line);
    stringstream ss(line.substr(line.find(":")+2));
    for(int i=0; i<seed_num; i++){
        ss >> seeds[i];
    }
    //read seed_to_soil;
    getline(file, line);
    getline(file, line);
    while(getline(file, line) && line != ""){
        stringstream ss(line);
        type src, dest, length;
        ss >> dest >> src >> length;
        seed_to_soil.push_back(make_tuple(src, dest, length));
    }
    getline(file, line);
    //read soil_to_fertilizer;
    while(getline(file, line) && line != ""){
        stringstream ss(line);
        type src, dest, length;
        ss >> dest >> src >> length;
        soil_to_fertilizer.push_back(make_tuple(src, dest, length));
    }
    getline(file, line);
    //read fertilizer_to_water;
    while(getline(file, line) && line != ""){
        stringstream ss(line);
        type src, dest, length;
        ss >> dest >> src >> length;
        fertilizer_to_water.push_back(make_tuple(src, dest, length));
    }
    getline(file, line);
    //read water_to_light;
    while(getline(file, line) && line != ""){
        stringstream ss(line);
        type src, dest, length;
        ss >> dest >> src >> length;
        water_to_light.push_back(make_tuple(src, dest, length));
    }
    getline(file, line);
    //read light_to_temperature;
    while(getline(file, line) && line != ""){
        stringstream ss(line);
        type src, dest, length;
        ss >> dest >> src >> length;
        light_to_temperature.push_back(make_tuple(src, dest, length));
    }
    getline(file, line);
    //read temperature_to_humidity;
    while(getline(file, line) && line != ""){
        stringstream ss(line);
        type src, dest, length;
        ss >> dest >> src >> length;
        temperature_to_humidity.push_back(make_tuple(src, dest, length));
    }
    getline(file, line);
    //read humidity_to_location;
    while(getline(file, line) && line != ""){
        stringstream ss(line);
        type src, dest, length;
        ss >> dest >> src >> length;
        humidity_to_location.push_back(make_tuple(src, dest, length));
    }

    //sort
    sort(seed_to_soil.begin(), seed_to_soil.end(), [](Tup a, Tup b){
        return get<0>(a) < get<0>(b);
    });
    sort(soil_to_fertilizer.begin(), soil_to_fertilizer.end(), [](Tup a, Tup b){
        return get<0>(a) < get<0>(b);
    });
    sort(fertilizer_to_water.begin(), fertilizer_to_water.end(), [](Tup a, Tup b){
        return get<0>(a) < get<0>(b);
    });
    sort(water_to_light.begin(), water_to_light.end(), [](Tup a, Tup b){
        return get<0>(a) < get<0>(b);
    });
    sort(light_to_temperature.begin(), light_to_temperature.end(), [](Tup a, Tup b){
        return get<0>(a) < get<0>(b);
    });
    sort(temperature_to_humidity.begin(), temperature_to_humidity.end(), [](Tup a, Tup b){
        return get<0>(a) < get<0>(b);
    });
    sort(humidity_to_location.begin(), humidity_to_location.end(), [](Tup a, Tup b){
        return get<0>(a) < get<0>(b);
    });

    if(get<0>(seed_to_soil[0]) != 0){
        seed_to_soil.emplace(seed_to_soil.begin(), make_tuple(0, 0, get<0>(seed_to_soil[0])));
    }
    if(get<0>(soil_to_fertilizer[0]) != 0){
        soil_to_fertilizer.emplace(soil_to_fertilizer.begin(), make_tuple(0, 0, get<0>(soil_to_fertilizer[0])));
    }
    if(get<0>(fertilizer_to_water[0]) != 0){
        fertilizer_to_water.emplace(fertilizer_to_water.begin(), make_tuple(0, 0, get<0>(fertilizer_to_water[0])));
    }
    if(get<0>(water_to_light[0]) != 0){
        water_to_light.emplace(water_to_light.begin(), make_tuple(0, 0, get<0>(water_to_light[0])));
    }
    if(get<0>(light_to_temperature[0]) != 0){
        light_to_temperature.emplace(light_to_temperature.begin(), make_tuple(0, 0, get<0>(light_to_temperature[0])));
    }
    if(get<0>(temperature_to_humidity[0]) != 0){
        temperature_to_humidity.emplace(temperature_to_humidity.begin(), make_tuple(0, 0, get<0>(temperature_to_humidity[0])));
    }
    if(get<0>(humidity_to_location[0]) != 0){
        humidity_to_location.emplace(humidity_to_location.begin(), make_tuple(0, 0, get<0>(humidity_to_location[0])));
    }

    cout << puzzle2(seed_to_soil, soil_to_fertilizer, fertilizer_to_water, water_to_light, 
                    light_to_temperature, temperature_to_humidity, humidity_to_location, seeds) 
         << endl;
    return 0;
}
