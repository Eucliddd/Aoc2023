#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <map>
#include <queue>

using namespace std;

void split(const string& s, char delim, vector<string>& v){
    stringstream ss(s);
    string token;
    while(getline(ss, token, delim)){
        // trim token
        token.erase(remove(token.begin(), token.end(), ' '), token.end());
        v.push_back(token);
    }
}

class Module{
protected:
    string type;
    vector<string> outputs;
public:
    Module() = default;
    virtual ~Module() = default;
    virtual string getType() const { return type; }
    virtual vector<string> getOutputs() const { return outputs; }
};

class FlipFlop : public Module{
public:
    FlipFlop(bool state, const vector<string>& outputs): state(state){
        this->outputs = outputs;
        type = "FlipFlop";
    }
    inline bool getState() const { return state; }
    inline void setState(bool state) { this->state = state; }
    inline void toggleState() { state = !state; }
private:
    bool state = false;
};

class Conjuction : public Module{
public:
    Conjuction(const map<string,bool>& inputs, const vector<string>& outputs): inputs(inputs) {
        this->outputs = outputs;
        type = "Conjuction";
    }
    inline map<string,bool> getInputs() const { return inputs; }
    inline void setInputs(const map<string,bool>& inputs) { this->inputs = inputs; }
    inline void setInput(const string& name, bool value) { inputs[name] = value; }
private:
    map<string,bool> inputs;
};

class Broadcaster : public Module{
public:
    Broadcaster(const vector<string>& outputs){
        this->outputs = outputs;
        type = "Broadcaster";
    }
};

map<string, Module*> modules;

struct message{
    string src;
    string dest;
    bool signal;
};


void pressButton(uint64_t& high, uint64_t& low){
    queue<message> messages;
    messages.push({"","broadcaster",false});
    while(!messages.empty()){
        auto msg = messages.front();
        messages.pop();
        if(msg.signal) high++;
        else low++;
        auto dest = modules.find(msg.dest);
        if(dest != modules.end()){
            Module* module = dest->second;
            if(module->getType() == "FlipFlop"){
                if(!msg.signal){
                    static_cast<FlipFlop*>(module)->toggleState();
                    for(auto& output : module->getOutputs()){
                        messages.push({msg.dest, output, static_cast<FlipFlop*>(module)->getState()});
                    }
                }
            }
            else if(module->getType() == "Conjuction"){
                static_cast<Conjuction*>(module)->setInput(msg.src, msg.signal);
                bool signal = false;
                for(auto& [name, input] : static_cast<Conjuction*>(module)->getInputs()){
                    if(!input){
                        signal = true;
                        break;
                    }
                }
                for(auto& output : module->getOutputs()){
                    messages.push({msg.dest, output, signal});
                }
            }
            else if(module->getType() == "Broadcaster"){
                for(auto& output : module->getOutputs()){
                    messages.push({msg.dest, output, msg.signal});
                }
            }
        }
    }
}

bool pressButton(string end, string& from){
    queue<message> messages;
    messages.push({"","broadcaster",false});
    bool found = false;
    while(!messages.empty()){
        auto msg = messages.front();
        messages.pop();
        auto dest = modules.find(msg.dest);
        if(dest != modules.end()){
            Module* module = dest->second;
            if(module->getType() == "FlipFlop"){
                if(!msg.signal){
                    static_cast<FlipFlop*>(module)->toggleState();
                    for(auto& output : module->getOutputs()){
                        messages.push({msg.dest, output, static_cast<FlipFlop*>(module)->getState()});
                    }
                }
            }
            else if(module->getType() == "Conjuction"){
                static_cast<Conjuction*>(module)->setInput(msg.src, msg.signal);
                if(msg.dest == end && msg.signal){
                    from = msg.src;
                    found = true;
                }
                bool signal = false;
                for(auto& [name, input] : static_cast<Conjuction*>(module)->getInputs()){
                    if(!input){
                        signal = true;
                        break;
                    }
                }
                for(auto& output : module->getOutputs()){
                    messages.push({msg.dest, output, signal});
                }
            }
            else if(module->getType() == "Broadcaster"){
                for(auto& output : module->getOutputs()){
                    messages.push({msg.dest, output, msg.signal});
                }
            }
        }
    }
    return found;
}  

uint64_t puzzle1(){
    uint64_t high = 0, low = 0;
    for(int i = 0 ; i < 1000; i++){
        pressButton(high, low);
    }
    return high*low;
}

template<typename T>
T gcd(T a, T b){
    if (a < b)
        swap(a, b);
    while (b != 0){
        T temp = a % b;
        a = b;
        b = temp;
    }
    return a;
}

template<typename T>
T lcm(T a, T b){
    return a * b / gcd(a, b);
}

uint64_t puzzle2(){
    map<string,uint64_t> key_modules;
    string sender;
    for(auto [name, module] : modules){
        auto out_v = module->getOutputs();
        if(find(out_v.begin(), out_v.end(), "rx") != out_v.end()){
            sender = name;
            break;
        }
    }
    for(auto [name, module] : modules){
        auto out_v = module->getOutputs();
        if(find(out_v.begin(), out_v.end(), sender) != out_v.end()){
            key_modules[name] = 0;
        }
    }
    int steps = 1;
    while(true){
        string from;
        if(pressButton(sender, from)){
            if(key_modules[from] == 0)
                key_modules[from] = steps;
        }
        bool all = true;
        for(auto [_, value] : key_modules){
            if(value == 0){
                all = false;
                break;
            }
        }
        if(all) break;
        steps++;
    }
    uint64_t ans = 1;
    for(auto [_, value] : key_modules){
        ans = lcm(ans, value);
    }
    return ans;
}

int main(){
    ifstream file("input.txt");
    string line;
    while(getline(file, line)){
        if(line.substr(0, line.find("->")-1) == "broadcaster"){
            string output = line.substr(line.find("->")+3);
            // split output by ','
            vector<string> outputs;
            split(output, ',', outputs);
            modules["broadcaster"] = new Broadcaster(outputs);
        }
        else if(line[0] == '%'){
            string output = line.substr(line.find("->")+3);
            string name = line.substr(1, line.find("->")-2);
            // split output by ','
            vector<string> outputs;
            split(output, ',', outputs);
            modules[name] = new FlipFlop(false, outputs);
        }
        else if(line[0] == '&'){
            string output = line.substr(line.find("->")+3);
            string name = line.substr(1, line.find("->")-2);
            // split output by ','
            vector<string> outputs;
            split(output, ',', outputs);
            modules[name] = new Conjuction(map<string,bool>(), outputs);
        }
    }
    for(auto& [name, module] : modules){
        for(auto& dest : module->getOutputs()){
            auto src = modules.find(dest);
            if(src != modules.end() && src->second->getType() == "Conjuction"){
                static_cast<Conjuction*>(src->second)->setInput(name, false);
            }
        }
    }
    cout << puzzle2() << endl;
}